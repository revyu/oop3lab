// oop3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "empiric.h"
#include "mixture.h"
#include "primary.h"
#include <windows.h> 
#include <fstream> // Добавьте эту строку в начало файла

#include <limits>  // Для std::numeric_limits

#include <cstdlib>
#include <algorithm>
#include <chrono>


int main()
{
    srand(time(0));

    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::string command;


    std::getline(std::cin, command);

    std::vector<primary> primaries(100, primary(1.0, 0.0, 1.0));
    std::vector<primary> single_primary = { primary(1.0, 0.0, 1.0) };
    std::vector<mixture> mixtures(100, mixture(single_primary));
    std::vector<empiric> distributions(100,empiric(3,single_primary[0], 1, 3));

    
    single_primary[0].set_v(2.0,false);
    empiric emp1(1000,single_primary[0], 1, 10000);
    single_primary[0].set_v(3.0, false);
    //std::cout << emp1.get_primary_source()->get_v();
    empiric emp2 = emp1;

    //emp2.get_primary_source()->set_v(3.0,false); ЗА ЭТО РАЗБАНА НЕ БУДЕТ потому что const
    
    
    //std::cout << emp2.get_primary_source()->get_v();

    //std::cout <<"emp2 max" << emp2.getMax() << std::endl;

    empiric emp3(10000, emp2, 1, 10000);

    //std::cout <<"emp3 max" << emp3.getIntervals << std::endl;

    for (int i = 0; i < emp3.getK(); i++) {
        
        std::cout << emp3.getIntervals()[i].first<< emp3.getIntervals()[i].second << std::endl;
    }

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

            std::cin.ignore(1000, '\n'); //Игнорируем оставшийся символ новой строки
            std::getline(std::cin, command);

            if (command == "primary") {
                primaries[key] = primary(" ");//вызов будет из консоли (попросит ввести n,v,mu, и т.д.)
            }
            else if (command == "mixture") {
                key1 = input_int("key");
                key2 = input_int("key");

                mixtures[key] = mixture({ primaries[key1],primaries[key2] });

            }
            

            if (command == "empiric") {
                
                std::cout << "Откуда?"<<std::endl;
                //std::cin.ignore(1000, '\n');
                std::getline(std::cin, command);
                

                key1 = input_int("key1 : ключ распределения которое симулируем "); // вводим ключ откуда хотим получить
                n0 = input_int("n0");
                k0 = input_int("k0");
                spn = input_int("spn");

                if (command == "mixture") {
                    distributions[key1] = empiric(n0, mixtures[key1], k0, spn);
                };

                if (command == "primary") {
                    distributions[key1] = empiric(n0, primaries[key1], k0, spn);
                };

                
            }

             
        }

        //to do : maybe сделать так что бы можно было показывать прямо из prim и mix не создавая перед этим empiric?
        if (command == "show") {
            
            
        key1 = input_int("key");
        std::cout << "kal";
        distributions[key1].save_hist("hist1.txt");
        distributions[key1].save_density("density1.txt");
         
        for (int i = 0; i < emp3.getK(); i++) {

            std::cout << emp3.getIntervals()[i].first << emp3.getIntervals()[i].second << std::endl;
        }

            system("python hist1.py");
        }

        //to do 
        // надо доделать эту конструкцию
        // для этого необходимо добавить сохранение копии mixture и primary (вот и глубокое копирование понадобилось) 
        // в обьект empiric 

        // еще надо добавить сейвы и лоадеры (все таки для галочки надо сделать лоадеры для всех ) в интерфейсы
        // интерфейсы на get и set (сделаем set для галочки)
        // 
        // добавить команду на замер времени  
        if (command == "show2et") {


            key1 = input_int("key");

            distributions[key1].save_hist("hist1.txt");
            distributions[key1].save_density("density.txt");

            
            empiric copy(distributions[key1].getN(), distributions[key1], distributions[key1].getK(), distributions[key1].getSpn());

            copy.save_hist("hist2.txt");


            system("python draw.py hist2");
        }

        

        std::getline(std::cin, command);
    }
    
}
