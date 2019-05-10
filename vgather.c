//本例演示组内通信子上的可变长度收集操作
#include"mpi.h"
#include<stdio.h>

int main(int argc,char**argv)
{
    int rank,root;
    int gsize,ssize,rsize,i,disp;
    MPI_Comm comm=MPI_COMM_WORLD;
    root=0;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(comm,&rank );
    MPI_Comm_size(comm,&gsize);
    ssize=rank+2*(rank+1);//n send count
    rsize=(3*gsize*gsize+gsize)/2;//root receive count
    int buf[rsize];
    char outstr[rsize*10];
    int displs[gsize];
    int rcounts[gsize];
    int sb[ssize];
    for(i=0;i<ssize;i++)
    {
        sb[i]=i+ssize*rank;
    }
    disp=0;
    for(i=0;i<gsize;i++)
    {
        displs[i]=disp;
        rcounts[i]=3*i+2;
        disp=disp+rcounts[i];
    }
    MPI_Gatherv(sb,ssize,MPI_INT,buf,rcounts,displs,MPI_INT,root,MPI_COMM_WORLD);
    if(rank==root)
    {
        sprintf(outstr,"%d,receive",rank);
        for(i=0; i<rsize;i++)
        {
            sprintf(outstr,"%s,buf[%d]=%d",outstr,i,buf[i]);
        }
        fprintf(stdout,"%s\n",outstr);
    }
    MPI_Finalize();
    return 0;
}
         







    
