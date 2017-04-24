#pragma once
#include "node.h"
#include <queue>
#include <iostream>
class binHeap
{
private:
	void swap(node*, node*);		//OK
	node* leftRoot(node*);						//Для экстренного поиска предыдущего корня (если вспомогательного указателя под рукой нет)
	void supersede(binHeap*);			//OK
	node* search(node*, int);		//OK
public:
	node *root;

	binHeap();
	binHeap(node*);
	binHeap(node);
	node* getMinimum();				//OK
	//Осторожно! Метод удаляет исходные кучи!
	binHeap* merge(binHeap*);		//OK
	void setNodeKey(node*, int);	//OK
	node* insert(int);			//OK
	node* insert(node);			//OK
	node* search(int);				//OK
	void removeNode(node*);			//OK
	void remove(int);
	void clear();
	node* extractMin();				//OK
	~binHeap();
};

