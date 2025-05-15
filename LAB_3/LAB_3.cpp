//LAB - 3

#include <iostream>
#include <string>

// Класс, описывающий игрового персонажа
class Unit {
private:
    std::string _id;
    int _hp;
    int _atk;
    int _def;

public:
    Unit(const std::string& id, int hp, int atk, int def)
        : _id(id), _hp(hp), _atk(atk), _def(def) {}

    // Сравнение по имени и здоровью
    bool operator==(const Unit& other) const {
        return _id == other._id && _hp == other._hp;
    }

    // Вывод информации о персонаже
    friend std::ostream& operator<<(std::ostream& out, const Unit& u) {
        out << "Unit: " << u._id 
            << " | HP: " << u._hp 
            << " | ATK: " << u._atk 
            << " | DEF: " << u._def;
        return out;
    }
};

// Класс оружия
class Armament {
private:
    std::string _label;
    int _power;

public:
    Armament(const std::string& label, int power)
        : _label(label), _power(power) {}

    // Объединение двух оружий
    Armament operator+(const Armament& rhs) const {
        std::string mergedName = _label + " & " + rhs._label;
        int totalPower = _power + rhs._power;
        return Armament(mergedName, totalPower);
    }

    // Сравнение по урону
    bool operator>(const Armament& rhs) const {
        return _power > rhs._power;
    }

    // Вывод информации об оружии
    friend std::ostream& operator<<(std::ostream& out, const Armament& a) {
        out << "Weapon: " << a._label 
            << " | Damage: " << a._power;
        return out;
    }
};

int main() {
    Unit knight("Knight", 100, 30, 20);
    Unit knightCopy("Knight", 100, 30, 20);
    Unit rogue("Rogue", 80, 40, 10);

    if (knight == knightCopy) {
        std::cout << "The knight and his copy are identical.\n";
    }

    if (!(knight == rogue)) {
        std::cout << "The knight and the rogue are distinct.\n";
    }

    std::cout << knight << "\n\n";

    Armament axe("Axe", 35);
    Armament dagger("Dagger", 20);

    Armament hybrid = axe + dagger;

    std::cout << hybrid << "\n\n";

    if (axe > dagger) {
        std::cout << "Axe is mightier than Dagger.\n";
    } else {
        std::cout << "Dagger matches or exceeds Axe.\n";
    }

    return 0;
}
