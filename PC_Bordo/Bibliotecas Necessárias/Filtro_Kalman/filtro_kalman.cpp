#include "filtro_kalman.h"

EstadoKalman::EstadoKalman(){ }

EstadoKalman::EstadoKalman(float valor, float erro_cov, float ruido_cov_bruto, float ruido_cov_processado){
  this->valor = valor;
  this->erro_cov = erro_cov;
  this->ruido_cov_bruto = ruido_cov_bruto;
  this->ruido_cov_processado = ruido_cov_processado;
  this->ganho_kalman = this->erro_cov / (this->erro_cov + this->ruido_cov_bruto);
}

FiltroKalman::FiltroKalman(){ }

EstadoKalman* FiltroKalman::inicializar(float valor_inicial){
  return new EstadoKalman(valor_inicial, KALMAN_ERRO, KALMAN_RUIDO_BRUTO, KALMAN_RUIDO_PROCESADO);
}

void FiltroKalman::atualizar(EstadoKalman* estado, float medicao){
  // https://www.kalmanfilter.net/kalman1d.html
  estado->erro_cov = estado->erro_cov + estado->ruido_cov_processado;
  estado->ganho_kalman = estado->erro_cov / (estado->erro_cov + estado->ruido_cov_bruto);
  estado->valor = estado->valor + estado->ganho_kalman * (medicao - estado->valor);
  estado->erro_cov = (1 - estado->ganho_kalman) * estado->erro_cov;
}
