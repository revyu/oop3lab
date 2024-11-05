import matplotlib.pyplot as plt
import seaborn as sns
import os 

# Загрузка данных из файла
with open(fr"{os.getcwd()}\density.txt", "r") as file:
    density = file.readlines()

    x_density=[float(i.split()[0]) for i in density ]
    y_density=[float(i.split()[1]) for i in density ]

with open(fr"{os.getcwd()}\data.txt", "r") as file:
    data = file.readlines()

print(density[:10])
print(x_density[:10])
print(y_density[:10])
sns.lineplot(x=x_density,y=y_density)


plt.show()