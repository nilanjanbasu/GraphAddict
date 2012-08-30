%module graph
%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structdef.h"

%}
struct vertex     //node corresponding to vertex
{
    char vname[50];   //name  of vertex
    int vno;          //vertex no. assigned to each vertex
    struct vertex *nextv;//pointer to next vertex node
    struct edge *start;  //pointer to an edge node
    int num;	 //stores the degree or indegree of each vertex
    char visited;
};
struct edge      		//node corresponding to edge
{
    struct edge *nexte;		//pointer to an edge node
    struct vertex *pvertex;	//pointer to a vertex node
    float weight;			//weight corresponding to each edge
    char visited;		//keeps track whether an edge node is visited or not
};
struct pass
{
    struct vertex *head;	//head pointer of the vertex nodes
    char flag;			//holds informations of a textfile to be opened and saved 
    char flag2;			//holds info of graph:whether it is directed or undirected
    int v_count;		//total number of vertices currently in memory-graph
    int dmax;	
    char status;		
};
struct stack{ 			//stack to be used in Short_dijkstra.c
	int data;
	struct stack *next;
	};
typedef struct stack stack;
typedef struct vertex vertex;
typedef struct edge edge;
typedef struct pass pass;
#define NO_FILE_CRTD 0
#define NEW_FILE 1
#define EXISTING_FILE 2
#define ONCE_WRITTEN 3
#define INFINITY 5000
#define FOUND 1
#define NOT_FOUND 0

extern int readgraph(pass *,FILE *);   
extern vertex *locatev(const char *,vertex *);
extern vertex *locatev_by_no(int,vertex *);//in Short_dijkstra.c
extern int ShortestPath(const char *,const char *,pass *);//in Short_dijkstra.c
extern pass *createFlagPack(void);//free_data.c ONLY FOR SWIG
extern void init_pass(pass *);//free_data.c ONLY FOR SWIG
extern int readInterface(pass *,char *);//readg.c FOR SWIG
extern int graphviz(pass *,char *);  //graphviz.c
extern int visualize(char *,int);
extern void add(pass *);
extern void addonlyEdge(pass );
extern void addVertexandEdge(pass *);
extern void addVertex(pass *,char *);
extern void addEdge(pass *,vertex *,vertex *,float,char *);
extern void setFlag2 (pass *,char *); //in free_data.c
extern int save_interface(char *,pass *);
extern vertex *getHead(pass *inst); //freedata.c
extern int deleteNode(pass *inst,const char *name); //delete.c
extern void changeName(const char *,vertex *); //free_data.c
extern int deleteEdgeDirect(const char *, const char *,pass *); //delete.c
extern int gnuplot(pass *);//in gnuplot.c
extern float cluster_interface(pass *,char *);//Cluster.c
extern float clusterUndir(vertex *);//Cluster.c
extern float clusterDir(vertex *,pass *);//Cluster.c
extern int findEligibleNodes(vertex *,pass *);//Cluster.c
extern int completeTrianglesUndir(edge *,edge *);//Cluster.c
extern int completeTrianglesDir(edge *,edge *,pass *,vertex *,int stat[]);
extern void make_stat(vertex *head,int stat[],vertex *object);
extern edge *locate_e(vertex *object,edge *e);
extern int find_tail(vertex *object,pass *inst);
extern int return_info(char *vname,pass *inst);
extern void update_num(pass *inst);
extern vertex *showmax(pass *inst);
extern void setstatus(int weight,pass *);
extern int return_weight(pass *inst);
