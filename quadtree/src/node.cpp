#include "node.h"

node::node()
{
    //ctor
}

node::node(point cmi, point cma)
{
    cmin=cmi;
    cmax=cma;
    children.resize(4);
    for (int i=0; i<4;i++)
    {
        children[i]=0;
    }
}

node::~node()
{
    //dtor
}
