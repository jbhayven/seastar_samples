#include <iostream>
#include <stdexcept>
#include <stack>
#include <vector>

std::vector<int> read_heights() {
    int n; std::cin >> n;
    
    std::vector<int> vect;
    
    while (vect.size() != n) {
        int d_i, w_i;
        std::cin >> d_i >> w_i;
        
        vect.push_back(w_i);
    }
    
    return vect;
}

void f() {
    std::vector<int> heights = read_heights();
    std::stack<int> stairs;
    int result = 0;
    
    for (int h : heights) {
        while (!stairs.empty() && h < stairs.top()) {
            stairs.pop();
        }
        
        if (stairs.empty() || h > stairs.top()) {
            stairs.push(h);
            result++;
        }
    }
               
    std::cout << result << std::endl;
}

int main(int argc, char** argv) {
    try {
        f();
    } catch(...) {
        std::cerr << "Couldn't solve\n";
        return 1;
    }
    return 0;
}





