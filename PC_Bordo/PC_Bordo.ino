#include <math.h>
#include <string>

#include "Adafruit_BMP085.h"
#include "MPU9250.h"
#include "Servo.h"
#include "filtro_kalman.h"

/* ------- DEFINIÇÃO DE CONSTANTES -------- */
// Se a aceleração (em m/s^2) for <= a esse valor, interpretaremos como aceleração 0 (fim da propulsão).
#define FAIXA_ACELERACAO_NULA 2.5
// Tempo de precaução de ejeção pós fim da propulsão caso o apogeu não seja detectado
#define TEMPO_EJECAO_POS_FIM_PROPULSAO 6000
// Define o quantos loops a pressão tem que cair para o apice ser detectado
#define MIN_CONTADOR_PRESSAO_APOGEU 5

/* ------- DEFINIÇÃO DE OBJETOS DE MÓDULOS -------- */
MPU9250 gy91(Wire, 0x68);
Servo servo;
Adafruit_BMP085 bmp180;

/* ------- DEFINIÇÃO DE VARIÁVEIS PARA MEDIÇÃO -------- */
EstadoKalman* MPU9250_ABS_ACEL_KALMAN;
EstadoKalman* BMP180_ALTITUDE_KALMAN;
double aceleracao_absoluta = 9.8;
double altitude_atual = -1;
double altitude_var = -1;
int contador_altitude = 0;

/* ------- DEFINIÇÃO DE VARIÁVEIS PARA CONTROLE DE APOGEU -------- */
bool ejecao_disparada = false;
bool fim_propulsao = false;
unsigned long ms_fim_propulsao = -1;

/* ------- DEFINIÇÃO DE FUNÇÕES FUTURAS -------- */
void atualizar_leituras_gy91();
void atualizar_leituras_bmp180();
void teste_fim_propulsao();
void teste_apogeu();
void ejetar();

/* ------- DEFINIÇÃO DO MÉTODO SETUP -------- */
void setup() {
  Serial.begin(115200);

  gy91.begin();
  gy91.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  gy91.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
  gy91.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  gy91.setSrd(19);

  bmp180.begin();

  servo.attach(9);

  MPU9250_ABS_ACEL_KALMAN = FiltroKalman::inicializar(9.8);
  BMP180_ALTITUDE_KALMAN = FiltroKalman::inicializar(0);
}

/* ------- DEFINIÇÃO DO MÉTODO LOOP -------- */
void loop(){
  atualizar_leituras_gy91();
  atualizar_leituras_bmp180();

  relatar_leitura("Aceleracao processada", aceleracao_absoluta);
  relatar_leitura("Altitude processada", altitude_atual);

  teste_fim_propulsao();
  teste_apogeu();

  delay(20);
}

/* ------- DEFINIÇÃO DAS FUNÇÕES FUTURAS -------- */
void atualizar_leituras_gy91(){
  gy91.readSensor();
  double acel[3] = {gy91.getAccelX_mss(), gy91.getAccelY_mss(), gy91.getAccelZ_mss()};
  double abs_acel = sqrt(pow(acel[0], 2.0) + pow(acel[1], 2.0) + pow(acel[2], 2.0));
  FiltroKalman::atualizar(MPU9250_ABS_ACEL_KALMAN, abs_acel);
  aceleracao_absoluta = MPU9250_ABS_ACEL_KALMAN->valor;
}

void atualizar_leituras_bmp180(){
  double altitude = bmp180.readAltitude(1013.25);
  FiltroKalman::atualizar(BMP180_ALTITUDE_KALMAN, altitude);
  if(altitude_var == -1) altitude_var = 0;
  else altitude_var = BMP180_ALTITUDE_KALMAN->valor - altitude_atual;
  altitude_atual = BMP180_ALTITUDE_KALMAN->valor;
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

  if(altitude_var < 0) contador_altitude++;
  else contador_altitude = 0;

  if(contador_altitude >= MIN_CONTADOR_PRESSAO_APOGEU){
    Serial.println("Ejeção iniciada por pressão caindo");
    ejetar();
  }

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
