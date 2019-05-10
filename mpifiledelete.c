#include"mpi.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
    MPI_Fint handleA,handleB;
    int rc;
    int errs=0;
    int rank;
    MPI_File cFile;
  
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    rc=MPI_File_open(MPI_COMM_WORLD,"temp",
        MPI_MODE_RDWR|MPI_MODE_CREATE,
        MPI_INFO_NULL,&cFile);
    if(rc)
    {
        printf("Unable to open file\"temp\"\n");
        fflush(stdout);
    }
    else
    {
        printf("proc: %dfile \"temp\" opened  ,triying close...\n" ,rank  );
        rc=MPI_File_close(&cFile);
        if(rank==0)
        {
            printf("proc: %dfile \"temp\" closed  ,triying delete...\n" ,rank  ); 
            rc=MPI_File_delete("temp", MPI_INFO_NULL);  
            if(rc)
            {
                printf("proc: %d  unable delete file\"temp\"\n" ,rank  );     
                fflush(stdout);
            }
        }
        
    }
    MPI_Finalize();
    return 0;
}
