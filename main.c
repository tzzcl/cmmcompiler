#include "node.h"
#include "list.h"
#include "ir.h"
#include "syntax.tab.h"
#include <assert.h>
#define DEBUG(x) 
extern int error;
extern Node* root;
extern FieldList varfield;
extern Type structfield;
extern FuncList funcfield;
extern ListHead code;
FieldList getField(Node* node);
Type inttype;
Type getExp(Node* node);
Type nowreturntype;
int equal_type(Type a,Type b)
{
	DEBUG(printf("%s:%p %p\n",__FUNCTION__,a,b);)
	while(a!=NULL&&b!=NULL)
	{
		DEBUG(printf("%s: a:%d b:%d\n",__FUNCTION__,a->kind,b->kind);)
		if (a->kind!=b->kind) 
			return 0;
		if (a->kind==0)
			return a->u.basic==b->u.basic;
		else if (a->kind==1)
		{
			a=a->u.array.elem;
			b=b->u.array.elem;
		}
		else if (a->kind==2)
		{
			FieldList first=a->u.structure,second=b->u.structure;
			DEBUG(printf("%s:%p %p %p %p \n",__FUNCTION__,first,first->type,second,second->type));
			while (first!=NULL&&second!=NULL)
			{
				if (!equal_type(first->type,second->type))
					return 0;
				first=first->tail;second=second->tail;
			}
			if (first==NULL&&second==NULL)
				return 1;
			else return 0;
		}
	}
	if (a==NULL&&b==NULL)
		return 1;
	return 0;
}
Type getleftExp(Node* node){
	node->isvisited=1;
	DEBUG(printf("%s:%s\n",__FUNCTION__,node->content);)
	if (node->cnt<=1)//ID 
	{
		if (strcmp(node->next[0]->content,"Exp")==0)
		{
			return getleftExp(node->next[0]);
		}
		else
		{
			if (strncmp(node->next[0]->content,"ID: ",4)==0)
			{
				FieldList var=returnvar(node->next[0]->content+4);
				if (var==NULL)
				{
					printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",node->line,node->next[0]->content+4);
					return NULL;
				}
				return var->type;
			}
			else 
			{
				printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n",node->line);
				return NULL;
			}
		}
	}
	else if (node->cnt>=3&&strcmp(node->next[1]->content,"DOT")==0)
	{
		return getExp(node);
	}
	else if (node->cnt>=4&&strcmp(node->next[1]->content,"LB")==0)
	{
		return getExp(node);
	}
	else 
	{
		printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n",node->line);
		return NULL;
	}
}
Type getExp(Node* node){
	node->isvisited=1;
	DEBUG(printf("%s:%s %d\n",__FUNCTION__,node->content,node->line));
	if (node==NULL) return NULL;
	if (node->cnt<=1)//ID 
	{
		if (strcmp(node->next[0]->content,"Exp")==0)
		{
			return getExp(node->next[0]);
		}
		else
		{
			if (strncmp(node->next[0]->content,"ID: ",4)==0)
			{
				FieldList var=returnvar(node->next[0]->content+4);
				DEBUG(printf("%s:name:%s\n",__FUNCTION__,node->next[0]->content+4);)
				if (var==NULL)
				{
					printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",node->line,node->next[0]->content+4);
					return NULL;
				}
				DEBUG(printf("%s:var:%d\n",__FUNCTION__,var->type->kind));
				return var->type;
			}
			else if (strncmp(node->next[0]->content,"INT:",4)==0)
			{
				DEBUG(puts("int");)
				return newint();
			}
			else{
				DEBUG(puts("float");)
				return newfloat();
			}
		}
	}

	if (node->cnt>=3&&strcmp(node->next[1]->content,"LP")==0)//Func
	{
		if (findvar(node->next[0]->content+4)||findstruct(node->next[0]->content+4))
		{
			printf("Error type 11 at Line %d: \"%s\" is not a function.\n",node->line,node->next[0]->content+4);
			return NULL;
		}

		FuncList fun=returnfun(node->next[0]->content+4);
		if (fun==NULL)
		{
			printf("Error type 2 at Line %d: Undefined function \"%s\".\n",node->line,node->next[0]->content+4);
				return NULL;
		}
		else
		{
			FieldList paramlist=fun->params;
			int numbers=fun->paramnumber;
			Node* temp=node->next[2];
			DEBUG(printf("%s:bbb%s\n",__FUNCTION__,node->content));
			Node* temp1=temp;
			while (paramlist!=NULL)
			{
				if (numbers==0) break;
				DEBUG(printf("%s:aaa %d %s\n",__FUNCTION__,paramlist->type->kind,temp1->content));
				if (temp1==NULL||strcmp(temp1->content,"Args")!=0)
				{
					printf("Error type 9 at Line %d:.\n",node->line);
					//outputfunc(fun,temp);
					return NULL;
				}
				Type answer=getExp(temp1->next[0]);
				DEBUG(printf("answer:%d %d\n",answer->kind,numbers));
				if (!equal_type(paramlist->type,answer))
				{
					printf("Error type 9 at Line %d:.\n ",node->line);
					//outputfunc(fun,temp);
					return NULL;
				}

				paramlist=paramlist->tail;

				temp1=temp1->next[2];
				numbers--;
			}
			if (numbers==0&&(temp1==NULL||strcmp(temp1->content,"Args")!=0))
			{
				DEBUG(puts(fun->returnname);)
				if (strcmp(fun->returnname,"int")==0)
				{
					return newint();
				}		
				else if (strcmp(fun->returnname,"float")==0)
				{
					return newfloat();
				}
				else if (strcmp(fun->name,"write")==0)
					return NULL;
				else 
					return returnvar(fun->returnname)->type;
			}
			if (numbers==0&&strcmp(temp1->content,"Args")==0)
			{
					printf("Error type 9 at Line %d:.\n ",node->line);
					//outputfunc(fun,temp);
					return NULL;
			}	
		}
	} 
	if (node->cnt>=3&&strcmp(node->next[1]->content,"DOT")==0)//struct
	{

		Type type=getExp(node->next[0]);
		if (type==NULL) return NULL;
		DEBUG(printf("%s:struct %d\n",__FUNCTION__,type->kind);)
		if (type->kind!=STRUCTURE)
		{
			printf("Error type 13 at Line %d: Illegal use of \".\".\n",node->line);

			return NULL;
		}
		FieldList paramlist=type->u.structure;

		while (paramlist!=NULL)
		{
			DEBUG(printf("%s:%s %s\n",__FUNCTION__,node->next[2]->content,paramlist->name);)
			if (strcmp(node->next[2]->content+4,paramlist->name)==0)
			{
				DEBUG(puts("PARAM RETURN");)
				return paramlist->type;
			}
				
			paramlist=paramlist->tail;
		}
		printf("Error type 14 at Line %d: Non-existent field \"%s\".\n",node->line,node->next[2]->content+4);	
		return NULL;
	}
	if (node->cnt>=3&&strcmp(node->next[1]->content,"ASSIGNOP")==0)
	{
		Type left=getleftExp(node->next[0]);
		Type right=getExp(node->next[2]);
		if (left==NULL||right==NULL) return NULL;
		DEBUG(printf("%s:left:%d right:%d\n",__FUNCTION__,left->kind,right->kind);)
		if (!equal_type(left,right))
		{
			printf("Error type 5 at Line %d: Type mismatched for assignment.\n",node->line);
			return NULL;
		}
		return left;
	}
	else if (node->cnt>=4&&strcmp(node->next[1]->content,"LB")==0)
	{
		Type arr=getExp(node->next[0]);
		DEBUG(printf("%s:arr:%d\n",__FUNCTION__,arr->kind));
		if (arr==NULL) return NULL;
		if (arr->kind!=ARRAY)
		{
			Node* temp=node->next[0];
			while (!strncmp(temp->next[0]->content+4,"ID: ",4)) temp=temp->next[0];
			if (strncmp(temp->next[0]->content,"ID: ",4)==0) printf("Error type 10 at Line %d: \"%s\" is not an array.\n",node->line,temp->next[0]->content+4);
			else printf("Error type 10 at Line %d: \"%f\" is not an array.\n",node->line,temp->type_float);
			return NULL;
		}
		Type size=getExp(node->next[2]);
		if (size==NULL) return NULL;
		if (!equal_type(size,inttype))
		{
			printf("Error type 12 at Line %d: \"%f\" is not an integer.\n",node->line,node->next[2]->next[0]->type_float);
			return NULL;
		}
		DEBUG(printf("%d\n",arr->u.array.elem->kind);)
		return arr->u.array.elem;
	}
	else if (node->cnt>=3&&(strcmp(node->next[1]->content,"RELOP")==0||strcmp(node->next[1]->content,"AND")==0
		||strcmp(node->next[1]->content,"OR")==0))
	{
		Type left=getExp(node->next[0]);
		Type right=getExp(node->next[2]);
		if (left==NULL||right==NULL) return NULL;
		if (!equal_type(left,inttype)||!equal_type(right,inttype))
		{
			printf("Error type 7 at Line %d: Type mismatched for operands.\n",node->line);
			return NULL;
		}
		return left;
	}
	else if (node->cnt>=2&&strcmp(node->next[0]->content,"NOT")==0)
	{
		Type right=getExp(node->next[1]);
		if (right==NULL) return NULL;
		if (!equal_type(right,inttype))
		{
			printf("Error type 7 at Line %d: Type mismatched for operands.\n",node->line);
			return NULL;
		}
		return right;
	}
	else{
		Type temptype[2];
		memset(temptype,0,sizeof(temptype));
		int cnt=0;
		int i=0,j=0;
		for (i=0;i<node->cnt;i++)
		if (strcmp(node->next[i]->content,"Exp")==0)
		{
			Type temp=getExp(node->next[i]);
			if (temp==NULL) return NULL;
			temptype[cnt++]=temp;
		}
		
		for (i=0;i<cnt;i++)
			for (j=i+1;j<cnt;j++)
				if (!equal_type(temptype[i],temptype[j]))
				{
					printf("Error type 7 at Line %d: Type mismatched for operands.\n",node->line);
					return NULL;
				}
		return temptype[0];
	}
	return NULL;
}
void dfs_type(Type type,Node* node){
	
	if (node==NULL) return ;
	if (node->isvisited) return ;
	DEBUG(printf("%s:name:%s %d\n",__FUNCTION__,node->content,node->line));
	node->isvisited=1;
	int i=0;
	if (strcmp(node->content,"Def")==0||strcmp(node->content,"ExtDef")==0)
	{
		FieldList temp=getField(node);
		DEBUG(printf("%s:temp:%s %d\n",__FUNCTION__,temp->name,temp->type->kind));
		if (type->u.structure==NULL)
			type->u.structure=temp;
		else
		{
			FieldList cnt=type->u.structure;
			for (cnt=type->u.structure;cnt!=NULL;cnt=cnt->tail)
				if (strcmp(cnt->name,temp->name)==0)
				{
					printf("Error type 15 at Line %d: Redefined Field \"%s\".\n",node->line,temp->name);
				}
			cnt=type->u.structure;
			while (cnt->tail!=NULL) cnt=cnt->tail;
			cnt->tail=temp;
		}

	}
	else if (strcmp(node->content,"LC")==0)
	{
		pushstack();
	}
	else if (strcmp(node->content,"RC")==0)
	{
		popstack();
	}
	for (;i<node->cnt;i++)
		dfs_type(type,node->next[i]);
	DEBUG(printf("%s:%p\n",__FUNCTION__,type->u.structure));
}
Type getType(Node* node){
	DEBUG(printf("%s:%s %d\n",__FUNCTION__,node->content,node->line));
	if (strcmp(node->content,"ExtDef")!=0&&strcmp(node->content,"Def")!=0&&strcmp(node->content,"ParamDec")!=0)
		return NULL;
	Type ans=NULL;
	Type type=newtype();
	Node* start=NULL;
	if (strcmp(node->next[1]->content,"ExtDecList")==0||strcmp(node->next[1]->content,"DecList")==0)
	{
		if (strcmp(node->next[1]->content,"ExtDecList")==0){
			start=node->next[1]->next[0];
		}
		else{
			start=node->next[1]->next[0]->next[0];
		}
		Node* temp=start;
		int cnt=0;
		while (strcmp(temp->content,"VarDec")==0) {

				temp=temp->next[0];
				cnt++;
		}
		DEBUG(printf("%s:cnt:%d\n",__FUNCTION__,cnt);)
		if (cnt>1)
		{
			Type new=type;
			while (cnt>1&&strcmp(start->content,"VarDec")==0)
			{
				Type newarray=newtype();
				newarray->kind=ARRAY;
				newarray->u.array.size=start->next[2]->type_int;
				newarray->u.array.elem=new;
				new=newarray;
				start=start->next[0];
				cnt--;
			}

			ans=new;
		}
	}

	if (ans==NULL) ans=type;
	if (strcmp(node->next[0]->next[0]->content,"StructSpecifier")==0)
	{
		type->kind=STRUCTURE;
		dfs_type(type,node->next[0]->next[0]->next[3]);
	}
	else
	{
		if (strcmp(node->next[0]->next[0]->content,"TYPE: int")==0)
		{
			type->kind=BASIC;
			type->u.basic=1;
		}
		else
		{
			type->kind=BASIC;
			type->u.basic=2;
		}
	}
	if (strcmp(node->next[1]->content,"DecList")==0){
		Node* Expnode=node->next[1]->next[0];
		if (Expnode->cnt>1){
			Type Ex=getExp(Expnode->next[2]);
			DEBUG(printf("%s:ans:%d Ex:%d\n",__FUNCTION__,ans->kind,Ex->kind);)
			if (!equal_type(ans,Ex))
			{
				printf("Error type 5 at Line %d: Type mismatched for assignment.\n",Expnode->line);
			}
			}
	}
	DEBUG(printf("%s:%d\n", __FUNCTION__,ans->kind);)
	return ans;
}
FieldList getField(Node* node)
{
	DEBUG(printf("%s:Node: %s %d\n",__FUNCTION__,node->content,node->line));
	if (strcmp(node->content,"ExtDef")!=0&&strcmp(node->content,"Def")!=0&&strcmp(node->content,"ParamDec")!=0)
		return NULL;
	Node* temp=node->next[1];
	FieldList answer=NULL;
	while (1){
		DEBUG(printf("%s:Node: %s %s %d\n",__FUNCTION__,node->content,temp->content,node->line));
		FieldList field=newfield();
		if (temp==NULL)
			break;
		if (strcmp(temp->content,"SEMI")==0){
			Type type=getType(node);
			StructList newst=newstruct();
			strcpy(newst->name,node->next[0]->next[0]->next[1]->next[0]->content+4);
			if (strcmp(node->next[0]->next[0]->next[1]->content,"OptTag")==0)
				if (findname(newst->name))
					printf("Error type 16 at Line %d: Duplicated name \"%s\".\n",node->line,newst->name);

			newst->type=type;
			DEBUG(printf("%s:rawstruct:%s %p\n",__FUNCTION__,newst->name,newst->type);)
			addstructfield(newst);
			return NULL;
		}

		if (strcmp(node->content,"ParamDec")==0)
		{
			if (strcmp(temp->content,"VarDec")!=0) break;
		}
		else if (strcmp(node->content,"ExtDef")==0){
			if (strcmp(temp->next[0]->content,"VarDec")!=0)
			break;
		}
		else if (strcmp(node->content,"Def")==0){
			if (strcmp(temp->next[0]->next[0]->content,"VarDec")!=0)
				break;
		}
		Node *temp1=temp;
		while (temp1!=NULL&&strncmp(temp1->content,"ID: ",4)!=0)
			temp1=temp1->next[0];
			strcpy(field->name,temp1->content+4);
			FieldList tempfield=returnvar(field->name);
			if (tempfield!=NULL&&tempfield->type!=NULL)
			printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",node->line,field->name);
			Type type=getType(node);
			field->type=type;
			DEBUG(printf("%s:name: %s\n",__FUNCTION__,field->name));
			if (strcmp(node->next[0]->next[0]->content,"StructSpecifier")==0){
			StructList newst=newstruct();
			if (strcmp(node->next[0]->next[0]->next[1]->content,"LC")!=0)
				strcpy(newst->name,node->next[0]->next[0]->next[1]->next[0]->content+4);
			else 
				pushstack();
			if (strcmp(node->next[0]->next[0]->next[1]->content,"OptTag")==0){
				if (findname(newst->name))
					printf("Error type 16 at Line %d: Duplicated name \"%s\".\n",node->line,newst->name);
				newst->type=type;
				if (!findstruct(newst->name)) addstructfield(newst);
			}
			else{
				if (!findstruct(newst->name))
				{
					if (strcmp(newst->name,"")==0)
					{
						newst->type=type;
						addstructfield(newst);
					}
					else{
						printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",node->line,newst->name);
						return NULL;
					}
					
				}
				field->type=returnstruct(newst->name)->type;
			}

			}
		if (answer==NULL)
			answer=field;
		else 
		{
			FieldList tempf=answer;
			while (tempf->tail!=NULL)
				tempf=tempf->tail;
			tempf->tail=field;
		}

		int i=0;
		temp=temp->next[2];
	}
	DEBUG(printf("%s:answer:%s %p\n",__FUNCTION__,answer->name,answer->type));
	return answer;
}
void dfs_param(FuncList fun,Node* node,int* numbers)
{
	DEBUG(printf("%s:name->%s\n",__FUNCTION__,node->content);)
	if (node==NULL) return ;
	if (node->isvisited) return ;
	node->isvisited=1;
	if (strcmp(node->content,"ParamDec")==0)
	{
		(*numbers)++;
		FieldList field=getField(node);
		DEBUG(printf("%s:field->name->%s\n",__FUNCTION__,field->name);)
		assert(field!=NULL);
		if (fun->params==NULL)
			fun->params=field;
		else 
		{
			FieldList temp=fun->params;
			
			while (temp->tail!=NULL) 
				temp=temp->tail;
			temp->tail=field;
			//if (field!=NULL) addvarfield(field);
		}
	}
	int i=0;
	for (;i<node->cnt;i++)
		dfs_param(fun,node->next[i],numbers);
}
int equal_fun(FuncList first,FuncList second)
{
	if (strcmp(first->name,second->name)!=0)
		return 0;
	if (strcmp(first->returnname,second->returnname)!=0)
		return 0;
	FieldList firstfield=first->params;
	FieldList secondfield=second->params;
	while (firstfield!=NULL&&secondfield!=NULL)
	{
		if (strcmp(firstfield->name,secondfield->name)!=0) return 0;
		if (!equal_type(firstfield->type,secondfield->type)) return 0;
		firstfield=firstfield->tail;secondfield=secondfield->tail;
	}
	if (firstfield!=NULL||secondfield!=NULL)
		return 0;
	return 1;
}
void func_def(Node* node){
	FuncList newfun=newfunc();
	DEBUG(printf("%s:Node: %s %d\n",__FUNCTION__,node->content,node->line));
	//return type
	if (strcmp(node->next[0]->next[0]->content,"StructSpecifier")==0)
	{
		strcpy(newfun->returnname,node->next[0]->next[0]->next[1]->next[0]->content+4);
		if (!findstruct(newfun->returnname))
		{
			printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",node->line,newfun->returnname);
		}
	}
	else{
		strcpy(newfun->returnname,node->next[0]->next[0]->content+6);
	}
	//name
	strcpy(newfun->name,node->next[1]->next[0]->content+4);
	dfs_param(newfun,node->next[1],&newfun->paramnumber);
	FuncList fun=returnfun(newfun->name);
	DEBUG(printf("%s:%s\n",__FUNCTION__,fun->name));
	if (fun!=NULL&&fun->isdefined==1)
	{
		printf("Error type 4 at Line %d: Redefined function \"%s\".\n",node->line,newfun->name);
		return ;
	}
	//params
	if (strcmp(node->next[2]->content,"CompSt")==0)
	{
		if (fun==NULL){
			addfuncfield(newfun);
			newfun->isdefined=1;
			newfun->line=node->line;
			addvarfield(newfun->params);
			FieldList funparam=newfun->params;
			while (funparam!=NULL)
			{
				funparam->nowstack=0;
				funparam=funparam->tail;
			}
			fun=newfun;
		}
		else{
			if (!equal_fun(fun,newfun))
			{
				printf("Error type 19 at Line %d: Inconsistent declaration of function \"%s\".\n",node->line,newfun->name);
				return ;
			}
			fun->isdefined=1;
			fun->line=node->line;
			addvarfield(newfun->params);
			fun->params=newfun->params;
			FieldList funparam=newfun->params;
			while (funparam!=NULL)
			{
				funparam->nowstack=0;
				funparam=funparam->tail;
			}
			
		}
	}
	else
	{
		if (fun==NULL)
		{
			addfuncfield(newfun);
			newfun->line=node->line;
		}
		else{
			if (!equal_fun(fun,newfun))
			{
				printf("Error type 19 at Line %d: Inconsistent declaration of function \"%s\".\n",node->line,newfun->name);
				return ;
			}
			fun->line=node->line;
		}
	}
}
void var_def(Node* node){
		DEBUG(printf("%s:Node: %s\n",__FUNCTION__,node->content));
		//var definition
		FieldList new=getField(node);
		DEBUG(printf("%s:Field: %s\n",__FUNCTION__,new->name));
		if (new!=NULL) addvarfield(new);
		
}

