#pragma once
class node
{
public:
	int key;
	node *p;
	node *child;
	node *brother;				//������ ���� (���� �������� � ���� ������������ ������)
	int degree;
	node(int key);
	node();
	~node();
};

