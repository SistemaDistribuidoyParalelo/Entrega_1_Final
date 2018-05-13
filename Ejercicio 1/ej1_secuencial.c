#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



double *A; //Matriz(arreglo) COMPLETO pero recorremos una porcion de la misma
double *A2; //Matriz(arreglo) COMPLETO pero recorremos una porcion de la misma
double *R;
int T,N;

//imprimir matriz ORGANIZADA POR COLUMNAS
void imprimeMatriz(double *S,int N){
// Imprime la matriz pasada por parametro
// N es la cantidad de bloques, r dimension de cada bloque
  printf(" \n");
  printf(" \n");
  for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%f  ",S[j+i*N]);
        }
        printf(" \n");
  }
}


int main(int argc,char*argv[]){

 int i,j,k;
 int check=1;
 double timetick;
 double sec;
 struct timeval tv;
 double num=1;
 //Controla los argumentos al programa
 if (argc != 2)
  {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }else{
      N = atoi(argv[1]);
  }

 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  A2=(double*)malloc(sizeof(double)*N*N);
  R=(double*)malloc(sizeof(double)*N*N);

  //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  //inicia tiempos
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;

  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	//A2 es la transpuesta de la matriz
        A[i*N+j] = num;
        num++;
	      A2[j*N+i] = A[i*N+j];
   }
  }

//LINEAL
 //Realiza la multiplicacion
 for(i=0;i<N;i++){
  for(j=0;j<N;j++){
   for(k=0;k<N;k++){
     R[i*N+j]+=(A[i*N+k]*A2[k*N+j]);
   }
  }
 }

// toma el tiempo
 gettimeofday(&tv,NULL);
 timetick = tv.tv_sec + tv.tv_usec/1000000.0;
  printf("Tiempo en segundos %f\n", timetick - sec);

 //Verifica el resultado //Esto funciona solamente si es todo 1
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(R[i+j*N]==N);
   }
  }

  if(check){
   printf("Multiplicacion de matrices resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices resultado erroneo\n");
  }
  //hasta aca verificaciones
  //Como sabemos que es por columnas el resultado, lo imprimimos y recorremos d tal forma
  imprimeMatriz(R,N);
  imprimeMatriz(A,N);
  free(A);
  free(A2);
  free(R);
  return(0);

}
