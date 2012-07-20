#include "WorkerThread.h"

WorkerThread::WorkerThread() :
	_shutdown(false)
{
	Start();
}

WorkerThread::~WorkerThread()
{
	Shutdown();

	while (!_tasksWaiting.empty())
	{
		delete _tasksWaiting.front();
		_tasksWaiting.pop();
	}
}

// Accepts new tasks into the queue, rejects new tasks when shutting down
void WorkerThread::QueueTask(ITask* task)
{
	Threading::ScopedLock lock(_taskMutex);

	_tasksWaiting.push(task);
	_taskWaiting.Raise();
}

unsigned WorkerThread::ThreadMain()
{
	while(!IsShuttingDown())
	{
		// Wait for thread to be pulsed (either by new task or shutdown)
		_taskWaiting.Wait(); 

		ITask* task = NULL;

		{
			Threading::ScopedLock lock(_taskMutex);

			// Aquire a new task from the queue if available
			if (!_tasksWaiting.empty())
			{
				task = _tasksWaiting.front();
				_tasksWaiting.pop();
			}

			// Reset wake event if no more tasks remain
			if (_tasksWaiting.empty())
				_taskWaiting.Reset();
		}

		// Complete active task and move to finished queue
		if (task != NULL)
		{
			task->DoTask();

			{
				Threading::ScopedLock lock(_taskMutex);
				_tasksComplete.push(task);
			}
		}
	}

	return 0;
}

void WorkerThread::Shutdown()
{
	{
		Threading::ScopedLock lock(_taskMutex);

		if (_shutdown)
			return;

		_shutdown = true;

		_taskWaiting.Raise();
	}

	Join();

	Syncrhonise();
}

bool WorkerThread::IsShuttingDown()
{
	Threading::ScopedLock lock(_taskMutex);
	
	return _shutdown;
}

void WorkerThread::Syncrhonise()
{
	std::queue<ITask*> tasksDoneLocal;
	
	{
		Threading::ScopedLock lock(_taskMutex);

		tasksDoneLocal.swap(_tasksComplete);
	}
	
	while(!tasksDoneLocal.empty())
	{
		tasksDoneLocal.front()->TaskComplete();
		delete tasksDoneLocal.front();
		tasksDoneLocal.pop();
	}
}