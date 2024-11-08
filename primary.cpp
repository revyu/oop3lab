#include "primary.h"
#include <cmath>
#include <stdexcept>
#include <fstream>
#include <iostream>

const double pi = 3.1415;



long double primary::density(double x) {
    auto f = [](double x, long double v) {
        return (v / (2 * tgamma(1 / v))) * std::exp(-std::pow(std::abs(x), v));
    };
    return f((x - mu) / lambda, v) / lambda;
}

long double primary::mean() {
    return get_mu();

}

//for compatibility reasons
long double primary::skewness() {
    return 0;

}


double input_int(std::string par) {

    int n;

    bool is_success;


    do {
        is_success = true;

        std::string str;
        std::cout << "Введите параметр " << par << ":";
        std::cin >> str;
        // Проверяем, что строка не пуста
        if (str.empty()) {
            std::cout << "Неподходящее значение " << par << ":";

            is_success = false;

        }


        // Проверяем каждый символ строки
        for (size_t i = 0; i < str.length(); ++i) {
            char c = str[i];
            if (!std::isdigit(c)) {
                std::cout << "Неподходящее значение  " << par << ":";
                is_success = false;
                break;
            }
        }

        // Преобразуем строку в число и проверяем, что оно больше нуля
        n = std::stoi(str);

        if (n <= 0) {
            std::cout << "Неподходящее значение " << par << ":";
            is_success = false;

        }


    } while (not is_success);
    return n;
}

double input_double(std::string par) {

    double v;

    bool is_success;
    bool less_then_zero = false;
    std::string str;
    do {
        is_success = true;

        std::cout << "Введите параметр " << par << ":";
        std::cin >> str;
        // Проверяем, что строка не пуста
        if (str.empty()) {
            std::cout << "Неподходящее значение " << par << ":";

            is_success = false;
            break;
        }

        if (str == ".") {
            std::cout << "Неподходящее значение " << par << ":";

            is_success = false;
            break;
        }

        bool decimalPointSeen = false;
        //добавить обработку минуса
        // Проверяем каждый символ строки



        if (str[0] == '-') {
            less_then_zero = true;
            str.erase(0, 1);
        }



        for (size_t i = 0; i < str.length(); ++i) {
            char c = str[i];
            // Допускаем одну десятичную точку
            if (c == '.') {
                if (decimalPointSeen) {
                    std::cout << "Неподходящее значение " << par << ":";

                    is_success = false;
                    break;
                }
                decimalPointSeen = true;
            }
            // Разрешаем цифры и одну десятичную точку
            else if (!std::isdigit(c)) {
                std::cout << "Неподходящее значение " << par << ":";
                is_success = false;
                break;
            }
        }

        //is_success = true;//не останавливает плохие строки бтв

    } while (not is_success);

    v = std::stod(str);

    if (less_then_zero) {
        return -v;
    }
    else {
        return v;
    }

}


long double primary::dispersion() {
    return lambda * tgamma(3 / v) / tgamma(1 / v) + mu;
}

long double primary::kurtosis() {
    return lambda * lambda * ((tgamma(5 / v) * tgamma(1 / v)) / (std::pow(tgamma(3 / v), 2)) - 3);
}


double primary::expectation(std::vector<long double> distribution) {
    long double sum = 0;
    for (long double val : distribution) {
        sum += val;
    }
    return sum / distribution.size();
}


double primary::dispersion(std::vector<long double> distribution) {
    double mean = expectation(distribution);
    double sum = 0;
    for (long double val : distribution) {
        sum += std::pow(val - mean, 2);
    }
    return sum / (distribution.size() - 1);
}


double primary::skewness(std::vector<long double> distribution) {
    double mean = expectation(distribution);
    double stdev = std::sqrt(dispersion(distribution));
    double sum = 0;
    for (long double val : distribution) {
        sum += std::pow((val - mean) / stdev, 3);
    }
    return sum / distribution.size();
}


double primary::kurtosis(std::vector<long double> distribution) {
    double mean = expectation(distribution);
    double stdev = std::sqrt(dispersion(distribution));
    double sum = 0;
    for (long double val : distribution) {
        sum += std::pow((val - mean) / stdev, 4);
    }
    return sum / distribution.size() - 3;
}


