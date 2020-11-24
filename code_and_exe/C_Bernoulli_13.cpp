#include <math.h>
#include <stdio.h>
#include <stdlib.h> 
#include <malloc.h>
#include <time.h> 
#include <io.h> 
// sorting data
int divide_Conquer_X_step(double *doubledata, int datasize)
{
  int i,count1,count2;
  double MAX,MIN,compare_point;
  double *tepdata1,*tepdata2;
  
  if (datasize<=1) return(0); 
  MAX=doubledata[0];
  MIN=doubledata[0];
  
  for(i=1;i<datasize;++i) 
  {
  	if (doubledata[i]>MAX) MAX=doubledata[i]; 
	if (doubledata[i]<MIN) MIN=doubledata[i]; 
  }
  compare_point=(MIN+MAX)/(double)2;
  tepdata1=(double *)malloc(datasize*sizeof(double));
  tepdata2=(double *)malloc(datasize*sizeof(double));
  count1=0;  count2=0;    
  for(i=0;i<datasize;++i) 
  {
  	if (doubledata[i]<compare_point) 
  	{ 
  	  tepdata1[count1]=doubledata[i];
  	  ++count1;
	  }
	else  
	{ 
  	  tepdata2[count2]=doubledata[i];
  	  ++count2;
	  }
  }
  if (count1>=1) divide_Conquer_X_step(tepdata1,count1);
  if (count2>=1) divide_Conquer_X_step(tepdata2,count2);
 
  for(i=0;i<count1;++i)
    doubledata[i]=tepdata1[i];
  for(i=0;i<count2;++i)
    doubledata[i+count1]=tepdata2[i];
  free(tepdata1);  
  free(tepdata2);   
}
void frequecy_table(int n,double *getvalue1)
{
   int i,ni,classno1,count;
   double len1,tmin1,tmax1,width1,*pdfvalue1,*upper1,*lower1,cuumlative_pr=0,class_midpoint;
   FILE *file;
   
    tmin1=getvalue1[0]; tmax1=getvalue1[0];
    for(ni=1;ni<n;++ni)
     {  if (getvalue1[ni]<tmin1) tmin1=getvalue1[ni];
        if (getvalue1[ni]>tmax1) tmax1=getvalue1[ni];
                 }
     len1=tmax1-tmin1;
     classno1=200;
     width1=len1/(double)classno1;
     pdfvalue1=(double *)malloc(classno1*sizeof(double));
     upper1=(double *)malloc(classno1*sizeof(double));
     lower1=(double *)malloc(classno1*sizeof(double));
     for(ni=0;ni<classno1;++ni)   pdfvalue1[ni]=0;   
     upper1[0]=tmin1+width1;
     for(ni=1;ni<classno1;++ni)
     {  lower1[ni]=upper1[ni-1]; upper1[ni]=lower1[ni]+width1; }
     upper1[classno1-1]=tmax1+1;
     lower1[0]=tmin1-1;
     for(ni=0;ni<n;++ni)
     { 
        count=(long)((getvalue1[ni]-tmin1)/width1);
        if (count>=0 && count<classno1) pdfvalue1[count]+=1; 
		 }
    for(ni=0;ni<classno1;++ni)  pdfvalue1[ni]/=(double)(n); 
    lower1[0]=tmin1;upper1[classno1-1]=tmax1;
    file=fopen("C:\\C_Bernoulli\\tep\\frequency_table.txt","w"); 
    cuumlative_pr=0;
    for(ni=0;ni<classno1;++ni)
    {
     class_midpoint=(lower1[ni]+upper1[ni])/2;
     cuumlative_pr=cuumlative_pr+pdfvalue1[ni];
     fprintf(file,"%20.10f          %20.10f       %20.10f         %20.10f\n",class_midpoint,pdfvalue1[ni]/width1,class_midpoint,cuumlative_pr); 
     }
   fclose(file);
   free(pdfvalue1); free(upper1); free(lower1);
}
// the random number 
double simulated_uniform(double alpha,double beta)
{
   double tep1,tep=0,dividewangx;
 
   dividewangx=(double)32767*(double)32767*(double)32767;
   again:
   tep1=((double)rand()*(double)32767*(double)32767+(double)rand()*(double)32767+(double)rand())/dividewangx;
   if (tep1<0 || tep1>1) goto again;
   tep=alpha+tep1*(beta-alpha);   
   return(tep);
}
// The comparison of two strings and the index upper limit is len
int strcompare_spc(char *s,char *ss,int len)
{
  int i=0;
  for(;s[i]!=0 && ss[i]!=0 && i<len;++i)
   if (s[i]!=ss[i]) break;
  return(s[i]-ss[i]);
// return 0 when two strings are eqully
}
// Continuous Bernoulli distribution simulator
double simulated_continue_bernoulli(double lamda)
{
   double tep=0,tep1;
   char ss[1024];
   int kk=1;
    
    // checking the lamda is 0.5 or not 
    if (lamda>=0.499 && lamda<=0.501)
    {
    sprintf(ss,"%12.10f",lamda);
    kk=strcompare_spc(ss,(char *)"0.5000000000",12);
    	}
    // kk is 0 when lamda=0.5, X~Uniform(0,1) if lamda=0.5
    if (kk!=0) 
    {
     // the inverse distribution function is the simulated data
         tep1=simulated_uniform(0,1); 
         tep1=(2*lamda-1)*tep1-lamda+1;
         tep1=tep1/(1-lamda);
         tep=log(tep1)/log(lamda/(1-lamda));
		}
	else
    tep=simulated_uniform(0,1);  
   return(tep);
}
// computing the mean,variance,skewed coefficient and kurtosis coefficient
void moment_X_order(int n,double *datastore,int index)
{
	int i;
	double EX,VarX,skew,kurtosis;
	double sumX,sumX2,sumX3,sumX4,sigmaX,tep;
	
	sumX=0; sumX2=0; 
    for(i=0;i<n;++i)
       	sumX=sumX+datastore[i];
	sumX=sumX/(double)n;
	for(i=0;i<n;++i)
      	sumX2=sumX2+(datastore[i]-sumX)*(datastore[i]-sumX);
	sumX2=sumX2/(double)n;
	if (sumX2>0) 
	{
		sigmaX=exp(0.5*log(sumX2)); 
		sumX3=0;
        sumX4=0;
		for(i=0;i<n;++i)
		{
		  tep=(datastore[i]-sumX)/sigmaX;
		  sumX3=sumX3+tep*tep*tep;
		 sumX4=sumX4+tep*tep*tep*tep;
			}
		sumX3=sumX3/(double)n;
		sumX4=sumX4/(double)n;
		EX=sumX;VarX=sumX2;skew=sumX3;kurtosis=sumX4;
		printf(" E(X(%d))=%20.10f,\n Var(X(%d))=%20.10f,\n",index,EX,index,VarX);
        printf(" skewed coefficient(X(%d))=%20.10f,\n kurtosis coefficient(X(%d))=%20.10f\n",index,skew,index,kurtosis);
      }
      else
       printf(" E(X(%d))=%20.10f\n",index,EX);
}
int main()
{  
    int i,j,n=100000000,samplesize,index;
    double *datastore,lamda,tepdata,*sampledata;
    char *ss;
    
    ss=(char *)malloc(1024*sizeof(char));
    // input the parameter lamda
    loop1:
    printf(" The Continuous Bernoulli, lamda=");
	scanf("%s",ss);
	lamda=atof(ss);
	if (lamda<=0 || lamda>=1 )
	{ 
	  printf(" The Continuous Bernoulli(lamda=%f), it is error, 0<lamda<1,\n please input again.\n",lamda);
	  goto loop1;
	  }  
	loop2:
    printf(" The sample size=");
	scanf("%s",ss);
	samplesize=atoi(ss);
	if (samplesize<=0 )
	{ 
	  printf(" The sample size, it is error, sample size must greater than 0,\n please input again.\n",samplesize);
	  goto loop2;
	  }  
	loop3:
    printf(" The order index of order statistic(1~%d)=",samplesize);
	scanf("%s",ss);
	index=atof(ss);
	if (index<=0 || index>=samplesize )
	{ 
	  printf(" The index number=%d, it is error, range is [1,%d],\n please input again.\n",index,samplesize);
	  goto loop1;
	  }  
    datastore=(double *)malloc(n*sizeof(double));
    printf(" please wait a moment to collect the order statistic X(%d) data,....\n",index);
    // getting the simulated data and the amount=100,000,000, 
	// the moment of this database is closed to the distribution
	sampledata=(double *)malloc(samplesize*sizeof(double));
	printf(" finished %10.8f%%\n",(double)1/(double)n*100);
	for(i=0;i<n;++i)
	{
		if ((i+1)%10000000==0) printf(" finished %10.8f%%\n",(double)(i+1)/(double)n*100);
		tepdata=0;
		for(j=0;j<samplesize;++j)
		  sampledata[j]=simulated_continue_bernoulli(lamda); // simulated data summation
		divide_Conquer_X_step(sampledata,samplesize);
		datastore[i]=sampledata[index-1]; // the order statistic 
	}
	printf(" X1,X2,...,X%d iid~Continuous Bernoulli(lamda=%f),\n",samplesize,lamda);
	printf(" X(%d) is the %d order of the order statistic, sample size=%d,\n",index,index,samplesize);
	moment_X_order(n,datastore,index);
	printf(" the X bar frequency table is c:\\C_Bernoulli\\tep\\frequency_table.txt\n");
	i=chdir("c:\\C_Bernoulli"); // check this subdirectory is existed
    if (i!=0) mkdir("c:\\C_Bernoulli"); // make this subdirectory
    i=chdir("c:\\C_Bernoulli\\tep"); // check this subdirectory is existed
    if (i!=0) mkdir("c:\\C_Bernoulli\\tep"); // make this subdirectory
	frequecy_table(n,datastore);
	free(ss); free(datastore); free(sampledata);
	system("pause");
	return 0;
}
      
      
