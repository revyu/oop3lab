
import matplotlib.pyplot as plt

# Загрузка данных из файла
with open(r"C:\Users\revya\OneDrive\Рабочий стол\work\c++ projects\oop3\oop3\data.txt", "r") as file:
    data = [float(line.strip()) for line in file]

# Построение гистограммы
plt.hist(data, bins=10, edgecolor='black')
plt.xlabel('Значения')
plt.ylabel('Частота')
plt.title('Гистограмма данных из файла')
plt.show()
print()