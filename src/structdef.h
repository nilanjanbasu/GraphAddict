#include<stdio.h>

struct vertex     //node corresponding to vertex
{
    char vname[50];   //name  of vertex
    int vno;          //vertex no. assigned to each vertex
    struct vertex *nextv;//pointer to next vertex node
    struct edge *start;  //pointer to an edge node
    int num;
    char visited;		 //stores the degree or indegree of each vertex
   
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
    char status;		//weighted or unweighted 			
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

extern void calculate(pass);   //located in compute.c
extern int count(vertex *);  //save.c
extern void informationDisplay(pass);  //info.c
extern int count(vertex *);  //save.c
extern int graphviz(pass *,char *);  //graphviz.c
extern int visualize(char *filename,int format); //visualize.c
extern void update_num(pass *);  //located in compute.c
extern vertex* showmax(pass *);  //compute.c

extern vertex *free_data(vertex *);  //free_data.c
extern void unvisit_all(vertex *);  //free_data.c
extern vertex *locatev_by_no(int,vertex *);//in Short_dijkstra.c
extern void init_found_and_shortest(int *,int *,int *,vertex *,int);//in Short_dijkstra.c
extern int quick_show(int,int,pass *,int *rec);//in Short_dijkstra.c
extern void q_show_dir(vertex *,int *,FILE *);//in Short_dijkstra.c
extern void q_show_undir(vertex *,int *,FILE *);//in Short_dijkstra.c
extern int ShortestPath(const char *,const char *,pass *);//in Short_dijkstra.c
extern void ShortPathNonNeg(pass *,int,int,int *,int *,int *);//in Short_dijkstra.c
extern int choose(int *,int *,int,pass *);//in Short_dijkstra.c
extern void PrepareRecforViewing(int *,int,int,int);//in Short_dijkstra.c
extern void graphvizDirected(FILE *,vertex *,const char *);//graphviz.c
extern void graphvizUndirected(FILE *,vertex *,const char *);//graphviz.c
extern float find_probability(pass*,int);  //probability.c
extern float avg_degree(pass*);  //avg_degree.c
extern float cluster_interface(pass *,char *);//Cluster.c
extern float clusterUndir(vertex *);//Cluster.c
extern float clusterDir(vertex *,pass *);//Cluster.c
extern int findEligibleNodes(vertex *,pass *);//Cluster.c
extern int completeTrianglesUndir(edge *,edge *);//Cluster.c
extern int completeTrianglesDir(edge *,edge *,pass *,vertex *,int stat[]);
extern void make_stat(vertex *head,int stat[],vertex *object);
extern edge *locate_e(vertex *object,edge *e);
extern int find_tail(vertex *object,pass *inst);
extern int gnuplot(pass *);//in gnuplot.c
extern pass *createFlagPack(void);//free_data.c ONLY FOR SWIG;
extern void init_pass(pass *);//free_data.c ONLY FOR SWIG
extern int readInterface(pass *,char *);//readg.c FOR SWIG
extern void visit(vertex *comp,vertex *t);//free_data.c
extern void addVertex(pass *inst,char *name);

extern vertex *locateid(int,vertex *);
extern void setFlag2 (pass *,char *);
extern vertex *getHead(pass *inst);//free_data.c
/*in delete.c*/
extern int deleteNode(pass *,const char *);
extern void deleteEdgesTo(vertex *,vertex *);
extern void deleteEdge(edge *,edge **);
extern void deleteVertexWithEdges(vertex *,vertex **);
extern vertex *locateprev(vertex *,vertex *);
extern int deleteEdgeDirect(const char *, const char *,pass *);
extern void renumberNodes(vertex *head);
/*in free_data.c*/
extern void changeName(const char *,vertex *);
extern int return_info(char *vname,pass *inst);
extern void mapNameToNumbers(FILE *,vertex *);
/*save.c*/
extern void directed_NIL(FILE *fp,vertex *head); 
extern void undirected_NIL(FILE *fp,vertex *head);
extern int save_interface(char *,pass *);
extern void printUnvisitedVertex(FILE *,vertex *);
extern void directed_SRJ(FILE *,vertex *,char);
extern void undirected_SRJ(FILE *,vertex *,char);
extern void mapNameToNumbers(FILE *,vertex *);
extern void resetNodeVisitField(vertex *);
/*readg.c*/
extern int readgraph_SRJ(pass *,FILE *);
extern void fetchNodesSRJ(FILE *,pass *);
extern int connectSRJ(FILE *,pass *);
extern vertex *locatev(const char *,vertex *); 
extern vertex *locatev(const char *,vertex *); 
extern int readgraph(pass *,FILE *);
extern vertex *read_dir(FILE *,int *,char *); 
extern vertex *read_un(FILE *,int *,char *);
/*addv.c*/
extern void addVertexWithNumber(pass *,char *,int);
extern void addVertexandEdge(pass *);  
extern void addonlyEdge(pass); 
extern void add(pass *);
extern void setstatus(int weight,pass *);
extern int return_weight(pass *inst);
extern void addEdge(pass *,vertex *,vertex *,float,char *);
/*visualize.c*/
char *autoChoose(FILE *);