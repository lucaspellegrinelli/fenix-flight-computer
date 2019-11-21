/*
  Testes para SoftwareSerial.h
  Lucas Pellegrinelli
*/

#include "SensorFalso.h"

class SoftwareSerial : public SensorFalso{
public:

  // Construtor que recebia um objeto do tipo wire e um endereço. Como no PC
  // de Bordo o objeto wire não foi utilizado, simplifiquei para um inteiro
  SoftwareSerial(int a, int b) : SensorFalso("", 0){ }

  // Funcoes que no nosso caso não vão fazer nada
  int begin(){ return 0; }
  bool available(){ return true; }

  // Funçoes que retornam os valores lidos
  char read(float *lat, float *lon){ return '\0'; }
};
