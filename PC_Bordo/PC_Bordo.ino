// Comente isso caso a ideia seja o "deploy" final. Caso você quiser ver logs
// do programa no Monitor Serial, deixe essa linha em paz.
#define PC_ENV

// Protocolos de comunicação (Wire.h para I2C e SPI.h para SPI)
#include <Wire.h>
#include <SPI.h>

// Componentes
#include <SD.h>
#include <Servo.h>
#include "i2c.h"
#include "i2c_MPU9250.h"
#include "i2c_BMP280.h"

// Definição de PINs
#define SD_PIN 10

// Definição de constantes
#define QUEDA_MINIMA_PARA_EJECAO_M 10 // Quantos metros o foguete precisa cair para que o paraquedas seja acionado
#define QTD_MEDICOES_ESTABILIZADORAS 10 // Quantas medidas depois do inicio do arduino serão descartadas

// Declaração de objetos
BMP280 bmp280;
MPU9250 mpu9250;
Servo fred;
File arquivo_saida;

// Declaração de variáveis
int contador_medicoes = 0;
int nome_arquivo = -1;
float maior_altitude = -9999.0f;

// A ideia desses vetores é retirar a necessidade de abrir, escrever e fechar o arquivo
// de saída em toda iteração do "loop". Com essa ideia, vamos salvando nesses vetores
// várias medições, e quando os vetores encherem, nós escrevemos todas as medições de uma
// vez e começamos denovo. Tempo de salvar em um vetor <<<<<<< Tempo de salvar no SD.
// O arduino nano tem pouca memória. Quero que os vetores a seguir não passem de
// 512 btes de tamanho, ou seja, cada um tem 256 bytes. Como cada float tem
// 4 bytes, 256 / 4 = 64 posições em cada vetor. Para segurança diminui de 64 para 48.
#define TAMANHO_VETORES 48
float aceleracoes[TAMANHO_VETORES];
float altitudes[TAMANHO_VETORES];
int pos_atual = 0;

void setup() { 
  // Inicializa os componentes
  bool sd_success = SD.begin(SD_PIN);
  mpu9250.initialize();
  bmp280.initialize();
  bmp280.setEnabled(0);
  bmp280.triggerMeasurement();

  // Define o nome do arquivo atual
  nome_arquivo = random(999999);

  // Se estivermos no PC, mostra na tela se deu para inicilizar o cartão SD
  #ifdef PC_ENV
    Serial.begin(9600);
    if(!sd_success){
      Serial.println("Módulo SD não pode ser inicializado");
      while(true);
    }else{
      Serial.println("Módulo SD inicializado com sucesso");
    }
  #endif
}
 
void loop() {
  // Definição das variáveis que serão medidas
  static float meters, xyz_GyrAccMag[9];
  float temperature, pascal;

  // Medições
  bmp280.awaitMeasurement();
  bmp280.getTemperature(temperature);
  bmp280.getPressure(pascal);
  bmp280.getAltitude(meters);
  bmp280.triggerMeasurement();
  mpu9250.getMeasurement(xyz_GyrAccMag);

  // Dê 10 medições para os valores iniciais se estabilizarem
  if(contador_medicoes < QTD_MEDICOES_ESTABILIZADORAS){
    contador_medicoes++;
  }else{
    // Se estivermos no PC, mostre na tela as variáveis lidas
    #ifdef PC_ENV
      Serial.print(xyz_GyrAccMag[0]);
      Serial.print(" ");
      Serial.print(xyz_GyrAccMag[1]);
      Serial.print(" ");
      Serial.print(xyz_GyrAccMag[2]);
      Serial.print(" ");
      Serial.println(meters);
    #endif

    // Salva os valores lidos em uma nova posição do vetor
    aceleracoes[pos_atual] = sqrt(pow(xyz_GyrAccMag[0], 2) + pow(xyz_GyrAccMag[1], 2) + pow(xyz_GyrAccMag[2], 2));
    altitudes[pos_atual] = meters;
    pos_atual++;

    // Checa se devemos acionar o sistema de ejeção
    if(maior_altitude < meters){
      maior_altitude = meters;
    }else if(maior_altitude - meters >= QUEDA_MINIMA_PARA_EJECAO_M){
      fred.attach(5);
      fred.write(0);

      #ifdef PC_ENV
        Serial.println("Sistema de ejeção acionado");
      #endif
    }

    // Se chegamos no limite de nosso vetor
    if(pos_atual >= TAMANHO_VETORES){
      // Cria/abre um arquivo de saída 
      arquivo_saida = SD.open(String(String(nome_arquivo) + ".txt"), FILE_WRITE);
      if(arquivo_saida){
        // Coloca no arquivo as acelerações absolutas e altitudes lidas
        for(int i = 0; i < TAMANHO_VETORES; i++){
          Serial.print(aceleracoes[i]);
          Serial.print(" ");
          Serial.println(altitudes[i]);
        }
        
        // Fecha o arquivo
        arquivo_saida.close();

        // Reseta o vetor
        pos_atual = 0;
      }else{
        #ifdef PC_ENV
          Serial.print("Problema ao abrir ");
          Serial.print(nome_arquivo);
          Serial.println(".txt");
        #endif
      }
    }
  }
}
