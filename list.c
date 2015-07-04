#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
FieldList varfield;
StructList structfield;
FuncList funcfield;
Type newtype(){
	Type new=malloc(sizeof(struct Type_));
	memset(new,0,sizeof(struct Type_));
	return new;
}
Type newint(){
	Type new=malloc(sizeof(struct Type_));
	memset(new,0,sizeof(struct Type_));
	new->kind=BASIC;
	new->u.basic=1;
	return new;
}
Type newfloat(){
	Type new=malloc(sizeof(struct Type_));
	memset(new,0,sizeof(struct Type_));
	new->kind=BASIC;
	new->u.basic=2;
	return new;
}
FieldList newfield(){
	FieldList new=malloc(sizeof(struct FieldList_));
	memset(new,0,sizeof(struct FieldList_));
	return new;
}
StructList newstruct(){
	StructList new=malloc(sizeof(struct StructList_));
	memset(new,0,sizeof(struct StructList_));
	return new;
}
FuncList newfunc(){
	FuncList new=malloc(sizeof(struct FuncList_));
	memset(new,0,sizeof(struct FuncList_));
	return new;
}
void addstructfield(StructList other){
	if (structfield==NULL)
		structfield=other;
	else
	{
		StructList temp=structfield;
		while (temp->tail!=NULL)
			temp=temp->tail;
		temp->tail=other;
	}
}
void addvarfield(FieldList other){
	
	if (other==NULL) return;
	if (varfield==NULL)
		varfield=other;
	else
	{
		FieldList tempvar=returnvar(other->name);
		if (tempvar==NULL)
		{
			FieldList temp=varfield;
			while (temp->tail!=NULL)
			temp=temp->tail;
			temp->tail=other;
		}
		else
		{
			tempvar->type=other->type;
		}
	}

}
void addfuncfield(FuncList other){
	if (funcfield==NULL)
		funcfield=other;
	else
	{
		FuncList temp=funcfield;
		while (temp->tail!=NULL)
			temp=temp->tail;
		temp->tail=other;
	}
}
int findstruct(const char* name)
{
	StructList tempstruct=structfield;
	while (tempstruct!=NULL)
	{
		if (strcmp(name,tempstruct->name)==0)
			return 1;
		tempstruct=tempstruct->tail;
	}
	return 0;
}
FieldList returnvar(const char* name)
{
	FieldList temp=varfield;
	while (temp!=NULL)
	{
		if (strcmp(name,temp->name)==0&&temp->type!=NULL)
			return temp;
		temp=temp->tail;
	}
	return NULL;
}
int findvar(const char* name)
{
	FieldList tempfield=varfield;
	while (tempfield!=NULL)
	{
		if (strcmp(name,tempfield->name)==0)
			return 1;
		tempfield=tempfield->tail;
	}
	return 0;
}
int findfunc(const char* name)
{
	FuncList tempfunc=funcfield;
	while (tempfunc!=NULL)
	{
		if (strcmp(name,tempfunc->name)==0)
			return 1;
		tempfunc=tempfunc->tail;
	}
	return 0;
}
int findname(const char* name)
{
	return findvar(name)||findfunc(name)||findstruct(name);
}
FuncList returnfun(const char* name){
	FuncList tempfunc=funcfield;
	while (tempfunc!=NULL)
	{
		if (strcmp(name,tempfunc->name)==0)
			return tempfunc;
		tempfunc=tempfunc->tail;
	}
	return NULL;
}
FieldList returnvarno(const int var_no){
	FieldList temp=varfield;
	while (temp!=NULL)
	{
		if (temp->var_no==var_no)
			return temp;
		temp=temp->tail;
	}
	return NULL;
}
StructList returnstruct(const char* name){
	StructList tempstruct=structfield;
	while (tempstruct!=NULL)
	{
		if (strcmp(name,tempstruct->name)==0)
			return tempstruct;
		tempstruct=tempstruct->tail;
	}
	return NULL;
}
void pushstack(){
		FieldList field=varfield;
		while (field!=NULL)
		{
			field->stack[field->nowstack++]=field->type;
			field=field->tail;
		}	
}
void popstack(){
		FieldList field=varfield;
		while (field!=NULL)
		{
			if (field->nowstack>0)
				field->type=field->stack[--field->nowstack];
			else field->type=NULL;
			field=field->tail;
		}
}