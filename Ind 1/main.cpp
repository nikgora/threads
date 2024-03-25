/*
 * У вас є банківський рахунок та кілька карток для членів сім'ї.
Максимальна сума на рахунку не повинна перевищувати певного
ліміту. Батьки поповнюють рахунок на довільну суму, а діти
відповідно витрачають гроші. Організувати синхронізовану взаємодію
всіх членів сім'ї таким чином, щоб у будь-який момент часу діти могли
зробити покупку, а батьки поповнити рахунок.
 */

#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
std::mutex mtx;
class Child{
protected:
    static std::atomic<double> amount;
    const static double max_amount;
public:
    bool makePurchase (double price){
        std::this_thread::sleep_for(std::chrono::milliseconds(430));
        if(amount.load() < price) {
            mtx.lock();
            std::cout<<"Thread: "<<std::this_thread::get_id()<<". Operation make purchase declined\n"<<"Money now: "<<amount.load()<<"\nTry to withdraw: "<<price<<"\nBalance if withdraw: "<<amount.load() - price<<"\n\n";
            mtx.unlock();
            return false;
        }
        amount.store(amount.load() - price);
        return true;
    }


    friend std::ostream &operator<<(std::ostream &os, const Child &c){
        mtx.lock();
        os << "amount: " << c.amount<<"\n";
        mtx.unlock();
        return os;
    };
};
class  Parent:Child{
public:
    bool addMoney(double money){
        std::this_thread::sleep_for(std::chrono::milliseconds(830));
        if(amount.load()+money > max_amount) {
            mtx.lock();
            std::cout<<"Thread: "<<std::this_thread::get_id()<<". Operation add money declined\n"<<"Money now: "<<amount.load()<<"\nTry to add: "<<money<<"\nMax amount: "<<max_amount<<"\nBalance if add: "<<amount.load()+money<<"\n\n";
            mtx.unlock();
            return false;
        }
        amount.store(amount.load()+money);
        return true;
    }

    friend std::ostream &operator<<(std::ostream &os, const Parent &parent) {
        mtx.lock();
        os << "amount: " << parent.amount<<"\n";
        mtx.unlock();
        return os;
    }
};

std::atomic<double> Child::amount = 0;
const double Child::max_amount = 120;


int main() {
    Child d1;
    Parent d2;
    Parent d3;
    bool succesfull;
    std::thread t1 ([&](){succesfull = d2.addMoney(113);});
    std::this_thread::sleep_for(std::chrono::milliseconds (1000));
    std::thread t2 ([&](){succesfull = d1.makePurchase(11);});
    std::thread t3 ([&](){succesfull = d2.addMoney(1);});
    std::thread t4 ([&](){succesfull = d3.addMoney(128);});
    std::thread t5 ([&](){succesfull = d1.makePurchase(102);});
    std::thread t6 ([&](){succesfull = d1.makePurchase(102);});



    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    std::cout<<d1;
    return 0;
}
