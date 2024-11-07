#pragma once
#include "primary.h"
#include "mixture.h"




class empiric {
private:
	std::vector<long double > distribution;// ������� ��������������� �� �������������
	int n;// ����� ������� 

	std::vector<std::pair<long double, long double>> density;// ������ ����������
	int spn; // split point n - ���������� ����� ��� ��������� 

	std::vector<int> histogramm;// ����������� ��� ������ 
	//�� i ����� ��������� ���������� ����� ���������� � i ��������

	
	std::vector<std::pair<long double, long double >> intervals;
	//������� ����������
	 
	int k;//���������� �������� � ����������� 

	long double delta; // ������ ���������
	long double min;
	long double max;


	void set_k(int k0);
	void set_n(int n);
	void set_spn(int spn);

public:

	// ������� ��� ������� ����� ������
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
	//��� ���������� 

	empiric(std::vector<long double> distribution ,int k0,int spn0);
	empiric(std::string filename, int k0,int spn0);//����������� �� ����� ��������� ������ 



	empiric& operator=(const empiric& emp);

	~empiric();
	

	long double expectation();
	long double dispersion();
	long double skewness();
	long double kurtosis();

	//��������� distribution
	void save(std::string filename);// ����� ��� ���������������
	
	void save_hist(std::string filename);

	void save_density(std::string filename);
	//void load(std::string filename);// ����� �� ? ������ ������ �� ������� ����� ������� �����������?


	//mb later undoc
	// ������ ������ ��� �� �������
	// ����� �� ������� ���� ������������� �����������
	//void set_k(int k0);
	//void set_n(int n);
	//void set_spn(int spn);


};
//save load ?
//����������� �� density?
