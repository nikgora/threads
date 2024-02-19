#include <thread>
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include <cmath>

//std::ofstream std::cout("out.txt");
std::mutex mtx;
void run () {
    for (int i=0; i<10; i++)
    {
        std::this_thread::sleep_for (std::chrono::seconds(2)); //затримка на 2 секунди
        mtx.lock();
        std::cout << "ID thread " <<std::this_thread::get_id() << " RUN WORK " << i << "\n";
        mtx.unlock();
        std::this_thread:: sleep_for (std::chrono::seconds(3)); //затримка на 3 секунди
        mtx.lock();
        std::cout << "\n";
        mtx.unlock();
    }

}
void computeX(std::string name,std::vector<double>& x) {
    double sum = 0;
    for (double xi : x) {
        sum += std::pow(xi * std::sin(xi * xi),-3);
    }
    mtx.lock();
    std::cout << "Sum  "<<name<<": " << sum << std::endl;
    mtx.unlock();
}
void computeY(std::string name,std::vector<double>& x) {
    double sum = 0;
    for (double xi : x) {
        sum += std::pow(xi * std::cos(xi * xi),-3);
    }
    mtx.lock();
    std::cout << "Sum  "<<name<<": " << sum << std::endl;
    mtx.unlock();
}


int main()
{
    std::cout<<"hardware_concurrency: "<<std::thread::hardware_concurrency()<<"\n";
    std::cout<<"Main thread id: "<<std::this_thread::get_id()<<"\n";
    std::cout<<"TASK 1: \n";
    //TASK1
    std::thread thread11(run);
    std::thread thread12(run);
    std::thread thread13(run);
    std::thread thread14(run);
    std::thread thread15(run);
    thread11.join();
    std::this_thread::sleep_for(std::chrono::milliseconds (150));
    thread12.join();
    std::this_thread::sleep_for(std::chrono::milliseconds (150));
    thread13.join();
    std::this_thread::sleep_for(std::chrono::milliseconds (150));
    thread14.join();
    thread15.join();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread th21 (run);
    std::thread th22 (run);
    std::thread th23 (run);
    th21.join();
    th22.detach ();
    for (int i=0; i<10; i++) {
        mtx.lock();
        std::cout << "ID thread " <<std::this_thread::get_id() << " WORK MAIN" << i << std::endl;
        mtx.unlock();
        std::this_thread :: sleep_for (std::chrono::milliseconds(500));
    }
    th23.join();
    std::vector<double> x(10);
    std::vector<double> y(15);
    int dmax=25, dmin=0;
    std::srand(std::time(nullptr));
    for (int i =0 ;i< x.size();i++) {
        int rand = std::rand()%(std::abs(dmax-dmin)+1);
        x[i]=rand+dmin;
//        std ::cout<<x[i]<<" ";
    }
    std::cout<<"\n";
    dmax=10;
    dmin=-10;
    for (int i =0 ;i< y.size();i++) {
        int rand = std::rand()%(std::abs(dmax-dmin)+1);
        y[i]=rand+dmin;
//        std ::cout<<y[i]<<" ";
    }
    std::cout<<"\n";
    std::thread t31(computeX,"X", std::ref(x));
    std::thread t32(computeY, "Y", std::ref(y));


    t31.join();
    t32.join();
    return 0;
}