void insert(Node* root,Node* node){
	if (node->isvisited) return ;
	node->isvisited=1;
	DEBUG(printf("%s:%s\n",__FUNCTION__,node->content);)
	if (strcmp(node->content,"ExtDef")==0||strcmp(node->content,"Def")==0)
	{
		if (strcmp(node->next[1]->content,"FunDec")==0)
		{
				func_def(node);
		}
		else
		{
			var_def(node);
		}
		
	}
	else if (strcmp(node->content,"Exp")==0)
	{
		node->isvisited=1;
		getExp(node);
	}
	else if (strcmp(node->content,"CompSt")==0)
	{
		if (strcmp(root->content,"ExtDef")==0)
		{
			FuncList fun=returnfun(root->next[1]->next[0]->content+4);
			if (strcmp(fun->returnname,"int")==0)
			{
				nowreturntype=newint();
			}
			else if (strcmp(fun->returnname,"float")==0)
			{
				nowreturntype=newfloat();
			}
			else
			{
				nowreturntype=returnstruct(fun->returnname)->type;
			}
		}
		
	}
	else if (strcmp(node->content,"RETURN")==0)
	{
		Type returntype=getExp(root->next[1]);
		if (returntype==NULL) return ;
		if (!equal_type(nowreturntype,returntype))
		{
			printf("Error type 8 at Line %d: Type mismatched for return.\n",node->line);
		}
	}
	else if (strcmp(node->content,"LC")==0)
	{
		//pushstack();
	}
	else if (strcmp(node->content,"RC")==0)
	{
		//popstack();
	}
		
	int i;

	
	for (i=0;i<node->cnt;i++)
		insert(node,node->next[i]);
}
void travel(Node* node,int now){
	if (node==NULL){
		return ;
	}
	int i=0;
	for (i=0;i<now+1;i++) printf("  ");
	if (node->isterm==1){
		printf("%s \n",node->content);
	}
	else if (node->number==2){
		printf("%s %f\n",node->content,node->type_float);
	}
	else if (node->number==1){
		printf("%s %d\n",node->content,node->type_int);
	}
	else
	{
	if (node->line!=0)
		printf("%s (%d)\n",node->content,node->line);
	else 
		printf("%s\n",node->content);
	}
	for (i=0;i<node->cnt;i++){
		travel(node->next[i],now+1);
	}
}
void findfun(){
	FuncList fun=funcfield;
	while (fun!=NULL)
	{
		if (fun->isdefined==0)
		{
			printf("Error type 18 at Line %d: Undefined function \"%s\" .\n",fun->line,fun->name);
		}
		fun=fun->tail;
	}
}
int main(int argc,char** argv){
	if (argc<3) return 1;
	FILE* f =fopen(argv[1],"r");
	if (!f) return 1;
	//yydebug=1;
	yyrestart(f);
	yyparse();
	if (error==0) DEBUG(travel(root,0);)
	inttype=newint();
	FuncList read=newfunc();
	strcpy(read->name,"read");
	strcpy(read->returnname,"int");
	read->isdefined=1;
	read->params=NULL;
	addfuncfield(read);
	FuncList write=newfunc();
	strcpy(write->name,"write");
	write->isdefined=1;
	write->params=newfield();
	write->paramnumber=1;
	write->params->type=newint();
	addfuncfield(write);
	if (error==0) insert(root,root);
	else return 1;
	findfun();
	list_init(&code);
	FILE* out=fopen(argv[2],"w");
	
	int debug=0;
	translate(root);
	//print_intercode(out);
	FILE* ir_out=fopen("ir.ir","w");
	print_intercode(ir_out);
	fclose(ir_out);
	obj_tran(out);
	return 0;
}
