#include<stdio.h>
#include<stdlib.h>
#include"structdef.h"
int graphviz(pass *inst,char *file)
{
    char graphname[50];
    FILE *fp;
    int i=0;
	
    if(inst->flag==NO_FILE_CRTD){
	    return 1; //ERROR : NO FILE CREATED
    }    
    fp=fopen(file,"w+");


    for(i=0;file[i]!='.'&&file[i]!='\n';i++)
	graphname[i]=file[i];  //extracting graph name which will be written in the file
    graphname[i]='\0';

    if(inst->flag2=='d'){
        graphvizDirected(fp,inst->head,graphname);  //handles Directed graphs
        }
    else if(inst->flag2=='u'){
        graphvizUndirected(fp,inst->head,graphname);  //handles Undirected graphs
        }
    else
       return 1; 
    
}

void graphvizDirected(FILE *fp,vertex *head,const char *graphname)
{
    edge *temp;
    vertex *temp_v=head;	
    fprintf(fp,"digraph %s {\n",graphname);
    /*temp_v holds the current vertex and temp holds the current edge in the edge list of temp_v*/
    while(temp_v!=NULL)   
    {
	    temp=temp_v->start;
		if(!temp)
		{
			fprintf(fp,"\"%s\"",temp_v->vname);
			fputs(";\n",fp);
		}
				
	    while(temp!=NULL)
		{
		fprintf(fp,"\"%s\" -> \"%s\"",temp_v->vname,temp->pvertex->vname);
		if(temp->weight!=1)  //if weight==1 it is omitted as it is default weight
			fprintf(fp,"[label=""%.2f""];\n",temp->weight);
		else
			fputs(";\n",fp);
		temp=temp->nexte;
	    }
	    temp_v=temp_v->nextv;
    }
	    
    fprintf(fp,"}");
    fclose(fp);
    return;
}

void graphvizUndirected(FILE *fp,vertex *head,const char *graphname)
{
    edge *temp;
    vertex *temp_v=head;

    fprintf(fp,"graph %s {\n",graphname);
    
   
    while(temp_v!=NULL)
    {
	    temp=temp_v->start;
		if(!temp)
		{
			fprintf(fp,"\"%s\"",temp_v->vname);
			fputs(";\n",fp);
		}
	    while(temp!=NULL)
	    {  
/*as in undirected graphs two-way connections exist,we introduce 'visited' flag. Whenever we visit an edge we "visit" 
the edge starting from the current destination node to source node as well. And mark the 'visited' flags=1, whenever we examine edges we check whether it is visited or not thus rulling out the possibility of writing the same edge in the file once again*/
		if((temp->visited)!=1)
		{
		    fprintf(fp,"\"%s\" -- \"%s\"",temp_v->vname,temp->pvertex->vname);
			if(temp->weight!=1)
			fprintf(fp,"[label=""%.2f""];\n",temp->weight);
			else
			fputs(";\n",fp);
		    temp->visited=1;
		    visit(temp->pvertex,temp_v); //searches for the reverse edge and marks visited=1
		}
		temp=temp->nexte;
	    }
	    temp_v=temp_v->nextv;
    }

   
    unvisit_all(head);  //makes all the visited flags 0.Otherwise, it would cause abnormality. defined in free_data.c
    fprintf(fp,"}");
    fclose(fp);
    return;
    
}




