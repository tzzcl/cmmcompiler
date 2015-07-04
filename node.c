#include "node.h"
Node* create_node(char* s){
	Node* cnt=(Node*)malloc(sizeof(Node));
	memset(cnt,0,sizeof(Node));
	strcpy(cnt->content,s);
	cnt->id=0;cnt->number=0;
	cnt->line=yylloc.first_line;
	cnt->isterm=0;
	cnt->isvisited=0;
	return cnt;
}
Node* create_node_term(char* s){
	Node* cnt=(Node*)malloc(sizeof(Node));
	memset(cnt,0,sizeof(Node));
	strcpy(cnt->content,s);
	cnt->id=0;cnt->number=0;
	cnt->line=yylloc.first_line;
	cnt->isterm=1;
	return cnt;
}
Node* create_node_ID(char* s){
	Node* cnt=(Node*)malloc(sizeof(Node));
	memset(cnt,0,sizeof(Node));
	strcpy(cnt->content,"ID: ");
	strcpy(cnt->content+4,s);
	cnt->id=1;cnt->number=0;
	cnt->line=0;
	cnt->isterm=0;
	return cnt;
}
Node* create_node_type(char* s,char* now){
	Node* cnt=(Node*)malloc(sizeof(Node));
	memset(cnt,0,sizeof(Node));
	strcpy(cnt->content,"TYPE: ");
	strcpy(cnt->content+6,now);
	cnt->id=0;cnt->number=0;
	cnt->line=0;
	cnt->isterm=0;
	return cnt;
}
Node* create_node_int(int now){
	Node* cnt=(Node*)malloc(sizeof(Node));
	memset(cnt,0,sizeof(Node));
	strcpy(cnt->content,"INT: ");
	cnt->id=0;cnt->number=1;
	cnt->type_int=now;
	cnt->line=0;
	cnt->isterm=0;
	return cnt;
}
Node* create_node_float(float now){
	Node* cnt=(Node*)malloc(sizeof(Node));
	memset(cnt,0,sizeof(Node));
	strcpy(cnt->content,"FLOAT: ");
	cnt->id=0;cnt->number=2;
	cnt->type_float=now;
	cnt->line=0;
	cnt->isterm=0;
	return cnt;
}
void insert_node(Node* father,Node* son){
	//printf("%p %p\n",father,son);
	if (son==NULL) return ;
	father->next[father->cnt++]=son;
	if (son->line!=0&&father->line>son->line) father->line=son->line;
}


