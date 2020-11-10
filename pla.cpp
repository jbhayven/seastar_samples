#include <seastar/core/app-template.hh>
#include <seastar/util/log.hh>
#include <seastar/core/sleep.hh>
#include <seastar/core/do_with.hh>
#include <seastar/core/loop.hh>
#include <iostream>
#include <stdexcept>
#include <stack>

seastar::future<std::vector<int>> read_heights() {
    int n; std::cin >> n;
    
    return seastar::do_with(std::vector<int>(), [n] (std::vector<int> &vect) {
        return seastar::do_until([&vect, n] { return vect.size() == n; }, [&vect, n] () mutable {
            int d_i, w_i;
            std::cin >> d_i >> w_i;
            
            vect.push_back(w_i); // ignore d_i
            return seastar::make_ready_future<>();
        }).then([&vect] {
            return seastar::make_ready_future<std::vector<int>>(std::move(vect));
        });
    });
}

seastar::future<> f() {
    return read_heights().then([] (std::vector<int> heights) {
        return seastar::do_with(std::move(heights), std::stack<int>(), int(0), [] (auto &heights, auto &stairs, int &result) {
            return seastar::do_for_each(heights, [&stairs, &result] (int h) mutable {
                while (!stairs.empty() && h < stairs.top()) {
                    stairs.pop();
                }
                
                if (stairs.empty() || h > stairs.top()) {
                    stairs.push(h);
                    result++;
                }
                
                return seastar::make_ready_future<>();
            }).then([&result] { 
                std::cout << result << std::endl;
                return seastar::make_ready_future<>();  
            });
        });
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




