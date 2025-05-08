//LAB_10
//Подключение библиотек
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <exception>
#include <locale.h>

//Базовый класс User
class User {
private:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& name, int id, int accessLevel) {
        setName(name);
        setId(id);
        setAccessLevel(accessLevel);
    }

    virtual ~User() = default;

    // Геттеры
    std::string getName() const { 
        return name;
    };
    
    int getId() const {
        return id;
    }
    
    int getAccessLevel() const {
        return accessLevel;
    }

    // Сеттеры
    void setName(const std::string& newName) {
        if (newName.empty()) {
            throw std::invalid_argument("Ошибка! Имя не может быть пустым.");
        }
        name = newName;
    }

    void setId(int newId) {
        if (newId < 0) {
            throw std::invalid_argument("Ошибка! ID не может быть отрицательным.");
        }
        id = newId;
    }

    void setAccessLevel(int newAccessLevel) {
        if (newAccessLevel < 0) {
            throw std::invalid_argument("Ошибка! Уровень доступа не может быть отрицательным.");
        }
        accessLevel = newAccessLevel;
    }

    virtual void displayInfo() const {
        std::cout << std::endl << "Пользователь: " << name << ", ID: " << id << ", Уровень доступа: " << accessLevel << std::endl;
    }
};

// Класс студента
class Student : public User {
private:
    std::string group;

public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group(group) { }

    // Сеттер
    void setGroup(const std::string& newGroup) {
        if (newGroup.empty()) {
            throw std::invalid_argument("Ошибка! Группа не может быть пустой.");
        }
        group = newGroup;
    }

    //Геттер
    std::string getGroup() const {
        return group;
    }

    // Переопределённый метод вывода информации
    void displayInfo() const override {
        std::cout << std::endl << "Студент: " << getName() << ", ID: " << getId()
            << ", Уровень доступа: " << getAccessLevel() << " Группа: " << group << std::endl;
    }
};

// Класс учителя
class Teacher : public User {
private:
    std::string department;

public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& department)
        : User(name, id, accessLevel), department(department) {}

    //Сеттер
    void setDepartment(const std::string& newDepartment) {
        if (newDepartment.empty()) {
            throw std::invalid_argument("Ошибка! Кафедра не может быть пустой.");
        }
        department = newDepartment;
    }

    // Геттер
    std::string getDepartment() const {
        return department;
    }

    // Переопределение метода
    void displayInfo() const override {
        std::cout << std::endl << "Преподаватель: " << getName() << ", ID: " << getId() << ", Уровень доступа: " << getAccessLevel() <<
            ", Кафедра: " << department << std::endl;
    }
};

//Класс Админа
class Administrator : public User {
private:
    int adminLevel;

public:
    Administrator(const std::string& name, int id, int accessLevel, int adminLevel)
        : User(name, id, accessLevel), adminLevel(adminLevel) {
        if (adminLevel < 0) {
            throw std::invalid_argument("Ошибка! Уровень администратора не может быть отрицательным!");
        }
    }

    void setAdminLevel(int newAdminLevel) {
        if (newAdminLevel < 0) {
            throw std::invalid_argument("Ошибка! Уровень администратора не может быть отрицательным!");
        }
        adminLevel = newAdminLevel;
    }

    int getAdminLevel() const {
        return adminLevel;
    }

    void displayInfo() const override {
        std::cout << std::endl << "Администратор: " << getName() << ", ID: " << getId()
            << ", Уровень доступа: " << getAccessLevel() << ", Уровень администратора: " << adminLevel << std::endl;
    }
};

//Система контроля доступа
class Resource {
private:
    std::string resourceName;
    int requiredAccessLevel;

public:
    Resource(const std::string& name, int accessLevel)
        : resourceName(name), requiredAccessLevel(accessLevel) {
        if (name.empty()) {
            throw std::invalid_argument("Ошибка! Имя ресурса не может быть пустым.");
        }
        if (accessLevel < 0) {
            throw std::invalid_argument("Ошибка! Уровень доступа не может быть отрицательным.");
        }
    }

    std::string getName() const {
        return resourceName;
    }

    int getRequiredAccessLevel() const {
        return requiredAccessLevel;
    }

    // Проверка на уровень доступа
    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    // Отображение требуемого уровня доступа
    void displayInfo() const {
        std::cout << "Ресурс: " << resourceName << ", Требуемый уровень доступа: " << requiredAccessLevel << std::endl;
    }
};

//Шаблонный класс для операций с юзерами и ресурсами
template <typename UserType, typename ResourceType>
class AccessControlSystem {
private:
    std::vector<std::shared_ptr<UserType>> users;
    std::vector<std::shared_ptr<ResourceType>> resources;

public:
    void addUser(std::shared_ptr<UserType> user) {
        users.push_back(user);
    }

    void addResource(std::shared_ptr<ResourceType> resource) {
        resources.push_back(resource);
    }

