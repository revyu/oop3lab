#include "mixture.h"
#include <cmath>
#include <fstream>
#include <random>       // Для std::random_device, std::mt19937, std::uniform_real_distribution
#include <algorithm>    // Для std::lower_bound, если используется в поиске по CDF
#include <iostream>
#include "empiric.h"


//инициализируется равновероятным
mixture::mixture(std::vector<primary> primaries_) {
	std::vector<primary> primaries(primaries_);
	std::vector<long double> p(primaries.size(),1.0/primaries.size());


	//создаем "placeholder" дальше в него считываем значения
	// lil bit crappy who cares
	component c = {primary(1.0,0.0,1.0),0};
	for (int i = 0; i < primaries.size();i++){

		components.push_back(c);
		components[i].f = primaries[i];
		components[i].p = p[i];
	}

};


mixture::mixture(std::vector<primary> primaries_, std::vector<long double> p_) {
	std::vector<primary> primaries(primaries_);
	std::vector<long double> p(p_);

	if (primaries.size() != p.size()) {
		throw std::runtime_error("Размеры векторов primary и p не совпадают");
	}

	mixture mix(primaries);

}


mixture::mixture(const mixture& other) {
	
	component c = { primary(1.0,0.0,1.0),0 };
	for (int i = 0; i < other.components.size(); i++) {

		this->components.push_back(c);
		this->components[i].f = other.components[i].f;
		this->components[i].p= other.components[i].p;
	}


}

long double mixture::density(double x) {
	long double res = 0;

	for (component c: components) {
		res = res + c.p*c.f.density(x);
	
	}

	return res;
}

long double mixture::mean() {
	long double res = 0;

	for (component c: components) {
		res = res + c.p*c.f.mean();

	}

	return res;

}

long double mixture::dispersion() {
	long double res = 0;
	
	long double mean_v = this->mean();

	for (component c : components) {
		res = res + c.p * (std::pow(c.f.mean(),2) +c.f.dispersion());
	}

	return res - std::pow(mean_v, 2);
}



long double mixture::skewness() {

	long double res = 0;
	long double dispersion_v = this->dispersion();
	long double mean_v = this->mean();

	for (component c : components) {
	
		res=res+c.p* (std::pow(c.f.mean() - mean_v, 3) + 
			3 * (c.f.mean() - mean_v) * c.f.dispersion() 
			+ std::pow(c.f.dispersion(), 1.5) * c.f.skewness()
			);
	}

	return res / std::pow(dispersion_v, 1.5);
}


long double mixture::kurtosis() {
	long double res = 0;
	long double dispersion_v = this->dispersion();
	long double mean_v = this->mean();

	for (component c : components) {
		res += c.p * (std::pow(c.f.mean() - mean_v, 4)
			+ 6 * std::pow(c.f.mean() - mean_v, 2) * c.f.dispersion()
			+ 4 * (c.f.mean() - mean_v) * std::pow(c.f.dispersion(), 1.5) * c.f.skewness()
			+ std::pow(c.f.dispersion(), 2) * (c.f.kurtosis() + 3));
	}

	res /= std::pow(dispersion_v, 2); // Делим результат на (Dξ)^2
	res -= 3; // Вычитаем 3 из финального результата, чтобы получить kurtosis

	return res;
}



void mixture::set_p(std::vector<long double > ps) {

	long double res = 0;

	for (long double p : ps) {
		if (res < 0) {
			throw - 1;
		}
		res += p;

	}

	if (std::abs(res - 1) < 1e-6) {
		
		for (int i = 0; i < ps.size();i++) {
			components[i].p = ps[i];
		}

	}

};

void mixture::save(std::string filename) {
	std::ofstream file(filename);

	for (component c : components) {
		file << c.p<<" " << c.f.get_lambda() << " " << c.f.get_mu() << " " <<c.f.get_v()<<std::endl;
	}

};

void mixture::load(std::string filename) {
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("Ошибка открытия файла для загрузки данных");
	}

	components.clear(); // Очищаем существующие компоненты перед загрузкой новых

	long double p, lambda, mu, v;
	while (file >> p >> lambda >> mu >> v) {
		primary prim(v, mu, lambda); // Создаем объект primary с считанными параметрами
		component c = { prim, p };     // Создаем компонент с объектом primary и значением p
		components.push_back(c);     // Добавляем компонент в вектор
	}

	file.close();
}


std::vector<long double> mixture::simulate_distribution(int n, int spn = 10000) {
	 

	long double sigma = std::sqrt(this->dispersion()); // Замените на актуальное значение сигмы
	long double min_x = this->mean() -   sigma;
	long double max_x = this->mean() +  sigma;
	long double step = (max_x - min_x) / (spn - 1);

	// 1. Создаём массив значений x и вычисляем плотности
	std::vector<long double> x_values(spn);
	std::vector<long double> densities(spn);
	for (int i = 0; i < spn; ++i) {
		x_values[i] = min_x + i * step;
		densities[i] = density(x_values[i]);
	}

	// 2. Строим префиксные суммы для CDF
	std::vector<long double> cdf(spn);
	cdf[0] = densities[0];
	for (int i = 1; i < spn; ++i) {
		cdf[i] = cdf[i - 1] + densities[i];

		//std::cout <<"x_i "<<x_values[i] << " density " << densities[i] << " cdf " << cdf[i] << std::endl;
		

	}

	// 3. Нормализуем CDF, чтобы последний элемент стал равен 1
	for (auto& value : cdf) {
		value /= cdf.back();
	}

	for (auto& value : cdf) {
		//std::cout << value;
	}
	

	// 4. Генерируем выборку на основе CDF
	std::vector<long double> distribution(n);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> uniform_dist(0.0, 1.0);

	for (int i = 0; i < n; ++i) {
		long double rand_val = uniform_dist(gen);

		// Находим первую точку в CDF, которая >= rand_val
		auto it = std::lower_bound(cdf.begin(), cdf.end(), rand_val);
		int index = std::distance(cdf.begin(), it);

		// Используем соответствующее значение x
		distribution[i] = x_values[index];
	}

	std::cout << " это из mixture.cpp" <<n ;

	return distribution;
	
}

std::vector<std::pair<long double, long double>> mixture::density_vector(int spn) {
	
	int num_points = spn; // Количество точек для дискретизации CDF

	long double sigma = std::sqrt(this->dispersion()); // Замените на актуальное значение сигмы
	long double min_x = this->mean() -2* sigma;
	long double max_x = this->mean() + 2*sigma;
	long double step = (max_x - min_x) / (num_points - 1);

	// 1. Создаём массив значений x и вычисляем плотности
	std::vector<long double> x_values(num_points);
	std::vector<long double> densities(num_points);
	for (int i = 0; i < num_points; ++i) {
		x_values[i] = min_x + i * step;
		densities[i] = density(x_values[i]);
	}

	std::vector<std::pair<long double, long double>> res(spn);

	for (int i = 0; i < spn; i++) {
		
		res[i].first = x_values[i];
		res[i].second = densities[i];
	}

	return res;
}

// тоже самое что empiric(int n,mixture &mix,int k)
empiric mixture:: simulate_distribution(int n, int k,int spn=10000) {
	
	return empiric(n, *this, k, spn);

};