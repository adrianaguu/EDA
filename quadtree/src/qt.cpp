#include "qt.h"

qt::qt()
{
    //ctor
}

qt::qt(int ts, int length_ejex, int length_ejey)
{
    treesheld=ts;
    ejex=length_ejex;
    ejey=length_ejey;
    root=new node(make_pair(ejex*-1,ejey*-1),make_pair(ejex,ejey));
}

typecor qt:: distance (point A, point B)
{
    typecor x= pow(A.first-B.first,2);
    typecor y = pow(A.second-B.second,2);
    return sqrt(x+y);
}

void qt::circle(node* p,vector <point>& points,point center,typecor radio)
{
    typecor left, right,down, top;
    right=center.first+radio;
    top=center.second+radio;
    down=center.second-radio;
    left=center.first-radio;

    vector <node*> rectangles;
    if(p->children[0]==0)
    {
        for(int unsigned i=0;i<p->data.size();i++)
        {
            typecor dis=distance(center,p->data[i]);
            if(dis<=radio)
            points.push_back(p->data[i]);
        }
        return;
    }
    for(int i=0;i<4;i++)
    {
        if(p->children[i]->cmin.first>right || p->children[i]->cmin.second>top || p->children[i]->cmax.first<left || p->children[i]->cmax.second<down)
            continue;
        rectangles.push_back(p->children[i]);
    }

    for(int unsigned i=0; i<rectangles.size();i++)
    {
            circle(rectangles[i],points,center,radio);
    }

}

void qt:: addpoint (point A)
{
    node** p;
    qtfind(A,p);
    if(findpoint(A))
        return;
    (*p)->data.push_back(A);
    cout<<A.first<<","<<A.second<<" added."<<endl;
   // cout<<"data size: "<<(*p)->data.size()<<endl;
    if((*p)->data.size()>treesheld)
    {

     //   cout<<"split"<<endl;
        split(p);
    }
}

void qt:: qtfind (point A, node**& p)
{

    p=&root;
    int ind=-1;
     typecor dis=(*p)->cmax.first-(*p)->cmin.first;
    // cout<<(*p)->cmin.first;
     dis/=2;
     //cout<<"dis"<<dis;
    while ((*p)->children[0]!=0)
    {
        dis=(*p)->cmax.first-(*p)->cmin.first;
        dis/=2;
        ind=-1;
        if (A.first>=(*p)->cmax.first-dis){
            ind+=2;
           // cout<<"AX: "<<(*p)->cmax.second-dis<<endl;
        }
        else
        {
            ind++;
        }
        if (A.second>=(*p)->cmax.second-dis)
        {
            //cout<<"THE: "<<A.second<<endl;
            ind+=2;
            //cout<<"1"<<endl;
        }
        p=&((*p)->children[ind]);
        //cout<<"indice: "<< ind<<endl;
    }
    //cout<<"ind: "<<ind<<",x: "<<A.first<<",y: "<<A.second<<endl;
}

void qt:: split (node**& rectangle )
{
   // cout<<"SPLIt";
    typecor dis=(*rectangle)->cmax.first-(*rectangle)->cmin.first;
    dis/=2;
    int ind=0;
    typecor miniX,miniY;
    miniX=(*rectangle)->cmin.first;
    miniY=(*rectangle)->cmin.second;
    for(int i=0;i<2;i++)
    {
        for (int j=0;j<2;j++)
        {
            (*rectangle)->children[ind]=new node(make_pair(miniX+(dis*j),miniY+(dis*i)),make_pair(miniX+(dis+dis*j),miniY+(dis+dis*i)));
            /*cout<<"minx: "<<(*rectangle)->children[ind]->cmin.first<<endl;
             cout<<"miny: "<<(*rectangle)->children[ind]->cmin.second<<endl;
              cout<<"maxx: "<<(*rectangle)->children[ind]->cmax.first<<endl;
               cout<<"maxy: "<<(*rectangle)->children[ind]->cmax.second<<endl;*/
            ind++;
        }
    }
   /* cout<<"rec: ";
     for(int unsigned i=0;i<(*rectangle)->data.size();i++)
            cout<<(*rectangle)->data[i].first<<","<<(*rectangle)->data[i].second<<"--";
    cout<<endl;
    node **p;*/
    for(int unsigned i=0;i<(*rectangle)->data.size();i++)
    {


        //qtfind((*rectangle)->data[i],p);
        //(*p)->data.push_back((*rectangle)->data[i]);
        addpoint((*rectangle)->data[i]);
    }
    (*rectangle)->data.resize(0);

}

bool qt::findpoint(point A)
{
    node**p;
    qtfind(A,p);
    for(int unsigned i=0;i<(*p)->data.size();i++)
        if (A==(*p)->data[i])
            return 1;
    return 0;
}

qt::~qt()
{
    //dtor
}
