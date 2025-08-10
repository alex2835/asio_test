#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <mutex>
#include <random>
#include <algorithm>
#include <print>

using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using namespace std::chrono_literals;


awaitable<void> sleep_sort(std::vector<int> numbers) {
    auto executor = co_await boost::asio::this_coro::executor;

    for (int number : numbers) {
        co_spawn(executor, [number]() -> awaitable<void> {
            boost::asio::steady_timer timer(co_await boost::asio::this_coro::executor);
            timer.expires_after(std::chrono::seconds(number));

            co_await timer.async_wait(use_awaitable);
            std::print("{}, ", number);
            co_return;
        }, detached);
    }
    // Wait for all coroutines to finish
    co_await boost::asio::this_coro::executor;
}


int main() {
    boost::asio::io_context io_context;
    
    co_spawn(io_context, sleep_sort({10,5,6,3,4,1}), detached);
    io_context.run();
    
    return 0;
}