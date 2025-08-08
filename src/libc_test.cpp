// check_threading.cpp
#include <iostream>
#include <version>

int main() {
    std::cout << "=== C++20 Threading Feature Support ===" << std::endl;
    
#ifdef _LIBCPP_VERSION
    std::cout << "libc++ version: " << _LIBCPP_VERSION << std::endl;
#endif
    
    std::cout << "__cplusplus: " << __cplusplus << std::endl;
    
    // Check specific feature macros
#ifdef __cpp_lib_jthread
    std::cout << "✅ __cpp_lib_jthread: " << __cpp_lib_jthread << std::endl;
#else
    std::cout << "❌ __cpp_lib_jthread: NOT AVAILABLE" << std::endl;
#endif

#ifdef __cpp_lib_atomic_wait
    std::cout << "✅ __cpp_lib_atomic_wait: " << __cpp_lib_atomic_wait << std::endl;
#else
    std::cout << "❌ __cpp_lib_atomic_wait: NOT AVAILABLE" << std::endl;
#endif

#ifdef __cpp_lib_semaphore
    std::cout << "✅ __cpp_lib_semaphore: " << __cpp_lib_semaphore << std::endl;
#else
    std::cout << "❌ __cpp_lib_semaphore: NOT AVAILABLE" << std::endl;
#endif

#ifdef __cpp_lib_latch
    std::cout << "✅ __cpp_lib_latch: " << __cpp_lib_latch << std::endl;
#else
    std::cout << "❌ __cpp_lib_latch: NOT AVAILABLE" << std::endl;
#endif

#ifdef __cpp_lib_barrier
    std::cout << "✅ __cpp_lib_barrier: " << __cpp_lib_barrier << std::endl;
#else
    std::cout << "❌ __cpp_lib_barrier: NOT AVAILABLE" << std::endl;
#endif

    return 0;
}