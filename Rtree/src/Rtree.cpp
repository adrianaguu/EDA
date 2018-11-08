#include "Rtree.h"

Rtree::Rtree()
{
    root=new node;
    m=0;
}

Rtree::Rtree(unsigned int max_children)
{
    m=max_children;
    root=new node;
}

bool Rtree::overlap(point p, point gmin, point gmax)
{
    if(p.first>gmin.first && gmax.first>p.first && p.second>gmin.second && gmax.second>p.second)
        return 1;
    return 0;
}

bool Rtree::findpointleaf (node**& p,point d)
{
    if ((*p)->data!=0){
        if ((*(*p)->data)==d)
            return 1;
        return 0;
    }

    vector<node**> possibleleafs;
    for (int unsigned i=0;i<(*p)->children.size();i++)
    {
        if(overlap(d,(*p)->children[i]->cmin,(*p)->children[i]->cmax))
            possibleleafs.push_back(&((*p)->children[i]));
    }
    cout<<possibleleafs.size();
    for(int unsigned j=0;j<possibleleafs.size();j++)
    {
            p=possibleleafs[j];
            if(findpointleaf(p,d)){
                break;
            }
    }

}

void Rtree::chooseleaf (point d, node**& p, stack<node**>& path)
{
    path.push(p);
    //cout<<"HIJOS: "<<(*p)->children.size()<<endl;
    if ((*p)->children.size()==0 || (*p)->children[0]->data!=0){
       // cout<<(*p)->data<<endl;
        return;
    }
   // cout<<"sale"<<endl;
    node** F=&((*p)->children[0]);
    for (int unsigned i=1;i<(*p)->children.size();i++)
    {
        if (distance_rec_point(d,(*p)->children[i]->cmin,(*p)->children[i]->cmax)<distance_rec_point(d,(*F)->cmin,(*F)->cmax))
            F=&((*p)->children[i]);
        else if  (distance_rec_point(d,(*p)->children[i]->cmin,(*p)->children[i]->cmax)==distance_rec_point(d,(*F)->cmin,(*F)->cmax))
        {
            if (area((*F)->cmin,(*F)->cmax)>area((*p)->children[i]->cmin,(*p)->children[i]->cmax))
                F=&((*p)->children[i]);
        }
    }
    p=F;
    chooseleaf(d,p,path);
  //  cout<<"DONE WELL CHoosE"<<endl;
}

void Rtree::rtinsert(point d)
{
    stack<node**> path;
    node** p=&root;
    chooseleaf(d,p,path);
    (*p)->children.push_back(new node (d));
    (*p)->children[(*p)->children.size()-1]->set_rec();
    if ((*p)->children.size()>m)
        {
            splitnode(p,path);
        }
    adjusttree(p,path);

   cout<<d.first<<","<<d.second<<" added."<<endl;
}

void Rtree::adjusttree (node**& p, stack<node**>& path)
{
   // cout<<path.size()<<endl;
  // cout<<path.size()<<endl;
  (*path.top())->set_rec();
    if((*path.top())->children.size()>m){
        splitnode(path.top(),path);
    (*path.top())->set_rec();
    }
    if(p==&root)
        return;
    path.pop();
    p=path.top();
    adjusttree(p,path);
    //cout<<"DONE WELL AJUST"<<endl;
}

