/*
 * nfa.c
 *
 *  Created on: Feb 3, 2020
 *      Author: WU BO
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "nfa.h"
#include "BitSet.h"
#include "dfa.h"


// In NFA, we consider the state could be a set of states
struct NFA {
	int initial_state;	 //we always start from state 0, so we could use 0
	int num_states;      //total number of states
	BitSet** table;		 //2D array, array of array that contains BitSets
	int* final_states;   //array of final(accepting)_states
	// finite input 0-127

};

/**
 * Allocate and return a new NFA containing the given number of states.
 */
 NFA new_NFA(int nstates){
	 // update the total number of states
	 NFA this = (NFA)malloc(sizeof(struct NFA));
	 this->initial_state = 0;
	 this->num_states = nstates;

	 //allocate space for the dynamic 2d table
	 this->table = (BitSet**)malloc(sizeof(BitSet*)*(this->num_states));

	 for (int i = 0; i< nstates; i++){
		 // table[i] indicates the ith row, ith state
		 this->table[i] = (BitSet*)malloc(sizeof(BitSet)*128);

		 // initialize the default empty BitSet in 2D table
		 for (int d = 0; d < 128; d++){
			 this->table[i][d] = new_BitSet();
		 }
	 }

	 // 0 indicates false. Index of array indicates the state number.
	 // By default, set all states to be non_accepting
	 this->final_states = (int*)malloc(sizeof(int)*(this->num_states));
	 for (int i=0; i<this->num_states; i++){
		 this->final_states[i] = 0;
	 }
	 return this;
 }

/**
 * Free the given NFA.
 */
void NFA_free(NFA nfa){
	//free table
	for (int i=0; i < nfa->num_states; i++){
		for (int d=0; d< 128; d++){
			BitSet_free(nfa->table[i][d]);
		}
		free(nfa->table[i]);
	}
	free(nfa->table);

	//free array of final states
	free(nfa->final_states);

	free(nfa);
}

/**
 * Return the number of states in the given NFA.
 */
int NFA_get_size(NFA nfa){
	return nfa->num_states;
}

/**
 * Return the set of next states specified by the given NFA's transition
 * function from the given state on input symbol sym.
 */
BitSet NFA_get_transitions(NFA nfa, int state, char sym){
	return nfa->table[state][(int)sym];
}

/**
 * For the given NFA, add the state dst to the set of next states from
 * state src on input symbol sym.
 */
void NFA_add_transition(NFA nfa, int src, char sym, int dst){
	BitSet_insert(nfa->table[src][(int)sym], dst);
}

/**
 * Add a transition for the given NFA for each symbol in the given str.
 */
void NFA_add_transition_str(NFA nfa, int src, char *str, int dst){
	// sizeof(str) include the NULL character \0, so we exclude it by -1
	for (int i=0; i < sizeof(str) -1; i++){
		BitSet_insert(nfa->table[src][(int)str[i]], dst);
	}
}

/**
 * Add a transition for the given NFA for each input symbol.
 */
void NFA_add_transition_all(NFA nfa, int src, int dst){
	for (int i=0; i<128; i++){
		BitSet_insert(nfa->table[src][i], dst);
	}
}

/**
 * Set whether the given NFA's state is accepting or not.
 */
void NFA_set_accepting(NFA nfa, int state, bool value){
	if ((int)value){
		nfa->final_states[state] = 1;

	}
	else {
		nfa->final_states[state] = 0;
	}
}

/**
 * Return true if the given NFA's state is an accepting state.
 */
bool NFA_get_accepting(NFA nfa, int state){
	if (nfa->final_states[state]){
		return true;
	}
	else {
		return false;
	}
}

