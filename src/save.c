#include<stdlib.h>
#include"structdef.h"
#define NIL 1 /*old file format*/
#define SRJ 2 /*new file format*/

int save_interface(char *filename,pass *inst)              //returns a number.
{
	FILE *fp;
	int i,wmode;
	
	for(i=0;filename[i]!='.';++i);
	if(filename[i+1]=='n'&&filename[i+2]=='i'&&filename[i+3]=='l')
		wmode=NIL;
	else if(filename[i+1]=='s'&&filename[i+2]=='r'&&filename[i+3]=='j')
		wmode=SRJ;
	else
		return 1; /*error: file format is not supported*/
		
	fp=fopen(filename,"w+");	
	fprintf(fp,"%d %c\n",inst->v_count,inst->flag2);
	if(wmode==SRJ)
	{
		mapNameToNumbers(fp,inst->head);
		if(inst->flag2=='d')
			directed_SRJ(fp,inst->head,inst->status);
		else if (inst->flag2=='u'){
			undirected_SRJ(fp,inst->head,inst->status);
			unvisit_all(inst->head);
		}
		else 
			printf("Flag2 is not set properly");
	}
	else
	{	
		if(inst->flag2=='d')
			directed_NIL(fp,inst->head);
		else if(inst->flag2=='u'){
			undirected_NIL(fp,inst->head);
			unvisit_all(inst->head);
			printUnvisitedVertex(fp,inst->head);
		}
		
	}
	
        fclose(fp);
        inst->flag=ONCE_WRITTEN;
        return 0;
}
void directed_NIL(FILE *fp,vertex *head)
{
    edge *temp=head->start;
    while(temp!=NULL)
    {
	//printf("%s %s",head->vname,temp->pvertex->vname);
        fprintf(fp,"\"%s\" \"%s\" ",head->vname,temp->pvertex->vname);
	fprintf(fp," [ %.1f ]\n",temp->weight);
	temp->pvertex->visited=1;
        temp=temp->nexte;
    }
    if(head->nextv!=NULL)
        directed_NIL(fp,head->nextv);  //recursive call
    else
        return;
}

void undirected_NIL(FILE *fp,vertex *head)
{
    edge *temp;
    void visit(vertex *,vertex *);

    temp=head->start;
    while(temp!=NULL)
    {/*as in undirected graphs two-way connections exist,we introduce 'visited' flag. Whenever we visit an edge we \
    "visit" the edge starting from the current destination node to source node as well. And mark the 'visited' flags=1,
    whenever we examine edges we check whether it is visited or not thus rulling out the possibility of writing the same
    edge in the file once again*/
        if((temp->visited)!=1)
        {
            fprintf(fp,"\"%s\" \"%s\" ",head->vname,temp->pvertex->vname);
	    fprintf(fp,"[ %.1f ]\n",temp->weight);
            temp->visited=1;
            visit(temp->pvertex,head);
        }
        temp->pvertex->visited=1;
        temp=temp->nexte;
    }
    if(head->nextv!=NULL)
        undirected_NIL(fp,head->nextv);
    else
        return;
}
int count(vertex *head)  //returns the count of vertices
{
    int i;
    if(head==NULL)
        return 0;
    else{
        i=1+count(head->nextv);
        return i;
    }
}
void mapNameToNumbers(FILE *fp,vertex *head)
{
	fprintf(fp,"{\n");
	while(head!=NULL)
	{
		fprintf(fp,"%d -> \"%s\"\n",head->vno,head->vname);
		head=head->nextv;
	}
	fprintf(fp,"}\n");
}
void directed_SRJ(FILE *fp,vertex *head,char status)
{
	vertex *lochead=head;
	edge *temp=head->start;
	while(lochead)
	{
		temp=lochead->start;
		while(temp)
		{
			fprintf(fp,"%d %d",lochead->vno,temp->pvertex->vno);
			//temp->pvertex->visited=1;
			if(status=='w')
				fprintf(fp," [ %.2f ]",temp->weight);
			fputc('\n',fp);
			temp=temp->nexte;
		}
		lochead=lochead->nextv;
	}
	return;
}
void undirected_SRJ(FILE *fp,vertex *head,char status)
{
	vertex *lochead=head;
	edge *temp=head->start;
	
	while(lochead)
	{
		temp=lochead->start;
		while(temp)
		{
			if(!(temp->visited))
			{
				fprintf(fp,"%d %d",lochead->vno,temp->pvertex->vno);
				temp->pvertex->visited=1;
				temp->visited=1;
            			visit(temp->pvertex,lochead);
				if(status=='w')
					fprintf(fp," [ %.2f ]",temp->weight);
				fputc('\n',fp);
			}
			//temp->pvertex->visited=1;
			temp=temp->nexte;
		}
		lochead=lochead->nextv;
	}
	return;	
}
void printUnvisitedVertex(FILE *fp,vertex *head)
{
	vertex *tmp=head;
	while(head)
	{
		if(head->visited!=1)
			fprintf(fp,"\"%s\"\n",head->vname);
		
		head=head->nextv;
	}
	resetNodeVisitField(tmp);
	return;
}
void resetNodeVisitField(vertex *head)
{
	while(head)
	{
		head->visited=0;
		head=head->nextv;
	}
}
