#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structdef.h"
int readInterface(pass *inst,char *filename)
{        FILE *fp;     
	 int i=0;
         fp=fopen(filename,"r+");  
         inst->flag=EXISTING_FILE; //may not be needed(TCL/TK)
         for(i=0;filename[i]!='.';++i);
         if(filename[i+1]=='n'&&filename[i+2]=='i'&&filename[i+3]=='l')
         	return (readgraph(inst,fp));
         else if(filename[i+1]=='s'&&filename[i+2]=='r'&&filename[i+3]=='j')
	 {
	 	return (readgraph_SRJ(inst,fp));
	 }
}
int readgraph_SRJ(pass *inst,FILE *fp)
{
	int no,retval=0;
	fscanf(fp,"%d %c\n",&no,&inst->flag2);
	fetchNodesSRJ(fp,inst);
	if(inst->v_count!=no)
		return 2; //vertex number mismatch
	retval=connectSRJ(fp,inst);
	fclose(fp);
	return retval; 
}
void fetchNodesSRJ(FILE *fp,pass *inst)
{
	char m;
	int d=-1;
	char fnodename[500];
	
	for(m=getc(fp);m=!'{';m=getc(fp));
	for(m=getc(fp);m=!'\n';m=getc(fp));
	
	while((m=getc(fp))!='}')
	{
		fseek(fp,-1,SEEK_CUR);
		fscanf(fp,"%d -> \"%[^\"]%*c\n",&d,fnodename);
		/*printf("%d %s\n",d,fnodename);
		getchar();*/
		addVertexWithNumber(inst,fnodename,d);
	}
	getc(fp); //take the newline also
}	
int connectSRJ(FILE *fp,pass *inst)
{
	int source,dest;
	float wt=1;
	vertex *phead=inst->head,*source_ptr=NULL,*dest_ptr=NULL;
	char fetchedline[500],m;
	long pos=0;
	FILE *frec=fp;
	pos=ftell(fp);
	
	while(!feof(fp))
	{
		fetchedline[0]=0;
		wt=1;
		fgets(fetchedline,500,fp);
		fflush(fp);
		
		m=sscanf(fetchedline,"%d %d [ %f ]\n",&source,&dest,&wt);
		if(m==3||m==2){		/*errorprone*/
			source_ptr=locatev_by_no(source,inst->head);
			dest_ptr = locatev_by_no(dest,inst->head);	
			addEdge(inst,source_ptr,dest_ptr,wt,(inst->flag2=='u'?"Undirected":"Directed"));
			
		}			
	}
	
	fseek(frec,pos,SEEK_SET);
	for(m=getc(frec);fetchedline[m]!='\0'&&m!='[';m=getc(frec));
	if(m=='[')
		inst->status='w';
	else
		inst->status='u';	
	return 0;
}
int readgraph(pass *inst,FILE *fp)
{
    int no;
    fscanf(fp,"%d %c\n",&no,&inst->flag2);      //reads the information about the no. of vertices and the type of the graph from the specified file
    if(inst->flag2=='u')
        inst->head=read_un(fp,&inst->v_count,&inst->status);        //read_un handles undirected graph-file loading
    else if(inst->flag2=='d')
        inst->head=read_dir(fp,&inst->v_count,&inst->status);	      //read_dir handles directed graph-file loading
    else {     
            //printf("Previous work with this file was terminated abruptly->file is corrupt. Please create a new file.");
	    fclose(fp);     /*check for other type of files as well as improperly written graph-files*/
            return 1; //error code for TCL
        }
    fclose(fp);
    if(inst->v_count==no)  /*no stores total number of vertices in graph according to file and inst.v_count stores 				    total number of vertices read,when both are equal,conclusion: Graph loaded properly */
           return 0; //success code
}//fclose necessery

