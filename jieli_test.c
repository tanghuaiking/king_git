#include <stdio.h>
#include <mpi.h>

int main( argc, argv )
int argc;
char **argv;
{
    int rank, value, size;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    /* 得到当前进程标识和总的进程个数*/
    do {
    /* 循环执行 直到输入的数据为负时才退出*/
      if (rank == 0) {
        fprintf(stderr, "\nPlease give new value=");
        /*进程0读入要传递的数据*/
        scanf( "%d", &value );
        fprintf(stderr,"%d read <-<- (%d)\n",rank,value);
        if (size>1) {
          MPI_Send( &value,1,MPI_INT, rank + 1, 0, MPI_COMM_WORLD );
          fprintf(stderr,"%d send (%d)->-> %d\n",rank,value,rank+1);
          /* 若不少于一个进程 则向下一个进程传递该数据*/
        }
      }
      else {
        MPI_Recv( &value,1,MPI_INT,rank - 1,0,MPI_COMM_WORLD,&status );
        /* 其它进程从前一个进程接收传递过来的数据*/
        fprintf(stderr,"%d receive (%d)<-<- %d\n",rank,value,rank-1);
        if (rank < size - 1) {
          MPI_Send( &value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD );
          fprintf(stderr,"%d send (%d)->-> %d\n",rank,value,rank+1);
          /*若当前进程不是最后一个进程 则将该数据继续向后传递*/
        }
      }
    MPI_Barrier(MPI_COMM_WORLD);
    /* 执行一下同步 加入它主要是为了将前后两次数据传递分开*/
    } while ( value>=0);
    MPI_Finalize( );
}

