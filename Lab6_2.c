#include <stdio.h>
#include <math.h>
#include "mpi.h"
//funkcja do obliczenia metoda trapezow
float func(float x)
{
	return powf(x,2);
}
int main(int argc, char **argv){
int p; // p-numer procesu
float calka=0;//wynik czesciowy
int i=1; 
int n;//liczba procesow
int tag=50;//tag i
int tag2= 60;//tag calki
int tag3=70;//tag xp
int tag4=80;//tag xk
int tag5=90;//tag dx
int tag6=100;//tag sumy
float xp=1;//kraniec poczatkowy
float xk=7;//kraniec koncowy
float dx=0;//zmienna pomocnicza(pierwszy i ostatni element z ciagu)
float suma=0;//wynik koncowy
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &p);
MPI_Comm_size(MPI_COMM_WORLD, &n);
MPI_Status status;
if(p==n-1){ // proces ostatni przekazuje do procesu o jeden mniejszego wartosci wszystkich zmiennych
dx=(xk-xp)/(float)n;
calka+=func(xp+i*dx);
calka*=dx;
suma+=calka;
calka=0;  
i++;
MPI_Send(&i, 1, MPI_INT ,p-1 ,tag, MPI_COMM_WORLD);
MPI_Send(&calka, 1, MPI_FLOAT ,p-1 ,tag2, MPI_COMM_WORLD);
MPI_Send(&xp, 1, MPI_FLOAT ,p-1 ,tag3, MPI_COMM_WORLD);
MPI_Send(&xk, 1, MPI_FLOAT ,p-1 ,tag4, MPI_COMM_WORLD);
MPI_Send(&dx, 1, MPI_FLOAT ,p-1 ,tag5, MPI_COMM_WORLD);
MPI_Send(&suma, 1, MPI_FLOAT ,p-1 ,tag6, MPI_COMM_WORLD);
} 
if((p>0)&&(p<n-1)){ // procesu od 1 do ostatniego
/* odbiera dane od prosesu o jeden wiekszzego */
MPI_Recv(&i, 1, MPI_INT, p+1, tag, MPI_COMM_WORLD, &status);
MPI_Recv(&calka, 1, MPI_FLOAT, p+1, tag2, MPI_COMM_WORLD, &status);
MPI_Recv(&xp, 1, MPI_FLOAT, p+1, tag3, MPI_COMM_WORLD, &status);
MPI_Recv(&xk, 1, MPI_FLOAT, p+1, tag4, MPI_COMM_WORLD, &status);
MPI_Recv(&dx, 1, MPI_FLOAT, p+1, tag5, MPI_COMM_WORLD, &status);
MPI_Recv(&suma, 1, MPI_FLOAT, p+1, tag6, MPI_COMM_WORLD, &status);
calka+=func(xp+i*dx);
calka+=(func(xp)+func(xk))/2;
calka*=dx;
suma+=calka;
calka=0;  
i++;
//proces wysyla dane do procesu o jeden mniejszego
MPI_Send(&i, 1, MPI_INT,p-1,tag,MPI_COMM_WORLD);
MPI_Send(&calka, 1, MPI_FLOAT,p-1,tag2,MPI_COMM_WORLD);
MPI_Send(&xp, 1, MPI_FLOAT ,p-1 ,tag3, MPI_COMM_WORLD);
MPI_Send(&xk, 1, MPI_FLOAT ,p-1 ,tag4, MPI_COMM_WORLD);
MPI_Send(&dx, 1, MPI_FLOAT ,p-1 ,tag5, MPI_COMM_WORLD); 
MPI_Send(&suma, 1, MPI_FLOAT ,p-1 ,tag6, MPI_COMM_WORLD); 
} 
if(p==0){ // proces pierwszy odbiera wyniki od procesu o jeden wiekszego i wypisuje wynik
MPI_Recv(&i, 1, MPI_INT, p+1, tag, MPI_COMM_WORLD, &status);
MPI_Recv(&calka, 1, MPI_FLOAT, p+1, tag2, MPI_COMM_WORLD, &status);
MPI_Recv(&xp, 1, MPI_FLOAT, p+1, tag3, MPI_COMM_WORLD, &status);
MPI_Recv(&xk, 1, MPI_FLOAT, p+1, tag4, MPI_COMM_WORLD, &status);
MPI_Recv(&dx, 1, MPI_FLOAT, p+1, tag5, MPI_COMM_WORLD, &status);
MPI_Recv(&suma, 1, MPI_FLOAT, p+1, tag6, MPI_COMM_WORLD, &status);
printf("Wynik: %f",suma);
}    
MPI_Finalize();
return 0;
}
