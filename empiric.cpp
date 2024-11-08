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
            this->k = k0;
            
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

void empiric::set_spn(int spn0) {
    if (n < 2) {
        throw std::invalid_argument("n должен быть больше или равен 2");
    }
    else {
        this->spn = spn0;
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



 primary* empiric::get_primary_source()  {
    return primary_source;  // Возвращает указатель на primary или nullptr
}

 mixture* empiric::get_mixture_source()  {
    return mixture_source;  // Возвращает указатель на mixture или nullptr
}


// реализация из вектора далее будет юзаться во всех трех конструкторов

empiric::empiric(std::vector<long double> distribution0, int k0, int spn0) {

    

    set_n(distribution0.size());

    
    this->set_k(k0);
    
    
    set_spn(spn0);

    this->distribution = distribution0;


    //this->density = prim.density_vector(spn0);
   
   

    this->max = *std::max_element(this->distribution.begin(), this->distribution.end());
    this->min = *std::min_element(this->distribution.begin(), this->distribution.end());

    this->delta = (1 / static_cast<float>(k)) * (this->max - this->min);

    //строим интервалы


    // Поиск минимального элемента
    this->intervals = std::vector<std::pair<long double, long double>>(k, std::make_pair(0.0L, 0.0L));
    
    for (int i = 1; i < k + 1; i++) {
       
        this->intervals[i - 1].first = this->min + (i - 1) * this->delta;
        this->intervals[i - 1].second = this->min + i * this->delta;
       
    }
    
    this->histogramm = std::vector<int>(k);

    
   


    for (int i = 0; i < this->n; i++) {
        //std::cout << i<<std::endl;
        for (int j = 0; j < this->k; j++) {
            if (this->distribution[i] <= this->intervals[j].second) {
                this->histogramm[j]++;
                break;
            }
        }

    }
    

};


empiric::empiric(int n0, primary& prim ,int k0=1 ,int spn0=10000) :empiric(prim.simulate_distribution(n0), k0, spn0) {
    
    this->density = prim.density_vector(spn0);
    this->primary_source = new primary(prim);  // Создаем копию объекта prim
    this->mixture_source = nullptr;  // Устанавливаем mixture_source в nullptr, поскольку он не используется

}


// to do доделать density
empiric::empiric(int n0, mixture& mixt, int k0 = 1,int spn0=10000) :empiric(mixt.simulate_distribution(n0,spn0), k0, spn0) {


    this->density = mixt.density_vector(spn0);
    this->primary_source = nullptr;  // Создаем копию объекта prim
    this->mixture_source = new mixture(mixt);
};

empiric::empiric(int n0, empiric& emp, int k0, int spn0)
    : empiric(
        emp.get_primary_source() ? emp.get_primary_source()->simulate_distribution(n0)
        : emp.get_mixture_source() ? emp.get_mixture_source()->simulate_distribution(n0, spn0)
        : throw std::runtime_error("Не установлен ни primary, ни mixture источник"),
        k0, spn0) // Используем список инициализации для вызова другого конструктора
{
    // Если источник - primary, то устанавливаем density и source
    if (emp.get_primary_source()) {
        this->density = emp.get_primary_source()->density_vector(spn0);
        this->primary_source = new primary(*emp.get_primary_source());
        this->mixture_source = nullptr;
    }
    // Если источник - mixture, то устанавливаем density и source
    else if (emp.get_mixture_source()) {
        this->density = emp.get_mixture_source()->density_vector(spn0);
        this->mixture_source = new mixture(*emp.get_mixture_source());
        this->primary_source = nullptr;
    }
}



empiric::empiric(const empiric& emp) {
    // Копируем данные
    this->distribution = emp.distribution;
    this->n = emp.n;
    this->density = emp.density;
    this->spn = emp.spn;
    this->histogramm = emp.histogramm;
    this->intervals = emp.intervals;
    this->k = emp.k;
    this->delta = emp.delta;
    this->min = emp.min;
    this->max = emp.max;

    // Создаем копию источника распределения, если он задан
    if (emp.primary_source) {
        this->primary_source = new primary(*emp.primary_source);
    }
    if (emp.mixture_source) {
        this->mixture_source = new mixture(*emp.mixture_source);
    }
}

empiric& empiric::operator=(const empiric& emp) {
    if (this != &emp) {  // Проверка на самоприсваивание
        // Копируем данные
        this->distribution = emp.distribution;
        this->n = emp.n;
        this->density = emp.density;
        this->spn = emp.spn;
        this->histogramm = emp.histogramm;
        this->intervals = emp.intervals;
        this->k = emp.k;
        this->delta = emp.delta;
        this->min = emp.min;
        this->max = emp.max;

        // Создаем копию источника распределения, если он задан
        if (emp.primary_source) {
            // Удаляем старый объект и создаем новый
            //delete this->primary_source;
            this->primary_source = new primary(*emp.primary_source);
        }
        else {
            //delete this->primary_source;
            this->primary_source = nullptr;
        }

        if (emp.mixture_source) {
            // Удаляем старый объект и создаем новый
            delete this->mixture_source;
            this->mixture_source = new mixture(*emp.mixture_source);
        }
        else {
            delete this->mixture_source;
            this->mixture_source = nullptr;
        }
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
    
    std::ofstream file(filename);

    file << this->n<<std::endl;
    for (const auto& item : this->distribution) {
        file << item << " ";
    };



}


// по факту сохраняет и гистограмму и интервалы 
// на первой строке k
// потом идет k строк вида граница интервала количество значений в нем 

void empiric::save_hist(std::string filename) {

    std::ofstream file(filename);


    std::cout << "from save hist";
    for (int i = 0; i < this->k; i++) {

        std::cout << this->getIntervals()[i].first << this->getIntervals()[i].second << std::endl;
    }

    file << this->k <<" "<< this->n << std::endl;
    for (int i = 0; i < this->k; i++) {
        file << this->intervals[i].first << " " << this->intervals[i].second << " " <<this->histogramm[i]<<std::endl;
    }

    file.close();
};

//сохраняет ключ пары x mixt.density(x) на участке +- 3 сигмы
void empiric::save_density(std::string filename) {
    

    std::ofstream file(filename);
    
    for (int i = 0; i < this->spn; i++) {
        file << this->density[i].first << " " << this->density[i].second <<  std::endl;
    }

    file.close();
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