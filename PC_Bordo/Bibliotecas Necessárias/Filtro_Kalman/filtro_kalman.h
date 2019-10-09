#ifndef FILTRO_KALMAN_H
#define FILTRO_KALMAN_H

#define KALMAN_ERRO 1
#define KALMAN_RUIDO_BRUTO 0.25
#define KALMAN_RUIDO_PROCESADO 0.01

#define MS2_PARA_G 0.101972

#include <math.h>

class EstadoKalman{
public:
  EstadoKalman();
  EstadoKalman(float valor, float erro_cov, float ruido_cov_bruto, float ruido_cov_processado);
  float valor;
  float ganho_kalman;
  float erro_cov;
  float ruido_cov_bruto;
  float ruido_cov_processado;
};

class FiltroKalman{
public:
  FiltroKalman();

  static EstadoKalman* inicializar(float valor_inicial);
  static void atualizar(EstadoKalman* estado, float medicao);
};

#endif
