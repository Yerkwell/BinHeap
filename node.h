#pragma once
class node
{
public:
	int key;
	node *p;
	node *child;
	node *brother;				//Правый брат (слой хранится в виде односвязного списка)
	int degree;
	node(int key);
	node();
	~node();
};

