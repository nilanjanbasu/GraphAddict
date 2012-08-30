#include<stdio.h>
#include"structdef.h"
float avg_degree(pass *inst)		//finds the average node degree of a graph
{

	int i=1;			
	float sum=0.0;
	
	do
	{		
		sum=sum+find_probability(inst,i);	
		++i;
	}while(i<=inst->dmax);
	return sum;
}
float find_probability(pass *inst,int k)		//finds the probability that a randomly selected node has the given degree
{
	int count=0,n=0;
	float prob;
	vertex *temp=inst->head,*max;
	max=showmax(inst);
	inst->dmax=max->num;
	if(k<0||k>inst->dmax)
	prob=0.0;
	while(temp!=NULL)
	{
		if(temp->num==k)
		++count;
		++n;
		temp=temp->nextv;
	}
	prob=(float)count/(float)n;
	return prob;
}
