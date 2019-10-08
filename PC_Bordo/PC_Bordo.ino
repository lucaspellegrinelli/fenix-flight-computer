// PC de Bordo para a Fênix
// Autor: Lucas Pellegrinelli

// Ligações no arduino
// - Módulo GY-91 (Acelerômetro, Bússola, Giroscópio e Termometro)
//    SCL - A5 - I2C
//    SDA - A4 - I2C
//    GND - DNG
//    VIN - 5V
//
//  Para o uso desse módulo é necessário instalar o pacote "Bolder Flight Systems
//  MPU9250" que pode ser encontrado procurando "MPU9250" na tela de "Manage Libraries"
//  no Arduino IDE.

#include <math.h>

#include "MPU9250.h"

// Constante de conversão de radianos para graus
#define RAD_PARA_GRAUS 57.295779513

// Inicia o objeto do MPU9250 utilizando o protocolo I2C (por isso o "Wire"
// nos parâmetros) no endereço 0x68.
MPU9250 IMU(Wire, 0x68);
EstadoKalman MPU9250_ABS_ACEL_KALMAN;

// Inicia o servo
Servo servo;

// Leituras do sensor MPU9250
void atualizar_leituras_imu();
double aceleracao[3] = {0.0, 0.0, 0.0};
double rotacao[3] = {0.0, 0.0, 0.0};
double orientacao[3] = {0.0, 0.0, 0.0};
double temperatura = 0.0;

// Dados processados pelo filtro kalman
double aceleracao_absoluta = 0;
double direcao = 0;

void setup() {
  // Inicia a comunicação com o Serial Monitor na frequencia 115200
  Serial.begin(115200);

  // Armazena o status do MPU. Caso < 0, então tem algum problema
  int status_mpu = IMU.begin();

  // Inicializa os filtros kalman
  MPU9250_ABS_ACEL_KALMAN = FILTRO_KALMAN::inicializar(1);
  MPU9250_ROT_KALMAN = FILTRO_KALMAN::inicializar(0);

  // Caso tenha algum problema com o módulo, mostre alguma mensagem de erro
  if (status_mpu < 0) {
    Serial.print("IMU não foi inicializado com sucesso (status de ID: ");
    Serial.print(status_mpu);
    Serial.println(")");
    while(true){ }
  }

  // Limita o quanto cada uma das medições pode estar
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G); // Aceleração não pode passar de +-8G
  IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS); // Rotação de +-500 graus/s

  // Seta o Digital Low Pass Filter para 20HZ
  // https://en.wikipedia.org/wiki/Low-pass_filter
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);

  // Update de 50Hz do sensor
  IMU.setSrd(19);
}

void loop(){
  // Atualiza os valores de aceleraçaõ, rotação, orientação e temperatura
  atualizar_leituras_imu();


  // 20ms de delay pois o MPU9250 foi setado com uma frequência de 50Hz, ou seja,
  // 1 segundo / 50 updates = 20ms por update
  delay(20);
}

void atualizar_leituras_imu(){
  // Atualiza as leituras do sensor
  IMU.readSensor();

  // Aceleração em m/s nos 3 eixos
  aceleracao[0] = IMU.getAccelX_mss();
  aceleracao[1] = IMU.getAccelY_mss();
  aceleracao[2] = IMU.getAccelZ_mss();

  // Aplicando o filtro de kalman
  double abs_acel = sqrt(pow(aceleracao[0], 2) + pow(aceleracao[1], 2) + pow(aceleracao[2], 2));
  FiltroKalman::atualizar(MPU9250_ABS_ACEL_KALMAN, abs_acel);
  aceleracao_absoluta = MPU9250_ABS_ACEL_KALMAN.valor;

  // Rotação em radianos nos 3 eixos
  rotacao[0] = IMU.getGyroX_rads();
  rotacao[1] = IMU.getGyroY_rads();
  rotacao[2] = IMU.getGyroZ_rads();

  // Orientação da bússola nos 3 eixos
  orientacao[0] = IMU.getMagX_uT();
  orientacao[1] = IMU.getMagY_uT();
  orientacao[2] = IMU.getMagZ_uT();

  // Temperatura
  temperatura = IMU.getTemperature_C();
}
