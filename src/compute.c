#include<stdio.h>
#include<string.h>
#include"structdef.h"


void update_num(pass *inst)			//updates the degree or indegree of each vertex node in its vertex field
{

    vertex *temp=inst->head;
    edge *edget;
	if(inst->head==NULL)
    {
        printf("No graph is loaded!");
        return;
    }

    while(temp!=NULL)//this is needed for more than 1-time calling of calculate funct.
    {                //then num is increased more than or equal to its double value for each call
        temp->num=0;
        temp=temp->nextv;
    }
    temp=inst->head;

    while(temp!=NULL)
    {
        edget=temp->start;
        while(edget!=NULL)
        {

            edget->pvertex->num=(edget->pvertex->num)+1;
            edget=edget->nexte;
        }
        temp=temp->nextv;
    }
}
vertex* showmax(pass *inst)	//returns pointer to the vertex node which has the highest degree(if undirected) or indegree(if directed)
{
	int max=0;			//variable which keeps track of maximum degree or indegree upto the current vertex node that is being visited while iterating in 						//the  while loop
	char number[50];
	vertex *temp,*maxi=NULL;
	update_num(inst);		
	temp=inst->head;
	while(temp!=NULL)
	{
	        if(max<(temp->num))
        {
        	max=temp->num;
		maxi=temp;		//holds the pointer to the vertex node which has the highest degree or indegree upto current iteration
        }
	if(!temp->nextv)
		return maxi;			//pointer to the vertex node with highest degree or indegree
        temp=temp->nextv;
    }
}	

int return_info(char *vname,pass *inst)
{
	vertex *temp;
	temp=locatev(vname,inst->head);
	return (temp->num);
}
