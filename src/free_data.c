#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structdef.h"

vertex *free_data(vertex *head)
{
	vertex *temp=head,*t;
	edge *temp_edge=NULL,*te=NULL;
	
	if(head==NULL)
		return NULL;

	while(temp!=NULL)
	{
		temp_edge=temp->start;		
		while(temp_edge!=NULL)
		{
			te=temp_edge->nexte;
			free(temp_edge);
			temp_edge=te;
		}
		t=temp->nextv;
		free(temp);
		temp=t;
	}
	return NULL;
}

void unvisit_all(vertex *head)
{
	vertex *temp=head;
	edge *t=NULL;

	while(temp!=NULL)
	{
		t=temp->start;
		while(t!=NULL)
		{
			t->visited=0;
			t=t->nexte;
		}
		temp=temp->nextv;
	}
}
pass *createFlagPack(void)
{
	pass *temp;
	temp=(pass *)calloc(1,sizeof(pass));
	init_pass(temp);
	return temp;
}
void init_pass(pass *inst)
{
	inst->flag=NO_FILE_CRTD;
	inst->flag2='d';
	if(inst->head!=NULL)
		inst->head=free_data(inst->head);
	inst->v_count=0;
	inst->dmax=0;
	inst->status=0;
}
void visit(vertex *comp,vertex *t)  //visits the edge comp----t i.e. makes the flag 'visited'=1
{
    edge *temp=comp->start;
    if(temp==NULL)
        printf("error:1");/*debugging*/

    while(temp!=NULL)
    {
        if(temp->pvertex==t)
        {
            temp->visited=1;
            break;
        }
        else
            temp=temp->nexte;
    }

    return;
}
vertex *getHead(pass *inst)
{
	return (inst->head);
}

void setFlag2 (pass *inst,char *edge) //is this needed?
{
	int flag;
	flag=atoi(edge);
	if(flag==1)
		inst->flag2='u';
	else if(flag==0)
		inst->flag2='d';
	else 
		printf("Error in: free_data.c->setFlag2");
}
void changeName(const char *name,vertex *ptr)
{
	strcpy(ptr->vname,name);
	return;
}
/*const char *getWeightStatus(pass *inst)
{
	if(inst->status=='w')
		return ("Weighted");
	else if(inst->status=='u')
		return ("Unweighted");
	else
		return ("Undefined");
}
void setWeightStatus(pass *inst,const char *wtst)
{
	if(wtst[0]=='U')
		inst->status='u';
	else if(wtst[0]=='W')
		inst->status='w';
	return 0;
}*/

