#include <seastar/core/app-template.hh>
#include <seastar/util/log.hh>
#include <seastar/core/sleep.hh>
#include <iostream>
#include <stdexcept>

seastar::future<std::vector<int>> slow_get(seastar::lw_shared_ptr<std::vector<int>> numbers) {
    numbers->at(0) = 42;
    
    using namespace std::chrono_literals;
    return seastar::sleep(1s).then([vect_ptr = std::move(numbers)] { return *vect_ptr; });
}

seastar::future<> f() {
    auto our_vector = seastar::make_lw_shared<std::vector<int>>(6, 7);
    
    return slow_get(our_vector).then([=] (std::vector<int> nums) {
        std::cout << "Got ";
        for (int i : nums) std::cout << i << " "; 
        std::cout << "\n";
        std::cout << "Our vector: ";
        for (int i : *our_vector) std::cout << i << " ";
        std::cout << "\n";
    });
}

int main(int argc, char** argv) {
    seastar::app_template app;
    try {
        app.run(argc, argv, f);
    } catch(...) {
        std::cerr << "Couldn't start application: "
                  << std::current_exception() << "\n";
        return 1;
    }
    return 0;
}


