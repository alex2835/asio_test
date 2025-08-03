#include <coroutine>
#include <iostream>
#include <optional>

template<typename T>
struct Generator {
    struct promise_type {
        T current_value;
        
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() { throw; }
        
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        
        void return_void() {}
    };
    
    std::coroutine_handle<promise_type> coro;
    
    Generator(std::coroutine_handle<promise_type> h) : coro(h) {}
    ~Generator() { if (coro) coro.destroy(); }
    
    // Iterator interface
    struct iterator {
        std::coroutine_handle<promise_type> coro;

        iterator(std::coroutine_handle<promise_type> h) : coro(h) {}

        T operator*() const {
            if (!coro) throw std::runtime_error("Dereferencing end iterator");
            return coro.promise().current_value;
        }

        iterator& operator++() {
            if (!coro) throw std::runtime_error("Incrementing end iterator");
            coro.resume();
            if (coro.done()) coro = nullptr;
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return coro != other.coro;
        }
    };

    iterator begin() {
        if (coro) coro.resume();
        return iterator{coro && !coro.done() ? coro : nullptr};
    }
    
    iterator end() { return iterator{nullptr}; }
};

// Usage: Fibonacci generator
Generator<int> fibonacci(int count) {
    int a = 0, b = 1;
    for (int i = 0; i < count; ++i) {
        co_yield a;
        auto temp = a + b;
        a = b;
        b = temp;
    }
}

int main() {
    auto generator = fibonacci(10);
    for (auto value : generator) {
        std::cout << value << " ";
    }
    // Output: 0 1 1 2 3 5 8 13 21 34
}