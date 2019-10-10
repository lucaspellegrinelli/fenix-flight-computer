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

#include "SensorFalso.h"

class MPU9250 : public SensorFalso{
public:
  // Dados lidos dos arquivos dos dados sintéticos
  float acel_x = 0, acel_y = 0, acel_z = 0;
  float rot_x = 0, rot_y = 0, rot_z = 0;
  float orie_x = 0, orie_y = 0, orie_z = 0;
  float temperatura = 0;
  float reading_ms = 0;

  // Construtor que recebia um objeto do tipo wire e um endereço. Como no PC
  // de Bordo o objeto wire não foi utilizado, simplifiquei para um inteiro
  MPU9250(int wire, int address) : SensorFalso("Dados/mpu9250.txt", 20){ }

  // Range das váriaveis
  static int ACCEL_RANGE_8G;
  static int GYRO_RANGE_500DPS;
  static int DLPF_BANDWIDTH_20HZ;

  // Funcoes que no nosso caso não vão fazer nada
  int begin(){ return 0; }
  void setAccelRange(int range){ }
  void setGyroRange(int range){ }
  void setDlpfBandwidth(int bandwidth){ }
  void setSrd(int a){ }

  // Método que lê os novos dados do sensor (no nosso caso, os novos dados dos
  // arquivos .txt)
  void readSensor(){
    float *leituras = new float[11];
    ler_sensor(leituras);

    this->reading_ms = leituras[0];
    this->acel_x = leituras[1];
    this->acel_y = leituras[2];
    this->acel_z = leituras[3];
    this->rot_x = leituras[4];
    this->rot_y = leituras[5];
    this->rot_z = leituras[6];
    this->orie_x = leituras[7];
    this->orie_y = leituras[8];
    this->orie_z = leituras[9];
    this->temperatura = leituras[10];

    relatar_leitura("Aceleracao bruta", acel_y);

    delete leituras;
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
