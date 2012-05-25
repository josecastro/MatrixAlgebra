
//-----------------------------------------------------------------------------
// Matrix.cpp
//
// Project: Tephra Irazu simulation - (C) CIC-ITCR
// Author : Santiago Nu\ez
// Date   : July 15, 2005
//
// Purpose:
//      CPP implementation of Square Sparce Matrix
//
//-----------------------------------------------------------------------------

#include "Matrix.h"


MatrixCursorClass Cursors(BUFFER_SIZE);
MatrixClass Matrices(BUFFER_SIZE);

#include "ConjugateGradient.cpp"
#include "ConjugateGradientPrecon.cpp"
#include "SparceArray.cpp"
#include "Vector.cpp"

const double Matrix::ZERO = 0.00000001;

MatrixCursor& MatrixCursor::operator=(double x)
{
  if (_pos != -1)
    if ((x > Matrix::ZERO) || (x < -Matrix::ZERO))
      _matrixRow->_entries[_pos].value = x;
    else
      {
        for (int i = _pos; i < _matrixRow->_num-1; ++i)
          _matrixRow->_entries[i] = _matrixRow->_entries[i+1];
	--_matrixRow->_num;
	_matrixRow->_entries[_matrixRow->_num].index = -1;
      }
  else
    if (_matrixRow->_num == SparseArray::MAX_ENTRS)
      throw MatrixError(1, "Too many entries in sparce array. MatrixCursor::operator=(double x)");
    else
      {
        _matrixRow->_entries[_matrixRow->_num].value = x;
        _matrixRow->_entries[_matrixRow->_num].index = _index;
        _matrixRow->_num++;
      }
  return *this;
}

MatrixCursorClass::MatrixCursorClass(int size)
{
  _size = size;
  _pos = 0;
  _List = new MatrixCursor[size];
}

MatrixCursorClass::~MatrixCursorClass(void)
{
  delete [] _List;
  _List = NULL;
}

MatrixCursor& MatrixCursorClass::newCursor(SparseArray& mr, int index, int pos)
{
  int i;

  _List[_pos].init(mr, index, pos);
  i = _pos;
  _pos = ++_pos % _size;

  return _List[i];
}

Matrix::Matrix(int dim)
{
  _dim       = dim;
  _rows      = new SparseArray[dim];
  _assigned  = false;
  _anonymous = false;
}

Matrix::Matrix(Matrix& other)
{
  _dim = other._dim;

  if (other._anonymous && !other._assigned)
    {
      _rows = other._rows;
      other._assigned = 1;
    }
  else
    {
      _rows = new SparseArray[_dim];
      for ( int i = 0; i < _dim; ++i ) _rows[i] = other._rows[i];
    }
  _anonymous = false;
  _assigned  = false;
}

Matrix::Matrix(const Matrix& other)
{
  _dim = other._dim;

  _rows = new SparseArray[_dim];
  for ( int i = 0; i < _dim; ++i ) _rows[i] = other._rows[i];
  _anonymous = false;
  _assigned  = false;
}

Matrix::~Matrix()
{
  if (!_anonymous || !_assigned)
    delete [] _rows;
  _rows = NULL;
  _assigned = true;
}

Matrix& Matrix::operator=(Matrix& other)
{
  if ( _dim && ( _dim != other._dim ) )
    throw MatrixError( 3, "Matrixes do not have the same size, Matrix::operator=(Matrix& other)" );

  _dim = other._dim;
  if (other._anonymous && !other._assigned)
    {
      if (_rows) delete [] _rows;
      _rows = other._rows;
      other._assigned = true;
    }
  else
    for ( int i = 0; i < _dim; ++i ) _rows[i] = other._rows[i];

  return *this;
}

Matrix& Matrix::operator=(const Matrix& other)
{
  if ( _dim && ( _dim != other._dim ) )
    throw MatrixError( 3, "Matrixes do not have the same size, Matrix::operator=(Matrix& other)" );

  for ( int i = 0; i < _dim; ++i ) _rows[i] = other._rows[i];

  return *this;
}

// ********* OJO ***********
// have to debug this 'out' is not working
ostream& operator << (ostream& out, Matrix& m)
  /* Metodo que despliega la matriz */
{
  int index;
  for (int i = 0; i < m.dimension(); i++)
    {
      m[i].sort();
      index = 0;

      for (int j = 0; j < m._dim; ++j)
        {
	  cout.width(4);
          /* Fila y columna coinciden, imprima la diagonal */
          if (m._rows[i]._entries[index].index != j)
            cout << "   0";
          else
            {
              cout << m._rows[i]._entries[index].value;
              ++index;
            }
        }
      cout << '\n';
    }
  return out;
}

Vector& Matrix::operator *(const Vector& p) const
  /* Multiplicacion de la matriz por un vector de divergencia
   *
   * Precondicion: el vector es de tama~o _dim
   */
{
  // Se recorre la matriz por filas
  Vector& q = Vectors.newVector(_dim);
  for (int i = 0; i < _dim; i++)
    q[i] = _rows[i] * p;

  return q;
}

Matrix& Matrix::operator *=(double x)
{
	for (int i = 0; i < _dim; ++i)
		_rows[i] *= x;
}

MatrixClass::MatrixClass(int size)
{
  _size = size;
  _pos = 0;
  _List = new Matrix* [size];
  for (int i = 0; i < _size; ++i)
    _List[i] = NULL;
}

MatrixClass::~MatrixClass(void)
{
  for (int i = 0; i < _size; ++i)
    if (_List[i] != NULL) {
      delete _List[i];
      _List[i] = NULL;
    }
  delete [] _List;
  _List = NULL;
}

Matrix& MatrixClass::newMatrix(int size)
{
  if (_List[_pos] != NULL)
    delete _List[_pos];

  _List[_pos] = new Matrix(size);
  _List[_pos]->_anonymous = true;
  int i = _pos;
  _pos = ++_pos % _size;

  return *_List[i];
}

void set(Matrix& m, int i, int j, double value)
{
  m[i][j] = value;
  if (i != j)
    m[j][i] = value;
}
