//LAB - 7_1

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <stdexcept>

// Базовый класс для всех объектов игры
class GameObject {
protected:
    std::string objectName;
    int hp;
    int rank;

public:
    GameObject(std::string name, int health, int level)
        : objectName(std::move(name)), hp(health), rank(level) {}

    virtual ~GameObject() = default;

    // Показать информацию об объекте
    virtual void printInfo() const {
        std::cout << "Name: " << objectName << ", HP: " << hp << ", Level: " << rank;
    }

    // Сериализация объекта в строку
    virtual std::string toString() const {
        std::ostringstream out;
        out << objectName << "," << hp << "," << rank;
        return out.str();
    }

    // Загрузка данных из строки
    virtual void fromString(const std::string& input) {
        std::istringstream stream(input);
        std::string segment;

        std::getline(stream, objectName, ',');
        std::getline(stream, segment, ',');
        hp = std::stoi(segment);
        std::getline(stream, segment, ',');
        rank = std::stoi(segment);
    }

    // Получить тип объекта
    virtual std::string getCategory() const = 0;
};

// Класс игрока
class Hero : public GameObject {
    int xp;  // Опыт

public:
    Hero(std::string name = "", int health = 0, int level = 0, int experience = 0)
        : GameObject(std::move(name), health, level), xp(experience) {}

    void printInfo() const override {
        GameObject::printInfo();
        std::cout << ", XP: " << xp << std::endl;
    }

    std::string toString() const override {
        return GameObject::toString() + "," + std::to_string(xp) + ",Hero";
    }

    void fromString(const std::string& input) override {
        GameObject::fromString(input);

        std::istringstream stream(input);
        std::string segment;

        // Пропустить name, hp, rank
        for (int i = 0; i < 3; ++i) std::getline(stream, segment, ',');

        std::getline(stream, segment, ',');
        xp = std::stoi(segment);
    }

    std::string getCategory() const override {
        return "Hero";
    }
};

// Класс врага
class Villain : public GameObject {
    std::string species;  // Тип врага

public:
    Villain(std::string name = "", int health = 0, int level = 0, std::string enemyType = "")
        : GameObject(std::move(name), health, level), species(std::move(enemyType)) {}

    void printInfo() const override {
        GameObject::printInfo();
        std::cout << ", Type: " << species << std::endl;
    }

    std::string toString() const override {
        return GameObject::toString() + "," + species + ",Villain";
    }

    void fromString(const std::string& input) override {
        GameObject::fromString(input);

        std::istringstream stream(input);
        std::string segment;

        // Пропустить name, hp, rank
        for (int i = 0; i < 3; ++i) std::getline(stream, segment, ',');

        std::getline(stream, species, ',');
    }

    std::string getCategory() const override {
        return "Villain";
    }
};

// Фабричная функция создания объекта из строки
std::unique_ptr<GameObject> createObject(const std::string& line) {
    size_t typeStart = line.rfind(',');
    if (typeStart == std::string::npos) {
        throw std::runtime_error("Incorrect format");
    }

    std::string type = line.substr(typeStart + 1);
    std::string rawData = line.substr(0, typeStart);

    std::unique_ptr<GameObject> obj;

    if (type == "Hero") {
        obj = std::make_unique<Hero>();
    } else if (type == "Villain") {
        obj = std::make_unique<Villain>();
    } else {
        throw std::runtime_error("Unknown object type: " + type);
    }

    // Заполнить данные объекта
    obj->fromString(rawData);
    return obj;
}

// Класс управления всеми объектами
class SceneController {
    std::vector<std::unique_ptr<GameObject>> sceneObjects;

public:
    // Добавить объект в сцену
    void insert(std::unique_ptr<GameObject> obj) {
        sceneObjects.push_back(std::move(obj));
    }

    // Показать все объекты
    void renderAll() const {
        for (const auto& obj : sceneObjects) {
            obj->printInfo();
        }
    }

    // Сохранить сцену в файл
    void saveToFile(const std::string& filePath) const {
        std::ofstream file(filePath);
        if (!file) throw std::runtime_error("Failed to open file for writing: " + filePath);

        for (const auto& obj : sceneObjects) {
            file << obj->toString() << "\n";
        }
    }

    // Загрузить сцену из файла
    void loadFromFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file) throw std::runtime_error("Failed to open file for reading: " + filePath);

        sceneObjects.clear();
        std::string line;

        while (std::getline(file, line)) {
            try {
                // Добавить объект, созданный из строки
                sceneObjects.push_back(createObject(line));
            } catch (...) {
                std::cerr << "Invalid line skipped: " << line << "\n";
            }
        }
    }
};

// Главная функция
int main() {
    try {
        SceneController gameScene;

        // Добавляем героев и врагов
        gameScene.insert(std::make_unique<Hero>("Knight", 120, 2, 50));
        gameScene.insert(std::make_unique<Villain>("Orc", 70, 1, "Brute"));
        gameScene.insert(std::make_unique<Hero>("Rogue", 90, 3, 130));
        gameScene.insert(std::make_unique<Villain>("Troll", 150, 4, "Tank"));

        std::cout << "Initial game objects:\n";
        gameScene.renderAll();

        // Сохраняем в файл
        gameScene.saveToFile("savefile.txt");
        std::cout << "\nData saved.\n\n";

        // Загружаем в новую сцену
        SceneController loadedScene;
        loadedScene.loadFromFile("savefile.txt");

        std::cout << "Objects after reloading:\n";
        loadedScene.renderAll();

    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