    void displayUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    void displayResources() const {
        for (const auto& resource : resources) {
            resource->displayInfo();
        }
    }

    //Проверка доступа
    bool checkUserAccessToResource(int userId, const std::string& resourceName) const {
        auto userIt = std::find_if(users.begin(), users.end(),
            [userId](const std::shared_ptr<UserType>& user) { return user->getId() == userId; });
        if (userIt == users.end()) {
            throw std::runtime_error("Пользователь не найден...");
        }

        auto resourceIt = std::find_if(resources.begin(), resources.end(),
            [&resourceName](const std::shared_ptr<ResourceType>& resource) { return resource->getName() == resourceName; });
        if (resourceIt == resources.end()) {
            throw std::runtime_error("Ресурс не найден...");
        }

        return (*resourceIt)->checkAccess(**userIt);
    }

    //Поиск пользователей по именам
    std::vector<std::shared_ptr<UserType>> searchUsersByName(const std::string& name) const {
        std::vector<std::shared_ptr<UserType>> result;
        for (const auto& user : users) {
            if (user->getName() == name) {
                result.push_back(user);
            }
        }
        return result;
    }

    //Поиск пользователей по ID
    std::shared_ptr<UserType> searchUserById(int id) const {
        auto it = std::find_if(users.begin(), users.end(),
            [id](const std::shared_ptr<UserType>& user) { return user->getId() == id; });
        if (it != users.end()) {
            return *it;
        }
        return nullptr;
    }

    //Сортировка пользователей
    void sortUsersByAccessLevel() {
        std::sort(users.begin(), users.end(),
            [](const std::shared_ptr<UserType>& a, const std::shared_ptr<UserType>& b) {
                return a->getAccessLevel() < b->getAccessLevel();
            });
    }

    //Сохранение пользователей и ресурсов в файл
    void saveToFile(const std::string& usersFile, const std::string& resourcesFile) const {
        std::ofstream uFile(usersFile);
        
        if (!uFile) {
            throw std::runtime_error("Ошибка! Не удалось открыть файл пользователей для записи.");
        }
        for (const auto& user : users) {
            //Сохраняем информацию о типе пользователя
            //Студент
            if (dynamic_cast<Student*>(user.get())) {
                uFile << "Student ";
                uFile << user->getName() << " " << user->getId() << " " << user->getAccessLevel() << " ";
                uFile << static_cast<Student*>(user.get())->getGroup() << std::endl;
            }
            //Учитель
            else if (dynamic_cast<Teacher*>(user.get())) {
                uFile << "Teacher ";
                uFile << user->getName() << " " << user->getId() << " " << user->getAccessLevel() << " ";
                uFile << static_cast<Teacher*>(user.get())->getDepartment() << std::endl;
            }
            //Администратор
            else if (dynamic_cast<Administrator*>(user.get())) {
                uFile << "Administrator ";
                uFile << user->getName() << " " << user->getId() << " " << user->getAccessLevel() << " ";
                uFile << static_cast<Administrator*>(user.get())->getAdminLevel() << std::endl;
            }
            else {
                uFile << "User ";
                uFile << user->getName() << " " << user->getId() << " " << user->getAccessLevel() << std::endl;
            }
        }
        uFile.close();

        std::ofstream rFile(resourcesFile);
        if (!rFile) {
            throw std::runtime_error("Ошибка! Не удалось открыть файл ресурсов для записи.");
        }
        for (const auto& resource : resources) {
            rFile << resource->getName() << " " << resource->getRequiredAccessLevel() << std::endl;
        }
        rFile.close();
    }

