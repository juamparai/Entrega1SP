#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Para calcular el tiempo
double dwalltime();

/*************** MIAN ***************/
int main(int argc, char *argv[])
{
  int n = atoi(argv[1]);
  double *a, *b, *bt, *r, *aux;

  double timetick;
  
  // alocacion de memoria para las matrices
  a = (double *)malloc(n*n*sizeof(double));
  b = (double *)malloc(n*n*sizeof(double));
  bt = (double *)malloc(n*n*sizeof(double));
  r = (double *)malloc(n*n*sizeof(double));
  aux = (double *)malloc(n*n*sizeof(double));
  
  // Inicializacion fuera de la medicion
  for (int i=0; i<n*n; i++) {
    a[i]=1.0;
    b[i]=1.0;
    aux[i]=0.0;
    r[i]=0.0;
  }

  timetick = dwalltime();

  // Calculos
  double maxA = -1.0, minA = 1e10, sumA = 0;
  double maxB = -1.0, minB = 1e10, sumB = 0;
  
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      int idx = i*n+j;

      if (a[idx]>maxA) maxA = a[idx];
      if (a[idx]<minA) minA = a[idx];
      sumA += a[idx];

      if (b[idx]>maxB) maxB = b[idx];
      if (b[idx]<minB) minB = b[idx];
      sumB += b[idx];

      bt[j * n + i] = b[idx]; // Traspuesta de b
    }
  }
  double t = (maxA * maxB - minA * minB) / ((sumA/(n*n)) * (sumB/(n*n)));

  // Producto A*B (usando BT para localidad espacial)
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      double suma = 0;
      for (int k=0; k<n; k++) {
        suma += a[i*n+k] * bt[j*n+k];
      }
      aux[i*n+j] = suma;
    }
  }

  // Producto (A*B)*BT (BT es traspuesta, usamos b para acceso fila-fila)
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      double suma = 0;
      for (int k=0; k<n; k++) {
        suma += aux[i*n+k] * b[j*n+k];
      }
      r[i*n+j] = t*suma;
    }
    }

  


  double workTime = dwalltime() - timetick;
  printf("Tiempo: %f\n", workTime);

  // libera la memoria reservada
  free(a);
  free(b);
  free(r);

  return(0);
}

//********************************************//

// Para calcular tiempo
double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}