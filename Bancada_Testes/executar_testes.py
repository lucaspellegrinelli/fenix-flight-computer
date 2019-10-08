import matplotlib.pyplot as plt

f_acel = open("Dados/mpu9250_aceleracao.txt", "r")
f_rot = open("Dados/mpu9250_giroscopio.txt", "r")
f_orien = open("Dados/mpu9250_orientacao.txt", "r")
f_temp = open("Dados/mpu9250_temperatura.txt", "r")

xs = []
acelz = []
for line in f_acel.readlines():
  items = line.split(" ")
  xs.append(float(items[0]))
  acelz.append(float(items[3]))

plt.plot(xs, acelz)
plt.show()
