#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

// update one step of the simulation
// G is the graph
// state is current state of individuals
// inf_rate is the infection rate
// rec_rate is the recovery rate
void step(Graph G, int* state, float inf_rate, float rec_rate);

// plot % infected (each '.' is 1%)
void plot(Graph G, int* state);

// vaccinate num individuals
void vaccinate(Graph G, int* state, int num);

int main() {	
	// Build Social Network
	Graph G;

	//G = watts_strogatz (10000, 10, 0.1);

	// You need to switch to the Barabasi&Albert model once implemented
	G = barabasi_albert(10000, 3);

	// Infection state  0 = susceptible, 1 = infected, 2 = recovered
	int* state = malloc(G.V * sizeof *state);

	/*******************************************************************/
	/*** Baseline ******************************************************/
	/*******************************************************************/

	// Initialise population
	for (int i = 0; i < G.V; i++) {
		state[i] = 0;
	}

	// Infect one individual
	state[0] = 1;

	// Run simulation for 50 time steps

	printf("No Vaccination\n");
	printf("     t|--------10--------20--------30--------40--------50--------60\n");
	for (int i = 0; i < 50; i++) {
		printf("B %4d|", i);
		plot(G, state);
		step(G, state, 0.2, 0.05);
	}
	printf("\n");

	/*******************************************************************/
	/*** Intervention **************************************************/
	/*******************************************************************/

	// Reinitialise

	for (int i = 0; i < G.V; i++) {
		state[i] = 0;
	}

	// Vaccinate 200 individuals
	vaccinate(G, state, 200);

	// Infect same individual
	state[0] = 1;

	// Run simulation for 50 time steps

	printf("With Vaccination\n");
	printf("     t|--------10--------20--------30--------40--------50--------60\n");
	for (int i = 0; i < 50; i++) {
		printf("V %4d|", i);
		plot(G, state);
		step(G, state, 0.2, 0.05);
	}
	printf("\n");

	return 0;
}


void vaccinate(Graph G, int* state, int num) {
	// TODO
	int c = 0;
	while (c < num) {
		int i = (int)rand() % G.V;
		if (state[i] != 2) {
			state[i] = 2;
			c++;
		}
	}
}


void step(Graph G, int* state, float inf_rate, float rec_rate) {
	// TODO
	for (int i = 0; i < G.V; i++) {
		if (state[i] == 1) {
			if (((float)rand() / RAND_MAX < rec_rate)) { state[i] = 2; }
			for (EdgeNodePtr l = G.edges[i].head; l!=NULL; l = l->next) {
				int to_vertex = l->edge.to_vertex;
				if (((float)rand() / RAND_MAX < inf_rate) && state[to_vertex] == 0) { state[to_vertex] = 1; }
			}
		}
	}
}

void plot(Graph G, int* state) {
	int c = 0;

	for (int i = 0; i < G.V; i++) {
		if (state[i] == 1)
			c++;
	}

	int p = (int)((float)c / G.V * 100.0);

	for (int i = 0; i < p; i++) {
		printf(".");
	}
	printf("\n");
}