#include "empiric.h"
#include <stdexcept>  // Для std::invalid_argument

#include <cmath>
#include "mixture.h"
#include <fstream>
#include <iostream>


const double pi = 3.1415;


void empiric::set_k(int k0) {
    if (k0 == 1) {
        this->k = std::floor(1.322 * log2(this->n)) + 1;
    }
    else {
        if (k0 > 1) {
            this->k = k;
        }
        else {
            throw std::invalid_argument("k должен быть больше или равен 1");
        }
    }
};

void empiric::set_n(int n) {
    if (n < 2) {
        throw std::invalid_argument("n должен быть больше или равен 2");
    }
    else {
        this->n = n;
    }
}

void empiric::set_spn(int n) {
    if (n < 2) {
        throw std::invalid_argument("n должен быть больше или равен 2");
    }
    else {
        this->n = n;
    }
}

const std::vector<long double>& empiric::getDistribution() const {
    return distribution;
}

int empiric::getN() const {
    return n;
}

const std::vector<std::pair<long double, long double>>& empiric::getDensity() const {
    return density;
}

int empiric::getSpn() const {
    return spn;
}

const std::vector<int>& empiric::getHistogramm() const {
    return histogramm;
}

const std::vector<std::pair<long double, long double>>& empiric::getIntervals() const {
    return intervals;
}

int empiric::getK() const {
    return k;
}

long double empiric::getDelta() const {
    return delta;
}

long double empiric::getMin() const {
    return min;
}

long double empiric::getMax() const {
    return max;
}



// реализация из вектора далее будет юзаться во всех трех конструкторов

empiric::empiric(std::vector<long double> distribution, int k0, int spn0) {


    set_n(distribution.size());
    set_k(k0);
    set_spn(spn0);

    this->distribution = distribution;
    //this->density = prim.density_vector(spn0);

    this->max = *std::min_element(this->distribution.begin(), this->distribution.end());
    this->min = *std::max_element(this->distribution.begin(), this->distribution.end());

    this->delta = (1 / static_cast<float>(k)) * (this->max - this->min);

    //строим интервалы


    // Поиск минимального элемента
    this->intervals = std::vector<std::pair<long double, long double>>(n, std::make_pair(0.0L, 0.0L));

    for (int i = 1; i < k + 1; i++) {
        this->intervals[i - 1].first = this->min + (i - 1) * this->delta;
        this->intervals[i - 1].second = this->min + i * this->delta;

    }


    for (int i = 0; i < this->n; i++) {

        for (int j = 0; j < this->k; j++) {
            if (this->distribution[i] >= this->intervals[j].first) {
                this->histogramm[j]++;
                break;
            }
        }

    }

};


empiric::empiric(int n0, primary& prim ,int k0=1 ,int spn0=10000) {
    
    empiric(prim.simulate_distribution(n),k0,spn0);
    this->density = prim.density_vector(spn0);

}


// to do доделать density
empiric::empiric(int n0, mixture& mixt, int k0 = 1,int spn0=10000)  {

    empiric( mixt.simulate_distribution(n),k0,spn0);
    this->density = mixt.density_vector(spn0);
    
};

empiric::empiric(const empiric& emp) {

    empiric(emp.getDistribution(), emp.getK(), emp.getSpn());

}

empiric& empiric::operator=(const empiric& emp) {
    if (this == &emp) {
        return *this;  // Проверка на самоприсваивание
    }

    

    return *this;
}





empiric::~empiric() {
    // пусто т.к. юзаем вектор
};



long double empiric::expectation() {
    long double sum = 0;
    for (long double val : this->distribution) {
        sum += val;
    }
    return sum / distribution.size();
}


long double empiric::dispersion() {
    double mean = expectation();
    double sum = 0;
    for (long double val : distribution) {
        sum += std::pow(val - mean, 2);
    }
    return sum / (distribution.size() - 1);
}


long double empiric::skewness() {
    double mean = expectation();
    double stdev = std::sqrt(dispersion());
    double sum = 0;
    for (long double val : distribution) {
        sum += std::pow((val - mean) / stdev, 3);
    }
    return sum / distribution.size();
}


long double empiric::kurtosis() {
    double mean = expectation();
    double stdev = std::sqrt(dispersion());
    double sum = 0;
    for (long double val : distribution) {
        sum += std::pow((val - mean) / stdev, 4);
    }
    return sum / distribution.size() - 3;
}


void empiric::save(std::string filename) {
    
    std::ofstream file("output.txt");

    file << this->n;
    for (const auto& item : this->distribution) {
        file << item << " ";
    };



}




//по факту load
empiric::empiric(std::string filename, int k0, int spn0) {

    try {
        // Открываем файл для чтения
        std::ifstream file("output.txt");

        // Проверка на существование файла
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка: файл не существует.");
        }

        int length;
        // Считываем длину массива
        if (!(file >> length) || length <= 0) {
            throw std::runtime_error("Ошибка: неверный формат данных — длина массива должна быть положительным числом.");
        }

        // Создаем вектор для хранения элементов
        std::vector<long double> vector(length);

        // Считываем элементы массива
        for (int i = 0; i < length; ++i) {
            if (!(file >> vector[i])) {
                throw std::runtime_error("Ошибка: неверный формат данных — количество элементов не соответствует указанной длине.");
            }
        }

        // Проверка на наличие лишних данных
        int extraData;
        if (file >> extraData) {
            throw std::runtime_error("Ошибка: в файле присутствуют лишние данные.");
        }

        file.close();

        // Выводим считанный массив на экран для проверки
        
        empiric(vector, k0, spn0);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
};