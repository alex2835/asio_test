#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio.hpp>
#include <iostream>

boost::asio::awaitable<int> cpu_intensive_calculation(int n) {
    // Get the current executor (I/O context)
    auto io_executor = co_await boost::asio::this_coro::executor;
    
    // Create or use a thread pool for CPU work
    static boost::asio::thread_pool cpu_pool(std::thread::hardware_concurrency());
    
    // Offload CPU work to thread pool
    int result = co_await boost::asio::co_spawn(cpu_pool, [n]() -> boost::asio::awaitable<int> {
        // Simulate heavy CPU work
        int sum = 0;
        for (int i = 0; i < n; ++i) {
            sum += i * i;
            // Yield periodically to allow cancellation
            if (i % 10000 == 0) {
                std::println("Yielding to allow cancellation...");
                co_await boost::asio::this_coro::executor;
            }
        }
        co_return sum;
    }, boost::asio::use_awaitable);
    
    co_return result;
}

boost::asio::awaitable<void> main_coroutine() {
    std::cout << "Starting CPU work...\n";
    
    int result = co_await cpu_intensive_calculation(1000000);
    
    std::cout << "Result: " << result << "\n";
}


int main() {
    boost::asio::io_context io_context;
    
    // Start the main coroutine
    boost::asio::co_spawn(io_context, main_coroutine(), boost::asio::detached);
    
    // Run the I/O context to execute the coroutine
    io_context.run();
    
    return 0;
}