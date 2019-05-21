//该例用于演示自定义数据类型MPI_Type_contiguous
#include"mpi.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define SIZE 4

int main (int argc, char **argv)
{
    int numtasks,rank,source=0,dest,tag=1,i,j;
    float a[SIZE][SIZE]=
    {
        {1.0,1.1,1.2,1.3},
        {2.0,2.1,2.2,2.3},
        {3.0,3.1,3.2,3.3},
        {4.0,4.1,4.2,4.3},
    };
//利用一个float类型的行向量作为新数据类型的接受缓冲区
    float b[SIZE];
    char outstr[600];
    MPI_Status stat;
    MPI_Datatype rowtype;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

    //利用SIZE次重复基本类型MPI_FLOAT,生成一个新的数据类型 rowtype
    //该类型实际为二维数组a的一行
    MPI_Type_contiguous(SIZE,MPI_FLOAT,&rowtype);
    //注册新的数据类型rowtype
    MPI_Type_commit(&rowtype);
    if(numtasks==SIZE)
    {
        if(rank==0)
        {
            sprintf(outstr,"rank:%d has \n{\n",rank);
            for(i=0;i<SIZE;i++)
            {
                sprintf(outstr,"%s\t{",outstr);
                for(j=0;j<SIZE;j++)
                {
                    if(j<SIZE-1)
                    {    sprintf(outstr,"%s%f,",outstr,a[i][j] );}
                    else
                    {    sprintf(outstr,"%s%f},\n",outstr,a[i][j]) ; }
                }
            }
            fprintf(stdout,"%s}\n",outstr);
            for(i=1;i<numtasks;i++)
            {
                sprintf(outstr,"rank:%d send to :%d{",rank,i);
                for(j=0;j<SIZE;j++)
                {
                    if(j<SIZE-1)
                    {    sprintf(outstr,"%s%f,",outstr,a[i][j] );}
                    else
                    {    sprintf(outstr,"%s%f},\n",outstr,a[i][j]) ; }
                }
                fprintf(stdout,"%s\n",outstr);
                //因为是rowtype类型的数据，因此发送函数的count 为1
                MPI_Send(&a[i][0],1,rowtype,i,tag,MPI_COMM_WORLD);
            }
        }
        else
        {
            //接收端使用MPI_FLOAT类型接受SIZE个float类型数据仍能成功
            MPI_Recv(b,SIZE,MPI_FLOAT,source,tag,MPI_COMM_WORLD,&stat);
            //MPI_Recv(b,1,rowtype,souce,tag,MPI_COMM_WORLD,&stat)
            sprintf(outstr,"rank:%d received :{",rank);
            for(j=0;j<SIZE;j++)
            {
                if(j<SIZE-1)
                {    sprintf(outstr,"%s%f,",outstr,b[j] );}
                else
                {    sprintf(outstr,"%s%f},\n",outstr,b[j]) ; }
            }
            fprintf(stdout,"%s\n",outstr);
        }
    }
    else
    {
        printf("Must specify %d processors. Terminating.\n ",SIZE);
        MPI_Type_free(&rowtype);
        MPI_Finalize();
        return 0;
    }
    MPI_Type_free(&rowtype);
    MPI_Finalize();
    return 0;
}







        




















