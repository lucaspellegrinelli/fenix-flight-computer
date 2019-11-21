/*
  Testes para TinyGPS.h
  Lucas Pellegrinelli

  Esse arquivo NÃO tem a intenção de fazer a conexão entre o hardware do
  GPS com o arduino. O propósito desse código é simular o .h importado
  pelo código principal do controlador do PC de Bordo e injetar dados artificiais
  de um lançamento para estudar e verificar bugs em um lançamento sem precisar
  fazer de fato o lançamento.

  A ideia é que para cada um dos sensores do GPS, teremos um arquivo .txt
  que irá conter cada uma dos valores artificiais no lançamento artificial.
*/

#include "SensorFalso.h"

class TinyGPS : public SensorFalso{
public:
  float latitude, longitude;

  // Construtor que recebia um objeto do tipo wire e um endereço. Como no PC
  // de Bordo o objeto wire não foi utilizado, simplifiquei para um inteiro
  TinyGPS() : SensorFalso("Dados/GPS6MV2.txt", 20){ }

  // Funcoes que no nosso caso não vão fazer nada
  bool encode(int c){ }
  void stats(unsigned long &chars, unsigned short &sentences, unsigned short &failed_checksum){ }

  // Método que lê os novos dados do sensor (no nosso caso, os novos dados dos
  // arquivos .txt)
  void readSensor(){
    float *leituras = new float[2];
    ler_sensor(leituras);

    this->latitude = leituras[0];
    this->longitude = leituras[1];

    delete leituras;
  }

  // Funçoes que retornam os valores lidos
  void f_get_position(float *lat, float *lon){ *lat = latitude; *lon = longitude; }
};
