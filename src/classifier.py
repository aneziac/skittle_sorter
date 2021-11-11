from matplotlib import pyplot as plt
import numpy as np


colors = ['green', 'none', 'red', 'purple', 'orange', 'yellow']
color_dict = {'R': 0, 'G': 1, 'B': 2}
plot_vs = ['R', 'B']

fig = plt.figure()
ax1 = fig.add_subplot(111)


for color in colors:
    color_data = open(f'calibration_data/{color}.txt', 'r')
    color_array = np.zeros([14, 3])

    for i, line in enumerate(color_data.readlines()):
        color_array[i,:] = line.rstrip().split('\t')[1::2]
        color_idx = [color_dict[plot_vs[0]], color_dict[plot_vs[1]]]
        plt.scatter(color_array[:, color_idx[0]], color_array[:, color_idx[1]], marker='o', color=color)


plt.xlim(0, 350)
plt.ylim(0, 350)
plt.xlabel(plot_vs[0])
plt.ylabel(plot_vs[1])
plt.show()
