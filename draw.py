
import matplotlib.pyplot as plt
import seaborn as sns
import os 
from sklearn.preprocessing import MinMaxScaler
import numpy as np
import argparse 


def density():
    # Загрузка данных из файла
    with open(fr"{os.getcwd()}\density.txt", "r") as file:
        density = file.readlines()

        x_density=[float(i.split()[0]) for i in density ]
        y_density=[float(i.split()[1]) for i in density ]


    sns.lineplot(x=x_density,y=y_density)


    plt.show()


# будет показывать гистограмму столбцами и теоретическую на density 
def hist1():
    # Чтение данных из файла hist.txt
    with open(fr"{os.getcwd()}\hist.txt", "r") as file:
        k, n = map(int, file.readline().split())
        lines = file.readlines()
        
        intervals_l = [float(i.split()[0]) for i in lines]
        intervals_r = [float(i.split()[1]) for i in lines]
        density = [(float(i.split()[2]) / n) for i in lines]

        max_hist=max(density)

    # Чтение данных из файла density.txt
    with open(fr"{os.getcwd()}\density.txt", "r") as file:
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


def hist2():
    # Чтение данных из первого файла hist.txt
    with open(fr"{os.getcwd()}\hist1.txt", "r") as file:
        k1, n1 = map(int, file.readline().split())
        lines = file.readlines()
        
        intervals_l1 = [float(i.split()[0]) for i in lines]
        intervals_r1 = [float(i.split()[1]) for i in lines]
        density1 = [(float(i.split()[2]) / n1) for i in lines]
        
        max_hist1 = max(density1)

    # Чтение данных из второго файла hist2.txt
    with open(fr"{os.getcwd()}\hist2.txt", "r") as file:
        k2, n2 = map(int, file.readline().split())
        lines = file.readlines()
        
        intervals_l2 = [float(i.split()[0]) for i in lines]
        intervals_r2 = [float(i.split()[1]) for i in lines]
        density2 = [(float(i.split()[2]) / n2) for i in lines]
        
        max_hist2 = max(density2)

    # Чтение данных теоретического распределения из файла density.txt
    with open(fr"{os.getcwd()}\density.txt", "r") as file:
        density_lines = file.readlines()
        
        x_density = [float(i.split()[0]) for i in density_lines]
        y_density = [float(i.split()[1]) for i in density_lines]

        # Приведение y_density к масштабу первого набора данных
        max_density = max(y_density)
        y_density = [y * max(max_hist1, max_hist2) / max_density for y in y_density]

    # Отображение теоретического распределения с помощью lineplot
    sns.lineplot(x=x_density, y=y_density, label='Theoretical Density')

    # Добавление столбцов для первого набора данных
    for left, right, d in zip(intervals_l1, intervals_r1, density1):
        plt.bar((left + right) / 2, d, width=(right - left), align='center', 
                color='skyblue', edgecolor='black', alpha=0.5, label='Histogram 1' if left == intervals_l1[0] else "")

    # Добавление столбцов для второго набора данных
    for left, right, d in zip(intervals_l2, intervals_r2, density2):
        plt.bar((left + right) / 2, d, width=(right - left), align='center', 
                color='salmon', edgecolor='black', alpha=0.5, label='Histogram 2' if left == intervals_l2[0] else "")

    plt.xlabel("X-axis")
    plt.ylabel("Density")
    plt.legend()
    plt.show()



if __name__ == "__main__":
    # Использование argparse для обработки аргументов командной строки
    parser = argparse.ArgumentParser(description="Draw histograms with theoretical density.")
    parser.add_argument("mode", nargs='?', default="hist1", help="Specify 'hist2' to draw the second histogram, otherwise 'hist1' will be used.")
    args = parser.parse_args()

    # Выбор функции в зависимости от аргумента
    if args.mode == "hist2":
        hist2()
    else:
        hist1()



        


    


