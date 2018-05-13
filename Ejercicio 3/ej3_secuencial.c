#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int *A;
int Pares;
int N;

int main ( int argc , char * argv []) {
int j,m;
double timetick;
double sec;
struct timeval tv;
if (argc != 2){
     printf("\nUsar: %s n\n  n: Dimension del vector (n)\n", argv[0]);
     exit(1);
 }
    m=2; // numero para el mod

    N=atoi(argv[1]);
    //DECLARACION DE VARIABLES
    Pares=0;
    //RESERVA DE MEMORIA DE VECTORES
    A=(int*)malloc(sizeof(int)*N);
    //  INICIALIZACION DE VECTORES
    for(int i=0; i<N;i++){
        A[i] = i;
    }
    printf("\n");

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    for(j = 0; j< N; j++){
      if(!(A[j]&(m-1))){  // A[j]&(m-1) es la equivalencia devuelve 0(par) 1(impar) uso ! para que entre al if
            Pares++;
        }

    }
    gettimeofday(&tv,NULL);
    timetick = tv.tv_sec + tv.tv_usec/1000000.0;
     printf("Tiempo en segundos %f\n", timetick - sec);
     printf("\nLa cantidad de numeros pares en el vector es : %d \n",Pares );
}