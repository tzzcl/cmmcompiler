#include "ir.h"
#include "node.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#define DEBUG(x) 
int var_total=0;
int temp_total=0;
int label_total=0;
int struct_error=0;
ListHead code;
void translate_compst(Node* node);
void translate_exp(Node* node,Operand* temp);
Operand new_op(){
	Operand newop=malloc(sizeof(struct Operand_));
	memset(newop,0,sizeof(struct Operand_));
	return newop;
}
InterCode new_intercode(){
	InterCode newinter=malloc(sizeof(struct InterCode_));
	memset(newinter,0,sizeof(struct InterCode_));
	return newinter;
}
void print_single_intercode(InterCode intercode,FILE* file)
{
	DEBUG(printf("%s: %p %d\n",__FUNCTION__,intercode,intercode->kind);)
	switch (intercode->kind){
		case DEF_LABEL_I:{
			fprintf(file,"LABEL %s :\n",intercode->u.one_op->name);
			break;
		}
		case DEF_FUN_I:{
			fprintf(file,"FUNCTION %s :\n",intercode->u.one_op->u.funname);
			break;
		}
		case GOTO_I:{
			fprintf(file,"GOTO %s\n",intercode->u.one_op->name);
			break;
		}
		case RETURN_I:{
			fprintf(file,"RETURN %s\n",intercode->u.one_op->name);
			break;
		}
		case ARG_I:{
			fprintf(file,"ARG %s\n",intercode->u.one_op->name);
			break;
		}
		case PARAM_I:{
			fprintf(file,"PARAM %s\n",intercode->u.one_op->name);
			break;
		}
		case READ_I:{
			fprintf(file,"READ %s\n",intercode->u.one_op->name);
			break;
		}
		case WRITE_I:{
			fprintf(file,"WRITE %s\n",intercode->u.one_op->name);
			break;
		}
		case ASSIGN_I:{
			fprintf(file,"%s := %s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name);
			break;
		}
		case ASSIGN_ADDR_I:{
			fprintf(file,"%s := &%s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name);
			break;
		}
		case ASSIGN_MEM_I:{
			fprintf(file,"%s := *%s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name);
			break;
		}
		case MEM_ASSIGN_I:{
			fprintf(file,"*%s := %s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name);
			break;
		}
		case DEC_SIZE_I:{
			fprintf(file,"DEC %s %s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name+1);
			break;
		}
		case ASSIGN_CALL_I:{
			fprintf(file,"%s := CALL %s\n",intercode->u.two_op.op1->name,intercode->u.two_op.op2->name);
			break;
		}
		case PLUS_I:{
			fprintf(file,"%s := %s + %s\n",intercode->u.three_op.op1->name,intercode->u.three_op.op2->name,intercode->u.three_op.op3->name);
			break;
		}
		case MINUS_I:{
			fprintf(file,"%s := %s - %s\n",intercode->u.three_op.op1->name,intercode->u.three_op.op2->name,intercode->u.three_op.op3->name);
			break;
		}
		case STAR_I:{
			fprintf(file,"%s := %s * %s\n",intercode->u.three_op.op1->name,intercode->u.three_op.op2->name,intercode->u.three_op.op3->name);
			break;
		}
		case DIV_I:{
			fprintf(file,"%s := %s / %s\n",intercode->u.three_op.op1->name,intercode->u.three_op.op2->name,intercode->u.three_op.op3->name);
			break;
		}
		case COND_I:{
			//fprintf(file,"IF %x GOTO ")
			fprintf(file,"IF %s %s %s GOTO %s\n",intercode->u.if_op.op1->name,intercode->u.if_op.relop,intercode->u.if_op.op2->name,intercode->u.if_op.op3->name);
			break;
		}
		default:{
			break;
		}
	}
}
Operand con_oper(int kind,...){
	Operand newop=new_op();
	newop->kind=kind;
	va_list arg;va_start(arg,kind);
	if (kind<=1){
		newop->u.int_value=va_arg(arg,int);
		sprintf(newop->name,"#%d",newop->u.int_value);
	}
	else if (kind==FUNCTION){
		char* temp=va_arg(arg,char*);
		strcpy(newop->u.funname,temp);
		strcpy(newop->name,temp);
	}
	else if (kind==LABEL){
		newop->u.var_no=va_arg(arg,int);
		strcpy(newop->name,"l");
		sprintf(newop->name+1,"%d",newop->u.var_no);
	}
	else if (kind==ADDRESS){
		newop->u.var_no=va_arg(arg,int);
		strcpy(newop->name,"&");
		strcpy(newop->name+1,"t");
		sprintf(newop->name+2,"%d",newop->u.var_no);
	}
	else {
		newop->u.var_no=va_arg(arg,int);
		strcpy(newop->name,"t");
		sprintf(newop->name+1,"%d",newop->u.var_no);
	}
	va_end(arg);
	return newop;
}
InterCode con_inter(int kind,...)
{
	InterCode newcode=new_intercode();
	newcode->kind=kind;
	va_list arg;va_start(arg,kind);
	if (kind<=WRITE_I){
		newcode->u.one_op=va_arg(arg,Operand);
	}
	else if (kind<=ASSIGN_I){
		newcode->u.two_op.op1=va_arg(arg,Operand);
		newcode->u.two_op.op2=va_arg(arg,Operand);
	}
	else if (kind<=DIV_I){
		newcode->u.three_op.op1=va_arg(arg,Operand);
		newcode->u.three_op.op2=va_arg(arg,Operand);
		newcode->u.three_op.op3=va_arg(arg,Operand);
	}
	else {
		newcode->u.if_op.op1=va_arg(arg,Operand);
		newcode->u.if_op.op2=va_arg(arg,Operand);
		newcode->u.if_op.op3=va_arg(arg,Operand);
		char * temp=va_arg(arg,char*);
		strcpy(newcode->u.if_op.relop,temp);
	}
	va_end(arg);
	return newcode;
}
void print_intercode(FILE* file)
{
	ListHead* p;
	list_foreach(p,&code)
	{
		InterCode temp=list_entry(p,struct InterCode_,list);
		print_single_intercode(temp,file);
	}
		
}
Operand getvar(Node* node){
	DEBUG(printf("%s:%s %d %d\n",__FUNCTION__,node->content,node->line,node->cnt);)
	while (strncmp(node->content,"ID: ",4)!=0) node=node->next[0];
	assert(node!=NULL);
	FieldList field=returnvar(node->content+4);
	char temp[31];memset(temp,0,sizeof(temp));
	if (field->var_no==0)
		field->var_no=++temp_total;
	sprintf(temp,"%d",field->var_no);
	Operand op=con_oper(VARIABLE,field->var_no);

	return op;
}
void translate_cond(Node* node,Operand op1,Operand op2){
	if (node->cnt==2){
		if (strcmp(node->next[0]->content,"NOT")==0)
		{
			translate_cond(node->next[1],op2,op1);
			return ;
		}
	}
	else if (node->cnt==3&&strcmp(node->next[1]->content,"AND")==0){
		Operand op_l= con_oper(LABEL,++label_total);
       		InterCode intercode = con_inter(DEF_LABEL_I,op_l);
      		translate_cond(node->next[0],op_l,op2);
      		list_add_before(&code,&intercode->list);
        		translate_cond(node->next[2],op1,op2);
	}
	else if (node->cnt==3&&strcmp(node->next[1]->content,"OR")==0){
		Operand op_l= con_oper(LABEL,++label_total);
       		InterCode intercode = con_inter(DEF_LABEL_I,op_l);
      		translate_cond(node->next[0],op_l,op2);
      		list_add_before(&code,&intercode->list);
        		translate_cond(node->next[2],op1,op2);
	}
	else if (node->cnt==3&&strcmp(node->next[1]->content,"RELOP")==0){
		Operand opn1=con_oper(TEMP,++temp_total);
		Operand opn2=con_oper(TEMP,++temp_total);
		translate_exp(node->next[0],&opn1);
		translate_exp(node->next[2],&opn2);
		InterCode intercode1=con_inter(COND_I,opn1,opn2,op1,node->next[1]->relop);
		InterCode intercode2=con_inter(GOTO_I,op2);
		list_add_before(&code,&intercode1->list);
		list_add_before(&code,&intercode2->list);
	}
	else {
		Operand temp=con_oper(TEMP,++temp_total);
		translate_exp(node,&temp);
		Operand zero=con_oper(CONSTANT,0);
		InterCode code0=con_inter(COND_I,temp,op1,zero,"!=");
		InterCode code1=con_inter(GOTO_I,op2);
		list_add_before(&code,&code0->list);
		list_add_before(&code,&code1->list);
	}
}
void condition(Node* node,Operand op){
	Operand op_l1=con_oper(LABEL,++label_total);
	Operand op_l2=con_oper(LABEL,++label_total);
	Operand zero=con_oper(CONSTANT,0);
	Operand one=con_oper(CONSTANT,1);
	InterCode code1=con_inter(ASSIGN_I,op,zero);
	list_add_before(&code,&code1->list);
	translate_cond(node,op_l1,op_l2);
	InterCode code2=con_inter(DEF_LABEL_I,op_l1);
	InterCode code3=con_inter(ASSIGN_I,op,one);
	InterCode code4=con_inter(DEF_LABEL_I,op_l2);
	list_add_before(&code,&code2->list);
	list_add_before(&code,&code3->list);
	list_add_before(&code,&code4->list);
}
int array_len(Node* node){
	int array=4;
	int depth=0;
	while (node->cnt!=1)
	{
		depth++;node=node->next[0];
	}
	FieldList temp=returnvar(node->next[0]->content+4);
	Type type=temp->type;
	DEBUG(printf("depth %d\n",depth);)
	while (depth){
		depth--;
		type=type->u.array.elem;
	}
	while (type->kind==ARRAY)
	{
		type=type->u.array.elem;
		if (type->kind==ARRAY)
			array=array*(temp->type->u.array.size);
		
	}
	return array;
}
void translate_array(Node* node,Operand op){
	DEBUG(printf("%s:%s %d %d\n",__FUNCTION__,node->content,node->line,node->cnt);)
	int len=array_len(node->next[0]);
	DEBUG(printf("%s:%s %d\n",__FUNCTION__,"array_len",len);)
	Operand lens=con_oper(CONSTANT,len);
	Operand op1=con_oper(TEMP,++temp_total);
	Operand offset=con_oper(TEMP,++temp_total);
	translate_exp(node->next[2],&op1);
	InterCode code1=con_inter(STAR_I,offset,op1,lens);
	list_add_before(&code,&code1->list);
	if (node->next[0]->cnt==1)
	{
		FieldList var=returnvar(node->next[0]->next[0]->content+4);
		assert(var!=NULL);
		if (var->isparam==0)
		{
			Operand addr=con_oper(ADDRESS,var->var_no);
			InterCode intercode=con_inter(PLUS_I,op,addr,offset);
			list_add_before(&code,&intercode->list);
		}
		else
		{
			Operand array=con_oper(VARIABLE,var->var_no);
			InterCode intercode=con_inter(PLUS_I,op,array,offset);
			list_add_before(&code,&intercode->list);
		}
	}
	else
	{
		Operand op2=con_oper(TEMP,++temp_total);
		translate_array(node->next[0],op2);
		InterCode intercode=con_inter(PLUS_I,op,op2,offset);
		list_add_before(&code,&intercode->list);
	}
}
void translate_args(Node* node,ListHead* head){
	assert(node!=NULL);
	DEBUG(printf("%s:%s %d %d\n",__FUNCTION__,node->content,node->line,node->cnt);)
	if (node->cnt==1)
	{
		Operand temp=con_oper(TEMP,++temp_total);
		translate_exp(node->next[0],&temp);
		if (node->next[0]->cnt==1)
		{
			if (node->next[0]->next[0]->number==0)
			{
				FieldList var=returnvar(node->next[0]->next[0]->content+4);
				if (var->type->kind==ARRAY)
				{
					Operand addr=con_oper(ADDRESS,temp->u.int_value);
					list_add_after(head,&addr->arg_list);
					return ;
				}
			}
			

		}

		list_add_after(head,&temp->arg_list);

	}
	else
	{
		Operand temp=con_oper(TEMP,++temp_total);
		translate_exp(node->next[0],&temp);
		Node* tempnode=node->next[0];
		if (tempnode->next[0]->cnt==1)
		{
			FieldList var=returnvar(tempnode->next[0]->next[0]->content+4);
			if (var->type->kind==ARRAY)
			{
				Operand addr=con_oper(ADDRESS,temp->u.int_value);
				list_add_after(head,&addr->arg_list);
				return ;
			}
		}
		list_add_after(head,&temp->arg_list);
		translate_args(node->next[2],head);
	}
}
void translate_exp(Node* node,Operand* temp){
	DEBUG(printf("%s:%s %d %d\n",__FUNCTION__,node->content,node->line,node->cnt);)
	Operand nowop=NULL;
	if (temp!=NULL) nowop=*temp;
	else nowop=con_oper(TEMP,++temp_total);
	if (node->cnt==1){
		if (node->next[0]->number==0)
		{
			Operand varop=getvar(node->next[0]);
			*temp=varop;
		}
		else if (node->next[0]->number==1)
		{
			Operand con_var=con_oper(CONSTANT,node->next[0]->type_int);
			*temp=con_var;
		}
	}
	else if (node->cnt==2)
	{
		Operand op_tmp =con_oper(TEMP,++temp_total);
                	translate_exp(node->next[1],&op_tmp);
             		Operand zero = con_oper(CONSTANT,0);
              	InterCode minuscode = con_inter(MINUS_I,nowop,zero,op_tmp);
              	list_add_before(&code,&minuscode->list);
	}
	else if (node->cnt==3)
	{
		InterCode intercode=NULL;
		if (strcmp(node->next[1]->content,"RELOP")==0
			||strcmp(node->next[1]->content,"AND")==0
			||strcmp(node->next[1]->content,"OR")==0)
		{
			condition(node,nowop);
		}
		else if (strcmp(node->next[1]->content,"ASSIGNOP")==0){
			Operand op_temp=con_oper(TEMP,++temp_total);
			translate_exp(node->next[2],&op_temp);

			if (node->next[0]->cnt==1)
			{
				Operand var=getvar(node->next[0]);
				InterCode code1=con_inter(ASSIGN_I,var,op_temp);
				InterCode code2=con_inter(ASSIGN_I,nowop,var);
				list_add_before(&code,&code1->list);
				list_add_before(&code,&code2->list);
			}
			else {
				Operand op1=con_oper(TEMP,++temp_total);
				translate_array(node->next[0],op1);
				InterCode code1=con_inter(MEM_ASSIGN_I,op1,op_temp);

				InterCode code2=con_inter(ASSIGN_MEM_I,nowop,op1);
				list_add_before(&code,&code1->list);
				list_add_before(&code,&code2->list);
			}
		}
		else if (strcmp(node->next[1]->content,"PLUS")==0){
			Operand op1=con_oper(TEMP,++temp_total);
			Operand op2=con_oper(TEMP,++temp_total);
			translate_exp(node->next[0],&op1);
			translate_exp(node->next[2],&op2);
			intercode=con_inter(PLUS_I,nowop,op1,op2);
			list_add_before(&code,&intercode->list);
		}
		else if (strcmp(node->next[1]->content,"MINUS")==0){
			Operand op1=con_oper(TEMP,++temp_total);
			Operand op2=con_oper(TEMP,++temp_total);
			translate_exp(node->next[0],&op1);
			translate_exp(node->next[2],&op2);
			 intercode=con_inter(MINUS_I,nowop,op1,op2);
			list_add_before(&code,&intercode->list);
		}
		else if (strcmp(node->next[1]->content,"STAR")==0){
			Operand op1=con_oper(TEMP,++temp_total);
			Operand op2=con_oper(TEMP,++temp_total);
			translate_exp(node->next[0],&op1);
			translate_exp(node->next[2],&op2);
			intercode=con_inter(STAR_I,nowop,op1,op2);
			list_add_before(&code,&intercode->list);
		}
		else if (strcmp(node->next[1]->content,"DIV")==0){
			Operand op1=con_oper(TEMP,++temp_total);
			Operand op2=con_oper(TEMP,++temp_total);
			translate_exp(node->next[0],&op1);
			translate_exp(node->next[2],&op2);
			intercode=con_inter(DIV_I,nowop,op1,op2);
			list_add_before(&code,&intercode->list);
		}
		else if (strcmp(node->next[1]->content,"LP")==0){
			if (strcmp(node->next[0]->content+4,"read")==0){
				intercode=con_inter(READ_I,nowop);
			}
			
			else{
				FuncList temp=returnfun(node->next[0]->content+4);
				Operand fun=con_oper(FUNCTION,temp->name);
				intercode=con_inter(ASSIGN_CALL_I,nowop,fun);
			}
			list_add_before(&code,&intercode->list);
		}
		else if (strcmp(node->next[1]->content,"DOT")==0){
			//TODO Struct
			struct_error=1;
			printf("%s\n","Cannot translate: Code contains variables or parameters of structure type.");
			exit(0);
		}
		else if (strcmp(node->next[0]->content,"LP")==0){
			translate_exp(node->next[1],&nowop);
		}
	}
	else if (node->cnt==4)
	{
		if (strcmp(node->next[1]->content,"LB")==0)
		{
			Operand addressop=con_oper(TEMP,++temp_total);
			translate_array(node,addressop);
			InterCode intercode=con_inter(ASSIGN_MEM_I,nowop,addressop);
			list_add_before(&code,&intercode->list);
		}
		else if (strcmp(node->next[2]->content,"Args")==0)
		{
			FuncList temp=returnfun(node->next[0]->content+4);

			ListHead arg_list;
			list_init(&arg_list);
			translate_args(node->next[2],&arg_list);
			
			if (strcmp(temp->name,"write")==0)
			{
				InterCode intercode=con_inter(WRITE_I,list_entry(arg_list.next,struct Operand_,arg_list));
				list_add_before(&code,&intercode->list);
			}
			else
			{
				ListHead* ptr;
				list_foreach(ptr,&arg_list){
					InterCode intercode=con_inter(ARG_I,list_entry(ptr,struct Operand_,arg_list));
					list_add_before(&code,&intercode->list);
				}
				Operand func=con_oper(FUNCTION,temp->name);
				InterCode intercode=con_inter(ASSIGN_CALL_I,nowop,func);
				list_add_before(&code,&intercode->list);
			}
		}
	}
}
void translate_stmt(Node* node){
	assert(node!=NULL);
	DEBUG(printf("%s:%s %d %d\n",__FUNCTION__,node->content,node->line,node->cnt);)

	if (strcmp(node->next[0]->content,"CompSt")==0) //CompSt
	{
		translate_compst(node->next[0]);
	}
	else if (node->cnt>=2&&strcmp(node->next[1]->content,"SEMI")==0) //Exp SEMI
	{
		translate_exp(node->next[0],NULL);
	}
	else if (node->cnt>=3&&strcmp(node->next[0]->content,"RETURN")==0){ //RETURN Exp SEMI;
		Operand temp=con_oper(TEMP,++temp_total);
		translate_exp(node->next[1],&temp);
		InterCode intercode=con_inter(RETURN_I,temp);
		list_add_before(&code,&intercode->list);
	}
	else if (strcmp(node->next[0]->content,"WHILE")==0)//WHILE LP EXP RP Stmt
	{
		Operand op1=con_oper(LABEL,++label_total);
		Operand op2=con_oper(LABEL,++label_total);
		Operand op3=con_oper(LABEL,++label_total);
		InterCode intercode1=con_inter(GOTO_I,op1);
		InterCode intercode2=con_inter(DEF_LABEL_I,op1);
		InterCode intercode3=con_inter(DEF_LABEL_I,op2);
		InterCode intercode4=con_inter(DEF_LABEL_I,op3);
		list_add_before(&code,&intercode2->list);
		translate_cond(node->next[2],op2,op3);
		list_add_before(&code,&intercode3->list);
		translate_stmt(node->next[4]);
		list_add_before(&code,&intercode1->list);
		list_add_before(&code,&intercode4->list);
	}
	else if (node->cnt>=7){ //IF ELSE
		Operand op1=con_oper(LABEL,++label_total);
		Operand op2=con_oper(LABEL,++label_total);
		Operand op3=con_oper(LABEL,++label_total);
		InterCode code1=con_inter(GOTO_I,op3);
		InterCode code2=con_inter(DEF_LABEL_I,op1);
		InterCode code3=con_inter(DEF_LABEL_I,op2);
		InterCode code4=con_inter(DEF_LABEL_I,op3);
		translate_cond(node->next[2],op1,op2);
		list_add_before(&code,&code2->list);
		translate_stmt(node->next[4]);
		list_add_before(&code,&code1->list);
		list_add_before(&code,&code3->list);
		translate_stmt(node->next[6]);
		list_add_before(&code,&code4->list);
	}
	else { //Simple IF
		Operand op1=con_oper(LABEL,++label_total);
		Operand op2=con_oper(LABEL,++label_total);
		InterCode code1=con_inter(DEF_LABEL_I,op1);
		InterCode code2=con_inter(DEF_LABEL_I,op2);
		translate_cond(node->next[2],op1,op2);
		list_add_before(&code,&code1->list);
		translate_stmt(node->next[4]);
		list_add_before(&code,&code2->list);
	}
}
Operand translate_vardec(Node* node){
	DEBUG(printf("%s:%s %d\n",__FUNCTION__,node->content,node->cnt);)
	if (node->cnt==1){
		return getvar(node->next[0]);
	}
	else {
		while (node->cnt==4){
			node=node->next[0];
		}

		FieldList field=returnvar(node->next[0]->content+4);
		puts(node->next[0]->content+4);
		assert(field->var_no==0);
		field->var_no=++temp_total;
		int nowsize=4;
		Type temp=field->type;
		while (temp->kind==ARRAY){
			nowsize*=temp->u.array.size;
			temp=temp->u.array.elem;
		}
		Operand op_size=con_oper(SIZE,nowsize);
		Operand op_var=con_oper(VARIABLE,field->var_no);
		InterCode intercode=con_inter(DEC_SIZE_I,op_var,op_size);
		list_add_before(&code,&intercode->list);
		return op_var;
	}
}
void translate_func(Node* node){

	Node* fun = node->next[1];
            char *func =fun->next[0]->content+4;
            Operand op_fun = con_oper(FUNCTION,func);
            InterCode funcode = con_inter(DEF_FUN_I,op_fun);
            list_add_before(&code,&funcode->list);
	if (strcmp(fun->next[2]->content,"VarList")==0)
	{
		Node* list=fun->next[2];
		
		while (list!=NULL)
		{
			Node* temp=list->next[0];
			Node* Dec=temp->next[1];
			while (Dec){
				if (Dec->cnt==1){
					Node* ID=Dec->next[0];
					FieldList field=returnvar(ID->content+4);
					field->isparam=1;
					Operand var=getvar(ID);

					InterCode intercode= con_inter(PARAM_I,var);
					list_add_before(&code,&intercode->list);
				}
				Dec=Dec->next[0];
			}
			list=list->next[2];
		}
	}
}
void translate_compst(Node* node)
{
	Node* DefList=node->next[1];
	Node* StmtList=node->next[2];
	DEBUG(printf("%s:%d %s %s \n",__FUNCTION__,node->line,DefList->content,StmtList->content);)
	while (DefList!=NULL&&strcmp(DefList->content,"DefList")==0)
	{
		Node* Def=DefList->next[0];
		Node* DecList=Def->next[1];
		while (DecList!=NULL&&DecList->cnt>0)
		{
			Node* Dec=DecList->next[0];
			if (Dec->cnt==1){
				translate_vardec(Dec->next[0]);
			}
			else {
				Operand var=translate_vardec(Dec->next[0]);
				Operand tmp=con_oper(TEMP,++temp_total);

				translate_exp(Dec->next[2],&tmp);
				InterCode intercode=con_inter(ASSIGN_I,var,tmp);
				list_add_before(&code,&intercode->list);
			}
			DecList=DecList->next[2];
		}
		DefList=DefList->next[1];
	}
	while (StmtList!=NULL&&strcmp(StmtList->content,"StmtList")!=0)
		StmtList=node->next[1];

	while (StmtList!=NULL)
	{
		Node* Stmt=StmtList->next[0];

		translate_stmt(Stmt);
		StmtList=StmtList->next[1];
	}
}
void translate(Node* node){
	if (strcmp(node->content,"ExtDef")==0)
	{
		if (strcmp(node->next[1]->content,"ExtDecList")==0)
		{
			Node* temp=node->next[1];
			while (temp!=NULL)
			{
				translate_vardec(temp->next[0]);
				temp=temp->next[2];
			}
		}
		else if (strcmp(node->next[1]->content,"FunDec")==0)
		{
			//func
			translate_func(node);
			translate_compst(node->next[2]);
		}
	}
	else
	{
		int i;
		for (i=0;i<node->cnt;i++)
			translate(node->next[i]);
	}
}