#include <seastar/core/app-template.hh>
#include <seastar/util/log.hh>
#include <iostream>
#include <stdexcept>

seastar::future<> do_something() {
    try {
        throw std::runtime_error("This will be caught as an exceptional future");
    } catch(...) {
        return seastar::make_exception_future(std::current_exception());
    }
    return seastar::make_ready_future<>();
}

seastar::future<> f() {
    return do_something().then([] {
        std::cout << "We run a continuation" << std::endl;
    }).finally([] {
        std::cout << "We can clean up now" << std::endl;
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
