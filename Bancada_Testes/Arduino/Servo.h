#ifndef SERVO_H
#define SERVO_H

/*
  Testes para Servo.h
  Lucas Pellegrinelli

  Esse arquivo NÃO tem a intenção de fazer a conexão entre o hardware do
  Servo com o arduino. O propósito desse código é simular o .h importado
  pelo código principal do controlador do PC de Bordo e injetar dados artificiais
  de um lançamento para estudar e verificar bugs em um lançamento sem precisar
  fazer de fato o lançamento.
*/

#include "SensorFalso.h"

class Servo : public SensorFalso{
public:
  Servo() : SensorFalso("", 0){ }

  int pin = -1;
  void attach(int pin){ this->pin = pin; }

  void write(int a){
    relatar_escrita("Servo [pin " + std::to_string(this->pin) + "]", a);
  }
};

#endif
