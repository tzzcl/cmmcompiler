#ifndef _IR_H_
#define _IR_H_
#include "list.h"
#include "node.h"
typedef struct Operand_* Operand;
struct Operand_ {
	enum {CONSTANT,SIZE,ADDRESS,VARIABLE,TEMP,LABEL,FUNCTION} kind;
	char name[32];
	union{
		int var_no;
		int int_value;
		float float_value;
		char funname[32];
	}u;
	ListHead arg_list;
};
typedef struct InterCode_* InterCode;
struct InterCode_{
	enum {DEF_LABEL_I,DEF_FUN_I,GOTO_I,RETURN_I,ARG_I,PARAM_I,READ_I,WRITE_I,ASSIGN_I,ASSIGN_ADDR_I,ASSIGN_MEM_I,MEM_ASSIGN_I,
	DEC_SIZE_I,ASSIGN_CALL_I,PLUS_I,MINUS_I,STAR_I,DIV_I,COND_I} kind;
	union {
		Operand one_op;
		struct {Operand op1,op2;} two_op;
		struct {Operand op1,op2,op3;} three_op;
		struct {Operand op1,op2,op3;char relop[3];} if_op;
	}u;
	int code_index;
	ListHead list;
};

Operand new_op();
InterCode new_intercode();
void translate(Node*);
void print_intercode(FILE*);

#endif 
