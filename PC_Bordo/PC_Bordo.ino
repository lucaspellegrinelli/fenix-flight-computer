#include <math.h>
#include <string>

#include "Adafruit_BMP085.h"
#include "MPU9250.h"
#include "Servo.h"
#include "filtro_kalman.h"

/* ------- DEFINIÇÃO DE CONSTANTES -------- */
// Se a aceleração (em m/s^2) for <= a esse valor, interpretaremos como aceleração 0 (fim da propulsão).
#define FAIXA_ACELERACAO_NULA 3.33
// Tempo de precaução de ejeção pós fim da propulsão caso o apogeu não seja detectado
#define TEMPO_EJECAO_POS_FIM_PROPULSAO 6000
// Define o quantos loops a pressão tem que cair para o apice ser detectado
#define MIN_CONTADOR_PRESSAO_APOGEU 5
// Tempo que esperaramos pós a detecção do apogeu para acionar o sistema de ejeção
#define DELAY_EJECAO_POS_APOGEU 1000

/* ------- DEFINIÇÃO DE OBJETOS DE MÓDULOS -------- */
MPU9250 gy91(Wire, 0x68);
Servo servo;
Adafruit_BMP085 bmp180;

/* ------- DEFINIÇÃO DE VARIÁVEIS PARA MEDIÇÃO -------- */
EstadoKalman* MPU9250_ABS_ACEL_KALMAN;
EstadoKalman* BMP180_ALTITUDE_KALMAN;
EstadoKalman* BMP180_TEMPERATURA_KALMAN;
double temperatura_atual = 25.0;
double aceleracao_absoluta = 9.8;
double altitude_atual = -1.0;
double altitude_var = -1.0;
int contador_altitude = 0;

/* ------- DEFINIÇÃO DE VARIÁVEIS PARA CONTROLE DE APOGEU -------- */
bool ejecao_disparada = false;
bool apogeu_detectado = false;
bool fim_propulsao = false;
unsigned long ms_fim_propulsao = -1;
unsigned long ms_apogeu_detectado = -1;
unsigned long ms_inicio_arduino = -1;

/* ------- DEFINIÇÃO DE FUNÇÕES FUTURAS -------- */
void atualizar_leituras_gy91();
void atualizar_leituras_bmp180();
void teste_fim_propulsao();
void teste_apogeu();
void detectar_apogeu();
void teste_ejecao();
void ejetar();

/* ------- DEFINIÇÃO DO MÉTODO SETUP -------- */
void setup() {
  ms_inicio_arduino = millis();

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
  BMP180_TEMPERATURA_KALMAN = FiltroKalman::inicializar(25);
}

/* ------- DEFINIÇÃO DO MÉTODO LOOP -------- */
void loop(){
  atualizar_leituras_gy91();
  atualizar_leituras_bmp180();

  // Tempo para as leituras se acalmarem
  if(millis() - ms_inicio_arduino >= 100){
    relatar_leitura("Aceleracao processada", aceleracao_absoluta);
    relatar_leitura("Altitude processada", altitude_atual);
    relatar_leitura("Temperatura processada", temperatura_atual);

    teste_fim_propulsao();
    teste_apogeu();
    teste_ejecao();
  }

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

  double temperatura = bmp180.readTemperature();
  FiltroKalman::atualizar(BMP180_TEMPERATURA_KALMAN, temperatura);
  temperatura_atual = BMP180_TEMPERATURA_KALMAN->valor;
}

void teste_fim_propulsao(){
  if(!fim_propulsao && aceleracao_absoluta < FAIXA_ACELERACAO_NULA){
    Serial.println("Fim da propulsao detectado");
    fim_propulsao = true;
    ms_fim_propulsao = millis();
  }
}

void teste_apogeu(){
  if(!fim_propulsao || ejecao_disparada || apogeu_detectado) return;

  if(altitude_var < 0) contador_altitude++;
  else contador_altitude = 0;

  if(contador_altitude >= MIN_CONTADOR_PRESSAO_APOGEU){
    Serial.println("Apogeu detectado por pressão caindo");
    detectar_apogeu();
  }

  unsigned long ms_depois_fim_prop = millis() - ms_fim_propulsao;
  if(ms_depois_fim_prop > TEMPO_EJECAO_POS_FIM_PROPULSAO){
    Serial.println("Ejecao disparada por tempo pos fim da propulsao");
    apogeu_detectado = true;
    ejetar(); // Nesse caso, como perdemos o apogeu, vamos ejetar direto
  }
}

void detectar_apogeu(){
  if(!apogeu_detectado){
    apogeu_detectado = true;
    ms_apogeu_detectado = millis();
  }
}

void teste_ejecao(){
  if(apogeu_detectado && !ejecao_disparada){
    unsigned long ms_depois_apogeu = millis() - ms_apogeu_detectado;
    if(ms_depois_apogeu > DELAY_EJECAO_POS_APOGEU){
      ejetar();
    }
  }
}

void ejetar(){
  if(apogeu_detectado && !ejecao_disparada){
    ejecao_disparada = true;
    Serial.print("Ejecao disparada ");
    Serial.print(DELAY_EJECAO_POS_APOGEU / 1000);
    Serial.println("s depois do apogeu");
    servo.write(10);
  }
}
