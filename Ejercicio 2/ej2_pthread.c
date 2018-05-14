#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<pthread.h>


//declaracion de variables

int N; // tam de la matriz
int T; //cantidad de HIlos
double *A,*A2,*B,*C,*D,*E,*F;
double *L,*U; // matriz triangular L superior U inferior
double *ulAAtrans,*parcialC,*bL,*BE,*bD,*UF; //matrices parciales
double *f1,*f2,*f3; // matrices resultado final
double *M; //resultado final
//double variables_Promedios[5];
double timetick;
double sec;
struct timeval tv;
double u,l,b,ul;
double temp,temp1,temp2;
pthread_mutex_t p1;
pthread_mutex_t p2;
pthread_mutex_t p3;
pthread_mutex_t p4;
pthread_mutex_t p5;
pthread_mutex_t p6;
pthread_mutex_t p7;

//DECLARACION DE FUNCIONES UTILIZADAS EN EL PROGRAMA


void imprimeMatriz(double *S, int tipo_fc) {
// Imprime la matriz pasada por parametro
// N es la cantidad de bloques, r dimension de cada bloque
  printf(" \n");
  printf(" \n");
  if(tipo_fc == 0){
    for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                printf("%f  ",S[i+j*N]);
            }
            printf(" \n");
    }
  }else{
      for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                printf("%f  ",S[i*N+j]);
            }
            printf(" \n");
    }
  }
}


void *procesoFuncion(void *arg){
    int parcial, inicio;
    int i,k,j;
    int ID = *(int*)arg;

    //printf("el ID = %d \n",ID );
    inicio = ((N)/T) * (ID);
    parcial = ((N)/T) * (ID +1);
    //printf("el Inicio = %d \n",inicio );
  //  printf("el Parcial = %d \n",parcial );


pthread_mutex_lock(&p1);
    for(i=inicio;i<parcial;i++){
        for(j=0;j<N;j++){
            for(k = 0;k<N;k++){
                ulAAtrans[i*N+j] +=ul*A[i*N+k]*A2[k*N+j];
            }
        }
    }
pthread_mutex_unlock(&p1);

    //L es inferior, recorrido parcial
pthread_mutex_lock(&p2);
    for(i=inicio;i<parcial;i++){
        for(j=0;j<N;j++){
            for(k = 0;k<i+1;k++){
                bL[i*N+j] +=b*L[k+((i*(i+1)/2))]*B[j*N+k];
            }

        }
    }
    pthread_mutex_unlock(&p2);

    //U es superior
pthread_mutex_lock(&p3);
    for(i=inicio;i<parcial;i++){
        for(j=0;j<N;j++){
            for(k = 0;k<j+1;k++){
              bD[i*N+j] +=b*D[i*N+k]*U[k+((j*(j+1))/2)];

            }
        }
    }
pthread_mutex_unlock(&p3);
pthread_mutex_lock(&p4);
    for(i=inicio;i<parcial;i++){
        for(j=0;j<N;j++){
            for(k = 0;k<N;k++){
              parcialC[i*N+j] += b*ulAAtrans[i*N+k]*C[k*N+j];
            }
        }
    }
    pthread_mutex_unlock(&p4);
    pthread_mutex_lock(&p5);
    for(i=inicio;i<parcial;i++){
        for(j=0;j<N;j++){
            for(k = 0;k<N;k++){
              BE[i*N+j] += bL[i*N+k]*E[k*N+j];
            }
        }
    }
    pthread_mutex_unlock(&p5);
    pthread_mutex_lock(&p6);
     for(i=inicio;i<parcial;i++){
        for(j=0;j<N;j++){
            for(k = 0;k<N;k++){
              UF[i*N+j] += bD[i*N+k]*F[k*N+j];
            }
        }
    }
pthread_mutex_unlock(&p6);
    //Resultado Final
    pthread_mutex_lock(&p7);
    for(i=inicio;i<parcial;i++){
        for(j=0;j<N;j++){
            for(k = 0;k<N;k++){
                M[i*N+j] = parcialC[i*N+j]+BE[i*N+j]+UF[i*N+j];
            }
        }
     }
     pthread_mutex_unlock(&p7);
  //  printf("Termino el Hilo d ejecucion unico\n");
    return NULL;

}

