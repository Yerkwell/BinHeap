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
		//Если одна из куч пустая, возвращаем вторую
	if (!b || !b->root)
		return a;
	if (!a->root)
	{
		supersede(b);					//Теперь эта куча имеет указатель на корень кучи b
		return a;
	}
	if (a == b)						//Если оба указателя указывают на одну и ту же кучу (!)
		return a;
	binHeap* H = new binHeap();		//Новая куча, образуемая при слиянии
	node* curA = a->root;
	node* curB = b->root;
	if (curA->degree <= curB->degree)				//Создаём новой куче корень
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
	//Совмещаем списки корней деревьев
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
	//Совместили
	//Теперь объединяем деревья одной степени
	node* prev = NULL;
	curH = H->root;
	while (curH->brother)
	{
		if ((curH->brother->brother) && (curH->degree == curH->brother->degree && curH->degree == curH->brother->brother->degree))	//Если подряд три дерева одной высоты
		{
			prev = prev ? prev->brother : curH;					//Если это был первый элемент, то указатель на предыдущий - первый, иначе просто сдвигаем
			curH = curH->brother;					//Если подряд три дерева одной высоты, то сдвигаемся дальше (4 подряд быть не может)
		}
		else if (curH->degree == curH->brother->degree)		//Если 2, то подвешиваем одно к другому
		{
			if (curH->key <= curH->brother->key)			//Дерево, чей корень меньше, должно оказаться выше
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
				if (prev)								//Если он не первый в списке
					prev->brother = curH->brother;
				else									//Иначе он корень
					H->root = curH->brother;
				curH->brother = curHBrotherChild;
				curH = curH->p;								//Возвращаемся в корневой список
			}
		}
		else
		{
			prev = prev ? prev->brother : curH;					//Если это был первый элемент, то указатель на предыдущий - первый, иначе просто сдвигаем
			curH = curH->brother;						//И да, сдвигаемся только если следующий больше текущего, иначе перепроверяем текущий
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
	delete source;					//Жестоко, а что поделать? Лишние указатели куда не надо нам ни к чему
}

node* binHeap::insert(int data)
{
	return insert(node(data));
}
node* binHeap::insert(node data)
{
	node* ins = new node(data.key);
	binHeap* heap = new binHeap();					//Создаём новую кучу из 1 элемента и сливаем
	heap->root = ins;
	merge(heap);		//Новая куча heap там спокойненько удалится, так что никаких утечек памяти
	return ins;
}

node* binHeap::extractMin()
{ 
	if (!root)
		return 0;
	else
	{
		node* waste = getMinimum();							//Находим минимальный элемент
		if (waste == root)
		{
			if (root->brother)
				root = root->brother;
			else
				root = 0;
		}
		else
			leftRoot(waste)->brother = waste->brother;			//Исключаем waste из кучи
		node* wasteChild = waste->child;
		if (wasteChild)
		{
			node* childSister = 0;									//Сестра (левый брат)
			do												//Поскольку куча должна быть упорядочена по степени корня,
			{												//Меняем порядок следования деревьев и обнуляем им родителя
				node* brother = wasteChild->brother;
				wasteChild->p = 0;
				wasteChild->brother = childSister;
				childSister = wasteChild;
				wasteChild = brother;
			} while (wasteChild);					//Под конец wasteChild становится NULL
			binHeap* ins = new binHeap(childSister);			//Тут сейчас корень подкучи, бывшей дочерней wasteChild
			merge(ins);											//Сливаем с тем, что было под waste
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
		while (place->p && place->key < place->p->key)		//Пока есть родитель с большим ключом
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
		int minimum = getMinimum()->key - 1;				//Вообще, надо присваивать минус бесконечность. Но мы обойдёмся значением на единичку меньше минимального
		setNodeKey(waste, minimum);
		delete extractMin();								//Нельзя, чтобы указатель повис
	}
}

node* binHeap::search(int key)
{
	return search(root, key);
}

/*
		Поскольку единственное, что известно о взаимном расположении ключей в куче - то, что меньшие элементы 
		находятся выше больших, искать по ключу можно только полным рекурсивным перебором (не проходя по детям
		элементов, больших искомого)
*/

node* binHeap::search(node* searchStart, int key)
{
	if (!searchStart)
		return 0;
	if (searchStart->key == key)
		return searchStart;
	else
	{
		node* found = search(searchStart->brother, key);			//Среди братьев может быть с меньшим ключом
		if (found)
			return found;
		else if (searchStart->key < key)							//А среди детей не может
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
	while (root)							//Сперва стираем все элементы внутри кучи, а потом и саму
		delete extractMin();
}

binHeap::~binHeap()
{
	clear();
}
