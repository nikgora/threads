#include <thread>
#include <iostream>
#include <string>
#include <fstream>
#include <random>
//std::ofstream std::cout("out.txt");

void run1(std::string threadName) {
    for (int i = 0; i < 10; i++) {
        std::cout << std::this_thread::get_id();
        std::string out = threadName + std::to_string(i) + "\n";
        std::cout<<out;
    }
}

void run211(std::string threadName, bool flag) {
    if (flag) {
        for (int i = 0; i < 10; i++) {
            std::cout << std::this_thread::get_id();
            std::string out = threadName + std::to_string(i) + " flag: " + std::to_string(flag) + "\n";
            std::cout << out;
        }
    }
}

void run212(std::string threadName, bool flag) {
    for (int i = 0; i < 10; i++) {
        std::cout << std::this_thread::get_id();
        std::string out = threadName + std::to_string(i) +" flag: " + std::to_string(flag) + "\n";
        std::cout << out;

    }
    flag = false;
}

void run22(std::string threadName) {
    for (int i = 0; i < 10; i++) {
        std::srand(std::time(nullptr));
        int rand = std::rand()%10;
        if (i - rand > 0) {
            std::cout << std::this_thread::get_id();
            std::string out = threadName + std::to_string(i) + "\n";
            std::cout << out;
        }
    }
}

void partialDotProduct(const std::vector<int>& v1, const std::vector<int>& v2) {
    int result = 0;

    for (size_t i = 0; i < v1.size(); ++i) {
        result += v1[i] * v2[i];
    }
    std::cout<<std::this_thread::get_id()<<" " << result;
}

int main()
{
    std::cout<<"hardware_concurrency: "<<std::thread::hardware_concurrency()<<"\n";
    std::cout<<"Main thread id: "<<std::this_thread::get_id()<<"\n";
    //TASK1
    std::thread thread111(run1,"first11");
    std::thread thread121(run1,"second11");
    std::thread thread112(run1,"first12");
    std::thread thread122(run1,"second12");
    std::thread thread113(run1,"first13");
    std::thread thread123(run1,"second13");

    //TASK 2.1
    std::thread thread2111(run211,"first211", true);
    std::thread thread2121(run212,"second211", true);
    std::thread thread2112(run211,"first212", true);
    std::thread thread2122(run212,"second212", true);
    std::thread thread2113(run211,"first213", true);
    std::thread thread2123(run212,"second213", true);
    std::thread thread2114(run211,"first214", true);
    std::thread thread2124(run212,"second214", true);
    std::thread thread2115(run211,"first215", true);
    std::thread thread2125(run212,"second215", true);


    //TASK 2.2
    std::thread thread221(run22,"first221");
    std::thread thread222(run22,"first222");
    std::thread thread223(run22,"first223");



    //TASK 3
    std::vector<int> vector1 = {1, 2, 3, 4, 5};
    std::vector<int> vector2 = {6, 7, 8, 9, 4};
    if (vector1.size()!=vector2.size()){
        std::cout <<std::this_thread::get_id() <<" Vectors must be the same size!\n";
    }
    else {
        std::cout<<"Task 3: ";
        std::thread thread3(partialDotProduct, std::ref(vector1), std::ref(vector2));
        thread3.join();
    }
    thread111.join();
    thread121.join();
    thread112.join();
    thread122.join();
    thread113.join();
    thread123.join();
    thread2111.join();
    thread2121.join();
    thread2112.join();
    thread2122.join();
    thread2113.join();
    thread2123.join();
    thread2114.join();
    thread2124.join();
    thread2115.join();
    thread2125.join();
    thread221.join();
    thread222.join();
    thread223.join();
    return 0;
}