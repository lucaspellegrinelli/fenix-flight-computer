import os
import random
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('seaborn-colorblind')


SHELL_EXECUTAR_TESTE = "executar_teste.sh"
DADOS_MPU9250 = "Dados/mpu9250.txt"


# Gera os dados de aceleração simulando os testes do vídeo
# https://www.youtube.com/watch?v=7WYBVW2gnr8
def gerar_dados():
  def gerar_acel_y(t):
    if t < 1000:
      return 9.8
    elif t < 3000:
      x = (t - 1000) / 2000
      return 103.88*x**3 - 143.81*x**2 - 20.23*x + 49.54 + 9.8
    else:
      return 0

  f_mpu9250 = open(DADOS_MPU9250, "w+")
  lines = ""
  for t in range(0, 15000, 20): # A cada 20ms em 15 segundos
    acel = [0, gerar_acel_y(t), 0]
    # acel[1] += np.random.poisson(5, 1)[0] - 5 # Adiciona ruído

    acel_str = " ".join(str(a) for a in acel)
    rot_str = " ".join(str(a) for a in [0, 0, 0])
    orien_str = " ".join(str(a) for a in [0, 0, 0])
    temp = str(0)

    lines += str(t) + " " + acel_str + " " + rot_str + " " + orien_str + " " + temp + "\n"

  f_mpu9250.write(lines)

# Faz a mágica de compilar o .ino em um .cpp, executa e pega o output
def get_exec_output():
  o = os.popen('./' + SHELL_EXECUTAR_TESTE).read()
  events = {}
  sensor_reads = {}
  module_writes = {}
  for output in o.split("\n")[1:-1]:
    tokens = output.split(" - ")
    ms = int(tokens[0][:-2])
    if "read" in output:
      text = tokens[1]
      reading = float(tokens[3])
      if text not in sensor_reads:
        sensor_reads[text] = []
      sensor_reads[text].append((ms, reading))
    elif "write" in output:
      text = tokens[1] + " write " + tokens[3]
      if text not in module_writes:
        module_writes[text] = []
      module_writes[text].append(ms)
    else:
      text = tokens[1]
      if text not in events:
        events[text] = []
      events[text].append(ms)
  return events, sensor_reads, module_writes

# Vários plots
def plot_dados_brutos():
  xs, acel = [], []
  f_mpu9250 = open(DADOS_MPU9250, "r")
  for line in f_mpu9250.readlines():
    xs.append(int(line.split(" ")[0]))
    ax, ay, az = float(line.split(" ")[1]), float(line.split(" ")[2]), float(line.split(" ")[3])
    acel.append((ax**2 + ay**2 + az**2)**(1/2))
  plt.plot(xs, acel, label="Aceleração absoluta bruta do arquivo", alpha=0.45, zorder=1)

def plot_leitura_sensores(ins):
  for name in ins:
    xs, ys = zip(*ins[name])
    plt.plot(xs, ys, label=name, linewidth=2.5, zorder=1)

def plot_escrita_modulos(outs, xmin, xmax, ymin, ymax):
  line_sp = (xmax - xmin) / 200
  for command in outs:
    plt.vlines(outs[command], ymin=ymin, ymax=ymax, zorder=2, color="#262626")
    for ms in outs[command]:
      plt.text(ms + line_sp, ymax, command, rotation=90, horizontalalignment='left',
               verticalalignment='top', color="#262626", fontsize=9)

def plot_eventos(events, xmin, xmax, ymin, ymax):
  line_sp = (xmax - xmin) / 200
  for event in events:
    plt.vlines(events[event], ymin=ymin, ymax=ymax, zorder=2, color="#262626")
    for ms in events[event]:
      plt.text(ms - line_sp * 0.5, ymax, event, rotation=90, horizontalalignment='right',
               verticalalignment='top', color="#262626", fontsize=9)

# Executando
gerar_dados()

events, ins, outs = get_exec_output()
plot_dados_brutos()
plot_leitura_sensores(ins)

xmin, xmax, ymin, ymax = plt.axis()
plot_escrita_modulos(outs, xmin, xmax, ymin, ymax)
plot_eventos(events, xmin, xmax, ymin, ymax)

plt.legend(loc="upper right")
plt.ylabel("Aceleração absoluta medida pelo acelerômetro (m/s²)")
plt.xlabel("Tempo (ms)")
plt.suptitle("Comportamento do PC de Bordo")
plt.show()
