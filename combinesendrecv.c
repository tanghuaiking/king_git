#include<stdio.h>
#include"mpi.h"
#define BUFSIZE 5

int main(int argc,char**argv)
{
    int rank,size;
    MPI_Status status;
    int left ,right, i ,tag;
    int sbuf[BUFSIZE],rbuf[BUFSIZE];

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    tag=123;

    left=rank-1;
    right=rank+1;
    if(left<0)left = size-1;
    if(right>=size)right=0;
    for(i=0;i<BUFSIZE;i++)
    {
        sbuf[i]=rank*size+i;
    }
    char pstr[BUFSIZE*(sizeof(int)+7)+100];
    MPI_Sendrecv(sbuf,BUFSIZE,MPI_INT,right,tag,rbuf,BUFSIZE,MPI_INT,left,tag,MPI_COMM_WORLD,&status);
    sprintf(pstr,"Process %d got message from %d\n",rank,status.MPI_SOURCE);
    for(i=0;i<BUFSIZE-1;i++)
    {
        sprintf(pstr,"%srbuf[%d]=%d",pstr,i,rbuf[i]);
    }
    sprintf(pstr,"%srbuf[%d]=%d\n",pstr,i,rbuf[i]);
    printf("%s",pstr);
    MPI_Finalize();
    return 0;
}
