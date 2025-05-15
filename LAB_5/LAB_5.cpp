//LAB - 5

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

template<typename Type>
class SimpleQueue {
private:
    std::vector<Type> data;

public:
    void enqueue(const Type& value) {
        data.push_back(value);
    }

    Type dequeue() {
        if (data.empty()) {
            throw std::runtime_error("Attempted to dequeue from empty queue.");
        }

        Type result = data.front();
        data.erase(data.begin());
        return result;
    }

    bool empty() const {
        return data.empty();
    }

    void print() const {
        std::cout << "Queue contents: ";
        for (const auto& item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    SimpleQueue<std::string> fruits;
    fruits.enqueue("Apple");
    fruits.enqueue("Banana");
    fruits.enqueue("Cherry");

    std::cout << "Fruits Queue:" << std::endl;
    fruits.print();

    std::cout << "Removed: " << fruits.dequeue() << std::endl;
    fruits.print();

    SimpleQueue<int> numbers;
    numbers.enqueue(100);
    numbers.enqueue(200);
    numbers.enqueue(300);

    std::cout << "\nNumbers Queue:" << std::endl;
    numbers.print();

    std::cout << "Removed: " << numbers.dequeue() << std::endl;
    numbers.print();

    return 0;
}
