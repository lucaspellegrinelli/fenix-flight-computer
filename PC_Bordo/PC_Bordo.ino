#include <math.h>

#include "MPU9250.h"
#include "Servo.h"

#include "filtro_kalman.h"

/* ------- DEFINIÇÃO DE CONSTANTES -------- */
#define FAIXA_ACELERACAO_APOGEU 0.15

/* ------- DEFINIÇÃO DE OBJETOS DE MÓDULOS -------- */
MPU9250 IMU(Wire, 0x68);
Servo servo;

/* ------- DEFINIÇÃO DE VARIÁVEIS PARA MEDIÇÃO -------- */
EstadoKalman* MPU9250_ABS_ACEL_KALMAN;
double aceleracao_absoluta = 1;

/* ------- DEFINIÇÃO DE FUNÇÕES FUTURAS -------- */
void atualizar_leituras_imu();
bool is_apogeu();

/* ------- DEFINIÇÃO DO MÉTODO SETUP -------- */
void setup() {
  Serial.begin(115200);

  IMU.begin();
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  IMU.setSrd(19);

  servo.attach(9);

  MPU9250_ABS_ACEL_KALMAN = FiltroKalman::inicializar(1);
}

/* ------- DEFINIÇÃO DO MÉTODO LOOP -------- */
void loop(){
  atualizar_leituras_imu();

  if(is_apogeu()){
    servo.write(10);
  }

  delay(20);
}

/* ------- DEFINIÇÃO DAS FUNÇÕES FUTURAS -------- */
void atualizar_leituras_imu(){
  IMU.readSensor();
  double acel[3] = {IMU.getAccelX_mss(), IMU.getAccelY_mss(), IMU.getAccelZ_mss()};
  double abs_acel = sqrt(pow(acel[0], 2.0) + pow(acel[1], 2.0) + pow(acel[2], 2.0));
  FiltroKalman::atualizar(MPU9250_ABS_ACEL_KALMAN, abs_acel);
  aceleracao_absoluta = MPU9250_ABS_ACEL_KALMAN->valor;
}

bool is_apogeu(){
  return aceleracao_absoluta < FAIXA_ACELERACAO_APOGEU;
}
