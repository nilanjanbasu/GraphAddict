#include<stdio.h>
#include<stdlib.h>
#include"structdef.h"
#include<string.h>
void add(pass *pinst)
{

    char x,c;
    if(!(pinst->flag)){   //i.e. flag==NO_FILE_CRTD
        printf("please go back to option 1 before coming here");
        return;
    }
    printf("\n\n***press <a> to update vertex and edge inforamtion.");
    printf("\n\n***press <b> to update only edge information.");
    printf("\nenter choice: ");
    scanf("%c",&x);
    if(x=='a')
        addVertexandEdge(pinst);
    else if(x=='b')
        addonlyEdge(*pinst);
    else
        printf("\nInvalid selection");

}
void addonlyEdge(pass inst)
{
    vertex *temp,*temp2;
    char x='y',choice;
    int w;
    char v1[50],v2[50];
    edge *tedge;

    while(x=='y')
    {
	w=1;
        printf("\nEnter the names: \n");
	printf("starts from: ");
	scanf("%[^\n]",v1);
	getchar();
	printf("\nEnds in: ");
	scanf("%[^\n]",v2);
	getchar();
        printf("\ndo you wish to add any weight to the edge you just entered?(y/n): ");
	scanf("%c",&choice);
	if(choice=='y')
	{
		printf("\nenter the weight corresponding to the edge: ");
		scanf("%d",&w);//flush may be needed
	}
	else if(choice=='n');
	else
		printf("invalid choice::");

        temp=locatev(v1,inst.head);
        temp2=locatev(v2,inst.head);
	if(temp==NULL||temp2==NULL)
	{
		printf("One or both of the nodes does not exist. Please try again.\n");
		continue;
	}

        tedge=temp->start;
        temp->start=(edge *)calloc(1,sizeof(edge));  //creates a new edge that points to 2nd node
        temp->start->weight=w;
        temp->start->pvertex=temp2;
        temp->start->nexte=tedge;

        if(inst.flag2=='u')
        {
            tedge=temp2->start;
            temp2->start=(edge *)calloc(1,sizeof(edge));  /*when graph is undirected the connection should be in both 								   ways*/
            temp2->start->weight=w;
            temp2->start->pvertex=temp;
            temp2->start->nexte=tedge;
        }
	getchar();
        printf("\nUpdate again?y/n:");
        x=getchar();//flush may be needed
	getchar();

    }

}
void addVertexandEdge(pass *inst)
{
   
	char x='y';
	char name[50];

	do
	{
		printf("\nenter vertex name:");
		scanf("%s",name);
		x=getchar();
        	addVertex(inst,name);
		printf("\nDo you want to enter another vertex?y/n:");
		x=getchar();
    	}while(x=='y');
        getchar();
        printf("\nUpdate edge info? y/n:");
        x=getchar();
        getchar();
        if(x=='y'||x=='Y')
        	addonlyEdge(*inst);  //calls this to updates edge infos
}
void addVertex(pass *inst, char *name)
{
	vertex *temp,*temp2;
	temp=(vertex *)calloc(1,sizeof(vertex));
	inst->v_count=inst->v_count+1;   //assignment of a unique code needed for fast operations	
	temp->vno=inst->v_count;  	//increment of total number of nodes
        strcpy(temp->vname,name);
        temp2=inst->head;
        inst->head=temp;
        (inst->head)->nextv=temp2;
}
void addVertexWithNumber(pass *inst,char *name,int vnumber) /*modifies the head and v_count*/
{
	vertex *temp,*temp2;
	temp=(vertex *)calloc(1,sizeof(vertex));
	temp->vno=vnumber;       	
	inst->v_count=inst->v_count+1;	//increment of total number of nodes
        strcpy(temp->vname,name);
        temp2=inst->head;
        inst->head=temp;
        (inst->head)->nextv=temp2;
}
void addEdge(pass *inst,vertex *from,vertex *to,float w,char *flag)
{
	edge *tedge=NULL;
	//printf("%s %s",from->vname,to->vname);
	tedge=from->start;
        from->start=(edge *)calloc(1,sizeof(edge));  //creates a new edge that points to 2nd node
        from->start->weight=w;
        from->start->pvertex=to;
        from->start->nexte=tedge;
	if(flag[0]=='U')
        {
            tedge=to->start;
            to->start=(edge *)calloc(1,sizeof(edge));  /*when graph is undirected the connection should be in both ways*/
            to->start->weight=w;
            to->start->pvertex=from;
            to->start->nexte=tedge;
        }	
}
