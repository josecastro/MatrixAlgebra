
//-----------------------------------------------------------------------------
// Vector.cpp
//
// Project: Tephra Irazu simulation - (C) CIC-ITCR
// Author : Santiago Nu\ez
// Date   : July 15, 2005
//
// Purpose:
//      CPP implementation of Vector arithmetic
//
//-----------------------------------------------------------------------------

VectorClass Vectors(BUFFER_SIZE);

Vector::Vector(int size)
{
  _size  = size;
  _array = new double[size];
  for (int i = 0; i < size; ++i)
    _array[i] = 0.0;
  _anonymous = false;
  _assigned  = false;
}

Vector::~Vector()
{
  if (!_assigned || !_anonymous)
    if (_size) delete [] _array;

  _array     = NULL;
  _size      = 0;
}

Vector& Vector::operator * (double x) const
{
  Vector& result = Vectors.newVector(_size);
  for (int i = 0; i < _size; ++i)
    result._array[i] = _array[i]*x;

  return result;
}

double Vector::operator * (const Vector& other) const
{
  double result = 0.0;

  for (int i = 0; i < _size; ++i)
    result += _array[i]*other._array[i];

  return result;
}

Vector& Vector::operator=(Vector& other)
{
  if (other._size == 0)
    {
      cout << "Cannot assign from an uninitialized vector, Vector::operator=(Vector&)\n";
      throw MatrixError( 4, "Cannot assign from an uninitialized vector, Vector::operator=(Vector&)" );
    }

  if ( _size && ( _size != other._size ) )
    throw MatrixError( 3, "Vectors do not have the same size, Vector::operator=(Vector& other)" );

  if (other._anonymous && !other._assigned)
    {
      if (_size) delete [] _array;
      _size  = other._size;
      _array = other._array;
      other._array    = NULL;
      other._assigned = true;
	  other._size     = 0;
    }
  else
    {
      if (_size == 0 )
	{
	  _size = other._size;
	  _array = new double[_size];
	}
      for ( int i = 0; i < _size; ++i ) _array[i] = other._array[i];
    }

  return *this;
}

Vector& Vector::operator=(const Vector& other)
{
  if ( _size && ( _size != other._size ) )
    throw MatrixError( 3, "Vectors do not have the same size, Vector::operator=(const Vector& other)" );

  for ( int i = 0; i < _size; ++i ) _array[i] = other._array[i];

  return *this;
}

Vector& Vector::operator=(SparseArray& other)
{
  for (int i = 0; i < _size; ++i)
    _array[i] = other[i];

  return *this;
}

Vector& Vector::operator+(const Vector& other) const
{
  if (_size != other._size)
    throw MatrixError(3, "Vectors are not the same size: Vector::operator+Vector");

  Vector& result = Vectors.newVector(_size);

  for (int i = 0; i < _size; ++i)
    result._array[i] = _array[i] + other._array[i];

  return result;
}

Vector& Vector::operator/(const Vector& other) const
{
  if (_size != other._size)
    throw MatrixError(3, "Vectors are not the same size: Vector::operator+Vector");

  Vector& result = Vectors.newVector(_size);

  for (int i = 0; i < _size; ++i)
    result._array[i] = _array[i] / other._array[i];

  return result;
}

Vector& Vector::operator-(const Vector& other) const
{
  if (_size != other._size)
    throw MatrixError(3, "Vectors are not the same size: Vector::operator+Vector");

  Vector& result = Vectors.newVector(_size);

  for (int i = 0; i < _size; ++i)
    result._array[i] = _array[i] - other._array[i];

  return result;
}

Vector& Vector::operator*=(double x)
{
  for (int i = 0; i < _size; ++i)
    _array[i] *= x;
}

Vector& Vector::operator+=(const Vector& other)
{
  if (_size != other._size)
    throw MatrixError(3, "Vectors are not the same size: Vector::operator+Vector");

  for (int i = 0; i < _size; ++i)
    _array[i] += other._array[i];

  return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
  if (_size != other._size)
    throw MatrixError(3, "Vectors are not the same size: Vector::operator+Vector");

  for (int i = 0; i < _size; ++i)
    _array[i] -= other._array[i];

  return *this;
}

VectorClass::VectorClass(int size)
{
  _size = size;
  _pos = 0;
  _List = new Vector* [size];
  for (int i = 0; i < _size; ++i)
    _List[i] = NULL;
}

VectorClass::~VectorClass(void)
{
  for (int i = 0; i < _size; ++i)
    if (_List[i] != NULL) {
      delete _List[i];
      _List[i] = NULL;
    }
  delete [] _List;
  _List = NULL;
}

Vector& VectorClass::newVector(int size)
{
  int i;
  if (_List[_pos] != NULL)
    delete _List[_pos];

  _List[_pos] = new Vector(size);
  _List[_pos]->_anonymous = true;
  i = _pos;
  _pos = ++_pos % _size;

  return *_List[i];
}

// ********* OJO ***********
// have to debug this 'out' is not working
ostream& operator << (ostream& out, const Vector& v)
  /* Metodo que despliega la matriz */
{
  for (int i = 0; i < v.dimension(); i++)
    {
      cout << v[i] << ' ';
    }
  cout << '\n';
  return out;
}

// ********* OJO ***********
// have to debug this 'out' is not working
ostream& operator << (ostream& out, Vector& v)
  /* Metodo que despliega la matriz */
{
  for (int i = 0; i < v.dimension(); i++)
    {
      cout << v[i] << ' ';
    }
  cout << '\n';
  return out;
}
