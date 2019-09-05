#include <iostream>

class Matrix{
public:
  double **data;
  int row;
  int col;

  Matrix(){ }

  Matrix(int row, int col) : Matrix(row, col, false){ }

  Matrix(int row, int col, bool ones){
    this->allocate_matrix(row, col, ones);
  }

  Matrix(double **data, int row, int col){
    this->data = data;
    this->row = row;
    this->col = col;
  }

  Matrix(double arr[], int n, int row, int col) : Matrix(row, col, true){
    this->row = row;
    this->col = col;

    int index = 0;
    for(int i = 0; i < row; i++){
      for(int j = 0; j < col; j++){
        index = i * col + j;
        if(index < n) this->data[i][j] = arr[index];
        else break;
      }

      if(index >= n) break;
    }
  }

  void allocate_matrix(int row, int col, bool ones){
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

  void deallocate_matrix(){
    for(int i = 0; i < row; i++){
      delete[] this->data[i];
    }

    delete[] this->data;
  }

  Matrix * multiply(Matrix *other){
    // Copia a matriz atual para o stack
    double c_data[this->row][this->col];
    int c_row = this->row;
    int c_col = this->col;
    for(int i = 0; i < this->row; i++){
      for(int j = 0; j < this->col; j++){
        c_data[i][j] = this->data[i][j];
      }
    }

    // Desaloca da memória a matriz
    this->deallocate_matrix();

    // Aloca na memória a matriz que será o resultado
    this->allocate_matrix(c_row, other->col, true);

    for(int i = 0; i < c_row; i++){
      for(int j = 0; j < other->col; j++){
        for(int k = 0; k < c_col; k++){
          this->data[i][j] += c_data[i][k] * other->data[k][j];
        }
      }
    }

    return this;
  }

  Matrix * transpose(){
    // Copia a matriz atual para o stack
    double c_data[this->row][this->col];
    int c_row = this->row;
    int c_col = this->col;
    for(int i = 0; i < this->row; i++){
      for(int j = 0; j < this->col; j++){
        c_data[i][j] = this->data[i][j];
      }
    }

    // Desaloca da memória a matriz
    this->deallocate_matrix();

    // Aloca na memória a matriz que será o resultado
    this->allocate_matrix(c_col, c_row, false);

    for(int i = 0; i < c_col; i++){
      for(int j = 0; j < c_row; j++){
        this->data[i][j] = c_data[j][i];
      }
    }

    return this;
  }

  Matrix * invert(){
    int c_row = this->row;
    int c_col = this->col;
    double a[c_row * 2][c_row * 2];

    // Copia a matriz atual para o stack (e prepara a matrix auxiliar para o problema)
    for(int i = 0; i < c_row; i++){
      for(int j = 0; j < c_row; j++){
        a[i][j] = this->data[i][j];
      }
    }

    // Desaloca da memória a matriz
    this->deallocate_matrix();

    // Aloca na memória a matriz que será o resultado
    this->allocate_matrix(c_row, c_col, false);

    for(int i = 0; i < c_row; i++){
      for(int j = c_row; j < 2 * c_row; j++){
        if(i == j - c_row) a[i][j] = 1;
        else a[i][j] = 0;
      }
    }

    for(int i = 0; i < c_row; i++){
      double t = a[i][i];
      for(int j = i; j < 2 * c_row; j++){
        a[i][j] /= t;
      }

      for(int j = 0; j < c_row; j++){
        if(i != j){
          t = a[j][i];

          for(int k = 0; k < 2 * c_row; k++){
            a[j][k] -= t * a[i][k];
          }
        }
      }
    }

    for(int i = 0; i < c_row; i++){
      for(int j = c_row; j < 2 * c_row; j++){
        this->data[i][j - c_row] = a[i][j];
      }
    }

    return this;
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
    this->deallocate_matrix();
  }
};

int main(){
  double m_arr[9] = {1, 3, 3, 1, 4, 3, 1, 3, 4};
  Matrix *m = new Matrix(m_arr, 9, 3, 3);
  m->print();

  std::cout << std::endl;

  m->transpose()->invert();
  m->print();

  delete m;

  return 0;
}
