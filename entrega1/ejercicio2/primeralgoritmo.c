#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

#define ORDENXFILAS 0
#define ORDENXCOLUMNAS 1

//Para calcular tiempo
double dwalltime();


//Retorna el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
double getValor(double *matriz,int fila,int columna, int n, int orden);

//Establece el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
void setValor(double *matriz, int fila,int columna, int n, int orden, double valor);

//Función para calcular máximo, mínimo y promedio de una matriz dada
void calcularEstadisticas(double *a, int n, double *max, double *min, double *prom);

int main(int argc,char*argv[]){
	double *a,*b,*d,*m;
	int i,j,k, n;
	int check=1, print;
	double timetick;

 //controla los argumentos al programa
	if ((argc != 2)) {
		printf("\nUsar: %s n\n", argv[0]);
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
			setValor(a,i,j,n,ORDENXFILAS,i);

	// Inicializa la matriz b de forma tal que la celda (i,j) tiene valor j
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			setValor(b,i,j,n,ORDENXFILAS,j);

    
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
    for(i=0;i<n;i++)
        for(j=0;j<n;j++){
             double suma = 0;
            for(k=0;k<n;k++)
                suma += getValor(b,i,k,n,ORDENXFILAS) * getValor(b,j,k,n,ORDENXFILAS);
            setValor(d,i,j,n,ORDENXFILAS,suma); 
        }

    
    // Cálculo de M = a x d
    for(i=0;i<n;i++)
        for(j=0;j<n;j++){
             double suma = 0;
            for(k=0;k<n;k++)
                suma += getValor(a,i,k,n,ORDENXFILAS) * getValor(d,k,j,n,ORDENXFILAS);
            setValor(m,i,j,n,ORDENXFILAS,suma); 
        }
    
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

//Retorna el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
double getValor(double *matriz,int fila,int columna, int n, int orden){
	if(orden==ORDENXFILAS)
		return(matriz[fila*n+columna]);
	else
		return(matriz[fila+columna*n]);
 }

//Establece el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
void setValor(double *matriz,int fila,int columna, int n, int orden,double valor){
	if(orden==ORDENXFILAS)
		matriz[fila*n+columna]=valor;
	else
		matriz[fila+columna*n]=valor;
}

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}