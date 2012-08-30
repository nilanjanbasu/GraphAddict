/*Implementation of Dijkstra's Algorithm for non-negative edges*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structdef.h"
#include"mystack.h"               
int ShortestPath(const char *source,const char *dest,pass *inst)
{
	vertex *store_s=NULL,*store_d=NULL;
	int *found,*shortest,*rec,m=0;
	
	if(inst->flag==NO_FILE_CRTD)
	{	printf("hola1");
		return 1; //ERROR:: NO FILE WAS LOADED
	}
	else if(inst->head==NULL)
	{             printf("hola2");
		return 2; //ERROR :: GRAPH NOT PROPERLY LOADED
	}	
	
	store_s=locatev(source,inst->head);
	store_d=locatev(dest,inst->head);

	if(store_s==NULL){printf("hola3");
		return 3; //ERROR: NO SUCH SOURCE NODE
	}
	else if(store_d==NULL){printf("hola4");
		return 4; //ERROR: NO SUCH DEST NODE
	}
	else if(store_s->vno==store_d->vno)
	{
		return 5; //ERROR : SOURCE AND DEST ARE SAME
	}	


	found=(int *)calloc((inst->v_count+1),sizeof(int));
	shortest=(int *)calloc((inst->v_count+1),sizeof(int));
	rec=(int *)calloc((inst->v_count+1),sizeof(int));

	if(found==NULL||shortest==NULL||rec==NULL){
		return 6; //ERROR: MEMORY ALLOCATION FAILURE
	}
	
	ShortPathNonNeg(inst,store_s->vno,store_d->vno,found,shortest,rec);
printf("%s %s\n",source,dest);
	if(shortest[store_d->vno]==INFINITY)
		return 7; //ERROR: NO FEASIBLE PATH	
	else	
		m=quick_show(store_s->vno,store_d->vno,inst,rec);

	free(found);
	free(shortest);
	free(rec);
	
	return m;
}	

void ShortPathNonNeg(pass *inst,int source,int dest,int found[],int shortest[],int rec[])    
{
	int total_v=inst->v_count;   int i;
	int cur;
	vertex *temp_v=NULL;
	edge *temp_e=NULL;	
	printf("hola");
	init_found_and_shortest(found,shortest,rec,inst->head,source);
	found[source]=FOUND;
	shortest[source]=0;
	
	while(!found[dest])
	{
		cur=choose(shortest,found,total_v,inst);
		if(cur==-1) {return;}   //case when two nodes of different disjoint sub-graphs are chosen
		found[cur]=FOUND;
		temp_v=locatev_by_no(cur,inst->head);
		temp_e=temp_v->start;

		while(temp_e!=NULL)
		{
			if(shortest[cur]+temp_e->weight<=shortest[temp_e->pvertex->vno])
			{
				shortest[temp_e->pvertex->vno]=shortest[cur]+temp_e->weight;
				rec[temp_e->pvertex->vno]=cur;//records the last change made by a previous node
			}				      //will be needed for tracing the path
			temp_e=temp_e->nexte;
		}
	}	
}

void init_found_and_shortest(int found[],int shortest[],int rec[],vertex *head,int source)//initialize found[] and shortest[]
{
	vertex *temp_v=NULL;          //int i;
	edge *temp_e=NULL;
	int x=0;
	
	temp_v=head;
	while(temp_v!=NULL)
	{
		x=temp_v->vno;
		found[x]=NOT_FOUND;
		shortest[x]=INFINITY;
		temp_v=temp_v->nextv;
	}


	temp_v=locatev_by_no(source,head);  //NOTE:No possibility of getting NULL
	temp_e=temp_v->start;
	while(temp_e!=NULL)
	{
		x=temp_e->pvertex->vno;
		shortest[x]=temp_e->weight;
		rec[x]=source;
		temp_e=temp_e->nexte;
	}
}


vertex *locatev_by_no(int x,vertex *head)  //This returns a pointer to the vertex that has unique-number x
{
    while(head!=NULL)
    {
       if(head->vno==x)
          return (head);
       else
          head=head->nextv;
    }
    return NULL;
}

int choose(int shortest[],int found[],int total_v,pass *inst)
{
	int i,min,minpos;
	min=INFINITY;
	minpos=-1;
	vertex *v;
	for(v=inst->head;v;v=v->nextv)
	{
		if(shortest[v->vno]<min&&!found[v->vno])
		{
			min=shortest[v->vno];
			minpos=v->vno;
		}
	}
	return minpos;
}

int quick_show(int source,int dest,pass *inst,int rec[])
{
	FILE *fp;
	char com[150],op;
	printf("hola");
	fp=fopen("./tmp/temp_12345.dot","w+");
	if(fp==NULL)
	{
		return 8; //INTERNAL ERROR
	}

	PrepareRecforViewing(rec,source,dest,inst->v_count);

	if(inst->flag2=='u')
		q_show_undir(inst->head,rec,fp);
	else
		q_show_dir(inst->head,rec,fp);
	
	strcpy(com,"dot ./tmp/temp_12345.dot -Tgif >./tmp/temp_12345.gif");
	system(com);
	//strcpy(com,"display temp_12345.gif");
	system(com);	
	
		
	return 0;
}

void q_show_dir(vertex *head,int rec[],FILE *fp)
{
    edge *temp;
    vertex *temp_v=head;

    fprintf(fp,"digraph G {\n");  
    
    while(temp_v!=NULL)
    {
	    temp=temp_v->start;
	    while(temp!=NULL)
	    {
		fprintf(fp,"\"%s\" -> \"%s\"",temp_v->vname,temp->pvertex->vname);
		if(temp->weight!=1)
			fprintf(fp,"[label=%.2f]",temp->weight);
		if(rec[temp_v->vno]==temp->pvertex->vno){  //Colour for the edge in the shortest path
			fprintf(fp,"[color=blue]");	    		
		}
			
		fprintf(fp,";\n");
		temp=temp->nexte;
	    }
	    temp_v=temp_v->nextv;
    }
	    
    fprintf(fp,"}");
    fclose(fp);
    return;
}
void q_show_undir(vertex *head,int rec[],FILE *fp)
{
    edge *temp;
    vertex *temp_v=head;
    fprintf(fp,"graph G {\n");
    
    while(temp_v!=NULL)
    {
	    temp=temp_v->start;
	    while(temp!=NULL)
	    {
		if((temp->visited)!=1)
		{
		    fprintf(fp,"\"%s\" -- \"%s\"",temp_v->vname,temp->pvertex->vname);
		    if(temp->weight!=1)
			fprintf(fp,"[label=%.2f]",temp->weight);
		    if(rec[temp_v->vno]==temp->pvertex->vno||rec[temp->pvertex->vno]==temp_v->vno){
			fprintf(fp,"[color=blue]");
		    }//end if
		    fprintf(fp,";\n");
		    temp->visited=1;
		    visit(temp->pvertex,temp_v);
		}//end if		    
	        temp=temp->nexte;
	        
	    }//end inner while
	    temp_v=temp_v->nextv;
    }//end outer while

   
    unvisit_all(head);
    fprintf(fp,"}");
    fclose(fp);
    return;
    
}
void PrepareRecforViewing(int rec[],int s,int d,int maxv) //s stands for source, d for destination
{
	int i,j,cur;
	stack *st;

	push(-1,&st);
	push(d,&st);
	cur=d;
	while(cur!=s)   //Enter the contents of rec into a stack 
	{
		i=rec[cur];
		push(i,&st);
		cur=i;
	}
	for(j=0;j<maxv;++j)
		rec[j]=-1;
	cur=s;
	while(cur!=d)  
	{	/*Rearrange accordingly,so that rec[s] now contains the index no of immediate next node.We can thus
		trace the path as every element in rec[] corresponding to unique-(identification)number of nodes in 			the path contains immediate next node number in the shortest path. rec[d]=-1 where the path ends.*/
		rec[cur]=pop(&st);
		cur=rec[cur];
	}
	pop(&st);//to pop out the -1 to prevent orphan node
}

