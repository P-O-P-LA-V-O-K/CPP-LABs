//LAB_9
//Подключение библиотек
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

#define NOMINMAX
#include <windows.h>
#include <limits>

//Объявление классов
//Logger, записывающий логи игры в файл
template <typename T>
class Logger {
private:
    std::ofstream logFile;
public:
    //Конструктор
    Logger(const std::string& filename) {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            throw std::runtime_error("Ошибка открытия Log файла!");
        }
    }

    //Деструктор
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const T& message) {
        logFile << message << std::endl;
    }
};

//Персонаж игрока, который имеет атрибуты(хп, атака и т.д.) и методы(атака, лечение и т.д.)
class Character {
private: 
    std::string name;
    int health;
    int attack;
    int defense;
    int level;
    int experience;

public:
    //Инициализация параметров
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d), level(1), experience(0) { }

    //Атака врага
    void attackEnemy(Character& enemy, Logger<std::string>& logger) {
        int damage = attack - enemy.defense;

        if (damage > 0) {
            enemy.takeDamage(damage, logger);
            logger.log(name + " атаковал " + enemy.name + ", нанеся " + std::to_string(damage) + "урона!");

            std::cout << name << " атаковал " << enemy.name << ", нанеся " << damage << " урона!" << std::endl;
        }
        else {
            logger.log(name + " атаковал " + enemy.name + ", но промахнулся!");

            std::cout << name << " атаковал " << enemy.name << ", но промахнулся!" << std::endl;
        }
    }

    //Получение урона
    void takeDamage(int damage, Logger<std::string>& logger) {
        health -= damage;
        if (health < 0) {
            health = 0;
            logger.log(name + " был убит!");
            throw std::runtime_error(name + " - здоровье упало ниже 0!");
        }
    }

    //Восстановление здоровья
    void heal(int amount, Logger<std::string>& logger) {
        health += amount;

        if (health > 100) {
            health = 100;
        }

        logger.log(name + " восстанавливает " + std::to_string(amount) + " здоровья!");
        std::cout << name << " восстанавливает" << amount << " здоровья!" << std::endl;
    }

    //Получение опыта
    void gainExperience(int exp, Logger<std::string>& logger) {
        experience += exp;
        if (experience >= 100) {
            level++;
            //Уточнить:
            experience = 0;
        }

        logger.log(name + " повысил уровень до " + std::to_string(level) + "!");
        std::cout << name << " повысил уровень до " << level << "!" << std::endl;
    }

    //Вывод информации
    void displayInfo() const {
        std::cout << "Имя: " << name << ", Здоровье: " << health
            << ", Атака: " << attack << ", Защита: " << defense
            << ", Уровень: " << level << ", Опыт: " << experience << std::endl;
    }

    // Геттеры
    std::string getName() const {
        return name;
    }

    int getHealth() const {
        return health;
    }

    int getAttack() const {
        return attack;
    }

    int getDefense() const {
        return defense;
    }

    int getLevel() const {
        return level;
    }

    int getExperience() const {
        return experience;
    }

    void setHealth(int h) {
        if (h < 0) {
            throw std::invalid_argument("Ошибка! Здоровье не может быть отрицательным!");
        }
    }
};

//Базовый класс для монстров
class Monster {
protected:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    virtual ~Monster() {}

    //Метод атаки
    virtual void attackEnemy(Character& enemy, Logger<std::string>& logger) {
        int damage = attack - enemy.getDefense();
        if (damage > 0) {
            enemy.setHealth(enemy.getHealth() - damage);

            logger.log(name + " атаковал " + enemy.getName() + ", нанеся " + std::to_string(damage) + " урона! ");
            std::cout << name << " атаковал " << enemy.getName() << ", нанеся " << damage << " урона! " << std::endl;
        }
    }

    //Вывод информации
    virtual void displayInfo() const {
        std::cout << "Монстр: " << name << ", HP: " << health
            << ", Атака: " << attack << ", Защита: " << defense << std::endl;
    }

