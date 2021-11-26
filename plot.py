import matplotlib.pyplot as plt
from numpy import polyfit

N = []
T = []

with open("data.txt", 'r') as data_file:
    for line in data_file.readlines():
        n, time = map(int, line.split())
        N.append(n)
        T.append(time)


p = polyfit(N, T, deg=1)
plt.plot(N, T, 'o', color='red')
plt.plot(N, [p[0]*n + p[1] for n in N], 'o', color='blue')
plt.ylabel("time in microseconds")
plt.xlabel("number of vertices + number of edges")
plt.show()
