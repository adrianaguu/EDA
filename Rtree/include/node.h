#ifndef NODE_H
#define NODE_H
#include <utility>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
using namespace std;
typedef double typecor;
typedef pair<typecor,typecor> point;
#define dis 0.25

class node
{
    public:
        node();
        virtual ~node();
        point cmin, cmax;
        vector <node*> children;
        point* data;
        void update_rec (point mi, point ma);
        void set_rec ();
        node (point mi, point ma);
        node (point d);

    protected:

    private:
};



#endif // NODE_H