    //Геттеры
    std::string getName() {
        return name;
    }

    int getHealth() {
        return health;
    }

    int getAttack() {
        return attack;
    }

    int getDefense() {
        return defense;
    }

    //Сеттер
    void setHealth(int h) {
        if (h < 0) {
            throw std::invalid_argument("Ошибка! Здоровье не может быть отрицательным!");
        }

        health = h;
    }
};

//Тролль
class Trol : public Monster {
public:
    //name, health, attack, defense
    Trol() : Monster("Тролль", 30, 5, 10) {}
};

//Дракон
class Dragon : public Monster {
public:
    Dragon() : Monster("Дракон", 100, 25, 15) {}
};

//Скелет
class Skeleton : public Monster {
public:
    Skeleton() : Monster("Скелет", 20, 10, 10) {}
};

//Инвентарь
class Inventory {
private:
    std::vector<std::string> items;

public:
    //Добавление предмета в инвентарь
    void addItem(const std::string& item) {
        items.push_back(item);
        std::cout << item << " добавлен в инвентарь." << std::endl;
    }

    //Удаление предмета из инвентаря
    void removeItem(const std::string& item) {
        for (auto thing = items.begin(); thing != items.end(); ++thing) {
            if (*thing == item) {
                items.erase(thing);

                std::cout << item << " удалён из инвентаря." << std::endl;
                return;
            }
        }
        std::cout << item << " не найден в инвентаре." << std::endl;
    }

    //Вывод содержимого инвентаря
    void displayInventory() const {
        std::cout << "Инвентарь:" << std::endl;
        for (const auto& item : items) {
            std::cout << "- " << item << std::endl;
        }
    }
};

//Игра
class Game {
private:
    Character player;
    Inventory inventory;
    Logger<std::string> logger;

public:
    Game(const std::string& playerName)
        : player(playerName, 100, 20, 10), logger("game_log.txt") { }

    //Старт игры
    void start() {
        std::cout << "RPG Игра запущена!" << std::endl;
        
        //Вывод информации по игроку и инвентарю
        player.displayInfo();
        inventory.displayInventory();
    }

