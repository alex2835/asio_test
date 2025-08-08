// #define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#include <boost/asio.hpp>
#include <iostream>
#include <chrono>
#include <boost/asio/experimental/awaitable_operators.hpp>
using namespace boost::asio::experimental::awaitable_operators;

using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::cancellation_signal;
using boost::asio::bind_cancellation_slot;
using namespace std::chrono_literals;

void debug_cancellation_state(const boost::asio::cancellation_state& state, const std::string& context) {
    auto type = state.cancelled();
    std::cout << "[" << context << "] Cancellation state: ";
    
    if (type == boost::asio::cancellation_type::none) {
        std::cout << "NONE (0)";
    } else {
        std::cout << "ACTIVE - ";
        if (type == boost::asio::cancellation_type::terminal) std::cout << "TERMINAL ";
        if (type == boost::asio::cancellation_type::partial) std::cout << "PARTIAL ";
        std::cout << "(raw: " << (int)type << ")";
    }
    std::cout << std::endl;
}

awaitable<void> cancellable_task() {
    auto executor = co_await boost::asio::this_coro::executor;
    boost::asio::steady_timer timer(executor);
    int i = 0;

    try {
        while (true) {
            auto state = co_await boost::asio::this_coro::cancellation_state;
            debug_cancellation_state(state, "Iteration " + std::to_string(i));

            // Check cancellation before operation
            if (state.cancelled() != boost::asio::cancellation_type::none) {
                std::cout << "Cancellation detected! Exiting..." << std::endl;
                co_return;
            }

            timer.expires_after(1s);
            co_await timer.async_wait(use_awaitable);
            ++i;
        }
    } catch (const boost::system::system_error& e) {
        if (e.code() == boost::asio::error::operation_aborted) {
            std::cout << "Timer cancelled! Task stopping..." << std::endl;
            // Check final state
            // debug_cancellation_state(co_await boost::asio::this_coro::cancellation_state,
            //                          "After cancellation");
        } else {
            std::cout << "Unexpected error: " << e.what() << std::endl;
        }
    }
}

awaitable<void> cancel_after_timeout() {
    auto executor = co_await boost::asio::this_coro::executor;
    boost::asio::steady_timer timer(executor);
    
    std::cout << "Starting timeout timer..." << std::endl;
    timer.expires_after(3500ms);
    co_await timer.async_wait(use_awaitable);
    // signal.emit(boost::asio::cancellation_type::total);
    std::cout << "Timeout reached! Cancelling task..." << std::endl;
}


awaitable<void> main_coroutine() {
    auto executor = co_await boost::asio::this_coro::executor;
    auto result = co_await (
        co_spawn(
            executor,
            cancellable_task(),
            use_awaitable
        )
        ||
        co_spawn(
            executor,
            cancel_after_timeout(),
            use_awaitable
        )
    );

    if (result.index() == 1) {
        std::println("timeout reached, task cancelled");
    }
    else {
        std::println("task completed successfully");
    }
}

int main() {
    boost::asio::io_context io_context;

    co_spawn(io_context, main_coroutine(), detached);
  
    io_context.run();
    return 0;
}