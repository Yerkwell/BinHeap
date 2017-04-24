#include "binHeap.h"


binHeap::binHeap()
{
	root = NULL;
}

binHeap::binHeap(node* root)
{
	this->root = root;
}

binHeap::binHeap(node root)
{
	this->root = new node(root.key);
}

node* binHeap::getMinimum()
{
	if (root)
	{
		node *min = root;
		node *cur = root;
		while (cur->brother)
		{
			if (cur->brother->key < min->key)
				min = cur->brother;
			cur = cur->brother;
		}
		return min;
	}
	else
		return 0;
}

node* binHeap::leftRoot(node* cur)
{
	node* searcher = root;
	while (searcher && searcher->brother != cur)
		searcher = searcher->brother;
	if (!searcher)
		return 0;
	if (searcher->brother == cur)
		return searcher;
	else
		return 0;
}

binHeap* binHeap::merge(binHeap* b)
{
	binHeap* a = this;
		//���� ���� �� ��� ������, ���������� ������
	if (!b || !b->root)
		return a;
	if (!a->root)
	{
		supersede(b);					//������ ��� ���� ����� ��������� �� ������ ���� b
		return a;
	}
	if (a == b)						//���� ��� ��������� ��������� �� ���� � �� �� ���� (!)
		return a;
	binHeap* H = new binHeap();		//����� ����, ���������� ��� �������
	node* curA = a->root;
	node* curB = b->root;
	if (curA->degree <= curB->degree)				//������ ����� ���� ������
	{
		H->root = curA;
		curA = curA->brother;
	}
	else
	{
		H->root = curB;
		curB = curB->brother;
	}
	node* curH = H->root;
	//��������� ������ ������ ��������
	while (curA && curB)
	{
		if (curA->degree <= curB->degree)
		{
			curH->brother = curA;
			curA = curA->brother;
			curH = curH->brother;
		}
		else
		{
			curH->brother = curB;
			curB = curB->brother;
			curH = curH->brother;
		}
	}
	if (curA)
		while (curA)
		{
			curH->brother = curA;
			curA = curA->brother;
			curH = curH->brother;
		}
	else
		while (curB)
		{
			curH->brother = curB;
			curB = curB->brother;
			curH = curH->brother;
		}
	//����������
	//������ ���������� ������� ����� �������
	node* prev = NULL;
	curH = H->root;
	while (curH->brother)
	{
		if ((curH->brother->brother) && (curH->degree == curH->brother->degree && curH->degree == curH->brother->brother->degree))	//���� ������ ��� ������ ����� ������
		{
			prev = prev ? prev->brother : curH;					//���� ��� ��� ������ �������, �� ��������� �� ���������� - ������, ����� ������ ��������
			curH = curH->brother;					//���� ������ ��� ������ ����� ������, �� ���������� ������ (4 ������ ���� �� �����)
		}
		else if (curH->degree == curH->brother->degree)		//���� 2, �� ����������� ���� � �������
		{
			if (curH->key <= curH->brother->key)			//������, ��� ������ ������, ������ ��������� ����
			{
				curH->degree++;
				node* curHChild = curH->child;
				curH->brother->p = curH;
				curH->child = curH->brother;
				curH->brother = curH->brother->brother;
				curH->child->brother = curHChild;
			}
			else
			{
				curH->brother->degree++;
				node* curHBrotherChild = curH->brother->child;
				curH->p = curH->brother;
				curH->brother->child = curH;
				if (prev)								//���� �� �� ������ � ������
					prev->brother = curH->brother;
				else									//����� �� ������
					H->root = curH->brother;
				curH->brother = curHBrotherChild;
				curH = curH->p;								//������������ � �������� ������
			}
		}
		else
		{
			prev = prev ? prev->brother : curH;					//���� ��� ��� ������ �������, �� ��������� �� ���������� - ������, ����� ������ ��������
			curH = curH->brother;						//� ��, ���������� ������ ���� ��������� ������ ��������, ����� ������������� �������
		}
	}
	supersede(H);
	b->root = NULL;
	delete b;
	return a;
}

void binHeap::supersede(binHeap* source)
{
	root = source->root;
	source->root = NULL;
	delete source;					//�������, � ��� ��������? ������ ��������� ���� �� ���� ��� �� � ����
}

node* binHeap::insert(int data)
{
	return insert(node(data));
}
node* binHeap::insert(node data)
{
	node* ins = new node(data.key);
	binHeap* heap = new binHeap();					//������ ����� ���� �� 1 �������� � �������
	heap->root = ins;
	merge(heap);		//����� ���� heap ��� ������������ ��������, ��� ��� ������� ������ ������
	return ins;
}

node* binHeap::extractMin()
{ 
	if (!root)
		return 0;
	else
	{
		node* waste = getMinimum();							//������� ����������� �������
		if (waste == root)
		{
			if (root->brother)
				root = root->brother;
			else
				root = 0;
		}
		else
			leftRoot(waste)->brother = waste->brother;			//��������� waste �� ����
		node* wasteChild = waste->child;
		if (wasteChild)
		{
			node* childSister = 0;									//������ (����� ����)
			do												//��������� ���� ������ ���� ����������� �� ������� �����,
			{												//������ ������� ���������� �������� � �������� �� ��������
				node* brother = wasteChild->brother;
				wasteChild->p = 0;
				wasteChild->brother = childSister;
				childSister = wasteChild;
				wasteChild = brother;
			} while (wasteChild);					//��� ����� wasteChild ���������� NULL
			binHeap* ins = new binHeap(childSister);			//��� ������ ������ �������, ������ �������� wasteChild
			merge(ins);											//������� � ���, ��� ���� ��� waste
		}
		return waste;
	}
}

void binHeap::setNodeKey(node* place, int value)
{
	if (value > place->key)
	{
		place->key = value;
	}
	else
	{
		place->key = value;
		while (place->p && place->key < place->p->key)		//���� ���� �������� � ������� ������
		{
			swap(place, place->p);
			place = place->p;
		}
	}
}

void binHeap::remove(int key)
{
	removeNode(search(key));
}

void binHeap::removeNode(node* waste)
{
	if (waste != 0)
	{
		int minimum = getMinimum()->key - 1;				//������, ���� ����������� ����� �������������. �� �� �������� ��������� �� �������� ������ ������������
		setNodeKey(waste, minimum);
		delete extractMin();								//������, ����� ��������� �����
	}
}

node* binHeap::search(int key)
{
	return search(root, key);
}

/*
		��������� ������������, ��� �������� � �������� ������������ ������ � ���� - ��, ��� ������� �������� 
		��������� ���� �������, ������ �� ����� ����� ������ ������ ����������� ��������� (�� ������� �� �����
		���������, ������� ��������)
*/

node* binHeap::search(node* searchStart, int key)
{
	if (!searchStart)
		return 0;
	if (searchStart->key == key)
		return searchStart;
	else
	{
		node* found = search(searchStart->brother, key);			//����� ������� ����� ���� � ������� ������
		if (found)
			return found;
		else if (searchStart->key < key)							//� ����� ����� �� �����
			return search(searchStart->child, key);
		else
			return 0;
	}
}

void binHeap::swap(node* node1, node* node2)
{
	int key = node1->key;
	node1->key = node2->key;
	node2->key = key;
}

void binHeap::clear()
{
	while (root)							//������ ������� ��� �������� ������ ����, � ����� � ����
		delete extractMin();
}

binHeap::~binHeap()
{
	clear();
}
