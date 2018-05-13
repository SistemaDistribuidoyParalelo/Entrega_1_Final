#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#define cant_threads 2

//Para calcular tiempo
double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	sec = tv.tv_sec + tv.tv_usec / 1000000.0;
	return sec;
}

int main(int argc, char *argv[]){
	double *A, *AT, *B, *C, *D, *E, *F, *U, *L, *M, *AA, *AAC, *LB, *LBE, *DU, *DUF;
	int N, i, j, k;
	double timetick, promedioB = 0.0, promedioU, promedioL, prodProm, acumulador;
	//Controla los argumentos al programa
	if (argc != 2 || ((N = atoi(argv[1])) <= 0))
	{
		printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
		exit(1);
	}

	N = atoi(argv[1]);

	A = (double *)malloc(sizeof(double) * N * N);
	AT = (double *)malloc(sizeof(double) * N * N);
	B = (double *)malloc(sizeof(double) * N * N);
	C = (double *)malloc(sizeof(double) * N * N);
	D = (double *)malloc(sizeof(double) * N * N);
	E = (double *)malloc(sizeof(double) * N * N);
	F = (double *)malloc(sizeof(double) * N * N);
	U = (double *)malloc(sizeof(double) * ((N*(N+1))/2));
	L = (double *)malloc(sizeof(double) * ((N*(N+1))/2));
	M = (double *)malloc(sizeof(double) * N * N);
	AA = (double *)malloc(sizeof(double) * N * N);
	AAC = (double *)malloc(sizeof(double) * N * N);
	LB = (double *)malloc(sizeof(double) * N * N);
	LBE = (double *)malloc(sizeof(double) * N * N);
	DU = (double *)malloc(sizeof(double) * N * N);
	DUF = (double *)malloc(sizeof(double) * N * N);

	for (i = 0; i < N; i++)	{
		for (j = 0; j < N; j++)		{
			A[i * N + j] = 1;
			B[i * N + j] = j;
			C[j * N + i] = 1; //C, E y F se almacenan por columnas para facilitar cuentas despues
			D[i * N + j] = 1;
			E[j * N + i] = 1;
			F[j * N + i] = 1;
			//Matrices triangulares
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

/*	for (i = 0; i < N; i++)
	{
		for (j = 0; j < i+1; j++)
		{
			printf("U[%d] = %f\n", j+((i*(i+1))/2), U[j+((i*(i+1))/2)]);
		}
	}
*/
timetick = dwalltime();
//Calculo la transpuesta de A
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			AT[j * N + i] = A[i * N + j];
		}
	}

//Calculo el promedio de B
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			promedioB += B[i * N + j];
		}
	}
	promedioB = promedioB / (N * N);

//Calculo el promedio de U
	for (i = 0; i < N; i++)
	{
		for (j = i; j < N; j++)
		{
			promedioU += U[i+((j*(j+1))/2)];
		}
	}

	promedioU = promedioU / (N * N);

//Calculo el promedio de L
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < (i + 1); j++)
		{
			promedioL += L[j+((i*(i+1)/2))];
		}
	}
	
	promedioL = promedioL / (N * N);

	//printf("El promedio de B es %f\n", promedioB);
	printf("El promedio de U es %f\n", promedioU);
	printf("El promedio de L es %f\n", promedioL);

	prodProm = promedioU * promedioL;

	printf("La multiplicacion de los promedios de U y L es %f\n",prodProm);

//Calculo A x el promedio de U x el promedio de L
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			A[i*N+j] = (A[i*N+j] * prodProm);
		}	
	}

//Calculo A x la traspuesta de A
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			acumulador = 0.0;
			for(k=0;k<N; k++){
				acumulador += A[i*N+k] * AT[k*N+j];
			}
			AA[i*N+j] = acumulador;
		}	
	}

//Calculo AA x C
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			acumulador = 0.0;
			for(k=0;k<N; k++){
				acumulador += AA[i*N+k] * C[k*N+j];
			}
			AAC[i*N+j] = acumulador;
		}	
	}

//Calculo L x el promedio de B
	for(i=0;i<N;i++){
		for(j=0;j<(i+1);j++){
			L[j+((i*(i+1)/2))] = (L[j+((i*(i+1)/2))] * promedioB);
		}	
	}

//Calculo L x B

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			acumulador = 0.0;
			for(k=0;k<i+1; k++){
				acumulador += L[k+((i*(i+1)/2))] * B[j*N+k];
			}
			LB[i*N+j] = acumulador;
		}	
	}

//Calculo LB x E

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			acumulador = 0.0;
			for(k=0;k<N; k++){
				acumulador += LB[i*N+k] * E[k*N+j];
			}
			LBE[i*N+j] = acumulador;
		}	
	}

//Calculo D x el promedio de B

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			D[i*N+j] = (D[i*N+j] * promedioB);
		}	
	}	
		
//Calculo D x U

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			acumulador = 0.0;
			for(k=0;k<j+1; k++){
				//printf("D[%d]* U[%d]\n", i*N+k, j+((k*(k+1))/2));
				acumulador += D[i*N+k] * U[k+((j*(j+1))/2)];
			}
			//printf("-----------------------\n");
			DU[i*N+j] = acumulador;
		}	
	}

//Calculo DU x F

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			acumulador = 0.0;
			for(k=0;k<N; k++){
				acumulador += DU[i*N+k] * F[k*N+j];
			}
			DUF[i*N+j] = acumulador;
		}	
	}

//Suma total

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			M[i*N+j] = AAC[i*N+j] + LBE[i*N+j] + DUF[i*N+j];
		}	
	}

	/*for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			//printf("DUF[%d] = %f\n", (i*N+j), DUF[i*N+j]);
			//printf("AT[%d] = %f\n", (i*N+j), AT[i*N+j]);
			//printf("LB[%d] = %f\n", (i*N+j), LB[i*N+j]);
			printf("M[%d] = %f\n", (i*N+j), M[i*N+j]);
		}	
	}*/

	printf("Tiempo total: %f\n", dwalltime()-timetick);
	free(A);
	free(B);
	free(C);
	free(D);
	free(E);
	free(F);
	free(U);
	free(L);
	free(M);
	free(AA);
	free(AAC);
	free(LB);
	free(LBE);
	free(DU);
	free(DUF);    

}