#include <seastar/core/app-template.hh>
#include <seastar/core/do_with.hh>
#include <seastar/core/future.hh>
#include <seastar/core/loop.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/sharded.hh>
#include <seastar/core/sleep.hh>
#include <seastar/util/log.hh>
#include <stdexcept>
#include <iostream>

class my_service {
public:
    size_t nums_cnt;
    size_t curr_cand;
    size_t total_primes;
    
    my_service(size_t nums_cnt) : nums_cnt(nums_cnt), curr_cand(2), total_primes(0) { }
    
    seastar::future<> run() {
        std::cerr << "running on " << seastar::this_shard_id() 
                  << " checking for primes from 2 to " << nums_cnt << "\n";
            
        return seastar::do_until( [&] { return curr_cand > nums_cnt; }, [&] () mutable {
    
                return seastar::do_with(int(2), bool(true), [&] (int &i, bool &result) {
                    return seastar::do_until([&] { return i >= curr_cand; }, [&] {
                        if (curr_cand % i == 0) {
                            i = curr_cand; // break
                            result = false;
                        } else {
                            ++i;
                        }
                            
                        return seastar::make_ready_future<>();
                    }).then([&] () { 
                        return seastar::make_ready_future<bool>(result);
                    });
                }).then([&] (bool is_prime) {
                    if (is_prime) {
                        total_primes++;
                    }
                    
                    curr_cand++; 
                });
                
        }).then([&] {
            std::cout << "Primes count: " << total_primes << std::endl; 
            return seastar::make_ready_future<>();
        });
    }
    
    seastar::future<> stop() {
        return seastar::make_ready_future<>();
    }
};

seastar::sharded<my_service> s;

seastar::future<> f() {
    return s.start(40000).then([] {
        auto service = s.invoke_on_all([] (my_service& local_service) {
            return local_service.run();
        });
        
        using namespace std::chrono_literals;
        seastar::sleep(3s).then([] {
            return s.invoke_on_all([] (my_service& local_service) {
                std::cout << "Primes count: " << local_service.total_primes << std::endl; 
                return seastar::make_ready_future<>();
            }); 
        });
        
        return service;
    }).then([] {
        return s.stop();
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

