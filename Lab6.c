#include <stdio.h>
#include <math.h>
#include "mpi.h"
int main(int argc, char **argv){
int p; // p-numer procesu
float suma=0;//wynik koncowy
int i=1; 
int n;//liczba procesow
int tag=50;//tag i
int tag2= 60;//tag sumy
float wynik=0;//wynik poteg
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &p);
MPI_Comm_size(MPI_COMM_WORLD, &n);
MPI_Status status;
if(p==0){ // proces zerowy przekazuje do procesu pierwszego wartosci i,suma
wynik=powf(-1, i-1) / (2 * i - 1);
suma=suma+wynik*4;
printf("npr procesu: %d ,i:%i wynik: %f, suma %f\n", p,i,wynik, suma);  
i++;
MPI_Send(&i, 1, MPI_INT ,p+1 ,tag, MPI_COMM_WORLD);
MPI_Send(&suma, 1, MPI_FLOAT ,p+1 ,tag2, MPI_COMM_WORLD);
} 
if((p>0)&&(p<n-1)){ // procesu od 1 do ostatniego
/* odbiera dane od procesu o jeden mnniejszego */
MPI_Recv(&i, 1, MPI_INT, p-1, tag, MPI_COMM_WORLD, &status);
MPI_Recv(&suma, 1, MPI_FLOAT, p-1, tag2, MPI_COMM_WORLD, &status);

wynik=powf(-1, i-1) / (2 * i - 1);
suma=suma+wynik*4;
printf("npr procesu: %d ,i:%i wynik: %f, suma %f\n", p,i,wynik, suma);  
i++;
if(p!=n-1){ // proces ostatni nie bedzie wysylal dalej  
/* po wykonaniu obliczen wysyla wyniki do procesu o 1 wiekszego */
MPI_Send(&i, 1, MPI_INT,p+1,tag,MPI_COMM_WORLD);
MPI_Send(&suma, 1, MPI_FLOAT,p+1,tag2,MPI_COMM_WORLD);
}
}    
MPI_Finalize();
return 0;
}
