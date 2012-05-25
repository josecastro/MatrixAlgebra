//---------------------------------------------------------------------
// Conjugate gradient preconditioner matrix solver to solve the equation
//      Ax = b
//      with known A and b.
//
//      PRECONDITION: A is positive semidefinite (symetric)
//
// A is a sparce matrix (in this context)
// b is an array of known values
// M is the preconditioner diagonal M[i] := A[i][i]
// x initially holds a guess, at the end it will hold the solution
// r holds the residuals, p and q hold intermediate values
// alpha, beta, rho, rhoOld and bnorm are scalars. The algorithm stops
// when iter iterations are done or when sqrt(rho) <= epsilon*bnorm
// --------------------------------------------------------------------

void ConjugateGradientPreconditioner(const Matrix& A, const Vector& M, Vector& x, const Vector& b, int iter, double epsilon)
{
  // Escalares
  double rho = 0.0;
  double rhoOld;
  double alpha, beta;
  double bnorm = sqrt(b*b);

  // Arreglos del problema
  Vector r, p, q, z, resTemp;
  // r es Arreglo que contiene residuos parciales
  // p y q contienen valores intermadios

  z = r = p = q = Vectors.newVector(A.dimension());
 
  int i;

  // Calcula la diferencia de r = b - Ax
  r = b - A*x;

  cout << "(epsilon * bnorm) == " << epsilon*bnorm << '\n';
  // Calculo iterativo
  for (i = 0; i < iter; i++)
    {
	  z = r / M;
      rhoOld = rho;
      rho = r*z;

      // Verifica la convergencia
      if(sqrt(rho) <= (epsilon * bnorm)) break;
      cout << "    rho == " << rho << '\n';

      if(i == 0)
        p = r;
      else
        {
          beta = rho / rhoOld;
          p *= beta;
          p += z;
        }

      q = A*p;
      alpha = rho /(p*q);

      x += p*alpha;
      r -= q*alpha;
    }
  cout << "Total de Iteraciones: " << i << '\n';
}
