#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    FILE *fp1;
    FILE *fp2; //申明2个文件指针，一个打开源文件，一个打开目标文件
    char buf[1024]; //申明一个大字符数组，用于保存文件每一个行的内容

    //只读方式打开源文件，这里得用两个反斜杠转义字符
    if( (fp1=fopen("test.txt","r"))==NULL )
    {
        printf("打开test.txt失败，可能文件还没有创建!");
        exit(0);
    }
    //创建目标文件，这里一般不会发生错误
    if( (fp2=fopen("dest.txt","w"))==NULL )
    {
        printf("创建dest.txt失败!");
        fclose(fp1);
        exit(0);
    }
    while(!feof(fp1)) //当文件指针fp1指向文件末尾时，feof返回0，否则返回1，该句作用是只要未到末尾则进入循环
    {
        int c1_B;
        fscanf(fp1,"%d",&c1_B);
        if(feof(fp1)) break;
        printf("%d\n",c1_B);
        memset(buf, 0, 1024); //buf字符串清0
        fgets(buf, 1024, fp1); //从fp1文件当前指针读取一行内容到buf
        //if(buf[0]!='#') //判断该行的第一个字符是否为'#'
        //{
            //fputs(buf, fp2); //是'#'，写内容到fp2文件
        //}
    }

fclose(fp1);
fclose(fp2); //循环结束，关闭fp1，fp2文件，程序结束
return 0;
} 
