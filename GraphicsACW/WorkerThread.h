// David Hart - 2012

#pragma once

#include "Threading.h"
#include <queue>

class ITask
{

public:

	virtual void DoTask() = 0;
	virtual void TaskComplete() { }
	virtual void Release() { } 

};

class WorkerThread : public Threading::Thread
{
public:
	WorkerThread();
	~WorkerThread();

	void QueueTask(ITask* task);

	void Shutdown();
	bool IsShuttingDown();

	void Syncrhonise();

private:
	
	unsigned ThreadMain();

	bool _shutdown;

	std::queue<ITask*> _tasksWaiting;
	std::queue<ITask*> _tasksComplete;

	Threading::Mutex _taskMutex;
	Threading::Event _taskWaiting;
};