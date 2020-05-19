#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"

Graph watts_strogatz(int n, int k, float beta) {
	Graph G;
	G.V = n;
	G.edges = malloc(G.V * sizeof *G.edges);

	for (int i = 0; i < G.V; i++) {
		G.edges[i].head = NULL;
	}

	for (int u = 0; u < G.V; u++) {
		// add edges for neighbours
		for (int i = 1; i <= k / 2; i++) {
			int v = (u+i) % G.V;

			// add edges to random vertices with probability beta
			if ((float)rand() / RAND_MAX < beta) {
				v = (int)(((float)rand() / RAND_MAX) * G.V);
			}

			// add edges in both directions
			EdgeNodePtr uv;
			uv = malloc(sizeof * uv);
			uv->edge.to_vertex = v;
			uv->next = G.edges[u].head;
			G.edges[u].head = uv;

			EdgeNodePtr vu;
			vu = malloc(sizeof * vu);
			vu->edge.to_vertex = u;
			vu->next = G.edges[v].head;
			G.edges[v].head = vu;
		}
	}

	return G;
}

Graph barabasi_albert(int n, int v0) {
	// TODO
	Graph G;
	G.V = n;
	G.edges = malloc(G.V * sizeof * G.edges);

	for (int i = 0; i < G.V; i++) {
		G.edges[i].head = NULL;
	}

	int* init_network = malloc(sizeof(int)*v0);
	for (int i = 0; i < v0; i++) {
		init_network[i] = (int)(((float)rand() / RAND_MAX) * G.V);
	}

	//degree of vertexes
	int* degree = malloc(sizeof(int) * G.V);
	for (int i = 0; i < G.V; i++) {
		degree[i] = 0;
	}
	
	//weight of vertexes
	float* weight = malloc(sizeof(float) * G.V);
	for (int i = 0; i < G.V; i++) {
		weight[i] = 0;
	}

	// completed connected work
	for (int i = 0; i < v0; i++) {
		for (int j = i + 1; j < v0; j++) {
			EdgeNodePtr uv;
			uv = malloc(sizeof * uv);
			uv->edge.to_vertex = init_network[j];
			uv->next = G.edges[init_network[i]].head;
			G.edges[init_network[i]].head = uv;
			degree[init_network[i]]++;

			EdgeNodePtr vu;
			vu = malloc(sizeof * vu);
			vu->edge.to_vertex = init_network[i];
			vu->next = G.edges[init_network[j]].head;
			G.edges[init_network[j]].head = vu;
			degree[init_network[j]]++;
		}
	}

	//calculate weight & degree of each vertex via degree of each vertex
	int total_degree = 0;
	for (int i = 0; i < G.V; i++) {
		if (degree[i]) {
			total_degree = total_degree + degree[i] / 2;
		}
	}

	for (int i = 0; i < G.V; i++) {
		if (degree[i]) {
			weight[i] = (float)degree[i] / total_degree;
		}
	}

	// add vertex to the network
	for (int i = 0; i < G.V; i++) {
		for (int j = 0; j < v0; j++) {
			float conn_rate = (float)rand() / RAND_MAX;
			int target = match_weight(G.V, weight, conn_rate, degree); // find a target


			int connected = 0;
			for (EdgeNodePtr e = G.edges[target].head; e != NULL; e = e->next) {
				if (e->edge.to_vertex == i) {
					connected = 1;
					break;
				}
			}

			// if the target does not connect to the No.i vertex, do connections
			if (connected == 0) {
				//printf("buliding the network :: vetex::%d --> target::%d\n", i, target);
				// add edges in both directions
				EdgeNodePtr uv;
				uv = malloc(sizeof * uv);
				uv->edge.to_vertex = target;
				uv->next = G.edges[i].head;
				G.edges[i].head = uv;
				degree[i]++;

				EdgeNodePtr vu;
				vu = malloc(sizeof * vu);
				vu->edge.to_vertex = i;
				vu->next = G.edges[target].head;
				G.edges[target].head = vu;
				degree[target]++;


				int total_degree = 0;
				for (int i = 0; i < G.V; i++) {
					if (degree[i]) {
						total_degree = total_degree + degree[i] / 2;
					}
				}

				for (int i = 0; i < G.V; i++) {
					if (degree[i]) {
						weight[i] = (float)degree[i] / total_degree;
					}
				}


			}
		}
	}

	return G;
}

int match_weight(int v, float* weight, float conn_rate, int* degree) {
	int target = -1;
	int curr_degree = 0;
	for (int i = 0; i < v; i++) {
		if (fabs((double)weight[i] - (double)conn_rate) < 0.000001 && curr_degree <= degree[i] && degree[i] != 0) {
			target = i;
			curr_degree = degree[i];
		}
	}

	// if no vertex found, return the vertex with biggest weight
	if (target == -1) {
		float rate = 0;
		for (int i = 0; i < v; i++) {
			if (rate < weight[i]) {
				target = i;
				rate = weight[i];
			}
		}
	}

	return target;
}