#include<stdlib.h>
#include<stdio.h>
#include<math.h>


double c_norm(double x1,double x2,double x3)
{
	double r= sqrt((x1*x1+x2*x2+x3*x3));
	printf("%10.6f\n",r);
	return r;
}



int main(void)
{
	double a[3]={3.0,4.0,7};
	//double b=1;
	double b=c_norm(a[0],a[1],a[2]);
	printf("%10.6f\n",b);
	return 0;
}
