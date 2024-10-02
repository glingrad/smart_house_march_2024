#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;
using namespace this_thread;

mutex mtx; // Мьютекс для защиты критических секций

// Базовый класс для виртуальной комнаты
class Virtual_room {
private:
    const int t = 100 - 90; // Температура по умолчанию
    const int t1 = 0; // Начальная температура для нагрева пола
    string person_name; // Имя пользователя

public:
    // Метод для отображения температуры
    virtual void show_temp() {
        cout << "Temp is " << t << endl;
    }

    // Метод для проверки состояния двери
    virtual void is_door_open(const bool& d) {
        if (d) {
            cout << "Door is open! " << endl;
        } else {
            cout << "Door is closed! " << endl;
        }
    }

    // Метод для нагрева пола до заданной температуры
    virtual void warm_floor() {
        int temp_to_need; // Переменная для хранения желаемой температуры
        {
            lock_guard<mutex> lock(mtx); // Защита критической секции при вводе
            cout << "Enter desired floor temperature: ";
            cin >> temp_to_need; // Ввод желаемой температуры
        }
        // Нагрев пола до желаемой температуры
        for (int i = t1; i < temp_to_need; i++) {
            this_thread::sleep_for(seconds(1)); // Задержка для имитации нагрева
            cout << "Heating to: " << i << endl;
        }
    }

    // Метод для идентификации пользователя
    virtual void identification() {
        {
            lock_guard<mutex> lock(mtx); // Защита критической секции при вводе
            cout << "Please login: ";
            cin >> person_name; // Ввод имени пользователя
        }
        // Проверка имени пользователя
        try {
            if (person_name == "Person") { // Если имя совпадает
                cout << "Access granted! " << endl;
            } else {
                throw 1; // Генерация исключения, если имя не совпадает
            }
        } catch (int a) {
            cerr << "Name not found! " << endl; // Сообщение об ошибке
        }
    }

    virtual ~Virtual_room() {} // Виртуальный деструктор для корректного освобождения памяти
};

// Класс для конкретной комнаты, наследующий от Virtual_room
class Room : public Virtual_room {
private:
    bool a = true; // Переменная для состояния двери
    int x; // Температура, которую чувствует пользователь
    const int t = 100 - 90; // Температура по умолчанию
    const int t2 = 0; // Начальная температура для нагрева пола
    string name; // Имя пользователя

public:
    // Конструктор, принимающий температуру
    Room(int& x) : x(x) {}

    // Переопределение метода для отображения температуры в комнате
    void show_temp() override {
        this_thread::sleep_for(seconds(2)); // Задержка для симуляции обработки
        cout << "Temp in this room now: " << t << endl;
        cout << "Temp is how you feel: " << x << endl;
    }

    // Переопределение метода для проверки состояния двери в комнате
    void is_door_open(const bool& a) override {
        this_thread::sleep_for(seconds(2)); // Задержка для симуляции обработки
        if (a) {
            cout << "Door in this Room open! " << endl;
        } else {
            cout << "Door in this Room closed! " << endl;
        }
    }

    // Переопределение метода для нагрева пола в комнате
    void warm_floor() override {
        int temp_to_need1; // Переменная для хранения желаемой температуры
        {
            lock_guard<mutex> lock(mtx); // Защита критической секции при вводе
            cout << "Enter desired floor temperature: ";
            cin >> temp_to_need1; // Ввод желаемой температуры
        }
        // Нагрев пола до желаемой температуры
        for (int i = t2; i <= temp_to_need1; i++) {
            this_thread::sleep_for(seconds(1)); // Задержка для имитации нагрева
            cout << "Heating to: " << i << endl;
        }
    }

    // Переопределение метода для идентификации пользователя в комнате
    void identification() override {
        {
            lock_guard<mutex> lock(mtx); // Защита критической секции при вводе
            cout << "Please enter your name: ";
            cin >> name; // Ввод имени пользователя
        }
        // Проверка имени пользователя
        try {
            if (name == "Gusein") { // Если имя совпадает
                cout << "Access granted! " << endl;
            } else {
                throw 1; // Генерация исключения, если имя не совпадает
            }
        } catch (int a) {
            cerr << "Access is denied " << endl; // Сообщение об ошибке
        }
    }

    ~Room() {} // Деструктор
};

int main() {
    int x1 = 30; // Температура, которую чувствует пользователь
    bool y1 = false; // Переменная для состояния двери

    vector<Virtual_room*> v_r; // Вектор для хранения указателей на объекты Virtual_room
    v_r.push_back(new Room(x1)); // Добавление нового объекта Room в вектор

    vector<thread> threads; // Вектор для хранения потоков

    // Создание потоков для выполнения методов объектов
    threads.emplace_back([&v_r]() { v_r[0]->identification(); }); // Поток для идентификации
    threads.emplace_back([&v_r, &y1]() { v_r[0]->is_door_open(y1); }); // Поток для проверки состояния двери
    threads.emplace_back([&v_r]() { v_r[0]->show_temp(); }); // Поток для отображения температуры
    threads.emplace_back([&v_r]() { v_r[0]->warm_floor(); }); // Поток для нагрева пола

    // Ожидание завершения каждого потока
    for (auto& t : threads) {
        t.join(); // Ждем завершения потока
    }

    // Освобождение памяти, выделенной для объектов Room
    for (auto room : v_r) {
        delete room; // Освобождение памяти для каждого объекта Room
    }

    return 0; // Завершение программы
}
