#pragma once
#include "primary.h"
#include "empiric.h"

struct component
{
	primary f;
	long double p;

	
};

class mixture {
private:
	
	std::vector <component> components ;
public:
	void set_p(std::vector<long double>);
	

	// Перегрузка оператора [] для доступа по ссылке
	primary& operator[](std::size_t index) {
		return components[index].f; // Возвращаем ссылку на элемент
	}

	mixture(std::vector<primary> primaries, std::vector<long double> p);
	mixture(std::string filename);
	mixture(const mixture& other);
	mixture(std::vector<primary> primaries);


	void save(std::string filename);
	
	void load(std::string filename);


	long double density(double x);
	std::vector<std::pair<long double, long double>> density_vector(int n);
	//double density(double& x);
	long double mean();
	long double dispersion();
	long double skewness();
	long double kurtosis();


	std::vector<long double> simulate_distribution(int n);

	empiric simulate_distribution(int n, int k); 
	

};
//
//данные не должны меняться не явно ?
//mixture.components[0].set_v() не должно менять распределение с которого создавалась mixture ?
