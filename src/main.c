/*
 * main.c
 *
 *  Created on: 2020Äê1ÔÂ28ÈÕ
 *      Author: WU BO
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "dfa.h"
#include "nfa.h"

void DFA_user_input(DFA dfa){
	// prompt the input
	printf("Enter your input (\"quit\" is quit): ");
	fflush(stdout);

	char* input = malloc(sizeof(char)*128);
	scanf("%s", input);
	fflush(stdin);
	int equality=strcmp(input,"quit");

	while (equality != 0){
		//printf("go to the loop\n");
		if((int)DFA_execute(dfa, input)){
			printf("True\n");
			fflush(stdout);
		}
		else{
			printf("False\n");
			fflush(stdout);
		}
		free(input);
		input = malloc(sizeof(char)*128);

		printf("Enter your input (\"quit\" is quit): ");
		fflush(stdout);
		scanf("%s", input);
		fflush(stdin);

		equality=strcmp(input,"quit");
	}
	free(input);
}


void DFA_question1(){
	printf("\nTesting DFA that recognizes exactly the string \"computer\"\n");
	// initialize the dfa
	DFA dfa_test1 = new_DFA(9);

	// set the dfa
	DFA_set_transition(dfa_test1, 0, 'c', 1);
	DFA_set_transition(dfa_test1, 1, 'o', 2);
	DFA_set_transition(dfa_test1, 2, 'm', 3);
	DFA_set_transition(dfa_test1, 3, 'p', 4);
	DFA_set_transition(dfa_test1, 4, 'u', 5);
	DFA_set_transition(dfa_test1, 5, 't', 6);
	DFA_set_transition(dfa_test1, 6, 'e', 7);
	DFA_set_transition(dfa_test1, 7, 'r', 8);
	//set the final states
	DFA_set_accepting(dfa_test1, 8, true);

	// Prompt user input
	DFA_user_input(dfa_test1);

	DFA_free(dfa_test1);

}

void DFA_question2(){
    printf("Testing DFA that recognizes exactly the string that starts with \"rofl\"\n");
    fflush(stdout);
    // initialize the dfa
    DFA dfa_test2 = new_DFA(5);

    DFA_set_transition(dfa_test2, 0, 'r', 1);
    DFA_set_transition(dfa_test2, 1, 'o', 2);
    DFA_set_transition(dfa_test2, 2, 'f', 3);
    DFA_set_transition(dfa_test2, 3, 'l', 4);
    DFA_set_transition_all(dfa_test2, 4,  4);

    DFA_set_accepting(dfa_test2, 4, true);

    DFA_user_input(dfa_test2);

    DFA_free(dfa_test2);
}


void DFA_question3(){

    printf("Testing DFA that recognizes binary input with an odd number of 0\' s \n");
    fflush(stdout);
    // initialize the dfa
    DFA dfa_test3 = new_DFA(2);

    DFA_set_transition(dfa_test3, 0, '0', 1);
    DFA_set_transition(dfa_test3, 0, '1', 0);
    DFA_set_transition(dfa_test3, 1, '0', 0);
    DFA_set_transition(dfa_test3, 1, '1', 1);

    DFA_set_accepting(dfa_test3, 1, true);

    DFA_user_input(dfa_test3);

    DFA_free(dfa_test3);
}

void DFA_question4(){

    printf("Testing DFA that recognizes binary input with an odd number of both 0\' s and 1\' s \n");
    fflush(stdout);
    // initialize the dfa
    DFA dfa_test4 = new_DFA(4);

    DFA_set_transition(dfa_test4, 0, '1', 1);
    DFA_set_transition(dfa_test4, 0, '0', 2);
    DFA_set_transition(dfa_test4, 1, '1', 0);
    DFA_set_transition(dfa_test4, 1, '0', 3);
    DFA_set_transition(dfa_test4, 2, '1', 3);
    DFA_set_transition(dfa_test4, 2, '0', 0);
    DFA_set_transition(dfa_test4, 3, '1', 2);
    DFA_set_transition(dfa_test4, 3, '0', 1);

    DFA_set_accepting(dfa_test4, 3, true);

    DFA_user_input(dfa_test4);

    DFA_free(dfa_test4);
}

void DFA_question5(){

    printf("Testing DFA that recognizes strings that its length is even \n");
    fflush(stdout);
    // initialize the dfa
    DFA dfa_test5 = new_DFA(2);

    DFA_set_transition_all(dfa_test5, 0,1);
    DFA_set_transition_all(dfa_test5, 1,0);


    DFA_set_accepting(dfa_test5, 0, true);

    DFA_user_input(dfa_test5);

    DFA_free(dfa_test5);
}


// NFA process on the user input
void NFA_user_input(NFA nfa){
	// prompt the input
	printf("Enter your input (\"quit\" is quit): ");
	fflush(stdout);
	char* input = (char*)malloc(sizeof(char)*128);
	scanf("%s", input);
	fflush(stdin);
	int equality=strcmp(input,"quit");


	while (equality != 0){
		if((int)NFA_execute(nfa, input)){
			printf("True\n");
			fflush(stdout);
		}
		else{
			printf("False\n");
			fflush(stdout);
		}
		free(input);
		input = (char*)malloc(sizeof(char)*128);

		//prompt again
        printf("Enter your input (\"quit\" is quit): ");
        fflush(stdout);
		scanf("%s", input);
		fflush(stdin);
		equality=strcmp(input,"quit");

	}
	free(input);
}

void NFA_ui(NFA nfa){
    printf("\n");

    DFA converted_dfa= NFA_convert_DFA(nfa);
    printf("Success in converting NFA to DFA! Try new converted DFA!\n");
    DFA_user_input(converted_dfa);

    DFA_free(converted_dfa);

}

NFA NFA_question1(){
	printf("Testing NFA that recognize the strings ending in \"emb\". \n");
	NFA nfa_test1 = new_NFA(4);

	NFA_add_transition(nfa_test1, 0, 'e', 1);
	NFA_add_transition_all(nfa_test1, 0,  0);
	NFA_add_transition(nfa_test1, 1, 'm', 2);
	NFA_add_transition(nfa_test1, 2, 'b', 3);
	NFA_add_transition_all(nfa_test1, 3,  0);

	NFA_set_accepting(nfa_test1, 3, true);


	NFA_user_input(nfa_test1);

	return  nfa_test1;
}

NFA NFA_question2(){
    printf("Testing NFA that recognize the strings containing in \"emb\". \n");
    NFA nfa_test2 = new_NFA(4);

    NFA_add_transition(nfa_test2, 0, 'e', 1);
    NFA_add_transition_all(nfa_test2, 0, 0);
    NFA_add_transition(nfa_test2, 1, 'm', 2);
    NFA_add_transition(nfa_test2, 2, 'b', 3);
    NFA_add_transition_all(nfa_test2, 3, 0);
    NFA_add_transition_all(nfa_test2, 3, 3);

    NFA_set_accepting(nfa_test2, 3, true);


    NFA_user_input(nfa_test2);

    return  nfa_test2;
}

void NFA_question3(){
    printf("Testing NFA that accept strings with more than one a,g,h,i,o,s,t,or w, or more than two n\'s \n");
    NFA nfa_test3 = new_NFA(20);

    char a[] ={'w','a','s','h','i','g','t','o',
               'W','A','S','H','I','G','T','O'};

    NFA_add_transition_all(nfa_test3, 0, 0);
    for (int i=1; i<=8; i++){
        NFA_add_transition(nfa_test3, 0, a[i-1], i*2-1);
        NFA_add_transition(nfa_test3, 0, a[i-1+8], i*2-1);

        NFA_add_transition_all(nfa_test3, i*2-1, i*2-1);

        NFA_add_transition(nfa_test3, i*2-1, a[i-1+8], i*2);
        NFA_add_transition(nfa_test3, i*2-1, a[i-1], i*2);

        NFA_add_transition_all(nfa_test3, i*2, i*2);

        NFA_set_accepting(nfa_test3, i*2,true);
    };

    // set states for 'n'
    char b[]="nN";
    NFA_add_transition_str(nfa_test3, 0, b, 17);
    NFA_add_transition_all(nfa_test3, 17, 17);

    NFA_add_transition_str(nfa_test3, 17, b, 18);
    NFA_add_transition_all(nfa_test3, 18, 18);

    NFA_add_transition_str(nfa_test3, 18, b, 19);
    NFA_add_transition_all(nfa_test3, 19, 19);

    NFA_set_accepting(nfa_test3, 19, true);


	// Prompt user input
    NFA_user_input(nfa_test3);

    NFA_free(nfa_test3);
}

void NFA_question4(){
    printf("\"Testing NFA that recognizes string that the third input from the right end is 1. \n");
    NFA nfa_test4 = new_NFA(4);


    NFA_add_transition_all(nfa_test4, 0, 0);
    NFA_add_transition_all(nfa_test4, 1, 2);
    NFA_add_transition_all(nfa_test4, 2, 3);
    NFA_add_transition(nfa_test4, 0, '1', 1);

    NFA_set_accepting(nfa_test4, 3, true);


    NFA_user_input(nfa_test4);

    NFA_free(nfa_test4);
}



int main(int argc, char *argv[]){

	printf("CSC173 Project 1 by Bo Wu and Ziyue Qiu \n");

	// start test DFA
	DFA_question1();
	printf("\n");
	DFA_question2();
	printf("\n");
    DFA_question3();
    printf("\n");
    DFA_question4();
    printf("\n");
    DFA_question5();
    printf("\n");

    // start test NFA
    NFA_ui(NFA_question1());
    printf("\n");
    NFA_ui(NFA_question2());
    printf("\n");

    NFA_question3();
    printf("\n");
    NFA_question4();
    //printf("\n");


	return 0;
}
