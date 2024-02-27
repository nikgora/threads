#include <thread>
#include <iostream>
#include <mutex>
#include <vector>
#include <cmath>

std::mutex mtx;
void times (double& left, double& right){
    mtx.lock();
    left *= right;
    mtx.unlock();
}

int main() {
    std::cout<<"hardware_concurrency: "<<std::thread::hardware_concurrency()<<"\n";
    std::cout<<"Main thread id: "<<std::this_thread::get_id()<<"\n";
    std::cout<<"TASK 1: \n";
    double left = 1.2;
    double right = 2;
    std::thread thread11(times,std::ref(left),std::ref(right));
    std::thread thread12(times,std::ref(left),std::ref(right));
    std::thread thread13(times,std::ref(left),std::ref(right));
    std::thread thread14(times,std::ref(left),std::ref(right));
    std::cout<<left<<"\n";
    thread11.join();
    std::
    std::cout<<left<<"\n";
    thread12.join();
    std::cout<<left<<"\n";
    thread13.join();
    std::cout<<left<<"\n";
    thread14.join();
    std::cout<<left<<"\n";
    return 0;
}
