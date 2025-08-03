#include <coroutine>
#include <iostream>
#include <optional>
#include <print>

template <typename T>
struct Task {
    struct promise_type {
        T value;

        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        std::suspend_always initial_suspend() {
            return {};
        }
        std::suspend_always final_suspend() noexcept {
            return {};
        }
        void unhandled_exception() {
            throw;
        }
        void return_value(T&& val) {
            value = std::move(val);
        }
    };

    Task(std::coroutine_handle<promise_type> h) : coro(h) {}
    ~Task() {
        if (coro)
            coro.destroy();
    }


    std::coroutine_handle<promise_type> coro;
};


Task<int> example_task() {
    std::println("Running example_task...\n");
    co_await std::suspend_always{}; // Simulate some asynchronous work
    std::println("example_task completed.\n");

    co_return 42; // Example task returning an integer
}


int main() 
{
    auto task = example_task();
    std::println("Task created, now resuming...\n");
    
    // Simulate resuming the coroutine
    task.coro.resume();

    while (not task.coro.done()) {
        std::println("Task is still running.\n");
        task.coro.resume(); // Ensure the coroutine is resumed to finalize it
    }
    std::println("Task finished with value: {}\n", task.coro.promise().value);

    return 0;
}