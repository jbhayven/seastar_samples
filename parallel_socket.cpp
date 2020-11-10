#include <seastar/core/app-template.hh>
#include <seastar/core/future.hh>
#include <seastar/util/log.hh>
#include <seastar/core/loop.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/sharded.hh>
#include <iostream>
#include <stdexcept>

seastar::future<> service_loop() {
    return seastar::do_with(seastar::listen(seastar::make_ipv4_address({6780})),
            [] (auto& listener) {
        return seastar::keep_doing([&listener] () {
            std::cout << seastar::this_shard_id() << " listening " << std::endl;
            return listener.accept().then(
                [] (seastar::accept_result res) {
                    std::cout << "Shard " << seastar::this_shard_id() 
                              << " accepted connection from " << res.remote_address << std::endl;
            });
        });
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

