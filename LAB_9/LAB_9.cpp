//LAB - 9

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <ctime>

// Универсальный логгер для записи сообщений в файл
template<typename MessageType>
class LogWriter {
private:
    std::ofstream outputFile;

public:
    LogWriter(const std::string& filepath) {
        outputFile.open(filepath, std::ios::app);
        if (!outputFile) {
            throw std::runtime_error("Cannot open log file");
        }
    }

    ~LogWriter() {
        if (outputFile.is_open()) {
            outputFile.close();
        }
    }

    void writeLog(const MessageType& msg) {
        time_t currentTime = time(nullptr);
        char* timeStr = ctime(&currentTime);
        outputFile << timeStr << ": " << msg << "\n";
    }
};

// Ошибка игры
class GameError : public std::runtime_error {
public:
    explicit GameError(const std::string& message) : std::runtime_error(message) {}
};

// Абстрактный предмет для инвентаря
class InventoryItem {
protected:
    std::string itemName;
    std::string itemDesc;

public:
    InventoryItem(const std::string& name, const std::string& desc) 
        : itemName(name), itemDesc(desc) {}

    virtual ~InventoryItem() = default;

    virtual void activate() = 0;
    virtual std::string toDataString() const = 0;
    virtual void fromDataString(const std::string& data) = 0;

    std::string getName() const { return itemName; }
    std::string getDescription() const { return itemDesc; }
};

// Оружие с бонусом к атаке
class Arms : public InventoryItem {
private:
    int bonusAttack;

public:
    Arms(const std::string& name, const std::string& desc, int bonus)
        : InventoryItem(name, desc), bonusAttack(bonus) {}

    void activate() override {
        std::cout << "Equipped " << itemName << " (Attack +" << bonusAttack << ")\n";
    }

    int getBonus() const { return bonusAttack; }

    std::string toDataString() const override {
        return "Arms," + itemName + "," + itemDesc + "," + std::to_string(bonusAttack);
    }

    void fromDataString(const std::string& data) override {
        size_t p1 = data.find(',');
        size_t p2 = data.find(',', p1 + 1);
        size_t p3 = data.find(',', p2 + 1);

        if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos) {
            throw GameError("Incorrect weapon data format");
        }

        itemName = data.substr(p1 + 1, p2 - p1 - 1);
        itemDesc = data.substr(p2 + 1, p3 - p2 - 1);
        bonusAttack = std::stoi(data.substr(p3 + 1));
    }
};

// Зелье для восстановления здоровья
class HealingPotion : public InventoryItem {
private:
    int healPoints;

public:
    HealingPotion(const std::string& name, const std::string& desc, int heal)
        : InventoryItem(name, desc), healPoints(heal) {}

    void activate() override {
        std::cout << "Used " << itemName << " (Heals " << healPoints << " HP)\n";
    }

    int getHeal() const { return healPoints; }

    std::string toDataString() const override {
        return "HealingPotion," + itemName + "," + itemDesc + "," + std::to_string(healPoints);
    }

    void fromDataString(const std::string& data) override {
        size_t p1 = data.find(',');
        size_t p2 = data.find(',', p1 + 1);
        size_t p3 = data.find(',', p2 + 1);

        if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos) {
            throw GameError("Incorrect potion data format");
        }

        itemName = data.substr(p1 + 1, p2 - p1 - 1);
        itemDesc = data.substr(p2 + 1, p3 - p2 - 1);
        healPoints = std::stoi(data.substr(p3 + 1));
    }
};

// Инвентарь для хранения предметов
class Storage {
private:
    std::vector<std::unique_ptr<InventoryItem>> inventoryItems;

public:
    void addNewItem(std::unique_ptr<InventoryItem> newItem) {
        inventoryItems.push_back(std::move(newItem));
    }

    void deleteItem(const std::string& name) {
        for (auto it = inventoryItems.begin(); it != inventoryItems.end(); ++it) {
            if ((*it)->getName() == name) {
                inventoryItems.erase(it);
                return;
            }
        }
        throw GameError("Item not found in storage");
    }

    InventoryItem* findItem(const std::string& name) {
        for (auto& item : inventoryItems) {
            if (item->getName() == name) {
                return item.get();
            }
        }
        return nullptr;
    }

    void showItems() const {
        if (inventoryItems.empty()) {
            std::cout << "Storage is empty\n";
            return;
        }
        std::cout << "Items in storage:\n";
        for (const auto& item : inventoryItems) {
            std::cout << "- " << item->getName() << ": " << item->getDescription() << "\n";
        }
    }

