#include <iostream>

class Matrix{
public:
  double **data;
  int row;
  int col;

  Matrix(){ }

  Matrix(int row, int col) : Matrix(row, col, false){ }

  Matrix(int row, int col, bool ones){
    this->row = row;
    this->col = col;

    this->data = new double*[this->row];
    for(int i = 0; i < this->row; i++){
      this->data[i] = new double[this->col];
      if(ones){
        for(int j = 0; j < this->col; j++){
          this->data[i][j] = 0;
        }
      }
    }
  }

  Matrix(double **data, int row, int col){
    this->data = data;
    this->row = row;
    this->col = col;
  }

  static Matrix * matrix_from_array(double arr[], int n, int r, int c){
    Matrix *m = new Matrix(r, c, true);

    int index = 0;
    for(int i = 0; i < r; i++){
      for(int j = 0; j < c; j++){
        index = i * c + j;

        if(index < n) m->data[i][j] = arr[index];
        else break;
      }

      if(index >= n) break;
    }

    return m;
  }

  Matrix * multiply(Matrix *other){
    Matrix *prod = new Matrix(this->row, other->col);

    for(int i = 0; i < this->row; i++){
      for(int j = 0; j < other->col; j++){
        for(int k = 0; k < this->col; k++){
          prod->data[i][j] += this->data[i][k] * other->data[k][j];
        }
      }
    }

    return prod;
  }

  Matrix * transpose(){
    Matrix *transposed = new Matrix(this->col, this->row);

    for(int i = 0; i < this->col; i++){
      for(int j = 0; j < this->row; j++){
        transposed->data[i][j] = this->data[j][i];
      }
    }

    return transposed;
  }

  Matrix * invert(){
    Matrix *inverse = new Matrix(this->row, this->col);
    double a[this->row * 2][this->col * 2];

    for(int i = 0; i < this->row; i++){
      for(int j = 0; j < this->col; j++){
        a[i][j] = this->data[i][j];
      }
    }

    for(int i = 0; i < this->row; i++){
      for(int j = this->row; j < 2 * this->row; j++){
        if(i == j - this->row) a[i][j] = 1;
        else a[i][j] = 0;
      }
    }

    for(int i = 0; i < this->row; i++){
      double t = a[i][i];
      for(int j = i; j < 2 * this->row; j++){
        a[i][j] /= t;
      }

      for(int j = 0; j < this->row; j++){
        if(i != j){
          t = a[j][i];

          for(int k = 0; k < 2 * this->row; k++){
            a[j][k] -= t * a[i][k];
          }
        }
      }
    }

    for(int i = 0; i < this->row; i++){
      for(int j = this->row; j < 2 * this->row; j++){
        inverse->data[i][j - this->row] = a[i][j];
      }
    }

    return inverse;
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

int main(){
  double m_arr[9] = {1, 3, 3, 1, 4, 3, 1, 3, 4};
  Matrix *m = Matrix::matrix_from_array(m_arr, 9, 3, 3);
  m->print();

  std::cout << std::endl;

  m->transpose()->invert()->print();

  return 0;
}
