#include "ThreadPool.h"

// Constructor initializes the ThreadPool with the specified number of worker threads.
// Each worker thread is created and starts executing the executeTask function.
ThreadPool::ThreadPool(int size) : _stop(false) {
    for (int i = 0; i < size; ++i) {
        _workerThreads.emplace_back([this] { this->executeTask(); });
    }
}

// Destructor performs cleanup by notifying all worker threads to stop and joining them.
// This ensures all tasks are completed before the ThreadPool is destroyed.
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(_taskQueueMutex);
        _stop = true;
    }
    _condition.notify_all(); 
    for (std::thread& worker : _workerThreads) {
        worker.join(); // Join all worker threads to wait for their completion
    }
}

// Function executed by each worker thread.
// It continuously waits for tasks in the task queue and executes them.
void ThreadPool::executeTask() {
    while (true) {
        std::function<void()> task;
        {
            // Acquire lock to access the task queue
            std::unique_lock<std::mutex> lock(_taskQueueMutex);
            _condition.wait(lock, [this] { return _stop || !_tasks.empty(); });

            if (_stop && _tasks.empty()) {
                return; // Exit the thread if stop is requested and no tasks are pending
            }

            // Get the next task from the task queue
            task = std::move(_tasks.front());
            _tasks.pop();
        }
        task(); // Execute the task
    }
}
