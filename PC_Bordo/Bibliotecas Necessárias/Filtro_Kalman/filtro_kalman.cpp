/*
  Lucas Pellegrinelli
*/

#include "filtro_kalman.h"

EstadoKalman* FiltroKalman::inicializar(float valor_inicial){
  EstadoKalman* estado;

  estado.valor = valor_inicial;
  estado.erro_cov = KALMAN_ERRO;
  estado.ruido_cov_bruto = KALMAN_RUIDO_BRUTO;
  estado.ruido_cov_processado = KALMAN_RUIDO_PROCESADO;

  return estado;
}

void FiltroKalman::atualizar(EstadoKalman* estado, float medicao){
  medicao *= MS2_PARA_G;
  estado->erro_cov = estado->erro_cov + estado->ruido_cov_processado;
  estado->ganho_kalman = estado->erro_cov / (estado->erro_cov + estado->ruido_cov_bruto);
  estado->valor = estado->ganho_kalman * (medicao - estado->valor) + estado->valor;
  estado->erro_cov = (1 - estado->ganho_kalman) * estado->erro_cov;
}
