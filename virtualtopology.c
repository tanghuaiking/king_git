//本例演示MPI-Cart-Creat,MPI-Comm-dup,MPI-Topo-test,MPI-Cart-get,
//MPI-Graph-creat,MPI_Graphdims-get,MPI-Graph-get
#include"mpi.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define CART_DIM 2
#define GRAPH_NEIBS 2
int main(int argc, char *argv[])
{
    int errs=0,i,k,rank;
    int dims[CART_DIM],periods[CART_DIM],wsize;
    int outdims[CART_DIM],outperiods[CART_DIM],outcoords[CART_DIM];
    int topo_type;
    int*index,*edges,*outindex,*outedges;
    MPI_Comm comm1,comm2;
    char outstr[700];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&wsize);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    //创建笛卡尔拓扑，取得其属性，然后复制之，检查副本通信子具有相同的属性
    dims[0]=dims[1]=0;
    MPI_Dims_create(wsize,CART_DIM,dims);
    periods[0]=periods[1]=0;

    //创建CART_DIM=2的二维笛卡尔拓扑
    MPI_Cart_create(MPI_COMM_WORLD,CART_DIM,dims,periods,0,&comm1);
    //利用复制的通信子对象进行管理，可见拓扑信息保存在通信子对象的属性缓存中
    MPI_Comm_dup(comm1,&comm2);
    MPI_Topo_test(comm2,&topo_type);

    if(topo_type !=MPI_CART)
    {
        errs++;
        printf("Topo type of duped cart was not cart\n");
        fflush(stdout);
    }
    else
    {
        MPI_Cart_get(comm2 ,CART_DIM,outdims,outperiods,outcoords);
        sprintf(outstr,"proc:%d",rank);
        for(i=0;i<CART_DIM;i++)
        {
            if(outdims[i]!=dims[i])
            {
                errs++;
                printf("%d=outdims[%d]!=dims[%d]=%d\n",outdims[i],i,i,dims[i]);
                fflush(stdout);
            }
            if(i<CART_DIM-1)
            {
                sprintf(outstr,"%soutdims[%d]=%d,dims[%d]=%d;",outstr,i,outdims[i],i,dims[i]  );
            }
            else
            {
                sprintf(outstr,"%soutdims[%d]=%d,dims[%d]=%d\n",outstr,i,outdims[i],i,dims[i]  );
            }
            if(outperiods[i]!=periods[i] )
            {
                errs++;
                printf("%d=outperiods[%d]!=periods[%d]=%d\n",outperiods[i],i,i,periods[i]);
                fflush(stdout);
            }                
            if(i<CART_DIM-1)
            {
                sprintf(outstr,"%soutperiods[%d]=%d,periods[%d]=%d;",outstr,i,outperiods[i],i,periods[i]  );
            }
            else
            {
                sprintf(outstr,"%soutperiods[%d]=%d,periods[%d]=%d\n",outstr,i,outperiods[i],i,periods[i]  );
            }
            fprintf(stdout,"%s",outstr);
        }
    }
    MPI_Comm_free(&comm2);
    MPI_Comm_free(&comm1);

    //对图形拓扑进行类似测试
    if(wsize>=3)
    {
        index=(int*)malloc(wsize*sizeof(int));
        edges=(int*)malloc(wsize*GRAPH_NEIBS*sizeof(int));
        if(!index || !edges)
        {
            printf("Unable to allocate %d words for index or edges\n", 3*wsize);
            fflush(stdout);
            MPI_Abort(MPI_COMM_WORLD,1  );
        }
        index[0]=GRAPH_NEIBS;
        for (i=1;i<wsize;i++)
        {
            index[i]=GRAPH_NEIBS+index[i-1];
        }
        k=0;
        for(i=0;i<wsize;i++)
        {
            edges[k++]=(i-1+wsize)%wsize;
            edges[k++]=(i+1)%wsize;
        }
        //创建wsize个节点的图拓扑，其中每节点有GRAPH_NEIBS=2个邻居
        MPI_Graph_create(MPI_COMM_WORLD,wsize,index,edges,0,&comm1);
        MPI_Comm_dup(comm1,&comm2);
        MPI_Topo_test(comm2,&topo_type);
        if(topo_type!=MPI_GRAPH)
        {
            errs++;
            printf("Topo type of duped graph was not graph\n");
            fflush(stdout);
        }
        else
        {
            int nnodes,nedges;
            MPI_Graphdims_get(comm2,&nnodes,&nedges);
            if (nnodes!=wsize)
            {
                errs++;
                printf("Nnodes=%d,should be %d \n",nnodes,wsize);
                fflush(stdout);
            }
            if (nedges!=GRAPH_NEIBS*wsize)
            {
                errs++;
                printf("Nedges=%d,should be %d \n",nedges,GRAPH_NEIBS*wsize);
                fflush(stdout);
            }
            outindex=(int*)malloc(wsize*sizeof(int));
            outedges=(int*)malloc(wsize*GRAPH_NEIBS*sizeof(int));
            if( !outindex||!outedges)
            {
                printf("Unable to allocate %d words for outindex or outedges\n",3*wsize);
                fflush(stdout);
                MPI_Abort(MPI_COMM_WORLD,1);
            }
            MPI_Graph_get(comm2,wsize,GRAPH_NEIBS*wsize,outindex,outedges);
            sprintf(outstr,"proc:%d",rank);
            for(i=0;i<wsize;i++)
            {
                if(index[i]!=outindex[i])
                {
                    printf("%d=index[%d]!=outindex[%d]=%d\n",index[i],i,i,outindex[i] );
                    fflush(stdout);
                    errs++;
                }
                if(i<GRAPH_NEIBS-1)
                {
                    sprintf(outstr,"%soutindex[%d]=%d,index[%d]=%d;",outstr,i,outindex[i],i,index[i] );
                } 
                else
                {
                    sprintf(outstr,"%soutindex[%d]=%d,index[%d]=%d\n",outstr,i,outindex[i],i,index[i] );
                } 
            }
            fprintf(stdout,"%s",outstr);
            sprintf(outstr,"proc:%d",rank);
            for(i=0;i<GRAPH_NEIBS*wsize;i++)
            {
                if(edges[i]!=outedges[i])
                {
                    printf("%d=edges[%d]!=outedges[%d]=%d\n",edges[i],i,i,outedges[i] );
                    fflush(stdout);
                    errs++;
                }
                if(i<GRAPH_NEIBS-1)
                {
                    sprintf(outstr,"%soutedges[%d]=%d,edges[%d]=%d;",outstr,i,outedges[i],i,edges[i] );
                } 
                else
                {
                    sprintf(outstr,"%soutedges[%d]=%d,edges[%d]=%d\n",outstr,i,outedges[i],i,edges[i] );
                } 
            }   
            fprintf(stdout,"%s",outstr);
            free(outindex);
            free(outedges);
        }
        free(index);
        free(edges);
        MPI_Comm_free(&comm2);           
        MPI_Comm_free(&comm1);    
    }
    MPI_Finalize();
    return 0;
}


















               








































































    
