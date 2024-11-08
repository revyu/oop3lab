
import matplotlib.pyplot as plt
import seaborn as sns
import os 
from sklearn.preprocessing import MinMaxScaler
import numpy as np
import argparse 


def hist1():
    # Чтение данных из файла hist.txt
    with open(fr"{os.getcwd()}\hist1.txt", "r") as file:
        k, n = map(int, file.readline().split())
        lines = file.readlines()
        
        intervals_l = [float(i.split()[0]) for i in lines]
        intervals_r = [float(i.split()[1]) for i in lines]
        density = [(float(i.split()[2]) / n) for i in lines]

        max_hist=max(density)

    # Чтение данных из файла density.txt
    with open(fr"{os.getcwd()}\density1.txt", "r") as file:
        density_lines = file.readlines()
        
        x_density = [float(i.split()[0]) for i in density_lines]
        y_density = [float(i.split()[1]) for i in density_lines]

        max_density=max(y_density)

        y_density=[float(i.split()[1])*max_hist/max_density for i in density_lines]

    # Отображение плотности с помощью lineplot
    sns.lineplot(x=x_density, y=y_density, label='Density Line')

    # Добавление столбцов (гистограммы)
    for left, right, d in zip(intervals_l, intervals_r, density):
        plt.bar((left + right) / 2, d, width=(right - left), align='center', color='skyblue', edgecolor='black', alpha=0.5)

    plt.xlabel("X-axis")
    plt.ylabel("Density")
    plt.legend()
    plt.show()

hist1()