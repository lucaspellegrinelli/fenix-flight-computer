#include "matrix/Matrix.h"

int main(){
  double m1_arr[4] = {1, 2, 3, 4};
  double m2_arr[4] = {1, 2, 3, 4};

  Matrix *m1 = Matrix::build_matrix(m1_arr, 4, 2, 2);
  Matrix *m2 = Matrix::build_matrix(m2_arr, 4, 2, 2);

  m1->transpose()->transpose()->invert();

  m1->deallocate_matrix();
  m2->deallocate_matrix();
  free(m1);
  free(m2);

  return 0;
}