    //Битва
    void battle(Monster& monster) {
        std::cout << "На пути вам встречается " << monster.getName() << "!" << std::endl;
        monster.displayInfo();

        while (player.getHealth() > 0 && monster.getHealth() > 0) {
            try {
                int damage = player.getAttack() - monster.getDefense();

                if (damage > 0) {
                    monster.setHealth(monster.getHealth() - damage);
                    
                    logger.log(player.getName() + " атаковал " + monster.getName() + " и нанёс " +
                        std::to_string(damage) + " урона!");

                    std::cout << player.getName() << " атаковал " << monster.getName() << " и нанёс " 
                        << damage << " урона!" << std::endl;
                }
                else {
                    logger.log(player.getName() + " атакует " + monster.getName() + ", но промахивается!");
                    std::cout << player.getName() << " атакует " << monster.getName() << ", но промахивается" << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
                break;
            }

            //Убийство монстра
            if (monster.getHealth() <= 0) {
                std::cout << monster.getName() << " убит!" << std::endl;
                player.gainExperience(50, logger);
                break;
            }

            try {
                monster.attackEnemy(player, logger);
            }
            catch(const std::exception& e){
                std::cout << e.what() << std::endl;
                break;
            }

            //Смерть игрока
            if (player.getHealth() <= 0) {
                std::cout << player.getName() << " помер!" << std::endl;
                break;
            }

        }
    }

    //Сохранение игры
    void saveGame(const std::string& filename) {
        std::ofstream saveFile(filename);

        if (!saveFile) {
            std::cout << "Ошибка! Не удалось открыть файл сохранения." << std::endl;
            return;
        }

        saveFile << player.getName() << std::endl;
        saveFile << player.getHealth() << std::endl;
        saveFile << player.getAttack() << std::endl;
        saveFile << player.getDefense() << std::endl;
        saveFile << player.getLevel() << std::endl;
        saveFile << player.getExperience() << std::endl;
        std::cout << "Игра сохранена в " << filename << std::endl;
    }

    //Загрузка игры
    void loadGame(const std::string& filename) {
        std::ifstream loadFile(filename);
        std::string name;
        int health, attack, defense, level, experience;

        if (!loadFile) {
            std::cout << "Ошибка! Не удалось открыть файл сохранения." << std::endl;
            return;
        }

        loadFile >> name >> health >> attack >> defense >> level >> experience;
        
        player = Character(name, health, attack, defense);

        try {
            player.setHealth(health);
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка при установке здоровья: " << e.what() << std::endl;
        }

        std::cout << "Загружен уровень игрока: " << level << ", опыт: " << experience << std::endl;
        std::cout << "Игра загружена из " << filename << std::endl;
    }

    //Остальные методы
    void showInventory() const {
        inventory.displayInventory();
    }

    void addItemToInventory(const std::string& item) {
        inventory.addItem(item);
    }

    void removeItemFromInventory(const std::string& item) {
        inventory.removeItem(item);
    }

    std::string getPlayerName() const {
        return player.getName();
    }

    int getPlayerHealth() const {
        return player.getHealth();
    }
};

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "Russian");

    try {
        Game game("Бедолага");
        game.start();

        bool running = true;
        while (running) {
            std::cout << "\n----------------\n";
            std::cout << "\nВыберите действие:\n";
            std::cout << "1. Сразиться с троллем\n";
            std::cout << "2. Сразиться с драконом\n";
            std::cout << "3. Сразиться со скелетом\n";
            std::cout << "\n";
            std::cout << "4. Показать инвентарь\n";
            std::cout << "5. Добавить предмет в инвентарь\n";
            std::cout << "6. Удалить предмет из инвентаря\n";
            std::cout << "\n";
            std::cout << "7. Сохранить игру\n";
            std::cout << "8. Загрузить игру\n";
            std::cout << "9. Выйти\n";
            std::cout << "\n";
            std::cout << "Введите выбор: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                //Сразиться с гоблином
                case 1: {
                    Trol trol;
                    game.battle(trol);
                    break;
                }
                //Сразиться с драконом
                case 2: {
                    Dragon dragon;
                    game.battle(dragon);
                    break;
                }
                //Сразиться со скелетом
                case 3: {
                    Skeleton skeleton;
                    game.battle(skeleton);
                    break;
                }
                //Показать инвентарь
                case 4: {
                    game.showInventory();
                    break;
                }
                //Добавить предмет в инвентарь
                case 5: {
                    std::cout << "Введите название предмета: ";
                    std::string item;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, item);
                    game.addItemToInventory(item);
                    break;
                }
                //Удалить предмет из инвентаря
                case 6: {
                    std::cout << "Введите название предмета: ";
                    std::string item;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, item);
                    game.removeItemFromInventory(item);
                    break;
                }
                //Сохранить игру
                case 7: {
                    std::cout << "Введите имя файла для сохранения игры: ";
                    std::string filename;
                    std::cin >> filename;
                    game.saveGame(filename);
                    break;
                }
                //Загрузить игры
                case 8: {
                    std::cout << "Введите имя файла для загрузки игры: ";
                    std::string filename;
                    std::cin >> filename;
                    game.loadGame(filename);
                    break;
                }
                //Выйти из игры
                case 9: {
                    running = false;
                    std::cout << "Выход из игры..." << std::endl;
                    break;
                }
                default: {
                    std::cout << "Неверный выбор. Пожалуйста, попробуйте снова." << std::endl;
                    break;
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка! " << e.what() << std::endl;
    }

    return 0;
    
}
