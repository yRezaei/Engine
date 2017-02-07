#ifndef OA_TASK_H
#define OA_TASK_H

#include <boost/shared_ptr.hpp>

class TaskManager;

class Task {
public:
	typedef boost::shared_ptr<Task> TaskPtr;

	struct TaskBeginning {
		TaskBeginning(TaskPtr t);
		TaskPtr mTask;
	};
	
	struct TaskCompleted {
		TaskCompleted(TaskPtr t);
		TaskPtr mTask;
	};
	
	enum {
		NONE = 0x0,
		
		REPEATING = 0x1 << 0,
		THREADSAFE = 0x1 << 1,
		FRAME_SYNC = 0x1 << 2,
		
		SINGLETHREADED = NONE,
		SINGLETHREADED_REPEATING = REPEATING,	
		BACKGROUND = THREADSAFE,
		BACKGROUND_REPEATING = THREADSAFE | REPEATING,
		BACKGROUND_SYNC = THREADSAFE | FRAME_SYNC,
		BACKGROUND_SYNC_REPEATING = THREADSAFE | REPEATING | FRAME_SYNC,
		
		ALL = ~0x0
	};
	
	Task(unsigned int flags = SINGLETHREADED_REPEATING);
	virtual ~Task();
	
	virtual void run() = 0;
	
protected:
	friend class TaskManager;
	
	unsigned int getTaskFlags() const;
	
private:
	unsigned int mTaskFlags;
};

#endif