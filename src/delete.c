#include<stdio.h>
#include<stdlib.h>
#include"structdef.h"

int deleteNode(pass *inst,const char *name)
{
	vertex *temp=NULL;
	if(inst->head==NULL)
		return 2; //i.e. the graph does not exist
	temp=locatev(name,inst->head);
	
	if(temp==NULL)
		return 1; //i.e. The graph does not have any such node.
	deleteEdgesTo(temp,inst->head);
	deleteVertexWithEdges(temp,&inst->head);
	renumberNodes(inst->head);
	inst->v_count=inst->v_count-1;
	return 0;
}
void deleteEdgesTo(vertex *todelete,vertex *head)
{
	vertex *temp=head;
	edge *tempedge=NULL,*prevedge=NULL;
	
	while(temp)
	{
		if(temp==todelete){
			temp=temp->nextv;
			continue;
		}
		tempedge=temp->start;
		prevedge=NULL;
		while(tempedge)
		{
			if((tempedge->pvertex)==todelete)
			{
				deleteEdge(prevedge,&(temp->start));
				break;
			}
			prevedge=tempedge;
			tempedge=tempedge->nexte;
		}
		temp=temp->nextv;
	}
	return;
}
void deleteEdge(edge *prev,edge **startptr)
{
	edge *tedge=NULL;
	if(prev==NULL)
	{
		tedge=(*startptr);
		(*startptr)=tedge->nexte;
		free(tedge);
	}
	else
	{
		tedge=prev->nexte;
		prev->nexte=tedge->nexte;
		free(tedge);
	}
	return;
}
void deleteVertexWithEdges(vertex *todelete,vertex **headptr)
{
	vertex *tempvertex=NULL;
	edge *tedge=NULL,*del=NULL;
	
	tedge=todelete->start;
	
	while(tedge)
	{
		del=tedge;
		tedge=tedge->nexte;
		free(del);
	}
	if(todelete==(*headptr))
	{
		tempvertex=(*headptr);
		(*headptr)=tempvertex->nextv;
		free(tempvertex);		
	}
	else
	{
		tempvertex=locateprev(todelete,(*headptr));
		tempvertex->nextv=todelete->nextv;
		free(todelete);
	}
	return;
}
vertex *locateprev(vertex *target,vertex *head)
{
	vertex *prev=NULL;
	if(target==head)
		return NULL;
	prev=head;
	while(prev->nextv)
	{
		if((prev->nextv)==target)
			return prev;
		else
			prev=prev->nextv;
	}
	return NULL;
}			
int deleteEdgeDirect(const char *from, const char *to,pass *inst)
{
	
	vertex *s=NULL,*d=NULL;
	edge *te=NULL,*prev=NULL;
	s=locatev(from,inst->head);
	d=locatev(to,inst->head);
	

	te=s->start;
	if(te->pvertex==d)
	{	
		s->start=te->nexte;
		free(te);
		return 0;
	}
	else
	{
		prev=te;
		te=te->nexte;
	}
	while(te)
	{
		if((te->pvertex)==d)
		{
			prev->nexte=te->nexte;
			free(te);
			return 0;
		}
		else
		{
			prev=te;
			te=te->nexte;
		}
	}
}
void renumberNodes(vertex *head)
{
	int i=1;
	
	while(head)
	{
		head->vno=i++;
		head=head->nextv;
	}
}
