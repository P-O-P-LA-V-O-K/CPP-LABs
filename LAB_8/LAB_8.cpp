//LAB - 8

#include <iostream>
#include <string>

// Класс, представляющий профиль пользователя
class UserProfile {
private:
    std::string fullName;
    int userAge;
    std::string emailAddress;
    std::string homeAddress;

public:
    // Получить имя
    std::string getFullName() const {
        return fullName;
    }

    // Получить возраст
    int getUserAge() const {
        return userAge;
    }

    // Получить email
    std::string getEmailAddress() const {
        return emailAddress;
    }

    // Получить адрес
    std::string getHomeAddress() const {
        return homeAddress;
    }

    // Установить имя
    void setFullName(const std::string& nameInput) {
        if (!nameInput.empty()) {
            fullName = nameInput;
        } else {
            std::cerr << "Ошибка: имя не должно быть пустым!\n";
        }
    }

    // Установить возраст
    void setUserAge(int ageInput) {
        if (ageInput >= 0 && ageInput <= 120) {
            userAge = ageInput;
        } else {
            std::cerr << "Ошибка: возраст должен быть от 0 до 120!\n";
        }
    }

    // Установить email
    void setEmailAddress(const std::string& emailInput) {
        if (emailInput.find('@') != std::string::npos) {
            emailAddress = emailInput;
        } else {
            std::cerr << "Ошибка: некорректный формат email!\n";
        }
    }

    // Установить адрес
    void setHomeAddress(const std::string& addressInput) {
        if (!addressInput.empty()) {
            homeAddress = addressInput;
        } else {
            std::cerr << "Ошибка: адрес не должен быть пустым!\n";
        }
    }

    // Показать всю информацию о пользователе
    void showProfile() const {
        std::cout << "Имя: " << fullName << ", Возраст: " << userAge
                  << ", Email: " << emailAddress << ", Адрес: " << homeAddress << std::endl;
    }
};

int main() {
    UserProfile client;

    // Устанавливаем корректные значения
    client.setFullName("Иван Петров");
    client.setUserAge(28);
    client.setEmailAddress("ivan.petrov@example.com");
    client.setHomeAddress("ул. Ленина, д.10, Москва");

    // Выводим значения
    std::cout << "Имя: " << client.getFullName() << std::endl;
    std::cout << "Возраст: " << client.getUserAge() << std::endl;
    std::cout << "Email: " << client.getEmailAddress() << std::endl;
    std::cout << "Адрес: " << client.getHomeAddress() << std::endl;

    // Пробуем задать некорректные данные
    client.setFullName("");
    client.setUserAge(999);
    client.setEmailAddress("wrong_email");
    client.setHomeAddress("");

    // Выводим текущий профиль
    std::cout << "\nПрофиль пользователя:\n";
    client.showProfile();

    // Обновляем данные
    client.setFullName("Мария Иванова");
    client.setUserAge(34);
    client.setEmailAddress("maria.ivanova@mail.ru");
    client.setHomeAddress("пр. Победы, д.25, Санкт-Петербург");

    // Показываем обновлённый профиль
    std::cout << "\nОбновлённый профиль:\n";
    client.showProfile();

    return 0;
}


