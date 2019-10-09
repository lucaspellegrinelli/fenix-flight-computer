#include <math.h>
#include <string>

#include "MPU9250.h"
#include "Servo.h"
#include "filtro_kalman.h"

/* ------- DEFINIÇÃO DE CONSTANTES -------- */
#define FAIXA_ACELERACAO_APOGEU 0.15
#define TEMPO_EJECAO_POS_APOGEU 500

/* ------- DEFINIÇÃO DE OBJETOS DE MÓDULOS -------- */
MPU9250 IMU(Wire, 0x68);
Servo servo;

/* ------- DEFINIÇÃO DE VARIÁVEIS PARA MEDIÇÃO -------- */
EstadoKalman* MPU9250_ABS_ACEL_KALMAN;
double aceleracao_absoluta = 1;

/* ------- DEFINIÇÃO DE VARIÁVEIS PARA CONTROLE DE APOGEU -------- */
bool ejecao_disparada = false;
bool apogeu_pendente = false;
unsigned long ms_apogeu = -1;

/* ------- DEFINIÇÃO DE FUNÇÕES FUTURAS -------- */
void atualizar_leituras_imu();
bool is_apogeu();

/* ------- DEFINIÇÃO DE FUNÇÕES DE LOG -------- */
void relatar_leitura(std::string descricao, float valor);
void relatar_evento(std::string descricao);
void relatar_escrita(std::string descricao, float valor);

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

  relatar_leitura("Aceleração absoluta com Kalman", aceleracao_absoluta * 9.80665);

  if(is_apogeu() && !ejecao_disparada){
    ejecao_disparada = true;
    servo.write(10);
    relatar_escrita("Servo [pin 9]", 10);
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
  if(!apogeu_pendente && aceleracao_absoluta < FAIXA_ACELERACAO_APOGEU){
    relatar_evento("Apogeu detectado");

    apogeu_pendente = true;
    ms_apogeu = millis();
    return false;
  }

  if(apogeu_pendente && (millis() - ms_apogeu) >= TEMPO_EJECAO_POS_APOGEU){
    return true;
  }
}

/* ------- DEFINIÇÃO DE FUNÇÕES DE LOG -------- */
void relatar_evento(std::string descricao){
  // 0ms - Descricao
  Serial.print(current_ms);
  Serial.print("ms - ");
  Serial.println(descricao);
}

void relatar_leitura(std::string descricao, float valor){
  // 0ms - Descricao - read - 0.2234343
  Serial.print(current_ms);
  Serial.print("ms - ");
  Serial.print(descricao);
  Serial.print(" - read - ");
  Serial.println(valor);
}

void relatar_escrita(std::string descricao, float valor){
  // 0ms - Descricao - write - 0.2234343
  Serial.print(current_ms);
  Serial.print("ms - ");
  Serial.print(descricao);
  Serial.print(" - write - ");
  Serial.println(valor);
}