void primary::set_v(long double v, bool try_again = true) {
    if (v >= 1) {
        this->v = v;
    }
    else if (try_again) {
        while (v < 1) {
            std::cout << "Неподходящее значение v : v должно быть больше 1" << std::endl;
            v = input_double("v");
        }
    }
    else {
        throw std::runtime_error("v должно быть больше 1");
    }
}

void primary::set_mu(long double mu) {
    this->mu = mu;
}

void primary::set_lambda(long double lambda) {
    this->lambda = lambda;
}

primary::primary(long double v, long double mu, long double lambda) {
    set_v(v); set_mu(mu); set_lambda(lambda);
}

primary::primary(const std::string& input)
    : v(input_double("v")), mu(input_double("mu")), lambda(input_double("lambda")) {
    // Здесь можно добавить дополнительную логику конструктора, если необходимо
}

primary::primary(const primary& other) {
    this->v = other.get_v();
    this->mu = other.get_mu();
    this->lambda = other.get_lambda();
    
};





long double primary::get_v() const{
    return v;
}

long double primary::get_mu() const{
    return mu;
}

long double primary::get_lambda() const {
    return lambda;
}


std::vector<long double> primary::simulate_distribution(int n) const {
    std::vector<long double> distribution(n);
    for (int i = 0; i < n; i++) {
        long double x = 0;
        if (v >= 0 && v < 2) {
            long double const a = 1 / v - 1;
            long double const b = 1 / std::pow(v, 1 / v);

            long double r1 = 0;
            long double r2 = 0;
            do {
                r1 = rand() / static_cast<long double>(RAND_MAX);
                if (r1 <= 0.5) {
                    x = b * std::log(2 * r1);
                }
                else {
                    x = -b * std::log(2 * (1 - r1));
                }
                r2 = rand() / static_cast<long double>(RAND_MAX);
            } while ((std::log(r2) > -1 * std::pow(std::abs(x), v) + std::abs(x) / b + a) || std::isinf(x));
        }
        else if (v >= 2) {
            long double const a = 1 / v - 0.5;
            long double const b = 1 / std::pow(v, 1 / v);
            long double const c = 2 * b * b;

            long double r1 = 0;
            long double r2 = 0;
            long double r3 = 0;
            do {
                r1 = rand() / static_cast<long double>(RAND_MAX);
                r2 = rand() / static_cast<long double>(RAND_MAX);
                x = b * std::sqrt(-2.0 * std::log(r1)) * std::cos(2.0 * pi * r2);
                r3 = rand() / static_cast<long double>(RAND_MAX);
            } while ((std::log(r3) > -1 * std::pow(std::abs(x), v) + std::abs(x) / b + a) || std::isinf(x));
        }
        distribution[i] = x * lambda + mu;
    }
    return distribution;
}


void primary::save(std::string file_name) const {
    std::ofstream outFile(file_name);
    if (!outFile) {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
        return;
    }
    outFile << v << " " << mu << " " << lambda << std::endl;
}


void primary::load(std::string file_name) {
    std::ifstream inFile(file_name);
    if (!inFile) {
        std::cerr << "Не удалось открыть файл для чтения." << std::endl;
        return;
    }
    if (!(inFile >> v >> mu >> lambda)) {
        std::cerr << "Ошибка при чтении данных из файла." << std::endl;
    }
}


std::vector<std::pair<long double, long double>> primary::density_vector(int n) {

    int num_points = n; // Количество точек для дискретизации CDF

    long double sigma = std::sqrt(this->dispersion()); // Замените на актуальное значение сигмы
    long double min_x = this->mean() - 3 * sigma;
    long double max_x = this->mean() + 3 * sigma;
    long double step = (max_x - min_x) / (num_points - 1);

    // 1. Создаём массив значений x и вычисляем плотности
    std::vector<long double> x_values(num_points);
    std::vector<long double> densities(num_points);
    for (int i = 0; i < num_points; ++i) {
        x_values[i] = min_x + i * step;
        densities[i] = density(x_values[i]);
    }

    std::vector<std::pair<long double, long double>> res(n);

    for (int i = 0; i < n; i++) {

        res[i].first = x_values[i];
        res[i].second = densities[i];
    }

    return res;
}