/**
 * Run the given NFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
bool NFA_execute(NFA nfa, char *input){
	// initialize BitSet that contains only the intial_state 0;
	BitSet current_state = new_BitSet();
	BitSet_insert(current_state, nfa->initial_state);

	// process input
	int index = 0;
	while (input[index] != '\0'){
		BitSet next_states = new_BitSet();
		BitSetIterator iterator= BitSet_iterator(current_state);

		// loop through all possible next state based on current_state
		while ((int)BitSetIterator_hasNext(iterator)){
			int state_in_current_set = BitSetIterator_next(iterator);

			// On my machine, it only works until 31
			BitSet_union(next_states, nfa->table[state_in_current_set][(int)input[index]]);
		}
		//printf("next_states: %s\n",BitSet_toString(next_states));


		// free current_state because I will not use it in this loop
		BitSet_free(current_state);
		current_state = new_BitSet();


		// check BitSet nextState
		if ((int)BitSet_isEmpty(next_states)){
			// invalid transition
			return false;
		}
		else {
			// copy states in next states into current
			BitSetIterator iterator_next = BitSet_iterator(next_states);
			while ((int)BitSetIterator_hasNext(iterator_next)){
				int state_in_nextState = BitSetIterator_next(iterator_next);
				BitSet_insert(current_state, state_in_nextState);
			}
			free(iterator_next);
		}

		BitSet_free(next_states);
		free(iterator);
		index++;
	}

	// check acceptableness of the last states
	BitSetIterator iterator = BitSet_iterator(current_state);
	while ((int)BitSetIterator_hasNext(iterator)){
		int end_state = BitSetIterator_next(iterator);
		if (nfa->final_states[end_state]){
			return true;
		}
	}
	free(iterator);
	return false;
}

/**
 * Print the given NFA to System.out.
 */
void NFA_print(NFA nfa){
	for (int i=0; i< nfa->num_states; i++){
		for (int p=0; p<128; p++){
			if (!BitSet_isEmpty(nfa->table[i][p])){
				// loop through the BitSet
				BitSetIterator iterator = BitSet_iterator(nfa->table[i][p]);
				printf("{");
				while ( BitSetIterator_hasNext(iterator) ){
					printf("%d,", BitSetIterator_next(iterator));
				}
				printf("} ");
			}
		}
		printf("\n");
	}
}

/**
 * Convert a NFA to DFA
 *
 */
DFA NFA_convert_DFA(NFA nfa){
	//create the DFA
	DFA dfa = new_DFA((int)pow(2,nfa->num_states));

	// set of states that has been discovered, maximum states of new_DFA is 2^(num_states)
	BitSet* discovered_states = (BitSet*)malloc(sizeof(BitSet)*pow(2,nfa->num_states));
	for (int i=0; i<pow(2,nfa->num_states); i++){
		discovered_states[i]=new_BitSet();
	}

	// set start state in discovered_states
	BitSet_insert(discovered_states[0], nfa->initial_state);
	int number_state = 1;	// start from 1 because {0} is the start set
	int current_state_index=0;
	// check acceptance of start state
	if (NFA_get_accepting(nfa,nfa->initial_state)){
		DFA_set_accepting(dfa, current_state_index, true);
	}


	while (current_state_index < number_state){
		BitSet current_states = discovered_states[current_state_index];

		//check each possible label
		for (int i=0; i<128; i++) {
			BitSet union_set = new_BitSet();

			BitSetIterator iterator = BitSet_iterator(current_states);
			// find next set_state at current char(i) and current_states
			while (BitSetIterator_hasNext(iterator) ){
				int state = BitSetIterator_next(iterator);
				BitSet_union(union_set, nfa->table[state][i]);
			}
			// free iterator on current label (128 possible labels)
			free(iterator);

			// if it is empty, then done, if not, go to condition
			if (!BitSet_isEmpty(union_set)){
				//check existence of next set_state in discovered_states
				bool new_set= true;
				for (int d=0; (d<number_state) & ((int)new_set); d++ ){
					// if it is already in the discovered_states
					if ( BitSet_equals(union_set, discovered_states[d]) ){
						new_set=false;
						DFA_set_transition(dfa, current_state_index, (char)i, d);
					}
				}

				// if it is a new set of states
				if ((int)new_set){
					BitSet_union(discovered_states[number_state], union_set);
					DFA_set_transition(dfa, current_state_index, (char)i, number_state);

					// check if the new set is a accepting state

					BitSetIterator iterator_new_set = BitSet_iterator(discovered_states[number_state]);
					while (BitSetIterator_hasNext(iterator_new_set)){

						int state = BitSetIterator_next(iterator_new_set);
						if (NFA_get_accepting(nfa, state)){
							DFA_set_accepting(dfa,number_state,true);
						}
					}
					free(iterator_new_set);
					number_state++;
				}
			}
			BitSet_free(union_set);
		}

		// finish loop through current state
		current_state_index++;
	}

	// free discovered_states
	for (int i=0; i < pow(2,nfa->num_states); i++){
		BitSet_free(discovered_states[i]);
	}
	printf("New DFA has total number of states: %d \n", number_state);
	free(discovered_states);
	NFA_free(nfa);
	return dfa;
}


