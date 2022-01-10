#include <bits/stdc++.h>
#define MAX 999
using namespace std;
int iter, V;

void initializare(int **mat){
	for(int i = 0; i < V; i++)
		for(int j = 0; j < V; j++)
			mat[i][j] = 0;
}

void initializareFloyd(int **mat){ //initilizare cu infinit in loc de 0
	for(int i = 0; i < V; i++)
		for(int j = 0; j < V; j++)
		    if(mat[i][j] == 0 && i != j) mat[i][j] = MAX;
}

void generare(double p, int **mat){
	int d = 1, i = 0, j = d + i, x = p*V*(V-1); //calculeaza cate muchii trebuie sa genereze
	srand(time(NULL));
	for(int k = 0; k < x; k+=2){ //parcurge matricea pe diagonale,paralele cu cea principala si genereaza atatea muchii x
		if(j >= V){
			d ++;
			i = 0;
			j = i + d;
		}
		mat[j][i] = rand()% 100+1;
		mat[i++][j++] = rand()% 100+1;
	}
	/*for(i = 0; i < V; i++)
        for(j = 0; j < V; j++)
            if(mat[i][j] != 0) cout << " " << i << " -> " << j << "\t\t" << mat[i][j] << endl;*/
}

int minDist(int dist[], bool set[]){
    int min = MAX, index;
    for(int v = 0; v < V; v++)
        if(set[v] == false && dist[v] <= min) min = dist[v], index = v;
    return index;
}

void path(int parent[], int j, int **mat){
    if(parent[j] == -1) return;
    path(parent, parent[j], mat);
    cout << "--(" << mat[parent[j]][j] << ")-->" << j;
}

void Dijkstra(int **mat, int src){
    cout << "\n\tDjikstra\n\n";
    iter = 0;
	int dist[V], parent[V];
    bool set[V];
    for(int i = 0; i < V; i++)
        dist[i] = MAX, set[i] = false, parent[0] = -1; //set dist infinit,nod vizitate cu false,nod anter -1
    dist[src] = 0;
    for(int n = 0; n < V-1; n++){
        int u = minDist(dist, set);
        set[u] = true; //marcheaza nodul curent vizitat
        for(int v = 0; v < V; v++){
            if(!set[v] && mat[u][v] && dist[u] != MAX && dist[u] + mat[u][v] < dist[v]){
                dist[v] = dist[u] + mat[u][v];
                parent[v] = u;
                iter ++;
			}
		}
    }
    /*for(int i = 1; i < V; i++){
        cout  << "\n " << src << " -> " << setw(3) << i << setw(10) << dist[i] << "     " << src;
        path(parent, i, mat);
	}*/
    cout << "\n " << iter << " iteratii   ";
}

void pathFloyd(int **path, int v, int u, int **mat, int *pref){
    if(path[v][u] == v) return;
    pathFloyd(path, v, path[v][u], mat, pref);
	*pref = path[v][u];
    cout << "--(" << mat[path[v][path[v][u]]][path[v][u]] << ")-->" << path[v][u];
}

void Floyd(int **mat){
    cout << "\n\tFloyd Warshall\n\n";
    iter = 0;
    int **cost, **path; //matricea consturilor si drumurilor
    cost = new int*[V];
    path = new int*[V];
	for(int i = 0; i < V; i++)
		cost[i] = new int[V];
	for(int i = 0; i < V; i++)
		path[i] = new int[V];
    for(int v = 0; v < V; v++)
        for(int u = 0; u < V; u++){
            cost[v][u] = mat[v][u]; //matricea costurilor devine graful initial
            if(v == u) path[v][u] = 0; //pe diagonala in matricea drumurilor se pune 0
            else if(cost[v][u] != MAX) path[v][u] = v; //daca exista muchie se pune in matricea drumurilor
            else path[v][u] = -1; //daca nu este muchie se pune -1
        }
    for(int k = 0; k < V; k++) //k nod intermediar, incepe cu 0 si termina cu V
        for(int v = 0; v < V; v++) //v rand
            for(int u = 0; u < V; u++){ //u coloana
				if(cost[v][k] != MAX && cost[k][u] != MAX && cost[v][k] + cost[k][u] < cost[v][u]){
                    cost[v][u] = cost[v][k] + cost[k][u];
                    path[v][u] = path[k][u];
                    iter ++;
                }
            }
	/*for(int v = 0; v < 1; v++)
        for(int u = 0; u < V; u++)
            if(u != v && path[v][u] != -1){
                int pref = v;
				cout << v << " -> " << setw(3) << u << setw(10) << cost[v][u] << "     " << v;
                pathFloyd(path, v, u, mat, &pref);
                cout << "--(" << mat[pref][u] << ")-->";
                cout << u << endl;
            }*/
    cout << " " << iter << " iteratii   ";
}

int main(){
	int **mat;
	double p;
	cout << " Introdu nr de varfuri ";
	cin >> V;
	mat = new int*[V];
	for(int i = 0; i < V; i++)
		mat[i] = new int[V];

	cout << "\n Graf Rar\n\n";
	p = 0.4;
	initializare(mat);
	generare(p, mat);
    auto startR = chrono::high_resolution_clock::now();
	Dijkstra(mat, 0);
    auto endR = chrono::high_resolution_clock::now();
	cout << " timp de " << chrono::duration_cast<chrono::nanoseconds>(endR - startR).count()* 1e-9 << " s\n\n ";
	initializareFloyd(mat);
    auto startRF = chrono::high_resolution_clock::now();
	Floyd(mat);
    auto endRF = chrono::high_resolution_clock::now();
	cout << " timp de " << chrono::duration_cast<chrono::nanoseconds>(endRF - startRF).count()* 1e-9 << " s\n\n ";

	cout << "\n Graf Mediu\n\n";
	p = 0.6;
	initializare(mat);
	generare(p, mat);
    auto startM = chrono::high_resolution_clock::now();
	Dijkstra(mat, 0);
    auto endM = chrono::high_resolution_clock::now();
	cout << " timp de " << chrono::duration_cast<chrono::nanoseconds>(endM - startM).count()* 1e-9 << " s\n\n ";
	initializareFloyd(mat);
    auto startMF = chrono::high_resolution_clock::now();
	Floyd(mat);
    auto endMF = chrono::high_resolution_clock::now();
	cout << " timp de " << chrono::duration_cast<chrono::nanoseconds>(endMF - startMF).count()* 1e-9 << " s\n\n ";

	cout << "\n Grad Dens\n\n";
	p = 0.9;
	initializare(mat);
	generare(p, mat);
    auto startD = chrono::high_resolution_clock::now();
	Dijkstra(mat, 0);
    auto endD = chrono::high_resolution_clock::now();
	cout << " timp de " << chrono::duration_cast<chrono::nanoseconds>(endD - startD).count()* 1e-9 << " s\n\n ";
	initializareFloyd(mat);
    auto startDF = chrono::high_resolution_clock::now();
	Floyd(mat);
    auto endDF = chrono::high_resolution_clock::now();
	cout << " timp de " << chrono::duration_cast<chrono::nanoseconds>(endDF - startDF).count()* 1e-9 << " s\n\n ";
}
