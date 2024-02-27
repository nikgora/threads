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
void computeX(std::vector<double>& x, double& res) {
    double sum = 0;
    for (double xi : x) {
        sum += std::pow(xi * std::sin(xi * xi),-3);
    }
    mtx.lock();
    res+=sum;
    mtx.unlock();
}
void computeY(std::vector<double>& x, double& res) {
    double sum = 0;
    for (double xi : x) {
        sum += std::pow(xi * std::cos(xi * xi),-3);
    }
    mtx.lock();
    res+=sum;
    mtx.unlock();
}


int main()
{
    std::cout<<"hardware_concurrency: "<<std::thread::hardware_concurrency()<<"\n";
    std::cout<<"Main thread id: "<<std::this_thread::get_id()<<"\n";
//    std::cout<<"TASK 1: \n";
//    //TASK1
//    std::thread thread11(run);
//    std::thread thread12(run);
//    std::thread thread13(run);
//    std::thread thread14(run);
//    std::thread thread15(run);
//    thread11.join();
//    std::this_thread::sleep_for(std::chrono::milliseconds (150));
//    thread12.join();
//    std::this_thread::sleep_for(std::chrono::milliseconds (150));
//    thread13.join();
//    std::this_thread::sleep_for(std::chrono::milliseconds (150));
//    thread14.join();
//    thread15.join();
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//    std::thread th21 (run);
//    std::thread th22 (run);
//    std::thread th23 (run);
//    th21.join();
//    th22.detach ();
//    for (int i=0; i<10; i++) {
//        mtx.lock();
//        std::cout << "ID thread " <<std::this_thread::get_id() << " WORK MAIN" << i << std::endl;
//        mtx.unlock();
//        std::this_thread :: sleep_for (std::chrono::milliseconds(500));
//    }
//    th23.join();
    std::cout<<"TASK 3\n";
    std::vector<double> x(10);
    std::vector<double> y(15);
    int dmax=25, dmin=0;
    std::srand(std::time(nullptr));
    for (int i =0 ;i< x.size();i++) {
        int rand = std::rand()%((std::abs(dmax-dmin))*100+1);
        x[i]=(rand/100.0+dmin);
//        std::cout<<x[i]<<" ";
    }
//    std::cout<<"\n";
    dmax=10;
    dmin=-10;
    for (int i =0 ;i< y.size();i++) {
        int rand = std::rand()%((std::abs(dmax-dmin))*100+1);
        y[i]=(rand/100.0+dmin);
//        std::cout<<y[i]<<" ";
    }
//    std::cout<<"\n";
    int size_of_chunk_x = 3;
    int number_of_chunks_x = x.size()/size_of_chunk_x+bool(x.size()%size_of_chunk_x);
    std::vector<std::vector<double>> vectors_x;
    std::vector<std::vector<double>> vectors_y;

    for (int i = 0; i< x.size(); i+=size_of_chunk_x){
        std::vector<double>sub_vector;
        int k = 0;
        for (int j = i; j <x.size() && k<size_of_chunk_x; ++k,++j){
            sub_vector.push_back(x[j]);
        }
        vectors_x.push_back(sub_vector);
    }
    int size_of_chunk_y = 3;
    int number_of_chunks_y = y.size()/size_of_chunk_y+bool(y.size()%size_of_chunk_y);

    for (int i = 0; i< y.size(); i+=size_of_chunk_y){
        std::vector<double>sub_vector;
        int k = 0;
        for (int j = i; j <y.size() && k<size_of_chunk_y; ++k,++j){
            sub_vector.push_back(y[j]);
        }
        vectors_y.push_back(sub_vector);
    }
    std::thread threads_x[number_of_chunks_x];
    std::thread threads_y[number_of_chunks_y];
    double res_y=0;
    double res_x=0;
    for(int i = 0; i< number_of_chunks_x; ++i){
        threads_x[i] = std::thread  (computeX,std::ref(vectors_x[i]), std::ref(res_x));
    }
    for(int i = 0; i< number_of_chunks_y; ++i){
        threads_y[i] = std::thread  (computeY,std::ref(vectors_y[i]), std::ref(res_y));
    }
    for(int i = 0; i< number_of_chunks_x; ++i){
        threads_x[i].join();
    }
    for(int i = 0; i< number_of_chunks_y; ++i){
        threads_y[i].join();
    }
    std::cout<<res_x<<"\n";
    std::cout<<res_y<<"\n";
    return 0;
}