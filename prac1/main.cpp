#include <thread>
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <mutex>
//std::ofstream std::cout("out.txt");

std::mutex mtx;

void run1(std::string threadName) {
    for (int i = 0; i < 10; i++) {
        mtx.lock();
        std::cout << std::this_thread::get_id();
        mtx.unlock();
        std::string out = threadName + std::to_string(i) + "\n";
        mtx.lock();
        std::cout<<out;
        mtx.unlock();
    }
}

void run211(std::string threadName, bool flag) {
    if (flag) {
        for (int i = 0; i < 10; i++) {
            mtx.lock();
            std::cout << std::this_thread::get_id();
            mtx.unlock();
            std::string out = threadName + std::to_string(i) + " flag: " + std::to_string(flag) + "\n";
            mtx.lock();
            std::cout << out;
            mtx.unlock();
        }
    }
}

void run212(std::string threadName, bool flag) {
    for (int i = 0; i < 10; i++) {
        mtx.lock();
        std::cout << std::this_thread::get_id();
        mtx.unlock();
        std::string out = threadName + std::to_string(i) +" flag: " + std::to_string(flag) + "\n";
        mtx.lock();
        std::cout << out;
        mtx.unlock();

    }
    flag = false;
}

void run22(std::string threadName) {
    std::srand(std::time(nullptr));
    for (int i = 0; i < 10; i++) {
        int rand = std::rand()%10;
        if (i - rand > 0) {
            mtx.lock();
            std::cout << std::this_thread::get_id();
            mtx.unlock();
            std::string out = threadName + std::to_string(i) + "\n";
            mtx.lock();
            std::cout << out;
            mtx.unlock();

        }
    }
}

void partialDotProduct(const std::vector<int>& v1, const std::vector<int>& v2, int& res) {
    int result = 0;

    for (size_t i = 0; i < v1.size(); ++i) {
        result += v1[i] * v2[i];
    }
    auto millisec_since_epoch =
            duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                    .count();
    mtx.lock();
    res+=result;

    std::cout<<std::this_thread::get_id()<<" time: "<<millisec_since_epoch%10000<<"\n";
    mtx.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));
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
    thread111.join();
    thread121.join();
    thread112.join();
    thread122.join();
    thread113.join();
    thread123.join();
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

    //TASK 2.2
    std::thread thread221(run22,"first221");
    std::thread thread222(run22,"first222");
    std::thread thread223(run22,"first223");


    thread221.join();
    thread222.join();
    thread223.join();

    //TASK 3
    std::vector<int> vector1 = {1, 2, 3, 4, 5};
    std::vector<int> vector2 = {6, 7, 8, 9, 4};
    if (vector1.size()!=vector2.size()){
        std::cout <<std::this_thread::get_id() <<" Vectors must be the same size!\n";
    }
    else {
        int res = 0;
        std::cout<<"Task 3: ";
        int size_of_chunk = 2;
        int number_of_chunks = vector1.size()/size_of_chunk+bool(vector1.size()%size_of_chunk);
        std::cout<<number_of_chunks<<"\n";
        std::vector<std::vector<int>> vectors1;
        std::vector<std::vector<int>> vectors2;
        for (int i = 0; i< vector1.size(); i+=size_of_chunk){
            std::vector<int>sub_vector1;
            std::vector<int>sub_vector2;
            int k = 0;
            for (int j = i; j <vector1.size() && k<size_of_chunk; ++k,++j){
                sub_vector1.push_back(vector1[j]);
                sub_vector2.push_back(vector2[j]);
            }
            vectors1.push_back(sub_vector1);
            vectors2.push_back(sub_vector2);
        }
        std::thread threads[number_of_chunks];
        for(int i = 0; i< number_of_chunks; ++i){
            threads[i] = std::thread  (partialDotProduct,std::ref(vectors1[i]), std::ref(vectors2[i]), std::ref(res));
        }
        for(int i = 0; i< number_of_chunks; ++i){
            threads[i].join();
        }
        std::cout<<res;
    }

    return 0;
}