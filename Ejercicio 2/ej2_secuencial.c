#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


//declaracion de variables

int N; // tam de la matriz
double *A,*A2,*B,*C,*D,*E,*F;
double *L,*U; // matriz triangular L superior U inferior
double *ulAAtrans,*parcialC,*bL,*BE,*bD,*UF; //matrices parciales
double *f1,*f2,*f3; // matrices resultado final
double *M; //resultado final
double ul,u,l,b; // promedios de las matrices U L y B
double timetick;
double sec;
struct timeval tv;
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


//MAIN
int main(int argc,char*argv[]){
  double ul;
  int i,j,k;
  if ((argc != 2) || ((N = atoi(argv[1])) <= 0) )
   {
     printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
     exit(1);
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

    //double num = 0;
    //INICIALIZACION DE LAS MATRICES
    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    for(i=0;i<N;i++){
     for(j=0;j<N;j++){
        ulAAtrans[i*N+j]=0;
        parcialC[i*N+j]=0;
        bL[i*N+j]=0;
        bD[i*N+j] = 0;
        BE[i*N+j] = 0;
        UF[i*N+j] = 0;
        //p1[i*N+j] = 0;
        //p2[i*N+j] = 0;
        A[i*N+j] = 1;
	    A2[j*N+i] = A[i*N+j]; //Transpuesta poner para el tiempo
        B[i*N+j]=1; //ORDENXFILAS
        C[j*N+i]=1; //ORDENXCOLUMNAS
        D[i*N+j]=1; //ORDENXFILAS
        E[j*N+i]=1; //ORDENXCOLUMNAS
        F[j*N+i]=1; //ORDENXCOLUMNAS
        //Matrices triangulares, PENSAR EN ARMAR LA TRANSPUESTA
			  if (i > j){
				L[j+((i*(i+1))/2)] = 1; //Almaceno L por filas
				//U[i * N + j] = 0;
		  	}
			  else if (i < j){
				//L[i * N + j] = 0;
				U[i+((j*(j+1))/2)] = 1; //Almaceno U por columnas
			  }
			else{
				L[j+((i*(i+1))/2)] = 1;
				U[i+((j*(j+1))/2)] = 1;
			}
     }
    }
    /*
     imprimeMatriz(A,1);
     imprimeMatriz(A2,0);
     imprimeMatriz(B,1);
     imprimeMatriz(C,0);
     imprimeMatriz(D,1);
     imprimeMatriz(E,0);
     imprimeMatriz(F,0);
        printf("\n");
     printf("Matriz U\n");
     //imprimeMatriz(U,0);//inferior
   for (i = 0; i < N; i++){
		for (j = 0; j < i+1; j++)
		{
			printf("U[%d] = %f\n", j+((i*(i+1))/2), U[j+((i*(i+1))/2)]);
		}
	}

       printf("\n");

   for (i = 0; i < N; i++){
		for (j = 0; j < i+1; j++)
		{
			printf("L[%d] = %f\n", j+((i*(i+1))/2), L[j+((i*(i+1))/2)]);
		}
	}

       printf("\n");
    */

    // SACO PROMEDIOS QUE NECESITO
    // PROMEDIO b
    double temp,temp1,temp2,u,l,b;
    temp=0;
    temp1=0;
    temp2=0;

    for(int i=0;i<N;i++){
          for(int j=0;j<N;j++){
              temp+=B[i*N+j];
            }
    }

    for(i=0;i<N;i++){
        for (j=i;j<N;j++){
            temp1+=U[i+((j*(j+1))/2)];
        }
    }

	for(i=0;i<N;i++){
		for(j=0;j<(i+1);j++)
		{
			temp2+=L[j+((i*(i+1)/2))];
		}
	}

    b=(temp/(N*N));
    u=(temp1/(N*N));
    l=(temp2/(N*N));
    ul=u*l;

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(k = 0;k<N;k++){
                ulAAtrans[i*N+j] +=ul*A[i*N+k]*A2[k*N+j];
            }
        }
    }

    //L es inferior, recorrido parcial
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(k = 0;k<i+1;k++){
                bL[i*N+j] +=b*L[k+((i*(i+1)/2))]*B[j*N+k];
            }

        }
    }

    //U es superior
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(k = 0;k<j+1;k++){
              bD[i*N+j] +=b*D[i*N+k]*U[k+((j*(j+1))/2)];

            }
        }
    }

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(k = 0;k<N;k++){
              parcialC[i*N+j] += b*ulAAtrans[i*N+k]*C[k*N+j];
            }
        }
    }

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(k = 0;k<N;k++){
              BE[i*N+j] += bL[i*N+k]*E[k*N+j];
            }
        }
    }

     for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(k = 0;k<N;k++){
              UF[i*N+j] += bD[i*N+k]*F[k*N+j];
            }
        }
    }

    //Resultado Final
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(k = 0;k<N;k++){
                M[i*N+j] = parcialC[i*N+j]+BE[i*N+j]+UF[i*N+j];
            }
        }
     }
     gettimeofday(&tv,NULL);
     timetick = tv.tv_sec + tv.tv_usec/1000000.0;
      printf("Tiempo en segundos %f\n", timetick - sec);
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            printf("%f  ",M[i*N+j]);
        }
    printf(" \n");
  }
free(A);
free(B);
free(C);
free(D);
free(E);
free(F);
free(U);
free(L);
free(M);
return(0);
}