//MAIN
int main(int argc,char*argv[]){
  int f,c;
  if (argc != 3 )
   {

     printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)-----Hilos\n", argv[0]);
     exit(1);
   }else{
     T =  atoi(argv[2]);
     N = atoi(argv[1]);
   }

   //ALOCACION DE MEMORIA PARA LAS MATRICES
    A=(double*)malloc(sizeof(double)*N*N);
    A2=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
    D=(double*)malloc(sizeof(double)*N*N);
    E=(double*)malloc(sizeof(double)*N*N);
    F=(double*)malloc(sizeof(double)*N*N);
    U=(double*)malloc(sizeof(double)*((N*(N+1))/2));
	  L=(double*)malloc(sizeof(double)*((N*(N+1))/2));
    M=(double*)malloc(sizeof(double)*N*N);
    ulAAtrans=(double*)malloc(sizeof(double)*N*N);
    parcialC=(double*)malloc(sizeof(double)*N*N);
    bL=(double*)malloc(sizeof(double)*N*N);
    BE=(double*)malloc(sizeof(double)*N*N);
    bD=(double*)malloc(sizeof(double)*N*N);
    UF=(double*)malloc(sizeof(double)*N*N);

    //variables_Promedios=(double*)malloc(sizeof(double)*4);
     if (pthread_mutex_init(&p1, NULL) != 0){
        printf("\n mutex Fallo\n");
    }

     if (pthread_mutex_init(&p2, NULL) != 0){
        printf("\n mutex Fallo\n");
    }

     if (pthread_mutex_init(&p3, NULL) != 0){
        printf("\n mutex Fallo\n");
    }

     if (pthread_mutex_init(&p4, NULL) != 0){
        printf("\n mutex Fallo\n");
    }

     if (pthread_mutex_init(&p5, NULL) != 0){
        printf("\n mutex Fallo\n");
    }

     if (pthread_mutex_init(&p6, NULL) != 0){
        printf("\n mutex Fallo\n");
    }

     if (pthread_mutex_init(&p7, NULL) != 0){
        printf("\n mutex Fallo\n");
    }

    double num = 1;
    //INICIALIZACION DE LAS MATRICES
    for(f=0;f<N;f++){
     for(c=0;c<N;c++){
        ulAAtrans[f*N+c]=0;
        parcialC[f*N+c]=0;
        bL[f*N+c]=0;
        bD[f*N+c] = 0;
        BE[f*N+c] = 0;
        UF[f*N+c] = 0;
        A[f*N+c] = num;
	    A2[c*N+f] = A[f*N+c]; //Transpuesta poner para el tiempo
        B[f*N+c]=1; //ORDENXFILAS
        C[c*N+f]=1; //ORDENXCOLUMNAS
        D[f*N+c]=1; //ORDENXFILAS
        E[c*N+f]=1; //ORDENXCOLUMNAS
        F[c*N+f]=1; //ORDENXCOLUMNAS
        //Matrices triangulares, PENSAR EN ARMAR LA TRANSPUESTA
			  if (f > c){
				L[c+((f*(f+1))/2)] = 1; //Almaceno L por filas
				//U[i * N + j] = 0;
		  	}
			  else if (f < c){
				//L[i * N + j] = 0;
				U[f+((c*(c+1))/2)] = 1; //Almaceno U por columnas
			  }
			else{
				L[c+((f*(f+1))/2)] = 1;
				U[f+((c*(c+1))/2)] = 1;
			}
     }
    }
    /*
     imprimeMatriz(A,1);
     imprimeMatriz(A2,1);
     imprimeMatriz(B,1);
     imprimeMatriz(C,0);
     imprimeMatriz(D,1);
     imprimeMatriz(E,0);
     imprimeMatriz(F,0);
     */

    // SACO PROMEDIOS QUE NECESITO
    // PROMEDIO b

temp=0;
temp1=0;
temp2=0;
  for(int i=0;i<N;i++){
          for(int j=0;j<N;j++){
              temp+=B[i*N+j];
            }
    }

    for(int i=0;i<N;i++){
        for (int j=i;j<N;j++){
            temp1+=U[i+((j*(j+1))/2)];
        }
    }

	for(int i=0;i<N;i++){
		for(int j=0;j<(i+1);j++)
		{
			temp2+=L[j+((i*(i+1)/2))];
		}
	}

    b=(temp/(N*N));
    u=(temp1/(N*N));
    l=(temp2/(N*N));
    ul=u*l;
    //printf("Los valores son b = %f0.0 , u=%f0.0 , l = %f0.0 , ul=%f0.0",b,u,l,ul);

  pthread_t misHilos[T]; // areglo de hilos para ejecutar
  int ID[T];
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
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
//printf("\nResultado final");

//imprimeMatriz(M,1);

pthread_mutex_destroy(&p1);
pthread_mutex_destroy(&p2);
pthread_mutex_destroy(&p3);
pthread_mutex_destroy(&p4);
pthread_mutex_destroy(&p5);
pthread_mutex_destroy(&p6);
pthread_mutex_destroy(&p7);
free(A);
free(A2);
free(B);
free(C);
free(D);
free(E);
free(F);
free(U);
free(L);
free(M);
free(ulAAtrans);
free(parcialC);
free(bL);
free(BE);
free(bD);
free(UF);
return(0);
}
