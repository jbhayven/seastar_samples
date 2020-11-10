#include <seastar/core/app-template.hh>
#include <seastar/util/log.hh>
#include <seastar/core/sleep.hh>
#include <iostream>
#include <stdexcept>

seastar::future<> f() {
    std::cout << "Sleeping... " << std::flush;
    using namespace std::chrono_literals;
    seastar::sleep(2000ms).then([] { std::cout << "2000ms " << std::flush; });
    seastar::sleep(4000ms).then([] { std::cout << "4000ms " << std::flush; });
    seastar::sleep(3000ms).then([] { std::cout << "3000ms " << std::flush; });
    seastar::sleep(2000ms).then([] { std::cout << "2000ms " << std::flush; });
    seastar::sleep(1000ms).then([] { std::cout << "1000ms " << std::flush; });
    return seastar::sleep(5s).then([] { std::cout << "Done.\n"; });
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
