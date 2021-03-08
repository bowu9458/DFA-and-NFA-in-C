/*
 * dfa.c
 *
 *  Created on: 2020Äê1ÔÂ28ÈÕ
 *      Author: WU BO
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "dfa.h"
#include "BitSet.h"

struct DFA{
	int initial_state;
	int num_states;   //total number of states
	int ** table;     //2D array, array of array
	int *final_state; //array of bool(1 or 0). Index of array indicates state number
	//int num_final;
};

void allocate_table(DFA dfa){
	// allocate space for the dynamic 2d table
	dfa->table = (int**)malloc(sizeof(int*)*(dfa->num_states));


	for (int i=0; i < dfa->num_states; i++){
		// table[i] indicates the row of ith state
		dfa->table[i] = (int*)malloc(sizeof(int)*128);

		// initialize the default value(-1) of 2D table
		// since we don't use state -1, so -1 could be used to indicate non-exists transition
		for (int d=0; d<128; d++){
			dfa->table[i][d] = -1;
		}
	}
}
DFA new_DFA(int nstates){
	// update the total number of states
	DFA this = (DFA)malloc(sizeof(struct DFA));
	this->initial_state = 0;
	this->num_states = nstates;

	allocate_table(this);

	// set all states to be non-accepting by default
	this->final_state = (int*)malloc(sizeof(int)*(this->num_states));
	for (int i=0; i < nstates; i++){
		this->final_state[i] = 0;
	}

	return this;

}

void DFA_free(DFA dfa){
	// free table
	for (int i=0; i < dfa->num_states; i++){
		free(dfa->table[i]);
	}
	free(dfa->table);

	//free array of final states
	free(dfa->final_state);

	//free dfa
	free(dfa);
}

/**
 * Return the number of states in the given DFA.
 */
int DFA_get_size(DFA dfa){
	return dfa->num_states;
}

/**
 * Return the state specified by the given DFA's transition function from
 * state src on input symbol sym.
 */
int DFA_get_transition(DFA dfa, int src, char sym){
	return dfa->table[src][(int)sym];
}

/**
 * For the given DFA, set the transition from state src on input symbol
 * sym to be the state dst.
 */
void DFA_set_transition(DFA dfa, int src, char sym, int dst){
	dfa->table[src][(int)sym] = dst;
}

/**
 * Set the transitions of the given DFA for each symbol in the given str.
 * This is a nice shortcut when you have multiple labels on an edge between
 * two states.
 */
void DFA_set_transition_str(DFA dfa, int src, char *str, int dst){
	// sizeof(str) include the NULL character \0, so exclude it by -1
	for (int i=0; i<sizeof(str)-1; i++){
		dfa->table[src][(int)str[i]] = dst;
	}
}

/**
 * Set the transitions of the given DFA for all input symbols.
 * Another shortcut method.
 */
void DFA_set_transition_all(DFA dfa, int src, int dst){
	for (int i=0; i<128; i++){
		dfa->table[src][i] = dst;
	}
}

/**
 * Set whether the given DFA's state is accepting or not.
 */
void DFA_set_accepting(DFA dfa, int state, bool value){
	dfa->final_state[state]=value;
}

/**
 * Return true if the given DFA's state is an accepting state.
 */
bool DFA_get_accepting(DFA dfa, int state){
	if (dfa->final_state[state] == 0){
		return false;
	}
	return true;
}

/**
 * Run the given DFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
bool DFA_execute(DFA dfa, char *input){
	int current_state=dfa->initial_state;
	int index=0;

	// process input
	while (input[index] != '\0'){
		int nextState=dfa->table[current_state][(int)input[index]];
		if (nextState != -1){
			// valid transition, -1 indicates invalid transition
			current_state = nextState;
		}
		else {
			// invalid transition, reject
			return false;
		}
		index++;
	}

	// check acceptableness of the last state
	if (dfa->final_state[current_state]){
		return true;
	}
	else{
		return false;
	}
}

/**
 * Print the given DFA to System.out.
 */
void DFA_print(DFA dfa){
	for (int i=0; i < dfa->num_states; i++){
		printf("%d ", i);
		for (int p=0; p< 128; p++){
			if (dfa->table[i][p] != -1){
				printf("%c%d  ",p , dfa->table[i][p]);
			}
		}
		printf("\n");
	}
}


