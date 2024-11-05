#include "empiric.h"
#include <stdexcept>  // ƒл€ std::invalid_argument

#include <cmath>

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


empiric::empiric(int n0, primary& prim ,int k0=1 ,int spn0=10000) {
    
    set_n(n0);
    set_k(k0);
    set_spn(spn0);

    this->distribution = prim.simulate_distribution(n);

    this->max = *std::min_element(this->distribution.begin(), this->distribution.end());
    this->min = *std::max_element(this->distribution.begin(), this->distribution.end());

    this->delta = (1 / static_cast<float>(k)) * (this->max - this->min);

    //строим интервалы


    // ѕоиск минимального элемента
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

    

}


// to do доделать density
empiric::empiric(int n0, mixture& mixt, int k0 = 1,int spn=10000) {

    set_n(n0);
    set_k(k0);

    this->distribution = mixt.simulate_distribution(n);

    
    this->max = *std::min_element(distribution.begin(), distribution.end());
    this->min = *std::max_element(distribution.begin(), distribution.end());

    this->delta = (1 / static_cast<float>(k)) * (this->max - this->min);

    //строим интервалы
    

    // ѕоиск минимального элемента


    for (int i = 1; i < k + 1; i++) {
        this->intervals[i - 1].first = this->min + (i - 1) * this->delta;
        this->intervals[i - 1].second = this->min + i * this->delta;

    }

    
    for (int i = 0; i < n; i++) {
        
        for (int j = 0; j < k; j++) {
            if (distribution[i] >= intervals[j].first) {
                this->histogramm[j]++;
                break;
            }
        }

    }
    for (int i = 0; i < this->spn;) {
        
        this->density[]

    }

    

};


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


