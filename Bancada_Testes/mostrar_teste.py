# -*- coding: utf-8 -*-

import os
import random
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('seaborn-colorblind')

SHELL_EXECUTAR_TESTE = "executar_teste.sh"
DADOS_MPU9250 = "Dados/mpu9250.txt"
DADOS_BMP180 = "Dados/bmp180.txt"
NOISE = 20

# Gera os dados de aceleração simulando os testes do vídeo
# https://www.youtube.com/watch?v=7WYBVW2gnr8
def gerar_dados():
  def gerar_acel_y(t):
    if t < 1000.0:
      return 9.8
    elif t < 3000.0:
      x = (t - 1000.0) / 2000.0
      return 103.88*x**3 - 143.81*x**2 - 20.23*x + 49.54 + 9.8
    else:
      return 0.0

  velocidade = [0]
  f_mpu9250 = open(DADOS_MPU9250, "w+")
  lines = ""
  for t in range(0, 15000, 20): # A cada 20ms em 15 segundos
    acel = [0, gerar_acel_y(t), 0]
    acel[1] += np.random.poisson(NOISE, 1)[0] - NOISE # Adiciona ruído

    acel_str = " ".join(str(a) for a in acel)
    rot_str = " ".join(str(a) for a in [0, 0, 0])
    orien_str = " ".join(str(a) for a in [0, 0, 0])
    temp = str(0)

    lines += str(t) + " " + acel_str + " " + rot_str + " " + orien_str + " " + temp + "\n"

    velocidade.append(velocidade[-1] + (gerar_acel_y(t) - 9.8) * 0.02)

  f_mpu9250.write(lines)

  altitude = [velocidade[0]]
  for vel in velocidade:
    altitude.append(max(0, altitude[-1] + vel * 0.02))

  altitude = [i + np.random.poisson(NOISE, 1)[0] - NOISE for i in altitude]

  s_bmp = ""
  f_bmp180 = open(DADOS_BMP180, "w+")
  for i, alt in enumerate(altitude):
    s_bmp += str(i * 20) + " " + str(alt) + " " + str(random.uniform(23.5, 26.5)) + " \n"
  f_bmp180.write(s_bmp)

# Faz a mágica de compilar o .ino em um .cpp, executa e pega o output
def get_exec_output():
  o = os.popen('./' + SHELL_EXECUTAR_TESTE).read()
  events = {}
  sensor_reads = {}
  module_writes = {}
  for output in o.split("\n")[1:-1]:
    if output.startswith("[BANCADA_TESTES_LOG]"):
      # É read ou write
      output = output.replace("[BANCADA_TESTES_LOG] ", "")
      tokens = output.split(" - ")
      desc = tokens[0]
      log_type = tokens[1] # read/write
      value = float(tokens[2])
      ms = float(tokens[3][:-2])

      if log_type == "write":
        # É write
        text = desc + " - " + str(value)
        if text not in module_writes:
          module_writes[text] = []
        module_writes[text].append(ms)
      elif log_type == "read":
        # É read
        text = desc
        if text not in sensor_reads:
          sensor_reads[text] = []
        sensor_reads[text].append((ms, value))

    else:
      # É um evento
      output = output.replace("[BANCADA_TESTES_LOG] ", "")
      tokens = output.split(" - ")
      desc = tokens[0]
      ms = float(tokens[1][:-2])

      if desc not in events:
        events[desc] = []
      events[desc].append(ms)

  return events, sensor_reads, module_writes

# Vários plots
def plot_dados_brutos():
  xs, acel = [], []
  f_mpu9250 = open(DADOS_MPU9250, "r")
  for line in f_mpu9250.readlines():
    xs.append(int(line.split(" ")[0]))
    ax, ay, az = float(line.split(" ")[1]), float(line.split(" ")[2]), float(line.split(" ")[3])
    acel.append((ax**2 + ay**2 + az**2)**(1/2))
  plt.plot(xs, acel, label="Aceleracao absoluta bruta do arquivo", alpha=0.45, zorder=1)

def plot_leitura_sensores(ins):
  for name in ins:
    xs, ys = zip(*ins[name])
    plt.plot(xs[1:], ys[1:], label=name, linewidth=2, zorder=1)

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
# plot_dados_brutos()
plot_leitura_sensores(ins)

xmin, xmax, ymin, ymax = plt.axis()
plot_escrita_modulos(outs, xmin, xmax, ymin, ymax)
plot_eventos(events, xmin, xmax, ymin, ymax)

plt.legend(loc="upper right")
plt.ylabel("Medições (m)")
plt.xlabel("Tempo (ms)")
plt.suptitle("Comportamento do PC de Bordo")
plt.show()