    //Загрузка пользователей и ресурсов из файлов
    void loadFromFile(const std::string& usersFile, const std::string& resourcesFile) {
        users.clear();
        resources.clear();

        std::ifstream uFile(usersFile);
        if (!uFile) {
            throw std::runtime_error("Ошибка! Не удалось открыть файл ресурсов для чтения.");
        }
        std::string userType;

        while (uFile >> userType) {
            //Студент
            if (userType == "Student") {
                std::string name, group;
                int id, accessLevel;
                
                uFile >> name >> id >> accessLevel >> group;
                addUser(std::make_shared<Student>(name, id, accessLevel, group));
            }
            //Учитель
            else if (userType == "Teacher") {
                std::string name, department;
                int id, accessLevel;
                
                uFile >> name >> id >> accessLevel >> department;
                addUser(std::make_shared<Teacher>(name, id, accessLevel, department));
            }
            //Администратор
            else if (userType == "Administrator") {
                std::string name;
                int id, accessLevel, adminLevel;

                uFile >> name >> id >> accessLevel >> adminLevel;
                addUser(std::make_shared<Administrator>(name, id, accessLevel, adminLevel));
            }
            //Пользователь
            else if (userType == "User") {
                std::string name;
                int id, accessLevel;
                uFile >> name >> id >> accessLevel;
                addUser(std::make_shared<User>(name, id, accessLevel));
            }
            //Неизвестный тип
            else {
                std::string skipLine;
                std::getline(uFile, skipLine);
            }
        }
        uFile.close();

        std::ifstream rFile(resourcesFile);
        if (!rFile) {
            throw std::runtime_error("Ошибка! Не удалось открыть файл ресурсов для чтения.");
        }

        std::string resourceName;
        int accessLevel;
        while (rFile >> resourceName >> accessLevel) {
            addResource(std::make_shared<Resource>(resourceName, accessLevel));
        }
        rFile.close();
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");

    try {
        AccessControlSystem<User, Resource> sys;

        //Добавление пользователей
        //name, ID, Уровень доступа, группа
        sys.addUser(std::make_shared<Student>("Dan", 1, 2, "T.22"));
        sys.addUser(std::make_shared<Student>("Steve", 2, 2, "T.23"));
        
        //name, ID, Уровень доступа, кафедра
        sys.addUser(std::make_shared<Teacher>("Olga", 3, 3, "T"));
        
        //name, ID, Уровень доступа, Уровень админа
        sys.addUser(std::make_shared<Administrator>("Admin", 4, 5, 1));

        //Добавление ресурсов
        //name, требуемый уровень
        sys.addResource(std::make_shared<Resource>("Audience", 1));
        sys.addResource(std::make_shared<Resource>("Laboratory", 3));
        sys.addResource(std::make_shared<Resource>("Admin_Panel", 5));

        bool running = true;
        while (running) {
            std::cout << "\nВыберите действие:\n";
            std::cout << "1. Показать пользователей\n";
            std::cout << "2. Показать ресурсы\n";
            std::cout << "\n";
            std::cout << "3. Проверить доступ пользователя к ресурсу\n";
            std::cout << "4. Поиск пользователей по имени\n";
            std::cout << "5. Сортировать пользователей по уровню доступа\n";
            std::cout << "\n";
            std::cout << "6. Сохранить данные в файлы\n";
            std::cout << "7. Загрузить данные из файлов\n";
            std::cout << "8. Выход\n";
            std::cout << "Введите выбор: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
            // Вывод информации о пользователях и ресурсах
            case 1: {
                sys.displayUsers();
                break;
            }
            case 2: {
                sys.displayResources();
                break;
            }

            //Проверка, поиск и сортировка
            case 3: {
                std::cout << "Введите ID пользователя: ";
                int userId;
                std::cin >> userId;

                std::cout << "Введите имя ресурса: ";
                std::string resourceName;
                std::cin.ignore();
                std::getline(std::cin, resourceName);

                try {
                    bool hasAccess = sys.checkUserAccessToResource(userId, resourceName);
                    std::cout << "Пользователь с ID" << userId << (hasAccess ? " имеет " : " не имеет ") << "доступ к" << resourceName << std::endl;
                }
                catch (const std::exception& e) {
                    std::cout << "Ошибка: " << e.what() << std::endl;
                }
                break;
            }
            case 4: {
                std::cout << "Введите имя для поиска: ";
                std::string searchName;

                std::cin.ignore();
                std::getline(std::cin, searchName);

                auto foundUsers = sys.searchUsersByName(searchName);
                std::cout << "Результаты поиска пользователей с именем \"" << searchName << "\":" << std::endl;
                
                for (const auto& user : foundUsers) {
                    user->displayInfo();
                }
                break;
            }
            case 5: {
                sys.sortUsersByAccessLevel();
                
                std::cout << "Пользователи отсортированы по уровню доступа:" << std::endl;
                sys.displayUsers();
                
                break;
            }

            //Сохранить, зайти и выйти
            case 6: {
                std::cout << "Введите имя файла для данных пользователей: ";
                std::string usersFile;
                std::cin >> usersFile;
                
                std::cout << "Введите имя файла для данных ресурсов: ";
                std::string resourcesFile;
                std::cin >> resourcesFile;
                
                try {
                    sys.saveToFile(usersFile, resourcesFile);
                    std::cout << "Данные сохранены в файлы." << std::endl;
                }
                catch (const std::exception& e) {
                    std::cout << "Ошибка при сохранении данных: " << e.what() << std::endl;
                }
                break;
            }
            case 7: {
                std::cout << "Введите имя файла для данных пользователей: ";
                std::string usersFile;
                std::cin >> usersFile;
                
                std::cout << "Введите имя файла для данных ресурсов: ";
                std::string resourcesFile;
                std::cin >> resourcesFile;
                
                try {
                    sys.loadFromFile(usersFile, resourcesFile);
                    std::cout << "Данные загружены из файлов:" << std::endl;
                    sys.displayUsers();
                    sys.displayResources();
                }
                catch (const std::exception& e) {
                    std::cout << "Ошибка при загрузке данных: " << e.what() << std::endl;
                }
                break;
            }
            case 8: {
                running = false;
                std::cout << "Выход из системы..." << std::endl;
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