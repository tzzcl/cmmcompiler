#ifndef _NODE_H_
#define _NODE_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "syntax.tab.h"
struct Node{
	char content[32];
	int id;
	int number;
	int cnt;
	int line;
	int isterm;
	int isvisited;
	union{
		int type_int;
		float type_float;
		char relop[3];
	};
	struct Node* next[20];
};
typedef struct Node Node;
Node* create_node(char* s);
Node* create_node_term(char* s);
Node* create_node_ID(char* s);
Node* create_node_type(char* s,char* now);
Node* create_node_int(int now);
Node* create_node_float(float now);
void insert_node(Node* father,Node* son);

#endif
