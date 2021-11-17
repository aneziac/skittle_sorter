from matplotlib import pyplot as plt
import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.multiclass import OneVsRestClassifier
from sklearn.svm import LinearSVC
from sklearn.neighbors import KNeighborsClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.linear_model import RidgeClassifier


colors = ['green', 'none', 'red', 'purple', 'orange', 'yellow']
color_dict = {'R': 0, 'G': 1, 'B': 2}
plot_vs = ['G', 'B']

n_samples_per_color = 21
n_colors = len(colors)
X = np.zeros((n_samples_per_color * n_colors, 3))
y = np.zeros((n_samples_per_color * n_colors, n_colors))

for c, color in enumerate(colors):
    color_data = open(f'calibration_data/{color}.txt', 'r')
    color_array = np.zeros([21, 3])

    for i, line in enumerate(color_data.readlines()):
        color_array[i, :] = line.rstrip().split('\t')[1::2]

        k = c * n_samples_per_color + i
        X[k] = color_array[i]
        y[k, c] = 1

        color_idx = [color_dict[plot_vs[0]], color_dict[plot_vs[1]]]
        plt.scatter(color_array[:, color_idx[0]], color_array[:, color_idx[1]], marker='o', color=color)

    print(repr(color_array.astype(int)))

#classifier = OneVsRestClassifier(LinearSVC())
classifier = OneVsRestClassifier(KNeighborsClassifier(n_neighbors=5))
#classifier = OneVsRestClassifier(LogisticRegression())
#classifier = OneVsRestClassifier(DecisionTreeClassifier())
#classifier = OneVsRestClassifier(RidgeClassifier())
classifier.fit(X, y)
print(classifier.score(X, y))

plt.xlim(0, 350)
plt.ylim(0, 350)
plt.xlabel(plot_vs[0])
plt.ylabel(plot_vs[1])
plt.show()
