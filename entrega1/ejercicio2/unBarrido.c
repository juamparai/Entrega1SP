#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>


#define BS 64

/* Init square matrix with a specific value */
void initvalmat(double *mat, int n, double val); 

/* Multiply square matrices, blocked version */
void matmulblks(double *a, double *b, double *c, int n, double *max_a, double *min_a, double *prom_a);

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int n, int collect_stats_a, double *max_a, double *min_a, double *sum_a, int *count_a);

//Para calcular tiempo
double dwalltime();

int main(int argc,char*argv[]){
	double *a,*b,*d,*m;
	int i,j,k, n;
    int check=1;
	double timetick;

 //controla los argumentos al programa
  if (argc < 2 || argc > 3) {
    printf("\nUsar: %s n [check]\n", argv[0]);
		exit(1);   
	}
    n = atoi(argv[1]);
    if (argc == 3)
        check = atoi(argv[2]);
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

    double max_a = 0.0, min_a = 0.0, prom_a = 0.0;
    double max_b = 0.0, min_b = 0.0, prom_b = 0.0;
    double sum_b = 0.0;
    int count_b = 0;

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

                          if (i == j && jj == 0) {
                            double val_b = b[(i+ii)*n + (k+kk)];
                            if (count_b == 0) {
                              max_b = val_b;
                              min_b = val_b;
                            } else {
                              if (val_b > max_b) max_b = val_b;
                              if (val_b < min_b) min_b = val_b;
                            }
                            sum_b += val_b;
                            count_b++;
                          }

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

    prom_b = sum_b / count_b;
    
    // Cálculo de M = a x d
    matmulblks(a, d, m, n, &max_a, &min_a, &prom_a);

    // Cálculo de C = (max_a * max_b - min_a * min_b) / prom_a * prom_b
    double C = (max_a * max_b - min_a * min_b) / (prom_a * prom_b);
        

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

/*****************************************************************/

/* Multiply square matrices, blocked version */
void matmulblks(double *a, double *b, double *c, int n, double *max_a, double *min_a, double *prom_a)
{
  int i, j, k;    /* Guess what... */
  double sum_a = 0.0;
  int count_a = 0;

  /* Init matrix c, just in case */  
  initvalmat(c, n, 0.0);
  
  for (i = 0; i < n; i += BS)
  {
    for (j = 0; j < n; j += BS)
    {

      for  (k = 0; k < n; k += BS)
      {
        blkmul(&a[i*n + k], &b[j*n + k], &c[i*n + j], n, j == 0, max_a, min_a, &sum_a, &count_a);
      }
    }
  }

  *prom_a = sum_a / count_a;
}

/*****************************************************************/

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int n, int collect_stats_a, double *max_a, double *min_a, double *sum_a, int *count_a)
{
  int i, j, k;    /* Guess what... again... */

  for (i = 0; i < BS; i++)
  {
    for (j = 0; j < BS; j++)
    {
      for  (k = 0; k < BS; k++)
      {
        if (collect_stats_a && j == 0)
        {
          double val_a = ablk[i*n + k];
          if (*count_a == 0)
          {
            *max_a = val_a;
            *min_a = val_a;
          }
          else
          {
            if (val_a > *max_a) *max_a = val_a;
            if (val_a < *min_a) *min_a = val_a;
          }
          *sum_a += val_a;
          (*count_a)++;
        }

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