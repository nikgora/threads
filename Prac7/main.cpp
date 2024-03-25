#include <iostream>
#include <mutex>
#include <thread>

class BankAccount {
private:
    mutable std::mutex m_mutex;
    double balance;

public:
    BankAccount(double initial_balance) : balance(initial_balance) {}

    void deposit(double amount) {
        std::this_thread::sleep_for(std::chrono::milliseconds(823));
        std::unique_lock<std::mutex> lock(m_mutex);
        balance += amount;
        lock.unlock();
    }


    bool withdraw(double amount) {
        std::this_thread::sleep_for(std::chrono::milliseconds(123));
        std::unique_lock<std::mutex> lock(m_mutex);
        if (amount <= balance) {
            balance -= amount;
            lock.unlock();
            return true;
        }
        lock.unlock();
        return false;
    }


    void transfer(BankAccount& to, double amount) {
        std::this_thread::sleep_for(std::chrono::milliseconds(523));
        std::unique_lock<std::mutex> lock_from(m_mutex, std::defer_lock);
        std::unique_lock<std::mutex> lock_to(to.m_mutex, std::defer_lock);
        std::lock(lock_from, lock_to);

        if (balance >= amount) {
            balance -= amount;
            to.balance += amount;
        }
    }

    double get_balance() const {
        std::unique_lock<std::mutex> lock(m_mutex);
        return balance;
    }

    friend std::ostream &operator<<(std::ostream &os, const BankAccount &account) {
        os << " balance: " << account.balance;
        return os;
    }
};

int main() {
    BankAccount bankAccount1(0);
    BankAccount bankAccount2(0);
    std::thread thread1 ([&](){bankAccount1.deposit(250);});
    std::thread thread2 ([&](){bankAccount1.transfer(bankAccount2,120);});
    std::thread thread3 ([&](){bankAccount1.transfer(bankAccount2,120);});
    std::thread thread4 ([&](){bankAccount1.deposit(120);});
    std::thread thread5 ([&](){bankAccount1.transfer(bankAccount2,100);});
    std::thread thread6 ([&](){bankAccount2.transfer(bankAccount1,0);});
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    std::cout<<"Account 1: "<<bankAccount1<<"\n";
    std::cout<<"Account 2: "<<bankAccount2<<"\n";
    return 0;
}
