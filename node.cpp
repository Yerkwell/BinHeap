#include "node.h"

node::node()
{
	key = 0;
	degree = 0;
}

node::node(int value)
{
	key = value;
	degree = 0;
}

node::~node()
{
}
