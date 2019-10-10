#include <math.h>
#include <string>

#include "MPU9250.h"
#include "Servo.h"
#include "filtro_kalman.h"

/* ------- DEFINIÇÃO DE CONSTANTES -------- */
#define FAIXA_ACELERACAO_NULA 0.3
#define TEMPO_EJECAO_POS_FIM_PROPULSAO 6000 // Tempo de precaução de ejeção pós fim da propulsão caso o apogeu não seja detectado

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
    relatar_evento("Fim da propulsão detectado");
    fim_propulsao = true;
    ms_fim_propulsao = millis();
  }
}

void teste_apogeu(){
  if(!fim_propulsao || ejecao_disparada) return;

  int ms_depois_fim_prop = millis() - ms_fim_propulsao;
  if(ms_depois_fim_prop > TEMPO_EJECAO_POS_FIM_PROPULSAO){
    relatar_evento("Ejeção iniciada por tempo pós propulsão");
    ejetar();
  }
}

void ejetar(){
  if(!ejecao_disparada){
    ejecao_disparada = true;
    servo.write(10);
    relatar_escrita("Servo [pin 9]", 10);
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
