#include <seastar/core/app-template.hh>
#include <seastar/core/future.hh>
#include <seastar/util/log.hh>
#include <seastar/core/loop.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/sharded.hh>
#include <iostream>
#include <stdexcept>

seastar::future<> service_loop() {
    return seastar::keep_doing([] () {
        std::cout << seastar::this_shard_id() << " says hello!" << std::endl;
        return seastar::make_ready_future<>();
    });
}

seastar::future<> f() {
    return seastar::parallel_for_each(boost::irange<unsigned>(0, seastar::smp::count),
            [] (unsigned c) {
        return seastar::smp::submit_to(c, service_loop);
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

