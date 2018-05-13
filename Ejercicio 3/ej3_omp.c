#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<omp.h>

int *A;
int *B;
int Pares;
int T;
int N;



int main ( int argc , char * argv []) {
int j,m;
double timetick;
double sec;
struct timeval tv;
if (argc != 3){
     printf("\nUsar: %s n\n  n: Dimension del vector (n) y luego la cantidad de Hilos y el valor a buscar\n", argv[0]);
     exit(1);
 }

    T=atoi(argv[2]);
    N=atoi(argv[1]);
    omp_set_num_threads(T);
    if ((T== 2)||(T==4)) {
      printf("Se van a utilizar %d Hilos \n",T );
      /* code */
    //DECLARACION DE VARIABLES
    Pares=0;
    m=2;
    //RESERVA DE MEMORIA DE VECTORES
    A=(int*)malloc(sizeof(int)*N);
    B=(int*)malloc(sizeof(int)*T);

    //  INICIALIZACION DE VECTORES
    for(int i=0; i<N;i++){
        A[i] = i;
    }
    printf("\n");
  //OPENMPI

  //ver si parametrizando el for funciona bien
  //veri si parametrizando en estrucutras fija va bien
  //hacerlo con sched
  //dinamic o como corno era


// #pragma omp parallel for schedule (dynamic,4) tarda demasiado a medida que aumenta el numero del arreglo
//   #pragma omp parallel for schedule (guided,4) aun disminuyendo enla cantidad a verificar, los tiempos son mejores los ultimos.

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
   #pragma omp parallel for shared(A,B) private(j) reduction(+: Pares)
    for(j = 0; j< N; j++){
      if(!(A[j]&(m-1))){  // A[j]&(m-1) es la equivalencia devuelve 0(par) 1(impar) uso ! para que entre al if
            Pares++;
        }
    }
    gettimeofday(&tv,NULL);
    timetick = tv.tv_sec + tv.tv_usec/1000000.0;
     printf("Tiempo en segundos %f\n", timetick - sec);

    for (int i = 0; i < T; i++) {
      Pares += B[i];
    }
    printf("\n" );
    printf("\nLa cantidad de numeros pares en el vector es : %d \n",Pares );
  }else{
    printf("El numero de hilos posibles es 2 o 4 \n" );
  }
}