vertex *read_dir(FILE *fp,int *v_count,char *st)
{
    char v[2][50],m;
    int i,c=1,flag=0,weightflag=0;
    vertex *head=NULL,*temp,*temp2=NULL,*pv[2];
    edge *x,*y;
    int SIZE_EDGE,SIZE_VERTEX;

    SIZE_EDGE=sizeof(edge);
    SIZE_VERTEX=sizeof(vertex);


   	while(!feof(fp))
	{
		fscanf(fp,"\"%[^\"]%*c",v[0]);
		if((getc(fp))=='\n')
			flag=1;
		else if((getc(fp))=='\"')
		{
			flag=0;
			fscanf(fp,"%[^\"]%*c",v[1]);
			for(m=getc(fp);m!='['&&m!='\n'&&m!=EOF;m=getc(fp));
			
			if(m=='[') 
			{
				fscanf(fp,"%d ]\n",&c);
				if(weightflag==0)
					weightflag=1;
			}
		}
		//printf("%s %s %c",v[0],v[1],m);
		pv[0]=locatev(v[0],head);
			
		if(pv[0]==NULL)
            	{
               	 	temp=(vertex *)calloc(1,SIZE_VERTEX);
                	strcpy(temp->vname,v[0]);
			*v_count=*v_count+1;
			temp->vno=*v_count;	
                	if(head==NULL)
                   	 	pv[0]=head=temp;
                	else
               		 {
                   		 temp2=head;
                   		 pv[0]=head=temp;
               	    		 head->nextv=temp2;
               		 }
           	 }
		if(!flag)
		{
			pv[1]=locatev(v[1],head);
			 if(pv[1]==NULL)
            		{
               	 		temp=(vertex *)calloc(1,SIZE_VERTEX);
                		strcpy(temp->vname,v[1]);
				//puts(temp->vname);
				*v_count=*v_count+1;
				temp->vno=*v_count;		
                		if(head==NULL)
                   	 		pv[1]=head=temp;
                		else
               			 {
                   			 temp2=head;
                   			 pv[1]=head=temp;
               	    		 	head->nextv=temp2;
               		 	}
           	 	}
			 if(pv[0]->start==NULL)  /*the vertex does not have any edges in memory yet*/
        		{
           			pv[0]->start=(edge *)calloc(1,SIZE_EDGE);	   
	  			pv[0]->start->weight=c;   /*make an edge*/
           			pv[0]->start->pvertex=pv[1];
				//pv[0]->start->nexte=NULL;
        		}
        		else
        		{
         		        x=pv[0]->start;
          		        y=(edge *)calloc(1,SIZE_EDGE);
            		        y->pvertex=pv[1];  /*make an edge and insert at front of the edge-list*/
	    		        y->weight=c;
           		        y->nexte=x;
           			pv[0]->start=y;
      			  }
		}
	}
	if(weightflag==1)
		*st='w';
	else
		*st='u';
	return head;
                 
}
vertex *read_un(FILE *fp,int *v_count,char *st)
{
    char v[2][50],m;
    int i,j,tot=1,c=1,flag=0,weightflag=0;
    vertex *head=NULL,*temp,*temp2=NULL,*pv[2];
    edge *x,*y;
    int SIZE_EDGE,SIZE_VERTEX;

    SIZE_EDGE=sizeof(edge);
    SIZE_VERTEX=sizeof(vertex);
 	 while(!feof(fp))
	{
		fscanf(fp,"\"%[^\"]%*c",v[0]);
		if((getc(fp))=='\n')
			flag=1;
		else if((getc(fp))=='\"')
		{
			flag=0;
			fscanf(fp,"%[^\"]%*c",v[1]);
			for(m=getc(fp);m!='['&&m!='\n'&&!feof(fp);m=getc(fp));
			if(m=='[') 
			{
				fscanf(fp,"%d ]\n",&c);
				if(weightflag==0)
					weightflag=1;
			}
		}
		pv[0]=locatev(v[0],head);
			
		if(pv[0]==NULL)
            	{
               	 	temp=(vertex *)calloc(1,SIZE_VERTEX);
                	strcpy(temp->vname,v[0]);
			//puts(temp->vname);
			temp->vno=*v_count;
			*v_count=*v_count+1;		
                	if(head==NULL)
                   	 	pv[0]=head=temp;
                	else
               		 {
                   		 temp2=head;
                   		 pv[0]=head=temp;
               	    		 head->nextv=temp2;
               		 }
           	 }
		if(!flag)
		{
			pv[1]=locatev(v[1],head);
			 if(pv[1]==NULL)
            		{
               	 		temp=(vertex *)calloc(1,SIZE_VERTEX);
                		strcpy(temp->vname,v[1]);
				//puts(temp->vname);
				temp->vno=*v_count;
				*v_count=*v_count+1;		
                		if(head==NULL)
                   	 		pv[1]=head=temp;
                		else
               			 {
                   			 temp2=head;
                   			 pv[1]=head=temp;
               	    		 	head->nextv=temp2;
               		 	}
           	 	}
			 for(i=0;i<2;++i)
        		{
           			 j=tot-i;
           			 if(pv[i]->start==NULL)  /*same logic repeated for both of the nodes ad in undirected graph according to our 

				    data structure there should be connection between two nodes starting from either 					    one*/
           			 {
            				    pv[i]->start=(edge *)calloc(1,SIZE_EDGE);
              				    pv[i]->start->weight=c;
               				    pv[i]->start->pvertex=pv[j];
					  // pv[i]->start->nexte=NULL;
                                 }
            			else
           			 {
                			x=pv[i]->start;
                			y=(edge *)calloc(1,SIZE_EDGE);
               				 y->pvertex=pv[j];
					y->weight=c;
                			y->nexte=x;
               				 pv[i]->start=y;
           			 }
        		}
		}
	}
	if(weightflag==1)
		*st='w';
	else
		*st='u';
	return head;
}    
vertex *locatev(const char *x,vertex *head)  //returns ptr to the vertexname passed if exists,else returns NULL
{
    while(head!=NULL)
    {
       if(strcmp(head->vname,x)==0)
          return (head);
       else
       	  head=head->nextv;
    }
    return NULL;
}

int return_weight(pass *inst)
{
	if(inst->status=='w')
		return 0;
	else
		return 1;
}




void setstatus(int weight,pass *inst)
{
	if(weight==0)
		inst->status='w';
	else
		inst->status='u';
}
