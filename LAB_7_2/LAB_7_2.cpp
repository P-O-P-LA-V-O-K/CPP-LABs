//LAB - 7_2

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <random>
#include <string>

using namespace std;

// Класс для героя
class Hero {
private:
    string heroName;
    int healthPoints;
    int attackPower;
    int defensePower;
    mutable mutex heroMutex; // mutex для синхронизации доступа

public:
    Hero(const string& name, int health, int attack, int defense)
        : heroName(name), healthPoints(health), attackPower(attack), defensePower(defense) {}

    // Получение урона с учетом защиты
    void receiveDamage(int damage) {
        lock_guard<mutex> lock(heroMutex);
        int effectiveDamage = max(1, damage - defensePower);
        healthPoints -= effectiveDamage;
        if (healthPoints < 0) healthPoints = 0;
    }

    // Проверка, жив ли герой
    bool isAlive() const {
        lock_guard<mutex> lock(heroMutex);
        return healthPoints > 0;
    }

    // Получить атаку
    int getAttack() const {
        lock_guard<mutex> lock(heroMutex);
        return attackPower;
    }

    // Вывод информации о герое
    void printStatus() const {
        lock_guard<mutex> lock(heroMutex);
        cout << heroName << " - HP: " << healthPoints
             << ", ATK: " << attackPower
             << ", DEF: " << defensePower << endl;
    }

    // Получить имя героя
    string getName() const {
        return heroName;
    }

    // Получить здоровье
    int getHealth() const {
        lock_guard<mutex> lock(heroMutex);
        return healthPoints;
    }
};

// Класс для монстров
class Monster {
private:
    string monsterName;
    int healthPoints;
    int attackPower;
    int defensePower;
    mutable mutex monsterMutex;

public:
    Monster(const string& name, int health, int attack, int defense)
        : monsterName(name), healthPoints(health), attackPower(attack), defensePower(defense) {}

    // Получение урона
    void receiveDamage(int damage) {
        lock_guard<mutex> lock(monsterMutex);
        int effectiveDamage = max(1, damage - defensePower);
        healthPoints -= effectiveDamage;
        if (healthPoints < 0) healthPoints = 0;
    }

    // Проверка на жизнь
    bool isAlive() const {
        lock_guard<mutex> lock(monsterMutex);
        return healthPoints > 0;
    }

    // Получить атаку
    int getAttack() const {
        lock_guard<mutex> lock(monsterMutex);
        return attackPower;
    }

    // Вывод информации
    void printStatus() const {
        lock_guard<mutex> lock(monsterMutex);
        cout << monsterName << " - HP: " << healthPoints
             << ", ATK: " << attackPower
             << ", DEF: " << defensePower << endl;
    }

    // Получить имя
    string getName() const {
        return monsterName;
    }

    // Получить здоровье
    int getHealth() const {
        lock_guard<mutex> lock(monsterMutex);
        return healthPoints;
    }
};

// Глобальный список монстров
vector<Monster> monsterList;
mutex monsterListMutex;

// Поток для генерации монстров
void spawnMonsters() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> hpDist(30, 100);
    uniform_int_distribution<> atkDist(5, 20);
    uniform_int_distribution<> defDist(1, 10);
    vector<string> monsterNames = {"Goblin", "Orc", "Troll", "Skeleton", "Zombie", "Dragon"};

    while (true) {
        this_thread::sleep_for(chrono::seconds(3)); // Каждые 3 секунды создается новый монстр

        uniform_int_distribution<> nameIndex(0, monsterNames.size() - 1);
        string name = monsterNames[nameIndex(gen)];
        int hp = hpDist(gen);
        int atk = atkDist(gen);
        int def = defDist(gen);

        // Добавляем монстра в список
        lock_guard<mutex> lock(monsterListMutex);
        monsterList.emplace_back(name, hp, atk, def);
        cout << "New monster appeared: " << name << " (HP: " << hp
             << ", ATK: " << atk << ", DEF: " << def << ")\n";
    }
}

// Бой между героем и монстром
void fight(Hero& hero, Monster& monster) {
    while (hero.isAlive() && monster.isAlive()) {
        // Герой атакует
        monster.receiveDamage(hero.getAttack());
        cout << hero.getName() << " attacks " << monster.getName() << "!\n";

        // Проверка, жив ли монстр
        if (!monster.isAlive()) {
            cout << monster.getName() << " has been slain!\n";
            break;
        }

        // Монстр атакует
        hero.receiveDamage(monster.getAttack());
        cout << monster.getName() << " attacks " << hero.getName() << "!\n";

        // Вывод состояния
        hero.printStatus();
        monster.printStatus();
        cout << "----------------------\n";

        // Задержка между ударами
        this_thread::sleep_for(chrono::seconds(1));
    }

    if (hero.isAlive()) {
        cout << hero.getName() << " won the fight!\n";
    } else {
        cout << hero.getName() << " was defeated by " << monster.getName() << "!\n";
    }
}

int main() {
    // Создаем главного героя
    Hero mainHero("Knight", 100, 15, 5);
    cout << "Hero created:\n";
    mainHero.printStatus();
    cout << endl;

    // Запускаем поток для создания монстров
    thread monsterSpawner(spawnMonsters);
    monsterSpawner.detach();

    // Главный игровой цикл
    while (mainHero.isAlive()) {
        this_thread::sleep_for(chrono::seconds(1));

        monsterListMutex.lock();
        if (!monsterList.empty()) {
            // Берем первого монстра
            Monster& enemy = monsterList.front();
            monsterListMutex.unlock();

            cout << "\n=== BATTLE BEGIN ===\n";
            cout << mainHero.getName() << " vs " << enemy.getName() << "\n";
            mainHero.printStatus();
            enemy.printStatus();
            cout << "----------------------\n";

            // Запускаем бой
            thread battleThread(fight, ref(mainHero), ref(enemy));
            battleThread.join();

            // Удаляем мертвого монстра
            monsterListMutex.lock();
            if (!monsterList.empty() && !monsterList.front().isAlive()) {
                monsterList.erase(monsterList.begin());
            }
            monsterListMutex.unlock();

        } else {
            monsterListMutex.unlock();
            cout << "Waiting for monsters...\n";
        }
    }

    cout << "\nGame Over!\n";
    return 0;
}
