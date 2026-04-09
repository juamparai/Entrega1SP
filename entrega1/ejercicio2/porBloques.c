#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>


#define BS 64

/* Init square matrix with a specific value */
void initvalmat(double *mat, int n, double val); 

/* Multiply square matrices, blocked version */
void matmulblks(double *a, double *b, double *c, int n);

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int n);

//Para calcular tiempo
double dwalltime();

//Función para calcular máximo, mínimo y promedio de una matriz dada
void calcularEstadisticas(double *a, int n, double *max, double *min, double *prom);

int main(int argc,char*argv[]){
	double *a,*b,*d,*m;
	int i,j,k, n;
	int check=1, print;
	double timetick;

 //controla los argumentos al programa
	if ((argc != 2)) {
		printf("\nUsar: %s n: tamaño de la matriz\n", argv[0]);
		exit(1);   
	}
    n = atoi(argv[1]);

    //aloca memoria para las matrices
	a=(double*)malloc(sizeof(double)*n*n);
	b=(double*)malloc(sizeof(double)*n*n);
  d=(double*)malloc(sizeof(double)*n*n);
  m=(double*)malloc(sizeof(double)*n*n);

	// Inicializa la matriz a de forma tal que la celda (i,j) tiene valor i
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			a[i*n+j] = i;

	// Inicializa la matriz b de forma tal que la celda (i,j) tiene valor j
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			b[i*n+j] = j;

    
    // INICIO CÓMPUTO
    timetick = dwalltime();

    // Cálculo de máximo, mínimo y promedio de la matriz a
    double max_a, min_a, prom_a;
    calcularEstadisticas(a, n, &max_a, &min_a, &prom_a);
    double max_b, min_b, prom_b;
    calcularEstadisticas(b, n, &max_b, &min_b, &prom_b);

    // Cálculo de C = (max_a * max_b - min_a * min_b) / prom_a * prom_b
    double C = (max_a * max_b - min_a * min_b) / (prom_a * prom_b);

    // Calculo d = B x Bt
    // Inicializo D
    for(i = 0; i < n*n; i++)
        d[i] = 0.0;

    for (i = 0; i < n; i += BS) {
        for (j = i; j < n; j += BS) {

            for (k = 0; k < n; k += BS) {

                for (int ii = 0; ii < BS; ii++) {
                    for (int jj = 0; jj < BS; jj++) {
                        for (int kk = 0; kk < BS; kk++) {

                            d[(i+ii)*n + (j+jj)] += 
                                b[(i+ii)*n + (k+kk)] * 
                                b[(j+jj)*n + (k+kk)];

                        }
                    }
                }
            }

            // Duplico
            if (i != j) {
                for (int ii = 0; ii < BS; ii++) {
                    for (int jj = 0; jj < BS; jj++) {
                        d[(j+jj)*n + (i+ii)] = 
                            d[(i+ii)*n + (j+jj)];
                    }
                }
            }
        }
    }
    
    // Cálculo de M = a x d
    matmulblks(a, d, m, n);
        
    // Multiplicación de M por C
    for(i=0;i<n*n;i++)
        m[i] *= C;

    printf("Tiempo en segundos %f\n", dwalltime() - timetick);



	free(a);
	free(b);
	free(d);
	free(m);

	return(0);
}

void calcularEstadisticas(double *a, int n, double *max, double *min, double *prom) {
    int i;
    int tamano = n * n;

    double max_a = a[0];
    double min_a = a[0];
    double sum_a = a[0];

    for(i = 1; i < tamano; i++) {
        double val = a[i];

        if(val > max_a) {
            max_a = val;
        } else if(val < min_a) {
            min_a = val;
        }

        sum_a += val;
    }

    *max = max_a;
    *min = min_a;
    *prom = sum_a / tamano;
}

/*****************************************************************/

/* Multiply square matrices, blocked version */
void matmulblks(double *a, double *b, double *c, int n)
{
  int i, j, k;    /* Guess what... */

  /* Init matrix c, just in case */  
  initvalmat(c, n, 0.0);
  
  for (i = 0; i < n; i += BS)
  {
    for (j = 0; j < n; j += BS)
    {

      for  (k = 0; k < n; k += BS)
      {
        blkmul(&a[i*n + k], &b[j*n + k], &c[i*n + j], n);
      }
    }
  }
}

/*****************************************************************/

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int n)
{
  int i, j, k;    /* Guess what... again... */

  for (i = 0; i < BS; i++)
  {
    for (j = 0; j < BS; j++)
    {
      for  (k = 0; k < BS; k++)
      {
        cblk[i*n + j] += ablk[i*n + k] * bblk[j*n + k];
      }
    }
  }
}
    
/*****************************************************************/

void initvalmat(double *mat, int n, double val)
{
  int i, j;      /* Indexes */

	for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  mat[i*n + j] = val;
		}
	  }
}

/*****************************************************************/


//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}