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
  
  // Inicializa la matriz a = matriz identidad
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
            if (i == j)
                a[i*n+j] = 1;
            else
    			a[i*n+j] = 0;

	// Inicializa la matriz b por columnas de forma tal que la celda (i,j) tiene valor i
	for(int j=0;j<n;j++)
		for(int i=0;i<n;i++)
			b[i+j*n] = i;

  for (int i=0; i<n*n; i++) {
    aux[i]=0.0;
    r[i]=0.0;
  }

  timetick = dwalltime();

  // Calculos
  double maxA = -1.0, minA = 1e10, sumA = 0;
  double maxB = -1.0, minB = 1e10, sumB = 0;
  int i, j;
  for (int k=0; k<n*n; k++) {
    if (a[k]>maxA) maxA = a[k];
    if (a[k]<minA) minA = a[k];
    sumA += a[k];
  }

  for (int k=0; k<n*n; k++) {
    if (b[k]>maxB) maxB = b[k];
    if (b[k]<minB) minB = b[k];
    sumB += b[k];

    i = k / n;
    j = k % n;
    bt[j*n + i] = b[k];
  }

  double t = (maxA * maxB - minA * minB) / ((sumA/(n*n)) * (sumB/(n*n)));

  // aux = A * B
  matmulblks(a, b, aux, n);

  // res = aux * BT
  matmulblks(aux, bt, r, n); 

  // Aplicar el escalar t al resultado
  for (int i=0; i<n*n; i++) {
    r[i] *= t;
  }
  
  printf("Tiempo solución por bloques: %f\n", dwalltime() - timetick);
  printf("C= %f\n", t);
  for (i=0; i<10; i++) {
    for (j=0; j<10; j++) {
      printf("%f ", r[i*n+j]);
    }
    printf("\n");
  }
  // libera la memoria reservada
  free(a);
  free(b);
  free(r);
  free(aux);
  free(bt);

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