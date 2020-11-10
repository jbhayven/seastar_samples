#include <seastar/core/app-template.hh>
#include <seastar/util/log.hh>
#include <iostream>
#include <stdexcept>

seastar::future<> f() {
    throw std::runtime_error("This will not be caught");
    return seastar::make_ready_future<>();
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
