#include "task.h"

Task::Task(unsigned int flags):
	mTaskFlags(flags)
{
}

Task::~Task() {
}

unsigned int Task::getTaskFlags() const {
	return mTaskFlags;
}

Task::TaskBeginning::TaskBeginning(Task::TaskPtr t):
	mTask(t)
{
}

Task::TaskCompleted::TaskCompleted(Task::TaskPtr t):
	mTask(t)
{
}