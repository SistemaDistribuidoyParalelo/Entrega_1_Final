#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


double *A; //Matriz(arreglo) COMPLETO pero recorremos una porcion de la misma
double *A2; //Matriz(arreglo) COMPLETO pero recorremos una porcion de la misma
double *R;
int T,N;
pthread_barrier_t p1;

//imprimir matriz ORGANIZADA POR COLUMNAS
void imprimeMatriz(double *S,int N){
// Imprime la matriz pasada por parametro
// N es la cantidad de bloques, r dimension de cada bloque
  printf(" \n");
  printf(" \n");
  for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%f  ",S[i+j*N]);
        }
        printf(" \n");
  }
}

void *procesoFuncion(void *arg){
  int ID = *(int*)arg;
  int parcial,inicio,i,j,k;
   inicio = ((N)/T) * (ID);
  parcial = ((N)/T) * (ID +1);
  pthread_barrier_wait(&p1);/*Sincronizo la multiplicación*/
  for(i=inicio;i<parcial;i++){
      for(j=0;j<N;j++){
        for(k=0;k<N;k++){
            R[i*N+j]+=(A[i*N+k]*A2[k*N+j]);
        }
      }
  }


    return NULL;

}

int main(int argc,char*argv[]){

 int i,j,k;
 int check=1;
 double timetick;
 double sec;
 struct timeval tv;
 double num=1;
 //Controla los argumentos al programa
 if (argc != 3)
  {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn) T: numeros de hilos\n", argv[0]);
    exit(1);
  }else{
      N = atoi(argv[1]);
      T = atoi(argv[2]);
  }
  //Inicio de semaforo
   if (pthread_barrier_init(&p1,NULL,T) != 0){
        printf("\n Barrera Fallo\n");
    }

 //Aloca memoria para las matrices
 A=(double*)malloc(sizeof(double)*N*N);
 A2=(double*)malloc(sizeof(double)*N*N);
 R=(double*)malloc(sizeof(double)*N*N);
//DEFINICION DE HILOS
pthread_t misHilos[T]; // areglo de hilos para ejecutar
int ID[T];
num=1;
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;

    for(i=0;i<N;i++){
     for(j=0;j<N;j++){
  	//A2 es la transpuesta de la matriz
          A[i*N+j] = num++;
  	      A2[j*N+i] = A[i*N+j];
     }
    }

  for(int id=0;id<T;id++){
    ID[id] = id;
    pthread_create(&misHilos[id], NULL, procesoFuncion,(void *)&ID[id]);
  }

  for(int id=0;id<T;id++){
    pthread_join(misHilos[id],NULL);
  }
  gettimeofday(&tv,NULL);
  timetick = tv.tv_sec + tv.tv_usec/1000000.0;
  printf("Tiempo en segundos %f\n", timetick - sec);

 //Verifica el resultado  LO SACARIA
  /*for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(getValor(R,i,j,ORDENXCOLUMNAS)==N);
   }
  }

  if(check){
   printf("Multiplicacion de matrices resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices resultado erroneo\n");
  }
*/
  //Como sabemos que es por columnas el resultado, lo imprimimos y recorremos d tal forma
  free(A);
  free(A2);
  free(R);
  return(0);
}
