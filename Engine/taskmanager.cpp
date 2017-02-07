#include "taskmanager.h"

//Dummy that ensures there's at least one background task
class BackgroundDummyTask: public Task {
public:
	BackgroundDummyTask(): Task(BACKGROUND_REPEATING) {}
	virtual ~BackgroundDummyTask() {}
	virtual void run() {}
};

TaskManager::TaskManager(unsigned int numThreads) {
	mNumThreads = numThreads;
	if (numThreads == 0)
		mNumThreads = boost::thread::hardware_concurrency() + 1;		

	mWriteList = 0;
	mReadList = 1;

	mNumTasksToWaitFor = 0;
}

TaskManager::~TaskManager() {
	mThreads.join_all();
}

void TaskManager::add(TaskPtr task) {
	unsigned flags = task->getTaskFlags();

	if (flags & Task::THREADSAFE) {
		if (flags & Task::FRAME_SYNC)
			mSyncTasks.push(task);
		else
			mBackgroundTasks.push(task);
	}
	else
		mTaskList[mWriteList].push(task);
}

void TaskManager::start() {
	mRunning = true;

	EventChannel chan;

	chan.add<Task::TaskCompleted>(*this);
	chan.add<StopEvent>(*this);

	add(TaskPtr(new BackgroundDummyTask()));

	for (unsigned int i = 0; i < mNumThreads; ++i)
		mThreads.add_thread(new boost::thread(boost::bind(&TaskManager::worker, this)));

	while (mRunning) {
		if (!mTaskList[mReadList].empty()) {
			TaskPtr t = mTaskList[mReadList].wait_pop();
			execute(t);
		}
		else {
			synchronize();
			std::swap(mReadList, mWriteList);				
		}

		boost::thread::yield();
	}
}
	
void TaskManager::synchronize() {
	ScopedLock lock(mSyncMutex);

	while (mNumTasksToWaitFor > 0)
		mCondition.wait(lock);

	mNumTasksToWaitFor = mSyncTasks.size();

	while (!mSyncTasks.empty())
		mBackgroundTasks.push(mSyncTasks.wait_pop());
}

void TaskManager::stop() {		
	mRunning = false;
}

void TaskManager::execute(TaskPtr t) {
	EventChannel chan;
		
	chan.broadcast(Task::TaskBeginning(t));
	t->run();
	chan.broadcast(Task::TaskCompleted(t));
}

void TaskManager::handle(const TaskManager::StopEvent& ) {		
	stop();
}

void TaskManager::handle(const Task::TaskCompleted& tc) {
	if (tc.mTask->getTaskFlags() & Task::REPEATING)
		add(tc.mTask);
}

void TaskManager::worker() {
	TaskPtr task;

	while (mRunning) {
		task = mBackgroundTasks.wait_pop();
		execute(task);

		if (task->getTaskFlags() & Task::FRAME_SYNC) {
			ScopedLock lock(mSyncMutex);
			--mNumTasksToWaitFor;
			lock.unlock();
						
			mCondition.notify_one();
		}			

		boost::thread::yield();
	}
}