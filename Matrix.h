//-----------------------------------------------------------------------------
// Matrix.h
//
// Project: Tephra Irazu simulation - (C) CIC-ITCR
// Author : José Castro
// Date   : July 15, 2005
//
// Purpose:
//      CPP specification of Sparce Matrix and
//      conjugate gradient method solver
//
//-----------------------------------------------------------------------------

#include <iostream>
#include <ostream>
#include <stdio.h>
#include <fstream>
#include <cmath>
#include <string.h>

#ifndef MATRIX
#define MATRIX

#define BUFFER_SIZE 20
using namespace std;

class Matrix;
class Vector;
class SparseArray;
class VectorClass;
class MatrixClass;
class MatrixCursor;
class MatrixCursorClass;

extern MatrixCursorClass Cursors;
extern MatrixClass Matrices;
extern VectorClass Vectors;

ostream& operator << (ostream& out, Matrix& m);
ostream& operator << (ostream& out, const Vector& v);
ostream& operator << (ostream& out, Vector& v);

class MatrixError
{
 public:
  static const int ERROR_SIZE = 64;
  MatrixError( int num, const char* string )
    {
      _errNo = num;
      strncpy( _errString, string, ERROR_SIZE );
    }
  const char* errString(void);
  int errNo(void);
 private:

  int _errNo;
  char _errString[ERROR_SIZE];
};

class SparseArray
{
  friend class MatrixCursor;
  friend class Matrix;
  friend ostream& operator << (ostream& out, Matrix& m);
 private:
  static const int MAX_ENTRS = 7;
  struct Entry
  {
    int    index;
    double value;
  };
  int   _num;                // Numero de terminos fuera de la diagonal
  Entry _entries[MAX_ENTRS]; // Valores dispersos a traves de la fila

 public:
  SparseArray();
  double         operator *(const Vector& other) const;
  SparseArray&   operator *=(double x);
  MatrixCursor&  operator[](int index);
  void   sort    ();
};

class Vector
{
  friend class VectorClass;
  friend ostream& operator << (ostream& out, Vector& v);
 private:
  double* _array;
  int     _size;
  bool    _assigned;
  bool    _anonymous;
  Vector(int size);
 public:
  ~Vector();
  Vector() : _array(0), _size(0), _assigned(false), _anonymous(false) {}
  Vector& operator * (double x) const;
  int     dimension() const { return _size; }
  double  operator * (const Vector& other) const;
  double  operator * (SparseArray& arr) const { return arr * *this; }
  double& operator [](int pos) { return _array[pos]; }
  double  operator [](int pos) const { return _array[pos]; }
  Vector& operator=(Vector& other);
  Vector& operator=(const Vector& other);
  Vector& operator=(SparseArray& other);
  Vector& operator+(const Vector& other) const;
  Vector& operator-(const Vector& other) const;
  Vector& operator/(const Vector& other) const;
  Vector& operator*=(double x);
  Vector& operator+=(const Vector& other);
  Vector& operator-=(const Vector& other);
  bool    operator==(double x)
  { for (int i = 0; i < _size; ++i)
	  if (x != _array[i]) return false;
	return true;
  }
};

class MatrixCursor
{
  friend ostream& operator << (ostream& out, Matrix& m);
  friend class MatrixCursorClass;
 private:
  SparseArray* _matrixRow;
  int _index;  // external index
  int _pos;    // internal index
  void init(SparseArray& mr, int index, int pos) { _matrixRow = &mr; _index = index; _pos = pos; }
  MatrixCursor(SparseArray& mr, int index, int pos) : _matrixRow(&mr), _index(index), _pos(pos) {}
  MatrixCursor() {}
 public:
  MatrixCursor& operator=(double);
  operator double() { return (_pos == -1 ? 0.0 : _matrixRow->_entries[_pos].value); }
};

class Matrix
{
  friend ostream& operator << (ostream& out, Matrix& m);
  friend class MatrixClass;
 private:
  int        _dim;     // Dimension de la matriz
  SparseArray* _rows;  // Filas de la matriz
  Matrix(int dim);                                    // Initializes as zero matrix
  Matrix(Matrix& other);
  Matrix(const Matrix& other);
  // Miscellaneous;
  bool  _assigned;
  bool  _anonymous;

 public:
  static const double ZERO;
  void display() const;                               // displays current matrix
  Vector& operator* (const Vector& p) const;  // Multiply matrix by a divergence vector
  Matrix() : _dim(0), _rows(0), _assigned(false), _anonymous(false) {}
  virtual ~Matrix();
  int dimension() const { return _dim; }
  SparseArray& operator[](int i) { return _rows[i]; }
  Matrix& operator=(Matrix& other);
  Matrix& operator=(const Matrix& other);
  Matrix& operator*=(double x);
};

class MatrixClass
{
 public:
  MatrixClass(int size);
  ~MatrixClass(void);
  Matrix&  newMatrix(int size);
 private:
  int _size;
  int _pos;
  Matrix** _List;
};

class MatrixCursorClass
{
 public:
  MatrixCursorClass(int size);
  ~MatrixCursorClass(void);
  MatrixCursor& newCursor(SparseArray& mr, int index, int pos);
 private:
  int _size;
  int _pos;
  MatrixCursor* _List;
};

class VectorClass
{
 public:
  VectorClass(int size);
  ~VectorClass(void);
  Vector& newVector(int size);
 private:
  int _size;
  int _pos;
  Vector** _List;
};

void ConjugateGradientSolve(const Matrix& m, Vector& x, const Vector& b, int iter,  double epsilon);
void ConjugateGradientPreconditioner(const Matrix& A, const Vector& M, Vector& x, const Vector& b, int iter, double epsilon);

#endif
