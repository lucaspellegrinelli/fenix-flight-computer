/*
  Matrix.h - Biblioteca para operações matriciais
  Criado por Lucas Pellegrinelli em 5 de Setembro, 2019
*/

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

// #include "Arduino.h"

class Matrix{
public:
  double **data;
  int row;
  int col;

  Matrix();

  static Matrix * build_matrix(int row, int col);
  static Matrix * build_matrix(int row, int col, bool ones);
  static Matrix * build_matrix(double **data, int row, int col);
  static Matrix * build_matrix(double arr[], int n, int row, int col);

  void allocate_matrix(int row, int col, bool ones);
  void deallocate_matrix();

  Matrix * sum(Matrix *other);
  Matrix * subtract(Matrix *other);
  Matrix * multiply(Matrix *other);
  Matrix * transpose();
  Matrix * invert();

  ~Matrix();
};

#endif
