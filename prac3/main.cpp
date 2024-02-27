#include <thread>
#include <iostream>
#include <mutex>
#include <vector>
#include <cmath>

std::mutex mtx;
class Test {

public:
    void run(std::string threadName) {
        for (int i = 0; i < 10; i++) {
            mtx.lock();
            mtx.unlock();
            std::string out = threadName + std::to_string(i) + "\n";
            mtx.lock();
            std::cout<<out;
            mtx.unlock();
        }
    }

    void times (double& left, double& right){
        left *= right;
    }
};
void times1 (double& left, double& right){
    left *= right;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

double time2 (double& left, double& right){
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return left * right;
}


int main() {
    std::cout<<"hardware_concurrency: "<<std::thread::hardware_concurrency()<<"\n";
    std::cout<<"Main thread id: "<<std::this_thread::get_id()<<"\n";
    std::cout<<"TASK 1: \n";
    double left = 1.2;
    double right = 2;
    int n = 5;
    std::vector<std::thread>threads(n);
    for (int i = 0; i< n;++i){
        threads[i]=  std::thread (times1,std::ref(left),std::ref(right));
    }
    for (int i = 0; i<n;++i){
        std::cout<<left<<"\n";
        threads[i].join();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    std::cout<<left<<"\n";
    left = 1.2;
    right = 3;
    double res;
    std::thread thread2 ([&left,&right, &res](){res = time2(std::ref(left),std::ref(right));});
    std::cout<<"TASK 2: ";
    thread2.join();
    std::cout<<res << "\n";
    std::cout<<"TASK 3: ";
    Test test;
    std::vector<std::thread>thread(n*2);
    for (int i = 0; i< 2*n;i+=2){
        thread[i]=  std::thread (&Test::run,test,std::string ("i = "+std::to_string(i)));
        thread[i+1]=  std::thread (&Test::times,test, std::ref(left),std::ref(right));
    }
    for (int i = 0; i<2*n;++i){
        thread[i].join();
        std::cout<<left<<"\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}
