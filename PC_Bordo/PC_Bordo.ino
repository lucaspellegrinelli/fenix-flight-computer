#include <math.h>
#include <string>

#include "MPU9250.h"
#include "Servo.h"
#include "filtro_kalman.h"

/* ------- DEFINIÇÃO DE CONSTANTES -------- */
// Se a aceleração (em g) for <= a esse valor, interpretaremos como aceleração 0 (fim da propulsão).
#define FAIXA_ACELERACAO_NULA 0.15
// Tempo de precaução de ejeção pós fim da propulsão caso o apogeu não seja detectado
#define TEMPO_EJECAO_POS_FIM_PROPULSAO 6000

/* ------- DEFINIÇÃO DE OBJETOS DE MÓDULOS -------- */
MPU9250 IMU(Wire, 0x68);
Servo servo;

/* ------- DEFINIÇÃO DE VARIÁVEIS PARA MEDIÇÃO -------- */
EstadoKalman* MPU9250_ABS_ACEL_KALMAN;
double aceleracao_absoluta = 1;

/* ------- DEFINIÇÃO DE VARIÁVEIS PARA CONTROLE DE APOGEU -------- */
bool ejecao_disparada = false;
bool fim_propulsao = false;
unsigned long ms_fim_propulsao = -1;

/* ------- DEFINIÇÃO DE FUNÇÕES FUTURAS -------- */
void atualizar_leituras_imu();
void teste_fim_propulsao();
void teste_apogeu();
void ejetar();

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

  teste_fim_propulsao();
  teste_apogeu();

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

void teste_fim_propulsao(){
  if(!fim_propulsao && aceleracao_absoluta < FAIXA_ACELERACAO_NULA){
    Serial.println("Fim da propulsao detectado");
    fim_propulsao = true;
    ms_fim_propulsao = millis();
  }
}

void teste_apogeu(){
  if(!fim_propulsao || ejecao_disparada) return;

  int ms_depois_fim_prop = millis() - ms_fim_propulsao;
  if(ms_depois_fim_prop > TEMPO_EJECAO_POS_FIM_PROPULSAO){
    Serial.println("Ejecao iniciada por tempo pos propulsao");
    ejetar();
  }
}

void ejetar(){
  if(!ejecao_disparada){
    ejecao_disparada = true;
    servo.write(10);
  }
}