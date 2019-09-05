#include "matrix/Matrix.h"

int main(){
  double m1_arr[4] = {1, 2, 3, 4};
  double m2_arr[4] = {1, 2, 3, 4};

  // Cada criação de matriz consome 64 bytes
  Matrix *m1 = Matrix::build_matrix(m1_arr, 4, 5, 5);
  Matrix *m2 = Matrix::build_matrix(m2_arr, 4, 5, 5);

  m1->transpose()->sum(m2)->multiply(m2)->invert();

  m1->deallocate_matrix();
  m2->deallocate_matrix();
  free(m1);
  free(m2);

  return 0;
}
