//LAB - 1.1

#include <iostream>
#include <string>

class Character {
private:
    // имя персонажа
    std::string name;  
    int health;        
    int attack;    
    int defense;       
    const int maxHealth = 100;

public:
    // Конструктор
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    // Геттер здоровья
    int getHealth() const {
        return health;
    }

    // Вывод информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name
            << ", HP: " << health
            << ", Attack: " << attack
            << ", Defense: " << defense
            << std::endl;
    }

    // Атака другого персонажа
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.takeDamage(damage);
            std::cout << name << " attacks " << enemy.name
                << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name
                << ", but it has no effect!" << std::endl;
        }
    }

    // Лечение
    void heal(int amount) {
        if (amount > 0) {
            health += amount;
            if (health > maxHealth) {
                health = maxHealth;
            }
            std::cout << name << " heals for " << amount << " HP!" << std::endl;
        }
    }

    // Получение урона
    void takeDamage(int amount) {
        if (amount > 0) {
            health -= amount;
            if (health < 0) {
                health = 0;
            }
            std::cout << name << " takes " << amount << " damage!" << std::endl;
        }
    }
};

int main() {
    // Создание персонажей
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // Вывод информации
    hero.displayInfo();
    monster.displayInfo();

    // Атака
    hero.attackEnemy(monster);
    monster.displayInfo();

    // Монстр лечится
    monster.heal(30);
    monster.displayInfo();

    // Герой дамажит
    monster.takeDamage(60);
    monster.displayInfo();

    return 0;
}
