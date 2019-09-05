#include <iostream>

class Matrix{
public:
  double **data;
  int row;
  int col;

  Matrix(){ }

  Matrix(double **data, int row, int col){
    this->data = data;
    this->row = row;
    this->col = col;
  }

  static Matrix * initialize_zero_matrix(int r, int c){
    double** zero = new double*[r];
    for(int i = 0; i < r; i++){
      zero[i] = new double[c];
      for(int j = 0; j < c; j++){
        zero[i][j] = 0;
      }
    }

    return new Matrix(zero, r, c);
  }

  static Matrix * matrix_from_array(double arr[], int n, int r, int c){
    Matrix *m = Matrix::initialize_zero_matrix(r, c);
    for(int i = 0; i < r; i++){
      for(int j = 0; j < c; j++){
        int index = i * c + j;

        if(index < n) m->data[i][j] = arr[index];
        else break;
      }
    }

    return m;
  }

  void print(){
    for(int i = 0; i < this->row; i++){
      for(int j = 0; j < this->col; j++){
        std::cout << this->data[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }

  ~Matrix(){
    for(int i = 0; i < row; i++){
      delete[] this->data[i];
    }
  }
};

class MatrixOperations{
public:
  static Matrix * transpose_matrix(Matrix *m){
    Matrix *transpose = Matrix::initialize_zero_matrix(m->col, m->row);

    for(int i = 0; i < m->col; i++){
      for(int j = 0; j < m->row; j++){
        transpose->data[i][j] = m->data[j][i];
      }
    }

    return transpose;
  }

  static Matrix * multiply_matrix(Matrix *m1, Matrix*m2){
    Matrix *prod = Matrix::initialize_zero_matrix(m1->row, m2->col);

    for(int i = 0; i < m1->col; i++){
      for(int j = 0; j < m2->col; j++){
        for(int k = 0; k < m1->col; k++){
          prod->data[i][j] += m1->data[i][k] * m2->data[k][j];
        }
      }
    }

    return prod;
  }

  static Matrix * get_cofactor(Matrix *m, int p, int q, int n){
    Matrix *cofactor = Matrix::initialize_zero_matrix(m->row, m->col);

    int i = 0;
    int j = 0;

    for(int r = 0; r < cofactor->row; r++){
      for(int c = 0; c < cofactor->col; c++){
        if(r != p && c != q){
          cofactor->data[i][j++] = m->data[r][c];

          if(j == n - 1){
            j = 0;
            i++;
          }
        }
      }
    }

    return cofactor;
  }

  static double determinant(Matrix *m, int n){
    double det = 0;

    if(n == 1) return m->data[0][0];

    Matrix *temp;
    int sign = 1;

    for(int i = 0; i < n; i++){
      temp = MatrixOperations::get_cofactor(m, 0, i, n);
      det += sign * m->data[0][i] * MatrixOperations::determinant(temp, n - 1);

      sign *= -1;
    }

    return det;
  }

  static Matrix * adjoint(Matrix *m, int n){
    Matrix *adj = Matrix::initialize_zero_matrix(n, n);

    if(n == 1){
      adj->data[0][0] = 1;
      return adj;
    }

    int sign = 1;
    Matrix *temp;

    for(int i = 0; i < n; i++){
      for(int j = 0; j < n; j++){
        temp = MatrixOperations::get_cofactor(m, i, j, n);
        sign = ((i + j) % 2 == 0)? 1 : -1;
        adj->data[j][i] = sign * MatrixOperations::determinant(temp, n - 1);
      }
    }

    return adj;
  }

  static Matrix * matrix_inverse(Matrix *m){
    Matrix *inverse = Matrix::initialize_zero_matrix(m->row, m->col);

    double det = MatrixOperations::determinant(m, m->row);

    if(det == 0){
      // A matrix é singular e não pode ser inversível
      return nullptr;
    }

    Matrix *adj = MatrixOperations::adjoint(m, m->row);

    for(int i = 0; i < inverse->row; i++){
      for(int j = 0; j < inverse->col; j++){
        inverse->data[i][j] = adj->data[i][j] / det;
      }
    }

    return inverse;
  }
};

int main(){

  double arr[6] = {1, 2, 3, 4, 5, 6};

  Matrix *m = Matrix::matrix_from_array(arr, 6, 2, 3);
  m = MatrixOperations::transpose_matrix(m);
  
  m->print();

  return 0;
}
