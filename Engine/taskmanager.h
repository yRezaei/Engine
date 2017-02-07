#ifndef OA_TASKMANAGER_H
#define OA_TASKMANAGER_H

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <algorithm>
#include "concurrent_queue.h"
#include "task.h"
#include "eventchannel.h"

class TaskManager {
public:
	typedef boost::shared_ptr<Task> TaskPtr;
	typedef ConcurrentQueue<TaskPtr> TaskList;
	
	struct StopEvent {};
	
	TaskManager(unsigned int numThreads = 0); //0 for autodetect
	~TaskManager();
	
	void add(TaskPtr task);
	
	void start();
	void stop();
	
	void handle(const StopEvent& );
	void handle(const Task::TaskCompleted& tc);
	
private:
	void worker();
	void execute(TaskPtr task);
	void synchronize();
	
	boost::thread_group mThreads;
	unsigned int mNumThreads;
	
	bool mRunning;
	
	TaskList mTaskList[2];
	TaskList mBackgroundTasks;
	TaskList mSyncTasks;
	
	unsigned int mReadList;
	unsigned int mWriteList;
	
	typedef boost::mutex Mutex;
	typedef boost::condition_variable Condition;
	typedef Mutex::scoped_lock ScopedLock;
	
	mutable Mutex mSyncMutex;
	Condition mCondition;
	unsigned int mNumTasksToWaitFor;
};

#endif