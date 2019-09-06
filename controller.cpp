#include "Matrix.h"

class KalmanFilter{
public:
  double T = 0.05;

  Matrix *H;
  Matrix *R;
  Matrix *Q;
  Matrix *A;
  Matrix *P;
  Matrix *K;

  KalmanFilter(){
    double H_arr[6] = {1, 0, 0, 0, 0, 1};
    double R_arr[4] = {35.8229, 0, 0, 0.0012};
    double Q_arr[9] = {0, 0, 0, 0, 0, 0, 0, 0, 1};
    double A_arr[9] = {1, T, T * T / 2, 0, 1, T, 0, 0, 1};

    H = Matrix::build_matrix(H_arr, 6, 2, 3);
    R = Matrix::build_matrix(R_arr, 4, 2, 2);
    Q = Matrix::build_matrix(Q_arr, 9, 3, 3);
    A = Matrix::build_matrix(A_arr, 9, 3, 3);
  }

  void build_kalman_gain(){
    double eye_3_arr[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    Matrix *eye_3 = Matrix::build_matrix(eye_3_arr, 9, 3, 3);

    P = eye_3->copy();

    Matrix *H_t = H->copy()->transpose();
    Matrix *A_t = A->copy()->transpose();

    Matrix *up;
    Matrix *down;

    for(int i = 0; i < 20; i++){
      up = P->copy()->multiply(H_t);
      down = H->copy()->multiply(P)->multiply(H_t)->sum(R);

      K = up->multiply(down->invert());
      P = eye_3->copy()->subtract(K->multiply(H))->multiply(P);
      P = A->copy()->multiply(P)->multiply(A_t)->sum(Q);
    }

    eye_3->deallocate_matrix();
    up->deallocate_matrix();
    down->deallocate_matrix();
    H_t->deallocate_matrix();
    A_t->deallocate_matrix();

    free(eye_3);
    free(up);
    free(down);
    free(H_t);
    free(A_t);
  }

  ~KalmanFilter(){
    H->deallocate_matrix();
    R->deallocate_matrix();
    Q->deallocate_matrix();
    A->deallocate_matrix();
    P->deallocate_matrix();
    K->deallocate_matrix();

    free(H);
    free(R);
    free(Q);
    free(A);
    free(P);
    free(K);
  }
};

int main(){
  KalmanFilter *kf = new KalmanFilter();
  kf->build_kalman_gain();

  return 0;
}
