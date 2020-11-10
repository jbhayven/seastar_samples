#include <seastar/core/app-template.hh>
#include <seastar/util/log.hh>
#include <seastar/core/sleep.hh>
#include <iostream>
#include <stdexcept>

seastar::future<int> slow_get(int val) {
    using namespace std::chrono_literals;
    return seastar::sleep(1s).then([val] { return val; });
}

seastar::future<> f() {
    return slow_get(42).then([] (int val) {
        std::cout << "Got " << val << "\n";
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
