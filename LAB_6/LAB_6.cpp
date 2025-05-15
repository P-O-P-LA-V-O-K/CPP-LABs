//LAB - 6

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>
class MyQueue {
private:
    std::vector<T> storage;

public:
    void enqueue(const T& value) {
        storage.emplace_back(value);
    }

    T dequeue() {
        if (storage.empty()) {
            throw std::out_of_range("Can't dequeue from an empty queue.");
        }
        T frontItem = storage.front();
        storage.erase(storage.begin());
        return frontItem;
    }

    bool is_empty() const {
        return storage.empty();
    }

    void show() const {
        if (is_empty()) {
            std::cout << "[Queue is empty]\n";
            return;
        }

        std::cout << "[Queue] ";
        for (const auto& item : storage) {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }
};

void testStringQueue() {
    MyQueue<std::string> q;

    q.enqueue("Orange");
    q.enqueue("Pear");
    q.show();

    try {
        std::cout << "Removed: " << q.dequeue() << "\n";
        std::cout << "Removed: " << q.dequeue() << "\n";
        q.show();

        // This will throw
        std::cout << "Removed: " << q.dequeue() << "\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "[String Queue Error] " << ex.what() << "\n";
    }
}

void testIntQueue() {
    MyQueue<int> q;

    q.enqueue(42);
    q.show();

    try {
        std::cout << "Removed: " << q.dequeue() << "\n";
        q.show();

        // This will also throw
        std::cout << "Removed: " << q.dequeue() << "\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "[Int Queue Error] " << ex.what() << "\n";
    }
}

int main() {
    testStringQueue();
    std::cout << "\n";
    testIntQueue();

    return 0;
}

