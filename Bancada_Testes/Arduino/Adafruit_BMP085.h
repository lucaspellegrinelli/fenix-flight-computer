/*
  Testes para Adafruit_BMP085.h
  Lucas Pellegrinelli

  Esse arquivo NÃO tem a intenção de fazer a conexão entre o hardware do
  BMP180 com o arduino. O propósito desse código é simular o .h importado
  pelo código principal do controlador do PC de Bordo e injetar dados artificiais
  de um lançamento para estudar e verificar bugs em um lançamento sem precisar
  fazer de fato o lançamento.

  A ideia é que para cada um dos sensores do BMP180, teremos um arquivo .txt
  que irá conter cada uma dos valores artificiais no lançamento artificial.
*/

#include "SensorFalso.h"

class Adafruit_BMP085 : public SensorFalso{
public:
  float pressao, temperatura;

  Adafruit_BMP085() : SensorFalso("Dados/bmp180.txt", 20){ }

  int begin(){ return 0; }

  void readSensor(){
    float *leituras = new float[3];
    if(ler_sensor(leituras)){
      this->pressao = leituras[1];
      this->temperatura = leituras[2];
    }
  }

  float readPressure(){
    this->readSensor();
    relatar_leitura("Altitude", this->pressao);
    return this->pressao;
  }

  float readTemperature(){
    this->readSensor();
    relatar_leitura("Temperatura", this->temperatura);
    return this->temperatura;
  }
};
