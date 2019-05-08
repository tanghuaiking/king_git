#include"mpi.h"
#include<stdio.h>
#define SIZE 4

int main(int argc,char**argv)
{
    int numtasks,rank,scnt,rcnt,src;
    float sb[SIZE][SIZE]={
        {1.0,2.0,3.0,4.0},
        {5.0,6.0,7.0,8.0},
        {9.0,10.0,11.0,12.0},
        {13.0,14.0,15.0,16.0}};
    float rb[SIZE];
    MPI_Comm comm;

    MPI_Init(&argc,&argv);
    comm=MPI_COMM_WORLD;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    if(numtasks==SIZE)
    {
        src=1;
        scnt=SIZE;
        rcnt=SIZE;
        MPI_Scatter(sb,scnt,MPI_FLOAT,rb,rcnt,MPI_FLOAT,src,comm);
        printf("rank=%d Results:%f %f %f %f\n",rank,rb[0],rb[1],rb[2],rb[3]);
    }
    else
    {
        printf("must specify %d processors,Terminating.\n",SIZE);

    }
    MPI_Finalize();
    return 0;
}









