#ifndef ARDUINO_CONSTS_H
#define ARDUINO_CONSTS_H

/*
  Constantes do arduino para o controlador principal
  Lucas Pellegrinelli

  Esse arquivo define o que cada função/objeto/constante do arduino fará
  no ambiente emulado.
*/

#include <iostream>

// No PC de Bordo utilizamos o objeto da classe "Wire" para inicializar nosso
// sensor MPU9250. Como é só para isso, nessa bancada de testes vamos fingir que
// "Wire" é um inteiro que será ignorado no arquivo contendo as definições do
// MPU9250
#define Wire 0

// Contador do tempo atual em ms
extern int current_ms;
int current_ms;

// Define um objeto para substituir o Serial.print para o cout do iostream
class SerialCaller{
public:
  SerialCaller(){ }
  void begin(int freq){ }
  void print(std::string text){ std::cout << text; }
  void print(float text){ std::cout << text; }
  void println(std::string text){ std::cout << text << " - " << current_ms << "ms" << std::endl; }
  void println(float text){ std::cout << text << " - " << current_ms << "ms" << std::endl; }
};

extern SerialCaller Serial;
SerialCaller Serial;

// Cria o método delay do arduino
extern void delay(int ns);
void delay(int ms){
  current_ms += ms;
  // struct timespec req = {0};
  // time_t sec = (int)(ms / 1000.0);
  // ms = ms - (sec * 1000.0);
  // req.tv_sec = sec;
  // req.tv_nsec = ms * 1000000L;
  // while(nanosleep(&req, &req) == -1) continue;
}

extern int millis();
int millis(){
  return current_ms;
}

extern void relatar_leitura();
void relatar_leitura(std::string descricao, float valor){
  // [BANCADA_TESTES_LOG] Descricao - read - 0.2234343 - 0ms
  Serial.print("[BANCADA_TESTES_LOG] ");
  Serial.print(descricao);
  Serial.print(" - read - ");
  Serial.println(valor);
}

extern void relatar_escrita();
void relatar_escrita(std::string descricao, float valor){
  // [BANCADA_TESTES_LOG] Descricao - write - 0.2234343 - 0ms
  Serial.print("[BANCADA_TESTES_LOG] ");
  Serial.print(descricao);
  Serial.print(" - write - ");
  Serial.println(valor);
}

#endif
