#ifndef QT_H
#define QT_H
#include "node.h"
#include <iostream>
#include <math.h>


class qt
{
    public:
        qt();
        int ejex, ejey;
        qt(int ts, int length_ejex, int length_ejey);
        unsigned int treesheld;
        void qtfind (point A, node**& p);
        bool findpoint (point A);
        node* root;
        typecor distance (point A, point B);
        void circle(node* p,vector <point>& points,point center,typecor radio);
        void addpoint (point A);
        void split(node**& rectangle);
        virtual ~qt();

    protected:

    private:
};



#endif // QT_H
