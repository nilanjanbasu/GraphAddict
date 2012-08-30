#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structdef.h"

int visualize(char *filename,int format)
{
	FILE *fp;
	char command[100],command1[100];	
	int i;
	fp=fopen(filename,"r");
	if(fp==NULL){		
		return 0; //ERROR: File loading error
	}	
	for(i=0;filename[i]!='.'&&filename[i]!='\n';i++)
		command1[i]=filename[i]; 
    	command1[i]='\0';
    	strcat(command1,"_temp_gr.gif");
	
	switch(format)
	{
		case 0:
		strcpy(command,autoChoose(fp));
		break;
		case 1:
		strcpy(command,"dot ");
		break;
		case 2:
		strcpy(command,"neato ");
		break;
		case 3:
		strcpy(command,"twopi ");
		break;
		case 4:
		strcpy(command,"circo ");
		break;
		case 5:
		strcpy(command,"fdp ");	
		break;
		case 6:
		strcpy(command,"sfdp ");
		break;
		case 7:
		return;
		
	}
	strcat(command,filename);	
	strcat(command," -Tgif > ./tmp/");
	strcat(command,command1);
			
	system(command);
	/*strcpy(command,"display ./tmp/");
		
	strcat(command,command1);
	system(command);*/
	command[0]='\0';
	command1[0]='\0';
	fclose(fp);
}
char *autoChoose(FILE *fp)
{
	char graphtype[10];
	
	fscanf(fp,"%s",graphtype);
	if(strcmp(graphtype,"graph")==0)
		return "neato ";
	else if(strcmp(graphtype,"digraph")==0) 
		return "dot ";
	else
	{
		printf("Debug info: Inside visualize.c, autoChoose() ; fetched value neither graph or digraph type\n");
		exit(0);
	}
}
		
	
