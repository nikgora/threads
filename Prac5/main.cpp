#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
std::mutex mtx;
class Node {
public:
    int data;
    std::atomic<Node*> next;
    std::atomic<Node*> prev;

    Node(int value) : data(value), next(nullptr), prev(nullptr) {}


};

class DoublyLinkedList {
public:
    std::atomic<Node*> head;
    std::atomic<Node*> tail;


    DoublyLinkedList() : head(nullptr), tail(nullptr) {}


    void insertBefore(Node* nextNode, Node* newNode) {
        if (!newNode)
        {
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1900));
        if (!nextNode) {
            if (!head) {
                head = newNode;
                tail = newNode;
            }
            return;
        }

        newNode->prev.store(nextNode->prev.exchange(newNode));
        newNode->next.store(nextNode);

        if (newNode->prev.load())
            newNode->prev.load()->next.store(newNode);
        else
            head.store(newNode);

        nextNode->prev.store(newNode);
    }

    void insertAfter(Node* prevNode, Node* newNode) {
        if (!newNode)
        {
            return;
        }
        if (!prevNode) {
            if (!head) {
                head = newNode;
                tail = newNode;
            }
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2100));

        newNode->next.store(prevNode->next.exchange(newNode));
        newNode->prev.store(prevNode);

        if (newNode->next.load())
            newNode->next.load()->prev.store(newNode);
        else
            tail.store(newNode);

        prevNode->next.store(newNode);
    }

    void deleteNode(Node* delNode) {
        if (!delNode)
            return;
        std::this_thread::sleep_for(std::chrono::milliseconds(2200));
        if (delNode->prev)
            delNode->prev.load()->next.store(delNode->next);
        else

            head.store(delNode->next);

        if (delNode->next)
            delNode->next.load()->prev.store(delNode->prev);
        else
            tail.store(delNode->prev);
        delete delNode;
    }


    void printForward() const {
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        std::atomic<Node*> current;
        current.exchange(head);
        mtx.lock();
        std::cout<<"Forward print: ";
        mtx.unlock();
        while (current != nullptr) {
            mtx.lock();
            std::cout << current.load()->data << " ";
            mtx.unlock();
            current.exchange(current.load()->next);
        }
        mtx.lock();
        std::cout << "\n";
        mtx.unlock();
    }

    void printBackward() const {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::atomic<Node*> current;
        current.exchange(tail);
        mtx.lock();
        std::cout<<"Backward print: ";
        mtx.unlock();
        while (current != nullptr) {
            mtx.lock();
            std::cout << current.load()->data << " ";
            mtx.unlock();
            current.exchange(current.load()->prev);
        }
        mtx.lock();
        std::cout << "\n";
        mtx.unlock();
    }

    bool isSymmetric() const {
        std::atomic<Node*> forward;
        forward.exchange(head);
        std::atomic<Node*> backward;
        backward.exchange(tail);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));

        while (forward && backward) {
            if (forward.load()->data != backward.load()->data)
                return false;
            forward.exchange(forward.load()->next);
            backward.exchange(backward.load()->prev);
        }
        return true;
    }
};

int main() {
    DoublyLinkedList list;

    list.insertBefore(nullptr, new Node(2));
    list.insertBefore(list.head, new Node(3));
    list.insertAfter(list.tail, new Node(1));
    list.insertBefore(list.head, new Node(4));
    list.insertAfter(list.head.load()->next, new Node(5));
    std::thread t1([&]() {
        list.printForward();
    });

    std::thread t2([&]() {
        list.printBackward();
    });
    bool isSymmetric;
    std::thread t3([&]() {
        isSymmetric = list.isSymmetric();
    });

    std::thread t4([&]() {
        list.deleteNode(list.head.load()->next);
    });

    std::thread t5([&]() {
        list.printForward();
    });


    std::thread t6([&]() {
        list.insertBefore(list.head, new Node(2));
    });
    std::thread t7([&]() {
        list.insertBefore(list.head, new Node(5));
    });
    std::thread t8([&]() {
        list.insertBefore(list.head, new Node(4));
    });
    std::thread t9([&]() {
        list.insertBefore(list.head, new Node(1));
    });

    std::thread t10([&]() {
        list.printForward();
    });

    std::thread t11([&]() {
        list.printBackward();
    });
    std::thread t12([&]() {
        isSymmetric = list.isSymmetric();
    });
    t1.join();

    t2.join();


    t3.join();
    mtx.lock();
    if (isSymmetric)
        std::cout << "The list is symmetric.\n";
    else
        std::cout << "The list is not symmetric.\n";
    mtx.unlock();
    t4.join();

    t5.join();

    t6.join();


    t7.join();

    t8.join();

    t9.join();

    t10.join();

    t11.join();

    t12.join();


    mtx.lock();
    if (isSymmetric)
        std::cout << "The list is symmetric.\n";
    else
        std::cout << "The list is not symmetric.\n";
    mtx.unlock();
    return 0;
}
