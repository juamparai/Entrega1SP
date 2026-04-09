#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BS 64 // Como constante

// Para calcular el tiempo
double dwalltime();

// Bloques
void matmulblks(double *a, double *b, double *c, int n);
void blkmul(double *ablk, double *bblk, double *cblk, int n);

/*************** MIAN ***************/
int main(int argc, char *argv[])
{
  int n = atoi(argv[1]);
  double *a, *b, *bt, *r, *aux;

  double timetick;

  // Chequeo de paramteros
  if((argc != 2) || ((n=atoi(argv[1]))<=0) || ((n%BS)!=0)) {
    printf("\nError en los parámetros. Usage: ./%s N (N debe ser multiple de BS)\n", argv[0]);
    exit(1);
  }

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

  // aux = A * B
  matmulblks(a, bt, aux, n); // Se usa bt (traspuesta de b) para aprovechar localidad espacial

  // res = aux * BT
  matmulblks(aux, b, r, n); // Se usa b (traspuesta de bt) para aprovechar localidad espacial

  // Aplicar el escalar t al resultado
  for (int i=0; i<n*n; i++) {
    r[i] *= t;
  }

  double workTime = dwalltime() - timetick;
  printf("Tiempo solución por bloques: %f\n", workTime);

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

//*******************************************//
// Bloques
void matmulblks(double *a, double *b, double *c, int n) {
  int i, j, k;
  for (i=0; i<n; i+=BS) {
    for (j=0; j<n; j+=BS) {
      for (k=0; k<n; k+=BS) {
        blkmul(&a[i*n+k], &b[j*n+k], &c[i*n+j], n);
      }
    }
  }
}

void blkmul(double *ablk, double *bblk, double *cblk, int n) {
  int i, j, k;
  for(i=0; i<BS; i++) {
    for(j=0; j<BS; j++) {
      for(k=0; k<BS; k++) {
        cblk[i*n+j] += ablk[i*n+k] * bblk[j*n+k];
      }
    }
  }
}