void Rtree::splitnode (node**& p, stack<node**>& path)
{
    int i,j;
    node *l,*r;
    vector <node*> temp=(*p)->children;
    pickseeds(i,j,p);
    l=new node;
    l->children.push_back((*p)->children[i]);
    l->set_rec();
    r=new node;
    r->children.push_back((*p)->children[j]);
    r->set_rec();
    temp.erase(temp.begin()+j);
    temp.erase(temp.begin()+i);
    typecor d1,d2;
    for (int unsigned k=0;k<(*p)->children.size()-2;k++)
    {
        if (l->children.size()>(m/2))
        {
            r->children.insert(r->children.begin(),temp.begin(),temp.end());
            r->set_rec();
            break;
        }
        else if (r->children.size()>(m/2))
        {
            l->children.insert(l->children.begin(),temp.begin(),temp.end());
            l->set_rec();
            break;
        }
        picknext(i,p,temp,l,r,d1,d2);
        if(d1<d2)
        {
            l->children.push_back(temp[i]);
            l->set_rec();
        }
        else if(d2<d1)
        {
            r->children.push_back(temp[i]);
            r->set_rec();
        }
        else
        {
            if(area(l->cmin,l->cmax)>area(r->cmin,r->cmax))
            {
                r->children.push_back(temp[i]);
                r->set_rec();
            }
            else if (area(l->cmin,l->cmax)<area(r->cmin,r->cmax))
            {
                l->children.push_back(temp[i]);
                l->set_rec();
            }
            else
            {
                if(l->children.size()<r->children.size())
                {
                    l->children.push_back(temp[i]);
                    l->set_rec();
                }
                else
                {
                    r->children.push_back(temp[i]);
                    r->set_rec();
                }
            }

        }
        temp.erase(temp.begin()+i);
    }


    if(p==&root)
    {
        node* aux=new node;
        aux->children.push_back(l);
        aux->children.push_back(r);
        root=aux;
    }
    else
    {
        path.pop();
        for (int unsigned e=0;e<(*path.top())->children.size();e++){
            if((*path.top())->children[e]==*p)
            {
                (*path.top())->children.erase((*path.top())->children.begin()+e);
            }
        }
        //(*path.top())->children.pop_back();
        (*path.top())->children.push_back(l);
        (*path.top())->children.push_back(r);
        (*path.top())->set_rec();
        p=path.top();
        if((*path.top())->children.size()>m)
        {
            splitnode(p,path);
        }

    }
}

void Rtree::bigger_Rec(point Amin,point Amax,point Bmin, point Bmax, point& Cmin, point& Cmax)
{
    Cmin=make_pair(min(Amin.first,Bmin.first),min(Amin.second,Bmin.second));
    Cmax=make_pair(max(Amax.first,Bmax.first),max(Amax.second,Bmax.second));
}

void Rtree::picknext (int& ii, node** p, vector<node*>&temp,node* l, node* r,typecor& d1f,typecor& d2f)
{
    typecor difference=0;
    typecor d1,d2;
    ii=0;
    for (int unsigned k=0;k<temp.size();k++)
    {
        point cmin,cmax;

        bigger_Rec(l->cmin,l->cmax,temp[k]->cmin,temp[k]->cmax,cmin,cmax);
        d1=area(cmin,cmax);
        d1=d1-area(l->cmin,l->cmax);

        bigger_Rec(r->cmin,r->cmax,temp[k]->cmin,temp[k]->cmax,cmin,cmax);
        d2=area(cmin,cmax);
        d2=d2-area(r->cmin,r->cmax);
        if (fabs(d2-d1)>difference)
        {
            ii=k;
            d1f=d1;
            d2f=d2;
        }
    }

}

void Rtree::pickseeds (int& ii, int& jj ,node** p)
{
    typecor maxArea=0;
    typecor aux;
    point cmin,cmax;
    ii=0;
    jj=1;
    for(int unsigned i=0;i<(*p)->children.size();i++)
            for(int unsigned j=i+1; j<(*p)->children.size();j++ )
                {
                    bigger_Rec((*p)->children[i]->cmin,(*p)->children[i]->cmax,(*p)->children[j]->cmin,(*p)->children[j]->cmax,cmin,cmax);
                    //cout<<cmin.first<<","<<cmin.second<<" "<<cmax.first<<","<<cmax.second<<endl;
                    aux=area(cmin,cmax)-area((*p)->children[i]->cmin,(*p)->children[i]->cmax)-area((*p)->children[j]->cmin,(*p)->children[j]->cmax);
                 ///   cout<<"aux: "<<aux<<endl;
                    if (aux>maxArea)
                    {
                        maxArea=aux;
                        ii=i;jj=j;
                    }
                }
}


typecor Rtree :: distance_points (point A, point B)
{
    typecor x = pow(A.first-B.first,2);
    typecor y = pow(A.second-B.second,2);
    return sqrt(x+y);
}
typecor Rtree :: area (point cmin, point cmax)
{
    return fabs((cmax.first-cmin.first)*(cmax.second-cmin.second));
}

typecor Rtree :: distance_rec_point(point d,point cmin, point cmax)
{
    point center = make_pair(cmax.first-((cmax.first-cmin.first)/2),cmax.second-((cmax.second-cmin.second)/2));
    return distance_points(center,d);
}


Rtree::~Rtree()
{
    //dtor
}
