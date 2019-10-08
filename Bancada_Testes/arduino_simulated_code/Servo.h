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

#include "arduino_consts.h"

#include <iostream>

class Servo{
public:
  Servo(){ }

  int pin = -1;
  void attach(int pin){ this->pin = pin; }

  void write(int a){
    std::cout << "Servo [pin " << this->pin << "] write(" << a << ")" << std::endl;
  }
};

#endif
