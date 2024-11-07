#pragma once
#include "primary.h"
#include "mixture.h"




class empiric {
private:
	std::vector<long double > distribution;// выборка сгенерированная из распределения
	int n;// длина выборки 

	std::vector<std::pair<long double, long double>> density;// массив плотностей
	int spn; // split point n - количество точек для разбиения 

	std::vector<int> histogramm;// гистограмма для показа 
	//на i месте находится количество точек попадающих в i интервал

	
	std::vector<std::pair<long double, long double >> intervals;
	//границы интервалов
	 
	int k;//количество столбцов в гистограмме 

	long double delta; // ширина интервала
	long double min;
	long double max;


	void set_k(int k0);
	void set_n(int n);
	void set_spn(int spn);

public:

	// Геттеры для каждого члена данных
	const std::vector<long double>& getDistribution() const;
	int getN() const;
	const std::vector<std::pair<long double, long double>>& getDensity() const;
	int getSpn() const;
	const std::vector<int>& getHistogramm() const;
	const std::vector<std::pair<long double, long double>>& getIntervals() const;
	int getK() const;
	long double getDelta() const;
	long double getMin() const;
	long double getMax() const;

	

	empiric(int n0, primary& prim, int k0 , int spn0);
	empiric(int n0, mixture& mixt, int k0 ,int spn0);  
	

	empiric(int n0, empiric& emp, int k0, int spn0);
	
	empiric(const empiric& emp);
	//чем отличаются 

	empiric(std::vector<long double> distribution ,int k0,int spn0);
	empiric(std::string filename, int k0,int spn0);//конструктор из файла принимает массив 



	empiric& operator=(const empiric& emp);

	~empiric();
	

	long double expectation();
	long double dispersion();
	long double skewness();
	long double kurtosis();

	//сохраняет distribution
	void save(std::string filename);// нужна для персистенстости
	
	void save_hist(std::string filename);

	void save_density(std::string filename);
	//void load(std::string filename);// нужна ли ? почему просто не сделать сразу готовый конструктор?


	//mb later undoc
	// нельзя просто так их вернуть
	// когда их меняешь надо пересчитывать гистограмму
	//void set_k(int k0);
	//void set_n(int n);
	//void set_spn(int spn);


};
//save load ?
//конструктор по density?
