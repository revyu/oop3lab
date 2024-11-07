// oop3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "empiric.h"
#include "mixture.h"
#include "primary.h"
#include <windows.h> 
#include <fstream> // Добавьте эту строку в начало файла

#include <cstdlib>

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

    
    int key,key1,key2;
    int n,n0,k0,spn;
    while (command != "exit") {
        if (command == "add_new") {

            key = input_int("key");
            while (key > 100) {
                std::cout << "Неподходящее значение key : key должно быть меньше 100";
                key = input_int("key");
            }
            std::cout << "Какой объект создаем?" << std::endl;
            std::getline(std::cin, command);

            if (command == "primary") {
                primaries[key] = primary(" ");//вызов будет из консоли (попросит ввести n,v,mu, и т.д.)
            }
            if (command == "mixture") {
                key1 = input_int("key");
                key2 = input_int("key");

                mixtures[key] = mixture({ primaries[key1],primaries[key2] });

            }
            if (command == "empiric") {
                n = input_int("n");
                std::cout << "Откуда?";
                std::getline(std::cin, command);
                

                key1 = input_int("key"); // вводим ключ откуда хотим получить
                n0 = input_int("n0");
                k0 = input_int("k0");
                spn = input_int("spn");

                if (command == "prim") {
                    distributions[key1] = empiric(n0, mixtures[key1], k0, spn);
                };

                if (command == "mix") {
                    distributions[key1] = empiric(n0, primaries[key1], k0, spn);
                };
            }

             
        }

        //to do : maybe сделать так что бы можно было показывать прямо из prim и mix не создавая перед этим empiric?
        if (command == "show") {
            
            
        key1 = input_int("key");

        distributions[key1].save_hist("hist.txt");
        distributions[key1].save_density("density.txt");
          

        sy
        }

        

        std::getline(std::cin, command);
    }
    
}
