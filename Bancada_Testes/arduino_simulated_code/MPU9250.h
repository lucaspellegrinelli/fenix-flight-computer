/*
  Testes para MPU9250.h
  Lucas Pellegrinelli

  Esse arquivo NÃO tem a intenção de fazer a conexão entre o hardware do
  MPU9250 com o arduino. O propósito desse código é simular o .h importado
  pelo código principal do controlador do PC de Bordo e injetar dados artificiais
  de um lançamento para estudar e verificar bugs em um lançamento sem precisar
  fazer de fato o lançamento.

  A ideia é que para cada um dos sensores do MPU9250, teremos um arquivo .txt
  que irá conter cada uma dos valores artificiais no lançamento artificial.
  Por exemplo, para a variável "Aceleração X" do sensor, teremos um arquivo
  "aceleracao_x.txt" que terá os dados da aceleração no eixo X durante o lançamento
  artificial e será o output da função "IMU.getAccelX_mss()".
*/

#include <string>
#include <fstream>

#include "arduino_consts.h"

class MPU9250{
public:
  // Arquivos de leitura dos dados sintéticos
  std::ifstream sensor_file;

  // Dados lidos dos arquivos dos dados sintéticos
  float acel_x = 0, acel_y = 0, acel_z = 0;
  float rot_x = 0, rot_y = 0, rot_z = 0;
  float orie_x = 0, orie_y = 0, orie_z = 0;
  float temperatura = 0;
  long reading_ms = 0;

  // Globais para configurar o que será lido
  int refresh_clock_ms = 20;

  // Construtor que recebia um objeto do tipo wire e um endereço. Como no PC
  // de Bordo o objeto wire não foi utilizado, simplifiquei para um inteiro
  MPU9250(int wire, int address){
    sensor_file.open("Dados/mpu9250.txt");
  }

  // Range das váriaveis
  static int ACCEL_RANGE_8G;
  static int GYRO_RANGE_500DPS;
  static int DLPF_BANDWIDTH_20HZ;

  // Código que era responsável por inicializar o módulo e retornar um número
  // menor que 0 caso desse algum problema
  int begin(){ return 0; }

  // Definições dos métodos de setar o range das variáveis
  void setAccelRange(int range){ }
  void setGyroRange(int range){ }
  void setDlpfBandwidth(int bandwidth){ }
  void setSrd(int a){
    if(a == 19){
      this->refresh_clock_ms += 20;
    }
  }

  // Método que lê os novos dados do sensor (no nosso caso, os novos dados dos
  // arquivos .txt)
  void readSensor(){
    sensor_file >> this->reading_ms;
    sensor_file >> this->acel_x >> this->acel_y >> this->acel_z;
    sensor_file >> this->rot_x >> this->rot_y >> this->rot_z;
    sensor_file >> this->orie_x >> this->orie_y >> this->orie_z;
    sensor_file >> this->temperatura;
  }

  // Funçoes que retornam os valores lidos
  double getAccelX_mss(){ return this->acel_x; };
  double getAccelY_mss(){ return this->acel_y; };
  double getAccelZ_mss(){ return this->acel_z; };

  double getGyroX_rads(){ return this->rot_x; };
  double getGyroY_rads(){ return this->rot_y; };
  double getGyroZ_rads(){ return this->rot_z; };

  double getMagX_uT(){ return this->orie_x; };
  double getMagY_uT(){ return this->orie_y; };
  double getMagZ_uT(){ return this->orie_z; };

  double getTemperature_C(){ return this->temperatura; };
};

extern int MPU9250::ACCEL_RANGE_8G;
extern int MPU9250::GYRO_RANGE_500DPS;
extern int MPU9250::DLPF_BANDWIDTH_20HZ;
