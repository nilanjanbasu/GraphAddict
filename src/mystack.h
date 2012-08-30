#include<stdlib.h>     //only to include in short_dijkstra.c
void *init(void);
void push(int,stack **);
int pop(stack **);

void *init(void){return NULL;}

void push(int i,stack **head)
{
	stack *cur;
	cur=*head;
	
	*head=(stack *)malloc(sizeof(stack));
	(*head)->data=i;
	(*head)->next=cur;
}

int pop(stack **head)
{
	stack *cur;
	int i;
	cur=*head;
	(*head)=(*head)->next;
	i=cur->data;
	free(cur);
	return i;
}
