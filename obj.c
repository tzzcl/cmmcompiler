#include "node.h"
#include "list.h"
#include "ir.h"

extern ListHead code;
extern int temp_total;
extern int label_total;
int space;
char args[1000][32];
int arg_number=0;
char params[1000][100][32];
char funcname[1000][80];
int funcnumber[1000];
int param_number[1000];
int function_cnt=0;
char nowfunction[80];
int temp_array[1000];
void dec_init(){
	ListHead* ptr;
	list_foreach(ptr,&code)
	{
		InterCode temp=list_entry(ptr,struct InterCode_,list);
		if (temp->kind==DEC_SIZE_I)
		{
			int now=atoi(temp->u.two_op.op1->name+1);
			int size=atoi(temp->u.two_op.op2->name+1);
			temp_array[now]=size;
		}
	}
}
int find_function(char* name)
{
	for (int i=0;i<function_cnt;i++)
	{
		if (strcmp(funcname[i],name)==0)
			return i;
	}
	assert(0);
	return -1;
}
void param_init(){
	ListHead* ptr;
	list_foreach(ptr,&code)
	{
		InterCode temp=list_entry(ptr,struct InterCode_,list);
		if (temp->kind==DEF_FUN_I)
		{
			strcpy(funcname[function_cnt],temp->u.one_op->u.funname);
			ptr=ptr->next;
			InterCode param=list_entry(ptr,struct InterCode_,list);
			while (param->kind==PARAM_I)
			{
				strcpy(params[function_cnt][param_number[function_cnt]],param->u.one_op->name);
				param_number[function_cnt]++;
				ptr=ptr->next;
				param=list_entry(ptr,struct InterCode_,list);
			}
			function_cnt++;
		}
	}
}
void variable_init(FILE* file){
	int i=0;
	for (i=0;i<temp_total;i++)
	{
		char name[10];memset(name,0,sizeof(name));
		sprintf(name,"t%d",i);
		if (temp_array[i]==0){
			fprintf(file, "%s:.%s %d\n",name,"word",0);
		}
		else
		{
			fprintf(file, "%s:.space %d\n",name,temp_array[i]);
		}
	}
	fflush(file);
}
void obj_init(FILE* file){
	fprintf(file, "%s\n", ".data");
	fprintf(file, "%s\n", "_prompt: .asciiz \"Enter an integer:\"");
	fprintf(file, "%s\n", "_ret: .asciiz \"\\n\"");
	variable_init(file);
	fprintf(file, "%s\n", ".globl main");
	fprintf(file, "%s\n", ".text");
	fprintf(file, "%s\n", "read:");
	fprintf(file, "%s\n", "\tli $v0, 4");
	fprintf(file,"la $a0, _prompt\n");
	fprintf(file, "%s\n", "\tsyscall");
	fprintf(file, "%s\n", "\tli $v0, 5");
	fprintf(file, "%s\n", "\tsyscall");
	fprintf(file, "%s\n", "\tjr $ra");
	fprintf(file, "%s\n", "");
	fprintf(file, "%s\n", "write:");
	fprintf(file, "%s\n", "\tli $v0, 1");
	fprintf(file, "%s\n", "\tsyscall");
	fprintf(file, "%s\n", "\tli $v0, 4");
	fprintf(file, "%s\n", "\tla $a0, _ret");
	fprintf(file, "%s\n", "\tsyscall");
	fprintf(file, "%s\n", "\tmove $v0, $0");
	fprintf(file, "%s\n", "\tjr $ra");
	fprintf(file, "%s\n", "");
	fflush(file);
}
void tran_inter(InterCode intercode,FILE* file)
{
	switch (intercode->kind)
	{
		case DEF_LABEL_I:{
			fprintf(file,"%s:\n",intercode->u.one_op->name);
			break;
		}
		case DEF_FUN_I:{
			if (strcmp(intercode->u.one_op->u.funname,"main")==0)
			{
				fprintf(file,"%s:\n",intercode->u.one_op->u.funname);
			}
			else fprintf(file,"_%s:\n",intercode->u.one_op->u.funname);
			memset(nowfunction,0,sizeof(nowfunction));
			strcpy(nowfunction,intercode->u.one_op->u.funname);
			space=4;
			break;
		}
		case GOTO_I:{
			fprintf(file,"j %s\n",intercode->u.one_op->name);
			break;
		}
		case RETURN_I:{
			if (intercode->u.one_op->name[0]=='#')
			{
				fprintf(file, "li $v0, %d\n",intercode->u.one_op->u.int_value);
				fprintf(file, "jr $ra\n");
				break;
			}
			else
			{
				fprintf(file, "la $s1, %s\n",intercode->u.one_op->name);
				fprintf(file, "lw $s0,0($s1)\n");
			}
			fprintf(file, "move $v0, $s0\n");
			fprintf(file, "jr $ra\n");
			break;
		}
		case ARG_I:{
			memset(args[arg_number],0,sizeof(char)*32);
			strcpy(args[arg_number],intercode->u.one_op->name);
			arg_number++;
			//fprintf(file,"ARG %s\n",intercode->u.one_op->name);
			break;
		}
		case PARAM_I:{
			// to do
			ListHead* p1;
			int cnt=0;
			int flag=0;
			list_foreach(p1,&intercode->list)
			{
				if (cnt>=1) break;
				InterCode temp=list_entry(p1,struct InterCode_,list);
				if (temp->kind!=PARAM_I)
				{
					flag=1;
				}
				cnt++;
			}
			if (flag)
			{
				int offset=0;
				int nowfun=find_function(nowfunction);
				for (int i=param_number[nowfun]-1;i>=0;i--)
				{
					fprintf(file, "lw $s0, %d($sp)\n",offset);
					fprintf(file, "la $s1, %s\n",params[nowfun][i]);
					fprintf(file, "sw $s0, 0($s1)\n");
					offset+=4;
				}
			}
			//fprintf(file,"PARAM %s\n",intercode->u.one_op->name);
			break;
		}
		case READ_I:{
			fprintf(file,"addi $sp, $sp, -4\n");
			fprintf(file, "sw $ra, 0($sp)\n");
			fprintf(file, "jal read\n");
			fprintf(file, "lw $ra, 0($sp)\n");
			fprintf(file, "addi $sp, $sp, 4\n");
			fprintf(file, "la $v1, %s\n",intercode->u.one_op->name);
			fprintf(file, "sw $v0, 0($v1)\n");
			//fprintf(file,"READ %s\n",intercode->u.one_op->name);
			break;
		}
		case WRITE_I:{
			if (intercode->u.one_op->name[0]=='#')
			{
				fprintf(file, "li $a0, %d\n",intercode->u.one_op->u.int_value);
			}
			else
			{
				fprintf(file, "la $s1, %s\n",intercode->u.one_op->name);
				fprintf(file, "lw $a0 0($s1)\n");
			}
			fprintf(file,"addi $sp, $sp, -4\n");
			fprintf(file, "sw $ra, 0($sp)\n");
			fprintf(file, "jal write\n");
			fprintf(file, "lw $ra, 0($sp)\n");
			fprintf(file, "addi $sp, $sp, 4\n");
			break;
		}
		case ASSIGN_I:{
			if (intercode->u.two_op.op2->name[0]=='#')
			{
				fprintf(file, "li $s0, %d\n",intercode->u.two_op.op2->u.int_value);
			}
			else
			{
				fprintf(file, "la $s1, %s\n",intercode->u.two_op.op2->name);
				fprintf(file, "lw $s0 0($s1)\n");
			}
			fprintf(file, "la $s1, %s\n",intercode->u.two_op.op1->name);
			fprintf(file, "sw $s0 0($s1)\n");
			//fprintf(file,"%s := %s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name);
			break;
		}
		case ASSIGN_ADDR_I:{
			//to do [array]
			fprintf(file, "la $s0, %s\n",intercode->u.two_op.op2->name);
			fprintf(file, "la $s1, %s\n",intercode->u.two_op.op1->name);
			fprintf(file, "sw $s0 0($s1)\n");
			//fprintf(file,"%s := &%s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name);
			break;
		}
		case ASSIGN_MEM_I:{
			//to do [array]
			fprintf(file, "la $s4, %s\n",intercode->u.two_op.op2->name);
			fprintf(file, "lw $s3, 0($s4)\n");
			fprintf(file, "la $s2, 0($s3)\n");
			fprintf(file, "lw $s1, 0($s2)\n");
			fprintf(file, "la $s0, %s\n",intercode->u.two_op.op1->name);
			fprintf(file, "sw $s1, 0($s0)\n");
			//fprintf(file,"%s := *%s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name);
			break;
		}
		case MEM_ASSIGN_I:{
			//to do [array]
			if (intercode->u.two_op.op2->name[0]=='#')
			{
				fprintf(file, "li $s5, %d\n",intercode->u.two_op.op2->u.int_value);
			}
			else
			{
				fprintf(file, "la $s6, %s\n",intercode->u.two_op.op2->name);
				fprintf(file, "lw $s5 0($s6)\n");
			}
			fprintf(file, "la $s4, %s\n",intercode->u.two_op.op1->name);
			fprintf(file, "lw $s3, 0($s4)\n");
			fprintf(file, "la $s1, 0($s3)\n");
			fprintf(file, "sw $s5, 0($s1)\n");
			//fprintf(file,"*%s := %s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name);
			break;
		}
		case DEC_SIZE_I:{
			//to do [array]
			//fprintf(file,"DEC %s %s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name+1);
			break;
		}
		case ASSIGN_CALL_I:{
			int offset=0;
			int nowfun=find_function(intercode->u.two_op.op2->name);
			int param_num=param_number[nowfun];
			offset=offset-param_num*4;
			fprintf(file,"addi $sp, $sp, %d\n",offset);
			int cnt=0;
			int i=0;
			//offset=offset- arg_number*4;
			for (i=param_num-1;i>=0;i--)
			{
				fprintf(file, "la $s0, %s\n",params[nowfun][i]);
				fprintf(file, "lw $s1, 0($s0)\n");
				fprintf(file, "sw $s1, %d($sp)\n",cnt);
				cnt+=4;
			}
			fprintf(file,"addi $sp, $sp, %d\n",-4-arg_number*4);
			cnt=0;
			i=0;
			for (i=arg_number-1;i>=0;i--)
			{
				if (args[i][0]=='#')
				{
					fprintf(file, "li $s1, %s\n", args[i]+1);
					fprintf(file, "sw $s1, %d($sp)\n",cnt);
				}
				else
				{
					fprintf(file, "la $s0, %s\n",args[i]);
					fprintf(file, "lw $s1, 0($s0)\n");
					fprintf(file, "sw $s1, %d($sp)\n",cnt);
				}
				
				cnt+=4;
			}
			fprintf(file, "sw $ra, %d($sp)\n",cnt);
			//fprintf(file,"addi $sp, $sp, %d\n",offset+4);
			fprintf(file, "jal _%s\n",intercode->u.two_op.op2->name);
			fprintf(file, "lw $ra, %d($sp)\n",cnt);
			fprintf(file, "addi $sp, $sp, %d\n",cnt+4);
			cnt=0;
			for (i=param_num-1;i>=0;i--)
			{
				fprintf(file, "la $s0, %s\n",params[nowfun][i]);
				fprintf(file, "lw $s1, %d($sp)\n",cnt);
				fprintf(file, "sw $s1, 0($s0)\n");
				cnt+=4;
			}
			fprintf(file,"addi $sp, $sp, %d\n",-offset);
			fprintf(file, "la $s1,%s\n", intercode->u.two_op.op1->name);
			fprintf(file, "sw $v0 0($s1)\n");
			arg_number=0;
			//fprintf(file,"%s := CALL %s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name);
			break;
		}
		case PLUS_I:{
			if (intercode->u.three_op.op2->name[0]=='#')
			{
				fprintf(file, "li $s0, %d\n",intercode->u.three_op.op2->u.int_value);
			}
			else if (intercode->u.three_op.op2->name[0]=='&')
			{
				fprintf(file, "la $s0, %s\n",intercode->u.three_op.op2->name+1);
			}
			else
			{
				fprintf(file, "la $s1, %s\n",intercode->u.three_op.op2->name);
				fprintf(file, "lw $s0 0($s1)\n");
			}
			if (intercode->u.three_op.op3->name[0]=='#')
			{
				fprintf(file, "li $s1, %d\n",intercode->u.three_op.op3->u.int_value);
			}
			else if (intercode->u.three_op.op3->name[0]=='&')
			{
				fprintf(file, "la $s0, %s\n",intercode->u.three_op.op3->name+1);
			}
			else
			{
				fprintf(file, "la $s2, %s\n",intercode->u.three_op.op3->name);
				fprintf(file, "lw $s1 0($s2)\n");
			}
			fprintf(file, "la $s4, %s\n",intercode->u.three_op.op1->name);
			fprintf(file, "add $s3 $s0 $s1\n");
			fprintf(file, "sw $s3 0($s4)\n");
			//fprintf(file,"%s := %s + %s\n",intercode->u.three_op.op1->name,intercode->u.three_op.op2->name,intercode->u.three_op.op3->name);
			break;
		}
		case MINUS_I:{
			if (intercode->u.three_op.op2->name[0]=='#')
			{
				fprintf(file, "li $s0, %d\n",intercode->u.three_op.op2->u.int_value);
			}
			else if (intercode->u.three_op.op2->name[0]=='&')
			{
				fprintf(file, "la $s0, %s\n",intercode->u.three_op.op2->name+1);
			}
			else
			{
				fprintf(file, "la $s1, %s\n",intercode->u.three_op.op2->name);
				fprintf(file, "lw $s0 0($s1)\n");
			}
			if (intercode->u.three_op.op3->name[0]=='#')
			{
				fprintf(file, "li $s1, %d\n",intercode->u.three_op.op3->u.int_value);
			}
			else if (intercode->u.three_op.op3->name[0]=='&')
			{
				fprintf(file, "la $s0, %s\n",intercode->u.three_op.op3->name+1);
			}
			else
			{
				fprintf(file, "la $s2, %s\n",intercode->u.three_op.op3->name);
				fprintf(file, "lw $s1 0($s2)\n");
			}
			fprintf(file, "la $s4, %s\n",intercode->u.three_op.op1->name);
			fprintf(file, "sub $s3 $s0 $s1\n");
			fprintf(file, "sw $s3 0($s4)\n");
			//fprintf(file,"%s := %s - %s\n",intercode->u.three_op.op1->name,intercode->u.three_op.op2->name,intercode->u.three_op.op3->name);
			break;
		}
		case STAR_I:{
			if (intercode->u.three_op.op2->name[0]=='#')
			{
				fprintf(file, "li $s0, %d\n",intercode->u.three_op.op2->u.int_value);
			}
			else if (intercode->u.three_op.op2->name[0]=='&')
			{
				fprintf(file, "la $s0, %s\n",intercode->u.three_op.op2->name+1);
			}
			else
			{
				fprintf(file, "la $s1, %s\n",intercode->u.three_op.op2->name);
				fprintf(file, "lw $s0 0($s1)\n");
			}
			if (intercode->u.three_op.op3->name[0]=='#')
			{
				fprintf(file, "li $s1, %d\n",intercode->u.three_op.op3->u.int_value);
			}
			else if (intercode->u.three_op.op3->name[0]=='&')
			{
				fprintf(file, "la $s0, %s\n",intercode->u.three_op.op3->name+1);
			}
			else
			{
				fprintf(file, "la $s2, %s\n",intercode->u.three_op.op3->name);
				fprintf(file, "lw $s1 0($s2)\n");
			}
			fprintf(file, "la $s4, %s\n",intercode->u.three_op.op1->name);
			fprintf(file, "mul $s3 $s0 $s1\n");
			fprintf(file, "sw $s3 0($s4)\n");
			//fprintf(file,"%s := %s * %s\n",intercode->u.three_op.op1->name,intercode->u.three_op.op2->name,intercode->u.three_op.op3->name);
			break;
		}
		case DIV_I:{
			if (intercode->u.three_op.op2->name[0]=='#')
			{
				fprintf(file, "li $s0, %d\n",intercode->u.three_op.op2->u.int_value);
			}
			else if (intercode->u.three_op.op2->name[0]=='&')
			{
				fprintf(file, "la $s0, %s\n",intercode->u.three_op.op2->name+1);
			}
			else
			{
				fprintf(file, "la $s1, %s\n",intercode->u.three_op.op2->name);
				fprintf(file, "lw $s0 0($s1)\n");
			}
			if (intercode->u.three_op.op3->name[0]=='#')
			{
				fprintf(file, "li $s1, %d\n",intercode->u.three_op.op3->u.int_value);
			}
			else if (intercode->u.three_op.op3->name[0]=='&')
			{
				fprintf(file, "la $s0, %s\n",intercode->u.three_op.op3->name+1);
			}
			else
			{
				fprintf(file, "la $s2, %s\n",intercode->u.three_op.op3->name);
				fprintf(file, "lw $s1 0($s2)\n");
			}
			fprintf(file, "la $s4, %s\n",intercode->u.three_op.op1->name);
			fprintf(file, "div $s0 $s1\n");
			fprintf(file, "mflo $s3\n");
			fprintf(file, "sw $s3 0($s4)\n");
			//fprintf(file,"%s := %s / %s\n",intercode->u.three_op.op1->name,intercode->u.three_op.op2->name,intercode->u.three_op.op3->name);
			break;
		}
		case COND_I:{
			//fprintf(file,"IF %x GOTO ")
			if (strcmp(intercode->u.if_op.relop,"==")==0)
			{
				if (intercode->u.if_op.op1->name[0]=='#')
				{
					fprintf(file, "li $s0, %d\n",intercode->u.if_op.op1->u.int_value);
				}
				else
				{
					fprintf(file, "la $s1, %s\n",intercode->u.if_op.op1->name);
					fprintf(file, "lw $s0 0($s1)\n");
				}
				if (intercode->u.if_op.op2->name[0]=='#')
				{
					fprintf(file, "li $s1, %d\n",intercode->u.if_op.op2->u.int_value);
				}
				else
				{
					fprintf(file, "la $s2, %s\n",intercode->u.if_op.op2->name);
					fprintf(file, "lw $s1 0($s2)\n");
				}
				fprintf(file, "beq $s0,$s1,%s\n",intercode->u.if_op.op3->name);				
			}
			else if (strcmp(intercode->u.if_op.relop,"!=")==0)
			{
				if (intercode->u.if_op.op1->name[0]=='#')
				{
					fprintf(file, "li $s0, %d\n",intercode->u.if_op.op1->u.int_value);
				}
				else
				{
					fprintf(file, "la $s1, %s\n",intercode->u.if_op.op1->name);
					fprintf(file, "lw $s0 0($s1)\n");
				}
				if (intercode->u.if_op.op2->name[0]=='#')
				{
					fprintf(file, "li $s1, %d\n",intercode->u.if_op.op2->u.int_value);
				}
				else
				{
					fprintf(file, "la $s2, %s\n",intercode->u.if_op.op2->name);
					fprintf(file, "lw $s1 0($s2)\n");
				}
				fprintf(file, "bne $s0,$s1,%s\n",intercode->u.if_op.op3->name);	
			}
			else if (strcmp(intercode->u.if_op.relop,">")==0)
			{
				if (intercode->u.if_op.op1->name[0]=='#')
				{
					fprintf(file, "li $s0, %d\n",intercode->u.if_op.op1->u.int_value);
				}
				else
				{
					fprintf(file, "la $s1, %s\n",intercode->u.if_op.op1->name);
					fprintf(file, "lw $s0 0($s1)\n");
				}
				if (intercode->u.if_op.op2->name[0]=='#')
				{
					fprintf(file, "li $s1, %d\n",intercode->u.if_op.op2->u.int_value);
				}
				else
				{
					fprintf(file, "la $s2, %s\n",intercode->u.if_op.op2->name);
					fprintf(file, "lw $s1 0($s2)\n");
				}
				fprintf(file, "bgt $s0,$s1,%s\n",intercode->u.if_op.op3->name);
			}
			else if (strcmp(intercode->u.if_op.relop,"<")==0)
			{
				if (intercode->u.if_op.op1->name[0]=='#')
				{
					fprintf(file, "li $s0, %d\n",intercode->u.if_op.op1->u.int_value);
				}
				else
				{
					fprintf(file, "la $s1, %s\n",intercode->u.if_op.op1->name);
					fprintf(file, "lw $s0 0($s1)\n");
				}
				if (intercode->u.if_op.op2->name[0]=='#')
				{
					fprintf(file, "li $s1, %d\n",intercode->u.if_op.op2->u.int_value);
				}
				else
				{
					fprintf(file, "la $s2, %s\n",intercode->u.if_op.op2->name);
					fprintf(file, "lw $s1 0($s2)\n");
				}
				fprintf(file, "blt $s0,$s1,%s\n",intercode->u.if_op.op3->name);
			}
			else if (strcmp(intercode->u.if_op.relop,">=")==0)
			{
				if (intercode->u.if_op.op1->name[0]=='#')
				{
					fprintf(file, "li $s0, %d\n",intercode->u.if_op.op1->u.int_value);
				}
				else
				{
					fprintf(file, "la $s1, %s\n",intercode->u.if_op.op1->name);
					fprintf(file, "lw $s0 0($s1)\n");
				}
				if (intercode->u.if_op.op2->name[0]=='#')
				{
					fprintf(file, "li $s1, %d\n",intercode->u.if_op.op2->u.int_value);
				}
				else
				{
					fprintf(file, "la $s2, %s\n",intercode->u.if_op.op2->name);
					fprintf(file, "lw $s1 0($s2)\n");
				}
				fprintf(file, "bge $s0,$s1,%s\n",intercode->u.if_op.op3->name);
			}
			else if (strcmp(intercode->u.if_op.relop,"<=")==0)
			{
				if (intercode->u.if_op.op1->name[0]=='#')
				{
					fprintf(file, "li $s0, %d\n",intercode->u.if_op.op1->u.int_value);
				}
				else
				{
					fprintf(file, "la $s1, %s\n",intercode->u.if_op.op1->name);
					fprintf(file, "lw $s0 0($s1)\n");
				}
				if (intercode->u.if_op.op2->name[0]=='#')
				{
					fprintf(file, "li $s1, %d\n",intercode->u.if_op.op2->u.int_value);
				}
				else
				{
					fprintf(file, "la $s2, %s\n",intercode->u.if_op.op2->name);
					fprintf(file, "lw $s1 0($s2)\n");
				}
				fprintf(file, "ble $s0,$s1,%s\n",intercode->u.if_op.op3->name);
			}
			//fprintf(file,"IF %s %s %s GOTO %s\n",intercode->u.if_op.op1->name,intercode->u.if_op.relop,intercode->u.if_op.op2->name,intercode->u.if_op.op3->name);
			break;
		}
		default:{
			break;
		}
	}
}
void obj_tran(FILE* file){
	dec_init();
	obj_init(file);
	param_init();
	ListHead* ptr;
	list_foreach(ptr,&code)
	{
		InterCode temp=list_entry(ptr,struct InterCode_,list);
		tran_inter(temp,file);
	}
}