    void saveToFile(const std::string& filepath) const {
        std::ofstream outFile(filepath);
        if (!outFile) {
            throw GameError("Cannot open file for saving");
        }
        for (const auto& item : inventoryItems) {
            outFile << item->toDataString() << "\n";
        }
    }

    void loadFromFile(const std::string& filepath) {
        std::ifstream inFile(filepath);
        if (!inFile) {
            throw GameError("Cannot open file for loading");
        }
        inventoryItems.clear();

        std::string line;
        while (std::getline(inFile, line)) {
            size_t typePos = line.find(',');
            if (typePos == std::string::npos) continue;

            std::string type = line.substr(0, typePos);
            std::unique_ptr<InventoryItem> itemPtr;

            if (type == "Arms") {
                itemPtr = std::make_unique<Arms>("", "", 0);
            } else if (type == "HealingPotion") {
                itemPtr = std::make_unique<HealingPotion>("", "", 0);
            } else {
                continue;
            }

            itemPtr->fromDataString(line);
            inventoryItems.push_back(std::move(itemPtr));
        }
    }
};

// Базовый монстр
class Enemy {
protected:
    std::string enemyName;
    int hp;
    int atk;
    int def;

public:
    Enemy(const std::string& name, int health, int attack, int defense)
        : enemyName(name), hp(health), atk(attack), def(defense) {}

    virtual ~Enemy() = default;

    virtual void hitTarget(class Hero& target) = 0;

    bool alive() const { return hp > 0; }

    void receiveDamage(int dmg) {
        hp -= dmg;
        if (hp < 0) hp = 0;
    }

    virtual std::string toDataString() const {
        return enemyName + "," + std::to_string(hp) + "," +
               std::to_string(atk) + "," + std::to_string(def);
    }

    virtual void fromDataString(const std::string& data) {
        size_t p1 = data.find(',');
        size_t p2 = data.find(',', p1 + 1);
        size_t p3 = data.find(',', p2 + 1);

        if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos) {
            throw GameError("Invalid enemy data");
        }

        enemyName = data.substr(0, p1);
        hp = std::stoi(data.substr(p1 + 1, p2 - p1 - 1));
        atk = std::stoi(data.substr(p2 + 1, p3 - p2 - 1));
        def = std::stoi(data.substr(p3 + 1));
    }

    virtual void showStats() const {
        std::cout << enemyName << " HP: " << hp << " ATK: " << atk << " DEF: " << def << "\n";
    }

    std::string getName() const { return enemyName; }
    int getHP() const { return hp; }
    int getATK() const { return atk; }
    int getDEF() const { return def; }
};

// Конкретный монстр — гоблин
class GoblinEnemy : public Enemy {
public:
    GoblinEnemy() : Enemy("Goblin", 30, 8, 3) {}

    void hitTarget(Hero& target) override {
        std::cout << "Goblin swings a club!\n";
        int damage = atk - target.getDEF();
        if (damage > 0) {
            target.receiveDamage(damage);
            std::cout << "Inflicted " << damage << " damage\n";
        } else {
            std::cout << "No damage dealt\n";
        }
    }

    std::string toDataString() const override {
        return "Goblin," + Enemy::toDataString();
    }
};

// Конкретный монстр — дракон
class DragonEnemy : public Enemy {
public:
    DragonEnemy() : Enemy("Dragon", 100, 20, 10) {}

    void hitTarget(Hero& target) override {
        std::cout << "Dragon breathes fire!\n";
        int damage = atk - target.getDEF();
        if (damage > 0) {
            target.receiveDamage(damage);
            std::cout << "Inflicted " << damage << " damage\n";
        } else {
            std::cout << "No damage dealt\n";
        }
    }
};
// Возвращает строку с сериализованными данными дракона
std::string toString() const override {
    return "Dragon," + Monster::toString();
}
};

class BoneWarrior : public Monster {
public:
    BoneWarrior() : Monster("BoneWarrior", 40, 10, 5) {}

    // Атака на цель с расчетом урона
    void strike(Character& target) override {
        std::cout << "BoneWarrior strikes with rusty blade!" << std::endl;
        int inflictedDamage = attackPower - target.getArmor();
        if (inflictedDamage > 0) {
            target.receiveDamage(inflictedDamage);
            std::cout << "Inflicted " << inflictedDamage << " damage!" << std::endl;
        } else {
            std::cout << "No damage inflicted!" << std::endl;
        }
    }

    std::string toString() const override {
        return "BoneWarrior," + Monster::toString();
    }
};

// Игровой персонаж
class Player {
private:
    std::string playerName;
    int currentHealth;
    int maxHealthPoints;
    int attackPower;
    int armor;
    int lvl;
    int xp;
    Inventory playerInventory;

public:
    Player(const std::string& name, int hp, int atk, int def)
        : playerName(name), currentHealth(hp), maxHealthPoints(hp),
          attackPower(atk), armor(def), lvl(1), xp(0) {}

