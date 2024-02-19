#include <iostream>
#include <thread>

void func1 () {

    std::cout<<"Hello\n";
}

int main() {
    std::thread t(func1);
    t.join();
    return 0;
}
