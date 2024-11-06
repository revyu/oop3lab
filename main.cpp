// oop3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "empiric.h"
#include "mixture.h"
#include "primary.h"
#include <windows.h> 
#include <fstream> // Добавьте эту строку в начало файла

int main()
{
    srand(time(0));

    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::string command;

    //std::getline(std::cin, command);

    std::vector<primary> primaries(100, primary(1.0, 0.0, 1.0));
    std::vector<primary> single_primary = { primary(1.0, 0.0, 1.0) };
    std::vector<mixture> mixtures(100, mixture(single_primary));
    std::vector<empiric> distributions;

    mixture mix({ primary(2, 0, 1), primary(2, 5, 1) });


    std::cout << mix.mean() << " " << mix.dispersion() << std::endl;


    std::vector<long double> dis = mix.simulate_distribution(10000);


    std::vector<std::pair<long double, long double>> den = mix.density_vector(10000);

    std::ofstream file("density.txt");
    if (file.is_open()) {
        for (auto value : den) {
            file << value.first << " " << value.second << "\n";  // Запись каждого значения в новой строке
        }
        file.close();
    }
    std::ofstream file1("dis.txt");
    if (file1.is_open()) {
        for (auto value : dis) {
            file << value << "\n";  // Запись каждого значения в новой строке
        }
        file.close();
    }





    /*
    while (command != "exit") {
        if (command == "add_new") {

            int key = input_int("key");
            while (key > 100) {
                std::cout << "Неподходящее значение key : key должно быть меньше 100";
                key = input_int("key");
            }
            std::cout << "Какой объект создаем?" << std::endl;
            std::getline(std::cin, command);

            if (command == "primary") {
                primaries[key] = primary(" ");
            }
            if (command == "mixture") {
                int key1 = input_int("key");
                int key2 = input_int("key");
            }
            if (command == "empiric") {
                std::cout << "Откуда?";
                int n = input_int("n");
                std::vector<long double> dis = mixtures[0].simulate_distribution(n);

                for (int i = 0; i < n; i++) {
                    std::cout << dis[i];
                }
            }
        }
        std::getline(std::cin, command);
    }
    */
}
