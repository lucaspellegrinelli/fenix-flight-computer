/*
  Matrix.cpp - Biblioteca para operações matriciais
  Criado por Lucas Pellegrinelli em 5 de Setembro, 2019
*/

// #include "Arduino.h"
#include "Matrix.h"

Matrix::Matrix(){ }

Matrix * Matrix::build_matrix(int row, int col){
  return Matrix::build_matrix(row, col, false);
}

Matrix * Matrix::build_matrix(int row, int col, bool ones){
  Matrix *m = (Matrix*)malloc(sizeof(Matrix));
  m->allocate_matrix(row, col, ones);
  return m;
}

Matrix * Matrix::build_matrix(double **data, int row, int col){
  Matrix *m = (Matrix*)malloc(sizeof(Matrix));
  m->data = data;
  m->row = row;
  m->col = col;
  return m;
}

Matrix * Matrix::build_matrix(double arr[], int n, int row, int col){
  Matrix *m = (Matrix*)malloc(sizeof(Matrix));
  m->allocate_matrix(row, col, true);

  int index = 0;
  for(int i = 0; i < row; i++){
    for(int j = 0; j < col; j++){
      index = i * col + j;
      if(index < n) m->data[i][j] = arr[index];
      else break;
    }

    if(index >= n) break;
  }

  return m;
}

void Matrix::allocate_matrix(int row, int col, bool ones){
  this->row = row;
  this->col = col;

  this->data = (double**)malloc(sizeof(double*) * this->row);
  for(int i = 0; i < this->row; i++){
    this->data[i] = (double*)malloc(sizeof(double) * this->col);

    if(ones){
      for(int j = 0; j < this->col; j++){
        this->data[i][j] = 0;
      }
    }
  }
}

void Matrix::deallocate_matrix(){
  for(int i = 0; i < row; i++){
    free(this->data[i]);
  }

  free(this->data);
}

Matrix * Matrix::sum(Matrix *other){
  for(int i = 0; i < this->row; i++){
    for(int j = 0; j < this->col; j++){
      this->data[i][j] += other->data[i][j];
    }
  }

  return this;
}

Matrix * Matrix::subtract(Matrix *other){
  for(int i = 0; i < this->row; i++){
    for(int j = 0; j < this->col; j++){
      this->data[i][j] -= other->data[i][j];
    }
  }

  return this;
}

Matrix * Matrix::multiply(Matrix *other){
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

Matrix * Matrix::transpose(){
  // Copia a matriz atual para o stack
  double c_data[this->row][this->col];
  int c_row = this->row;
  int c_col = this->col;
  for(int i = 0; i < this->row; i++){
    for(int j = 0; j < this->col; j++){
      c_data[i][j] = this->data[i][j];
    }
  }

  for(int i = 0; i < c_col; i++){
    for(int j = 0; j < c_row; j++){
      this->data[i][j] = c_data[j][i];
    }
  }

  return this;
}

Matrix * Matrix::invert(){
  int c_row = this->row;
  int c_col = this->col;
  double a[c_row * 2][c_row * 2];

  // Copia a matriz atual para o stack (e prepara a matrix auxiliar para o problema)
  for(int i = 0; i < c_row; i++){
    for(int j = 0; j < c_row; j++){
      a[i][j] = this->data[i][j];
    }
  }

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

Matrix * Matrix::copy(){
  Matrix *c = Matrix::build_matrix(this->row, this->col);
  for(int i = 0; i < this->row; i++){
    for(int j = 0; j < this->col; j++){
      c->data[i][j] = this->data[i][j];
    }
  }

  return c;
}

Matrix::~Matrix(){
  this->deallocate_matrix();
}
