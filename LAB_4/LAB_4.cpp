//LAB - 4

#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Класс для управления списком предметов
class Bag {
private:
    // динамический массив предметов
    std::unique_ptr<std::string[]> _slots;  
    // текущая вместимость
    size_t _limit;                          
    // количество добавленных предметов
    size_t _count;                          

    // Увеличение размера массива, если не хватает места
    void expand() {
        size_t newLimit = (_limit == 0) ? 2 : _limit * 2;
        auto temp = std::make_unique<std::string[]>(newLimit);

        for (size_t i = 0; i < _count; ++i) {
            temp[i] = std::move(_slots[i]);
        }

        _slots = std::move(temp);
        _limit = newLimit;
    }

public:
    Bag() : _slots(nullptr), _limit(0), _count(0) {}

    // Добавление нового предмета
    void insert(const std::string& thing) {
        if (_count >= _limit) {
            expand();
        }
        _slots[_count++] = thing;
    }

    // Отображение содержимого инвентаря
    void printContents() const {
        std::cout << "Bag Contents [" << _count << "/" << _limit << "]\n";
        for (size_t i = 0; i < _count; ++i) {
            std::cout << " * " << _slots[i] << '\n';
        }
    }
};

int main() {
    Bag playerBag;

    playerBag.insert("Iron Sword");
    playerBag.insert("Healing Potion");
    playerBag.insert("Wooden Shield");
    playerBag.insert("Elixir of Mana");

    playerBag.printContents();

    return 0;
}



