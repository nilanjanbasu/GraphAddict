#include<stdio.h>
#include<stdlib.h>
#include"structdef.h"

int gnuplot(pass *inst)
{
	int i=0;
	char x,name[100];
	float f;
	FILE *fp,*ft;
	fp=fopen("./tmp/plotdata.dat","w");		//opening the file in write mode to write the plotting information in it
	if(fp==NULL)
	{
		return 1;
	}
	do
	{
		f=find_probability(inst,i);		//writing in the file the degree and their corresponding probability with a space between them
		fprintf(fp,"%d\t%f\n",i,f);
		++i;
	}while(i<=inst->dmax);
	fclose(fp);
	ft=fopen("./tmp/batchfile","w");			//opening the script file containing the commands
	if(ft==NULL)
	{
		return 2;
	}
	fprintf(ft,"set terminal postscript;\\\n");   //redirect output to save
	fprintf(ft,"set output \"./tmp/image_12345.ps\";\\\n");  //output file name setting
	fprintf(ft,"set title 'degree distribution of a graph';\\");	//writing the commands in the script file
	fprintf(ft,"\nset xlabel 'degree';\\");  //gnuplot commands
	fprintf(ft,"\nset ylabel 'probability';\\");
	fprintf(ft,"\nset yrange [0:1];\\");  //set the range of y axis
	fprintf(ft,"\nplot'./tmp/plotdata.dat' smooth csplines;\\");
	fprintf(ft,"\nexit;");
	fclose(ft);
	system("gnuplot './tmp/batchfile'");	//calling the command to visualize the probability distribution
	system("convert -density 300 -rotate 90 ./tmp/image_12345.ps ./tmp/image_12345.gif");//imagemagik command to convert to jpg
	system("convert -resize 35%% -quality 80 ./tmp/image_12345.gif ./tmp/image_12345.gif"); //resizing command:imagemagik
	remove("./tmp/image_12345.ps");
	//system("display ./tmp/image_12345.gif");
		
	remove("./tmp/batchfile");
	remove("./tmp/plotdata.dat");
	return 0;			
}

