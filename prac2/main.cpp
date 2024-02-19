#include <thread>
#include <iostream>
#include <fstream>

std::ofstream cout("out.txt");

void run () {
    for (int i=0; i<10; i++)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(2000)); //затримка на 2 секунди
        cout << "ID thread " <<std::this_thread::get_id() << " RUN WORK " << i << "\n";
        std::this_thread:: sleep_for (std::chrono::milliseconds(3000)); //затримка на 3 секунди
    }
}

int main()
{
    cout<<"Main thread id: "<<std::this_thread::get_id()<<"\n";
    //TASK1
    std::thread thread1(run);
    thread1.join();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout<<"\n\n\n";
    std::thread th (run);
    th.join();
    th.detach ();
    for (int i=0; i<10; i++) {
        cout << "ID thread " <<std::this_thread::get_id() << " WORK MAIN" << i << std::endl;
        std::this_thread :: sleep_for (std::chrono::milliseconds(500));
    }
    th.join();
    return 0;
}