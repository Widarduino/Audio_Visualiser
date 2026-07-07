import numpy as np
import matplotlib.pyplot as plt

#print("enter file name:")
#file = input()

VALID_info = 2048

data = np.loadtxt("fftw_out.txt",max_rows=VALID_info)

plt.plot(data)
plt.show()




