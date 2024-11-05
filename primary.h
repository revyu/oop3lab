#pragma once

#include <vector>
#include <string>

class primary {
private:
	long double v;
	long double mu;
	long double lambda;


public:
	long double density(double x);
	long double mean();
	long double skewness();
	//double density(double& x);
	long double dispersion();
	long double kurtosis();
	 

	double expectation(std::vector <long double> distribution);
	double dispersion(std::vector <long double> distribution);
	double skewness(std::vector <long double> distribution);
	double kurtosis(std::vector <long double> distribution);


	primary(const std::string& input);
	primary(long double v, long double mu, long double lambda);
	primary(const primary& other);


	void set_v(long double v, bool try_again);
	void set_mu(long double mu);
	void set_lambda(long double lambda);
	long double get_v() const;
	long double get_mu() const;
	long double get_lambda() const;
	std::vector<long double> simulate_distribution(int n);
	void save(std::string) const;
	void load(std::string);


};

double input_int(std::string par);

double input_double(std::string par);