    // Атака на монстра
    void hitEnemy(Monster& foe) {
        int damageDealt = attackPower - foe.getDefense();
        if (damageDealt > 0) {
            foe.takeDamage(damageDealt);
            std::cout << playerName << " hits " << foe.getName()
                      << " for " << damageDealt << " damage!" << std::endl;
        } else {
            std::cout << playerName << " hits " << foe.getName()
                      << " but it doesn't work!" << std::endl;
        }
    }

    // Получение урона
    void receiveDamage(int damage) {
        currentHealth -= damage;
        if (currentHealth < 0) {
            currentHealth = 0;
            throw GameException(playerName + " is defeated!");
        }
    }

    // Восстановление здоровья
    void restoreHealth(int amount) {
        currentHealth += amount;
        if (currentHealth > maxHealthPoints)
            currentHealth = maxHealthPoints;
        std::cout << playerName << " restores " << amount << " HP!" << std::endl;
    }

    // Получение опыта и повышение уровня
    void earnXP(int amount) {
        xp += amount;
        if (xp >= 100) {
            lvl++;
            xp -= 100;
            maxHealthPoints += 10;
            currentHealth = maxHealthPoints;
            attackPower += 2;
            armor += 1;
            std::cout << playerName << " reached level " << lvl << "!" << std::endl;
        }
    }

    // Использование предмета из инвентаря
    void useInventoryItem(const std::string& itemName) {
        Item* item = playerInventory.getItem(itemName);
        if (!item) {
            throw GameException("No such item in inventory");
        }

        if (auto potion = dynamic_cast<Potion*>(item)) {
            restoreHealth(potion->getHealValue());
            playerInventory.removeItem(itemName);
        } else if (auto weapon = dynamic_cast<Weapon*>(item)) {
            attackPower += weapon->getBonusAttack();
            std::cout << "Equipped " << weapon->getName()
                      << ". Attack +" << weapon->getBonusAttack() << std::endl;
        }
    }

    // Отображение информации о персонаже
    void showStats() const {
        std::cout << "Player: " << playerName
                  << ", HP: " << currentHealth << "/" << maxHealthPoints
                  << ", ATK: " << attackPower
                  << ", DEF: " << armor
                  << ", LVL: " << lvl
                  << ", XP: " << xp << "/100" << std::endl;
    }

    // Сохранение состояния игрока в файл
    void saveState(const std::string& filepath) const {
        std::ofstream outFile(filepath);
        if (!outFile) {
            throw GameException("Cannot open save file");
        }

        outFile << playerName << "\n"
                << currentHealth << "\n"
                << maxHealthPoints << "\n"
                << attackPower << "\n"
                << armor << "\n"
                << lvl << "\n"
                << xp << "\n";

        playerInventory.saveToFile(filepath + "_inv");
    }

    // Загрузка состояния игрока из файла
    void loadState(const std::string& filepath) {
        std::ifstream inFile(filepath);
        if (!inFile) {
            throw GameException("Cannot open save file");
        }

        std::getline(inFile, playerName);
        std::string line;

        std::getline(inFile, line); currentHealth = std::stoi(line);
        std::getline(inFile, line); maxHealthPoints = std::stoi(line);
        std::getline(inFile, line); attackPower = std::stoi(line);
        std::getline(inFile, line); armor = std::stoi(line);
        std::getline(inFile, line); lvl = std::stoi(line);
        std::getline(inFile, line); xp = std::stoi(line);

        playerInventory.loadFromFile(filepath + "_inv");
    }

    // Добавление предмета в инвентарь
    void addItem(std::unique_ptr<Item> newItem) {
        playerInventory.addItem(std::move(newItem));
    }

    // Показать содержимое инвентаря
    void showInventory() const {
        playerInventory.display();
    }

    // Геттеры
    std::string getName() const { return playerName; }
    int getHealth() const { return currentHealth; }
    int getMaxHealth() const { return maxHealthPoints; }
    int getAttack() const { return attackPower; }
    int getDefense() const { return armor; }
    int getLevel() const { return lvl; }
    int getExperience() const { return xp; }
};

// Игровой контроллер
class AdventureGame {
private:
    Player hero;
    Logger<std::string> eventLogger;

public:
    AdventureGame(const std::string& heroName)
        : hero(heroName, 100, 10, 5), eventLogger("adventure_log.txt") {
        eventLogger.log("Game launched for hero: " + heroName);
    }

