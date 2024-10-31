#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool {
public:
	ThreadPool(int size);
	ThreadPool(const ThreadPool&) = delete;

	~ThreadPool();

	template <typename Func, typename... Args>
	auto enqueue(Func&& f, Args&&... args) -> std::future<void>;

private:
	void executeTask();

	std::vector<std::thread> _workerThreads;
	std::queue<std::function<void()>> _tasks;
	std::mutex _taskQueueMutex;
	std::condition_variable _condition;
	bool _stop;
};
// This function adds a task to the thread pool for execution. It accepts a callable object
// `f` along with its arguments `args`. The function returns a future that represents the
// result of the task.
template <typename Func, typename... Args>
auto ThreadPool::enqueue(Func&& f, Args&&... args) -> std::future<void> {
	// Create a packaged_task to wrap the function and its arguments
	auto task = std::make_shared<std::packaged_task<void()>>(
		std::bind(std::forward<Func>(f), std::forward<Args>(args)...)
	);

	// Get a future associated with the packaged_task's result
	std::future<void> result = task->get_future();
	{
		std::unique_lock<std::mutex> lock(_taskQueueMutex);
		_tasks.emplace([task]() { (*task)(); });
	}

	_condition.notify_one();

	return result;
}
