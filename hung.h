#ifndef HUNG_H
#define HUNG_H

#define N 150 //max number of vertices in one part
#define INF 1000 //just infinity

extern int cost[N][N]; //cost matrix
extern int n, max_match; //n workers and n jobs
extern int lx[N], ly[N]; //labels of X and Y parts
extern int xy[N]; //xy[x] - vertex that is matched with x,
extern int yx[N]; //yx[y] - vertex that is matched with y
extern bool S[N], T[N]; //sets S and T in algorithm
extern int slack[N]; //as in the algorithm description
extern int slackx[N]; //slackx[y] such a vertex, that
extern int prev[N]; //array for memorizing alternating paths

void init_labels();
void update_labels();
void augment(); //main function of the algorithm
int hungarian(int set_n);

#endif
