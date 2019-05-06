#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

void Hello( void );
int main(int argc, char *argv[])
{
    int me, option, namelen, size;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&me);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    /*得到当前进程标识和总的进程数*/
    if (size < 2) {
      /*若总进程数小于2 则出错退出*/
      fprintf(stderr, "systest requires at least 2 processes" );
      MPI_Abort(MPI_COMM_WORLD,1);
    }
    MPI_Get_processor_name(processor_name,&namelen);
    /*得到当前机器名字*/
    fprintf(stderr,"Process %d is alive on %s\n", me, processor_name);
    MPI_Barrier(MPI_COMM_WORLD);
    /*同步*/
    Hello();/*调用问候过程*/
    MPI_Finalize();
}

void Hello( void )
/*任意两个进程间交换问候信息 问候信息由发送进程标识和接收进程标识组成*/
{
    int nproc, me;
    int type = 1;
    int buffer[2], node;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    /*得到当前进程标识和总的进程数*/
    if (me == 0) {
      /* 进程0负责打印提示信息*/
      printf("\nHello test from all to all\n");
      fflush(stdout);
    }
    for (node = 0; node<nproc; node++) {
    /*循环对每一个进程进行问候*/
      if (node != me) {
        /* 得到一个和自身不同的进程标识*/
        buffer[0] = me; /*将自身标识放入消息中*/
        buffer[1] = node; /*将被问候的进程标识也放入消息中*/
        MPI_Send(buffer, 2, MPI_INT, node, type, MPI_COMM_WORLD);
        /*首先将问候消息发出*/
        MPI_Recv(buffer, 2, MPI_INT, node, type, MPI_COMM_WORLD,&status);
        /*然后接收被问候进程对自己发送的问候消息*/
        if ( (buffer[0] != node) || (buffer[1] != me) ) {
          /*若接收到的消息的内容不是问候自己的或不是以被问候方的身份问候自
己 则出错*/
          (void) fprintf(stderr, "Hello: %d!=%d or %d!=%d\n",
buffer[0], node, buffer[1], me);
          printf("Mismatch on hello process ids; node = %d\n", node);
        }
        printf("Hello from %d to %d\n", me, node);
        /*打印出问候对方成功的信息*/
        fflush(stdout);
      }
    }
}
