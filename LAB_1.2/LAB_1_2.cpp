//LAB - 1.2

#include <iostream>
#include <string>

class Entity {
protected:
    std::string name;
    int health;

public:
    Entity(const std::string& n, int h) : name(n), health(h) {}

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }

    virtual ~Entity() {}
};

class Player : public Entity {
private:
    int experience;

public:
    Player(const std::string& n, int h, int exp)
        : Entity(n, h), experience(exp) {
    }

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Experience: " << experience << std::endl;
    }
};

class Enemy : public Entity {
private:
    std::string type;

public:
    Enemy(const std::string& n, int h, const std::string& t)
        : Entity(n, h), type(t) {
    }

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Type: " << type << std::endl;
    }
};

// Дополнительный класс Boss — наследник Enemy
class Boss : public Enemy {
private:
    std::string specialAbility;

public:
    Boss(const std::string& n, int h, const std::string& t, const std::string& ability)
        : Enemy(n, h, t), specialAbility(ability) {
    }

    void displayInfo() const override {
        Enemy::displayInfo();
        std::cout << "Special Ability: " << specialAbility << std::endl;
    }
};

int main() {
    Player hero("Hero", 100, 50);
    Enemy goblin("Goblin", 60, "Goblin");
    Boss dragon("Dragon", 200, "Dragon", "Fire Breath");

    hero.displayInfo();
    goblin.displayInfo();
    dragon.displayInfo();

    return 0;
}