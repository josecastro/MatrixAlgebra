//-----------------------------------------------------------------------------
// SparceArray.cpp
//
// Project: Tephra Irazu simulation - (C) CIC-ITCR
// Author : José Castro
// Date   : July 15, 2005
//
// Purpose:
//      CPP implementation of Sparce Array
//
//-----------------------------------------------------------------------------

SparseArray::SparseArray() // Constructor
{
  _num = 0;

  for (int i = 0; i < MAX_ENTRS; i++)
    {
      _entries[i].value =  0.0;
      _entries[i].index = -1;
    }
}

double SparseArray::operator*(const Vector& v) const
  // Efectua la multiplicacion de una fila por un vector
{
  double q = 0;

  for(int i = 0; i < _num; i++)
    {
      q += _entries[i].value * v[_entries[i].index];
    }

  return q;
}

SparseArray& SparseArray::operator *= (double x)
{
  for (int i = 0; i < _num; ++i)
    _entries[i].value *= x;
}

MatrixCursor& SparseArray::operator [](int index)
{
  int i;
  for (i = _num-1; i >=0; --i)
    if (_entries[i].index == index) break;

  return Cursors.newCursor(*this, index, i);
}

void SparseArray::sort()
{
  bool sorted;
  
  do
    {
      sorted = true;
      int i = 0;
      while (i < _num-1)
        {
          if (_entries[i].index > _entries[i+1].index)
            {
              Entry temp = _entries[i];
              _entries[i] = _entries[i+1];
              _entries[i+1] = temp;
	      sorted = false;
            }
          ++i;
        }
    }
  while (!sorted);
}
