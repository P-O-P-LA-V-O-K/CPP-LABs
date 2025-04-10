//LAB - 1.3

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Entity {
protected:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getDefense() const { return defense; }
    int getAttack() const { return attack; }

    void takeDamage(int dmg) {
        health -= dmg;
        if (health < 0) health = 0;
    }

    virtual void attackTarget(Entity& target) {
        int damage = attack - target.getDefense();
        if (damage > 0) {
            target.takeDamage(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attackTarget(Entity& target) override {
        int damage = attack - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 20) { // крит
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.takeDamage(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Character: " << name << ", HP: " << health
            << ", Atk: " << attack << ", Def: " << defense << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attackTarget(Entity& target) override {
        int damage = attack - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 30) { // яд
                damage += 5;
                std::cout << "Poisonous attack! ";
            }
            target.takeDamage(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
            << ", Atk: " << attack << ", Def: " << defense << std::endl;
    }
};

class Boss : public Monster {
public:
    Boss(const std::string& n, int h, int a, int d)
        : Monster(n, h, a, d) {
    }

    void attackTarget(Entity& target) override {
        int damage = attack - target.getDefense();
        if (damage > 0) {
            std::cout << "Flaming strike! ";
            damage += 10;
            target.takeDamage(damage);
            std::cout << name << " hits " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " tries to attack " << target.getName() << ", but it's useless!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Boss: " << name << ", HP: " << health
            << ", Atk: " << attack << ", Def: " << defense
            << ", Ability: Flaming Strike\n";
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    Character hero("Knight", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss dragon("Dragon", 200, 30, 15);

    Entity* entities[] = { &hero, &goblin, &dragon };

    for (Entity* e : entities) {
        e->displayInfo();
    }

    std::cout << "\nBattle starts!\n\n";

    hero.attackTarget(goblin);
    goblin.attackTarget(hero);
    dragon.attackTarget(hero);

    return 0;
}
