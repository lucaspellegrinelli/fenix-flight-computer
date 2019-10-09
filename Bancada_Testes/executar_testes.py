import os
import matplotlib.pyplot as plt

def gerar_dados():
  f_mpu9250 = open("Dados/mpu9250.txt", "w+")

  lines = ""
  for i in range(0, 15000, 20):
    if i < 1000:
      acel = (0, 9.8, 0)
    elif i < 3000:
      x = (i - 1000) / 2000
      acel = (0, -13.3636*x**2 + 28.0636*x + 9.8, 0)
    elif i < 9000:
      x = (i - 3000) / 6000
      acel = (0, 68.6*x**2 - 83.3*x + 24.5, 0)
    else:
      acel = (0, 9.8, 0)

    rot = (0, 0, 0)
    orien = (0, 0, 0)
    temp = 0

    acel_str = " ".join(str(a) for a in acel)
    rot_str = " ".join(str(a) for a in rot)
    orien_str = " ".join(str(a) for a in orien)
    temp = str(temp)
    line = str(i) + " " + acel_str + " " + rot_str + " " + orien_str + " " + temp + "\n"
    lines += line

  f_mpu9250.write(lines)

def get_exec_output():
  o = os.popen('./exec_teste.sh').read()
  outs = {}
  for output in o.split("\n")[1:-1]:
    tokens = output.split(" - ")
    ms = int(tokens[0][:-2])
    comm = tokens[1]
    if comm not in outs:
      outs[comm] = []
    outs[comm].append(ms)
  return outs

outs = get_exec_output()
for command in outs:
  plt.scatter(outs[command], [0] * len(outs[command]), s=50, label=command)

xs = []
acelz = []

f_mpu9250 = open("Dados/mpu9250.txt", "r")
for line in f_mpu9250.readlines():
  xs.append(int(line.split(" ")[0]))
  acelz.append(float(line.split(" ")[2]))

plt.plot(xs, acelz, color="blue", label="Aceleration")
plt.legend(loc="upper right")
plt.show()
