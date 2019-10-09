import os
import random
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('seaborn-colorblind')

def gerar_dados():
  f_mpu9250 = open("Dados/mpu9250.txt", "w+")

  lines = ""
  for i in range(0, 15000, 20):
    if i < 1000:
      acel = [0, 9.8, 0]
    elif i < 3000:
      x = (i - 1000) / 2000
      acel = [0, -13.3636*x**2 + 28.0636*x + 9.8, 0]
    elif i < 9000:
      x = (i - 3000) / 6000
      acel = [0, 65.3333*x**2 - 80.0333*x + 24.5, 0]
    else:
      acel = [0, 9.8, 0]

    acel[1] += np.random.poisson(3, 1)[0] - 1.5
    rot = [0, 0, 0]
    orien = [0, 0, 0]
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
  out_responses = {}
  in_responses = {}
  for output in o.split("\n")[1:-1]:
    if "read" in output:
      tokens = output.split(" - ")
      ms = int(tokens[0][:-2])
      name = tokens[1]
      reading = float(tokens[3])
      if name not in in_responses:
        in_responses[name] = []
      in_responses[name].append((ms, reading))
    else:
      tokens = output.split(" - ")
      ms = int(tokens[0][:-2])
      comm = tokens[1] + " " + tokens[2] + " " + tokens[3] if len(tokens) > 2 else tokens[1]
      if comm not in out_responses:
        out_responses[comm] = []
      out_responses[comm].append(ms)
  return in_responses, out_responses

gerar_dados()

xs, acel = [], []
f_mpu9250 = open("Dados/mpu9250.txt", "r")
for line in f_mpu9250.readlines():
  xs.append(int(line.split(" ")[0]))
  ax, ay, az = float(line.split(" ")[1]), float(line.split(" ")[2]), float(line.split(" ")[3])
  acel.append((ax**2 + ay**2 + az**2)**(1/2))
plt.plot(xs, acel, label="Aceleração absoluta bruta do arquivo", alpha=0.35, zorder=1)

ins, outs = get_exec_output()

for name in ins:
  xs, ys = zip(*ins[name])
  plt.plot(xs, ys, label=name, linewidth=2.5, zorder=1)

xmin, xmax, ymin, ymax = plt.axis()
for command in outs:
  plt.vlines(outs[command], ymin=ymin, ymax=ymax, zorder=2, color="#262626")
  for ms in outs[command]:
    plt.text(ms, ymax, command, rotation=90, horizontalalignment='right', verticalalignment='top', color="#262626", fontsize=9)

plt.legend(loc="upper right")
plt.ylabel("Aceleração absoluta medida pelo acelerômetro (m/s²)")
plt.xlabel("Tempo (ms)")
plt.suptitle("Comportamento do PC de Bordo")
plt.show()