    // Бой с монстром
    void fight(Monster& enemy) {
        eventLogger.log("Battle started: " + hero.getName() + " vs " + enemy.getName());

        std::cout << "\n--- FIGHT BEGINS ---\n";
        hero.showStats();
        enemy.displayInfo();
        std::cout << "-------------------\n";

        try {
            while (hero.getHealth() > 0 && enemy.isAlive()) {
                std::cout << "\nYour move:\n";
                std::cout << "1. Attack\n2. Use item\nSelect: ";

                int choice;
                std::cin >> choice;

                if (choice == 1) {
                    hero.hitEnemy(enemy);
                    eventLogger.log(hero.getName() + " hits " + enemy.getName());
                } else if (choice == 2) {
                    hero.showInventory();
                    std::cout << "Enter item name: ";
                    std::string itemName;
                    std::cin.ignore();
                    std::getline(std::cin, itemName);

                    try {
                        hero.useInventoryItem(itemName);
                        eventLogger.log(hero.getName() + " used " + itemName);
                    } catch (const GameException& ex) {
                        std::cerr << "Error: " << ex.what() << std::endl;
                        continue;
                    }
                } else {
                    std::cout << "Invalid option!\n";
                    continue;
                }

                if (!enemy.isAlive()) {
                    int rewardXP = enemy.getAttack() * 5;
                    hero.earnXP(rewardXP);
                    std::cout << enemy.getName() << " defeated! Gained " << rewardXP << " XP.\n";
                    eventLogger.log(hero.getName() + " defeated " + enemy.getName() +
                                    " and earned " + std::to_string(rewardXP) + " XP");
                    return;
                }

                std::cout << "\nEnemy's turn:\n";
                enemy.attackTarget(hero);
                eventLogger.log(enemy.getName() + " attacks " + hero.getName());

                std::cout << "\nStatus update:\n";
                hero.showStats();
                enemy.displayInfo();
                std::cout << "-------------------\n";
            }
        } catch (const GameException& ex) {
            eventLogger.log("Battle ended: " + std::string(ex.what()));
            throw;
        }
    }

    void saveProgress() {
        try {
            hero.saveState("savefile.txt");
            eventLogger.log("Game saved");
            std::cout << "Progress saved!\n";
        } catch (const GameException& ex) {
            eventLogger.log("Save error: " + std::string(ex.what()));
            std::cerr << "Save failed: " << ex.what() << std::endl;
        }
    }

    void loadProgress() {
        try {
            hero.loadState("savefile.txt");
            eventLogger.log("Game loaded");
            std::cout << "Progress loaded!\n";
        } catch (const GameException& ex) {
            eventLogger.log("Load error: " + std::string(ex.what()));
            std::cerr << "Load failed: " << ex.what() << std::endl;
        }
    }

    void addInventoryItem(std::unique_ptr<Item> item) {
        hero.addItem(std::move(item));
    }

    void showHeroStats() const {
        hero.showStats();
    }

    void showHeroInventory() const {
        hero.showInventory();
    }
};

int main() {
    try {
        std::cout << "Enter hero name: ";
        std::string heroName;
        std::getline(std::cin, heroName);

        AdventureGame adventure(heroName);

        adventure.addInventoryItem(std::make_unique<Weapon>("Steel Sword", "Sharp sword", 5));
        adventure.addInventoryItem(std::make_unique<Potion>("Health Potion", "Restore 30 HP", 30));

        while (true) {
            std::cout << "\n--- MAIN MENU ---\n";
            std::cout << "1. Fight Goblin\n";
            std::cout << "2. Fight Dragon\n";
            std::cout << "3. Fight BoneWarrior\n";
            std::cout << "4. Show hero stats\n";
            std::cout << "5. Show inventory\n";
            std::cout << "6. Save game\n";
            std::cout << "7. Load game\n";
            std::cout << "8. Exit\n";
            std::cout << "Choose option: ";

            int option;
            std::cin >> option;

            try {
                switch (option) {
                    case 1: {
                        Goblin gob;
                        adventure.fight(gob);
                        break;
                    }
                    case 2: {
                        Dragon drake;
                        adventure.fight(drake);
                        break;
                    }
                    case 3: {
                        BoneWarrior bone;
                        adventure.fight(bone);
                        break;
                    }
                    case 4:
                        adventure.showHeroStats();
                        break;
                    case 5:
                        adventure.showHeroInventory();
                        break;
                    case 6:
                        adventure.saveProgress();
                        break;
                    case 7:
                        adventure.loadProgress();
                        break;
                    case 8:
                        return 0;
                    default:
                        std::cout << "Invalid option!\n";
                }
            } catch (const GameException& ex) {
                std::cerr << "Game over: " << ex.what() << std::endl;
                return 1;
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
