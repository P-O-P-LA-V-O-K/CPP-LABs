//LAB - 10

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <exception>
#include <sstream>

// Класс ошибки для неверных данных
class DataError : public std::exception {
    std::string message;
public:
    DataError(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

// Родительский класс для пользователей
class Person {
private:
    std::string fullName;
    int identifier;
    int level;

public:
    Person(const std::string& name, int id, int lvl) {
        setFullName(name);
        setIdentifier(id);
        setLevel(lvl);
    }

    virtual ~Person() = default;

    std::string getFullName() const { return fullName; }

    void setFullName(const std::string& name) {
        if (name.empty()) throw DataError("Имя не может быть пустым");
        fullName = name;
    }

    int getIdentifier() const { return identifier; }

    void setIdentifier(int id) {
        if (id < 0) throw DataError("ID должен быть >= 0");
        identifier = id;
    }

    int getLevel() const { return level; }

    void setLevel(int lvl) {
        if (lvl < 0) throw DataError("Уровень доступа не может быть отрицательным");
        level = lvl;
    }

    virtual void showInfo() const {
        std::cout << "Person: " << fullName << ", ID: " << identifier
                  << ", Level: " << level << std::endl;
    }

    virtual std::string toString() const {
        return fullName + "," + std::to_string(identifier) + "," + std::to_string(level);
    }
};

// Класс Студент
class Learner : public Person {
private:
    std::string cohort;

public:
    Learner(const std::string& name, int id, int lvl, const std::string& group)
        : Person(name, id, lvl), cohort(group) {}

    std::string getCohort() const { return cohort; }
    void setCohort(const std::string& group) { cohort = group; }

    void showInfo() const override {
        std::cout << "Learner: " << getFullName() << ", ID: " << getIdentifier()
                  << ", Level: " << getLevel() << ", Cohort: " << cohort << std::endl;
    }

    std::string toString() const override {
        return "Learner," + Person::toString() + "," + cohort;
    }
};

// Класс Преподаватель
class Instructor : public Person {
private:
    std::string faculty;

public:
    Instructor(const std::string& name, int id, int lvl, const std::string& dept)
        : Person(name, id, lvl), faculty(dept) {}

    std::string getFaculty() const { return faculty; }
    void setFaculty(const std::string& dept) { faculty = dept; }

    void showInfo() const override {
        std::cout << "Instructor: " << getFullName() << ", ID: " << getIdentifier()
                  << ", Level: " << getLevel() << ", Faculty: " << faculty << std::endl;
    }

    std::string toString() const override {
        return "Instructor," + Person::toString() + "," + faculty;
    }
};

// Класс Администратор
class Admin : public Person {
private:
    std::string position;

public:
    Admin(const std::string& name, int id, int lvl, const std::string& pos)
        : Person(name, id, lvl), position(pos) {}

    std::string getPosition() const { return position; }
    void setPosition(const std::string& pos) { position = pos; }

    void showInfo() const override {
        std::cout << "Admin: " << getFullName() << ", ID: " << getIdentifier()
                  << ", Level: " << getLevel() << ", Position: " << position << std::endl;
    }

    std::string toString() const override {
        return "Admin," + Person::toString() + "," + position;
    }
};

// Ресурс с уровнем доступа
class UniResource {
private:
    std::string resourceName;
    int requiredLevel;

public:
    UniResource(const std::string& name, int lvl)
        : resourceName(name), requiredLevel(lvl) {
        if (lvl < 0) throw DataError("Уровень доступа ресурса < 0");
    }

    std::string getResourceName() const { return resourceName; }

    int getRequiredLevel() const { return requiredLevel; }

    bool hasAccess(const Person& p) const {
        return p.getLevel() >= requiredLevel;
    }

    std::string toString() const {
        return resourceName + "," + std::to_string(requiredLevel);
    }
};

// Шаблон для системы контроля доступа
template<typename PersonType, typename ResourceType>
class AccessSystem {
private:
    std::vector<std::shared_ptr<PersonType>> personList;
    std::vector<std::shared_ptr<ResourceType>> resourceList;

public:
    void addPerson(std::shared_ptr<PersonType> p) {
        personList.push_back(p);
    }

    void addResource(std::shared_ptr<ResourceType> r) {
        resourceList.push_back(r);
    }

    void showAccessStatus() {
        for (auto& p : personList) {
            std::cout << "Доступ для " << p->getFullName() << ":" << std::endl;
            for (auto& r : resourceList) {
                bool access = r->hasAccess(*p);
                std::cout << "  " << r->getResourceName() << ": "
                          << (access ? "Доступ разрешен" : "Доступ запрещен") << std::endl;
            }
        }
    }

    std::shared_ptr<PersonType> findPersonByName(const std::string& name) {
        for (auto& p : personList) {
            if (p->getFullName() == name) return p;
        }
        return nullptr;
    }

    std::shared_ptr<PersonType> findPersonById(int id) {
        for (auto& p : personList) {
            if (p->getIdentifier() == id) return p;
        }
        return nullptr;
    }

    void sortByLevel() {
        std::sort(personList.begin(), personList.end(),
            [](const std::shared_ptr<PersonType>& a, const std::shared_ptr<PersonType>& b) {
                return a->getLevel() < b->getLevel();
            });
    }

    void savePeopleToFile(const std::string& filePath) {
        std::ofstream out(filePath);
        if (!out) throw std::runtime_error("Невозможно открыть файл для записи");
        for (auto& p : personList) {
            out << p->toString() << "\n";
        }
        out.close();
    }

    void loadPeopleFromFile(const std::string& filePath) {
        std::ifstream in(filePath);
        if (!in) throw std::runtime_error("Невозможно открыть файл для чтения");
        personList.clear();
        std::string line;
        while (std::getline(in, line)) {
            try {
                auto p = parsePerson(line);
                if (p) personList.push_back(p);
            }
            catch (const std::exception& e) {
                std::cerr << "Ошибка при парсинге: " << e.what() << std::endl;
            }
        }
        in.close();
    }

private:
    std::shared_ptr<PersonType> parsePerson(const std::string& str) {
        size_t pos1 = str.find(',');
        if (pos1 == std::string::npos) throw DataError("Неверный формат данных");

        std::string type = str.substr(0, pos1);
        std::string rest = str.substr(pos1 + 1);

        std::istringstream ss(rest);
        std::string name, idStr, levelStr, extra;
        std::getline(ss, name, ',');
        std::getline(ss, idStr, ',');
        std::getline(ss, levelStr, ',');
        std::getline(ss, extra, ',');

        int id = std::stoi(idStr);
        int lvl = std::stoi(levelStr);

        if (type == "Learner") {
            return std::make_shared<Learner>(name, id, lvl, extra);
        }
        else if (type == "Instructor") {
            return std::make_shared<Instructor>(name, id, lvl, extra);
        }
        else if (type == "Admin") {
            return std::make_shared<Admin>(name, id, lvl, extra);
        }
        else {
            throw DataError("Неизвестный тип пользователя");
        }
    }
};
