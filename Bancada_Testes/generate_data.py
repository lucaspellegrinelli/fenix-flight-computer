import random

def get_temperature(t):
  return 25 + (random.random() - 0.5)

def get_aceleration(t):
  if t < 1000: # 1 segundo parado
    return (random.random() - 0.5, random.random() - 0.5, -9.8 + (random.random() - 0.5))
  elif t < 2000:
    return (random.random() - 0.5, random.random() - 0.5, -9.8 + (19.8 / 1000) * (t - 1000) + (random.random() - 0.5))
  elif t < 4000: # 3 segundos do motor ligado gerando acel de 10m/s2
    return (random.random() - 0.5, random.random() - 0.5, 10 + (random.random() - 0.5))
  elif t < 5000:
    return (random.random() - 0.5, random.random() - 0.5, 10 - (19.8 / 1000) * (t - 4000) + (random.random() - 0.5))
  elif t < 15000: # Gravidade puxando
    return (random.random() - 0.5, random.random() - 0.5, -9.8 + (random.random() - 0.5))

def get_rotation(t):
  return (0, 0, 0)

def get_orientation(t):
  return (0, 0, 0)

f_acel = open("Dados/mpu9250_aceleracao.txt", "w+")
f_rot = open("Dados/mpu9250_giroscopio.txt", "w+")
f_orien = open("Dados/mpu9250_orientacao.txt", "w+")
f_temp = open("Dados/mpu9250_temperatura.txt", "w+")

sim_acel = ""
sim_rot = ""
sim_orien = ""
sim_temp = ""

for i in range(0, 15000, 20):
  sim_acel += str(i) + " " + " ".join(str(a) for a in get_aceleration(i)) + "\n"
  sim_rot += str(i) + " " + " ".join(str(a) for a in get_rotation(i)) + "\n"
  sim_orien += str(i) + " " + " ".join(str(a) for a in get_orientation(i)) + "\n"
  sim_temp += str(i) + " " + str(get_temperature(i)) + "\n"

f_acel.write(sim_acel)
f_rot.write(sim_rot)
f_orien.write(sim_orien)
f_temp.write(sim_temp)
