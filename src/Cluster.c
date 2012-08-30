#include<stdio.h>
#include"structdef.h"

float cluster_interface(pass *pinst,char *vname)
{
	char tname[200],op;
	vertex *temp=NULL;
	float coeff=0;
	unvisit_all(pinst->head);
	/*printf("\nEnter the name of the node to find the clustering coefficient of: ");
	scanf("%[^\n]",tname);	*/
	
	temp=locatev(vname,pinst->head);
	if(!temp)
	{
		printf("\nNo match! Try again?(y/n)(press 'n' to go back to main menu): ");
		op=getchar();
		getchar();
		if(op=='y'||op=='Y')
			cluster_interface(pinst,vname);
		return;
	}
	else{
	
		if(pinst->flag2=='u')
			coeff=clusterUndir(temp);
		else
			coeff=clusterDir(temp,pinst);
	}
	/*if(coeff>=0)
		printf("\nThe required clustering coefficient is=%f",coeff);
	else{
		printf("\nThe clustering coefficient is not defined because the specified node has D(v)=");
		coeff==-1 ? printf("0") : printf("1");
		putchar('\n');
	}*/
	return coeff;
}

float clusterUndir(vertex *object)
{
	edge *e=object->start,*temp;
	int count=0,total=0;
	float f=0;
	temp=object->start;
	if(!e)
		return -1;
	else if(e->nexte==NULL)
		return -2;

	while(e!=NULL)
	{
		count=count+completeTrianglesUndir(e,e->nexte); //description of completeTriangles() is below in definition
		total++;
		e=e->nexte;
	}
	
	printf("%d\n%d\n",total,count);
	f=(float)2*count/(total*(total-1));
	return f;
}

float clusterDir(vertex *object,pass *inst)
{
	edge *e=object->start;
	int count=0,total=0,stat[100],i;
	float f=0;
	make_stat(inst->head,stat,object);
	unvisit_all(inst->head);
	total=findEligibleNodes(object,inst); 
	//printf("total %d\n",total);	
	/*in a directed graph,the neighbour-nodes are those in which,two directed edges exists between object-node and a node in the edge-list of object node. findEligibleNodes identifies those nodes and returns the count */
	if(total==0)
		return -1;
	else if(total==-1)
		return -2;
	
	/*printf("***********************");
	for(i=0;i<inst->v_count;++i)
	{
		printf("%d",stat[i]);
	}
	printf("***********************");*/
	while(e)
	{
		
			count=count+completeTrianglesDir(e,e->nexte,inst,object,stat);  //count of edges between neighbour-nodes
			e=e->nexte;
	}
	//printf("%d\n",count);
	printf("%d\n%d\n",total,count);
	f=(float)count/(total*(total-1));
	//("%f",f);
	return f;
}
int findEligibleNodes(vertex *node,pass *inst)
{
	edge *cur=node->start,*temp=NULL;
	int total=0;
	/*if(!cur)
		return 0;
	else if(!(cur->nexte))
		return -1;*/
	while(cur)
	{
		cur=cur->nexte;
		++total;
	}
	total+=find_tail(node,inst);
	if(total==1)
		return -1;
	return total;
	
	/*while(cur)
	{
		temp=cur->pvertex->start;
		while(temp)
		{
			if(temp->pvertex==node)
			{
				cur->visited=1;  //we will only examine those nodes which has 'visited' flag=1 
				total++;
				break;
			}
			temp=temp->nexte;
		}
		cur=cur->nexte;
	}
	return total;*/
}
int completeTrianglesUndir(edge *search_in,edge *others)
{/*searches if the node pointed by search_in has an edge that points to a vertex which is also pointed to by an edge 
in the edge-list 'others'*/
	edge *cur,*s_cur=search_in->pvertex->start;
	int count=0;
	if(others==NULL)
		return 0;
	while(s_cur)
	{
		cur=others;
		while(cur)
		{
			if(cur!=search_in)    //do not search the same node in case it occurs in the list:others
				if(s_cur->pvertex==cur->pvertex)
				{	
					count++;
					break;
				}
			cur=cur->nexte;	
		}
		s_cur=s_cur->nexte;
	}
	return count;
}



int find_tail(vertex *object,pass *inst)
{
	int count=0;
	vertex *cur;
	for(cur=inst->head;cur;cur=cur->nextv)
	{
		if(locate_e(object,cur->start))
		++count;
	}
	return count;
}	



edge *locate_e(vertex *object,edge *e)
{
	edge *temp;
	//printf("OBJECT %s",object->vname);
	for(temp=e;temp;temp=temp->nexte)
	{
		//printf("wwwwwwwwwwwwwwwwwwwwwww\n\n");
		//printf("%s %d",temp->pvertex->vname,temp->visited);
		if(temp->pvertex==object && temp->visited==0)
		{
			temp->visited=1;
			return temp;
		}
	}
	return NULL;
}



int completeTrianglesDir(edge *search_in,edge *others,pass *inst,vertex *object,int stat[])
{
	int count=0,f,g;
	edge *cur,*e;
	vertex *cur1,*cur2;
	for(cur=others;cur;cur=cur->nexte)
	{
		if(locate_e(cur->pvertex,search_in->pvertex->start)||locate_e(search_in->pvertex,cur->pvertex->start))
		{
			printf("%s %s\n",cur->pvertex->vname,search_in->pvertex->vname);
			++count;
		}
	}//correct
	/*printf("\n\n%dhaha",count);
	for(cur=others;cur;cur=cur->nexte)
	{
		if(locate_e(search_in->pvertex,cur->pvertex->start))
		{
			printf("%s %s\n",search_in->pvertex->vname,cur->pvertex->vname);
		++count;
		}
	}
	printf("\n\n%dkaka",count);*/
	for(cur1=inst->head;cur1;cur1=cur1->nextv)
	{
		//printf("****%s %d",cur1->vname,cur1->vno);
		for(e=search_in;e;e=e->nexte)
		{
		if(stat[(cur1->vno)]&&((locate_e(search_in->pvertex,cur1->start)||locate_e(cur1,search_in->pvertex->start))))
		{
			
		//printf("kakakuku");
		printf("%s %s flag\n",search_in->pvertex->vname,cur1->vname);
		++count;
		}
		}
	}
	for(cur1=inst->head;cur1;cur1=cur1->nextv)
	{
		if(stat[(cur1->vno)])
		{
			for(cur2=cur1->nextv;cur2;cur2=cur2->nextv)
			{
				if(stat[(cur2->vno)])
				{
					if(locate_e(cur1,cur2->start)||locate_e(cur2,cur1->start))
					++count;
				}
			}
		}
	}
	//printf("\n\n%dkakaka",count);
	return count;
	
}
	
	
void make_stat(vertex *head,int stat[],vertex *object)
{
	vertex *cur1;
	for(cur1=head;cur1;cur1=cur1->nextv)
	{
		//("**%s**",cur1->vname);
		if(locate_e(object,cur1->start))
		stat[cur1->vno]=1;
		else
		stat[cur1->vno]=0;
	}
}

