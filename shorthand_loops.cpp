#include <seastar/core/app-template.hh>
#include <seastar/util/log.hh>
#include <seastar/core/sleep.hh>
#include <seastar/core/do_with.hh>
#include <seastar/core/loop.hh>
#include <iostream>
#include <stdexcept>

seastar::future<int> get_random_number() {
    return seastar::make_ready_future<int>(rand() % 101010 + 42);
}

seastar::future<std::vector<int>> get_random_elements(int cnt) {
    srand(time(NULL));
    
    return seastar::do_with(std::vector<int>(), [cnt] (std::vector<int> &vect) {
        return seastar::do_until([&vect, cnt] { return vect.size() >= cnt; }, [&vect, cnt] () mutable {
            std::cout << "Current size: " << vect.size() << std::endl;
            
            return get_random_number().then([&vect] (int number) mutable {
                vect.push_back(number);
            });
        }).then([&vect] {
            std::cout << "Final size: " << vect.size() << std::endl; 
            return seastar::make_ready_future<std::vector<int>>(std::move(vect));
        });
    });
}

seastar::future<> f() {
    return get_random_elements(10).then([] (std::vector<int> nums) {
        std::cout << "Got " << nums.size() << " numbers: ";
        for (int i : nums) std::cout << i << " "; 
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



