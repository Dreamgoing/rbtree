#include <iostream>
#include "RBtreeNode.h"
#include "RBtree.h"
#include <cstdlib>
#include <ctime>
using namespace std;

#define SHOW_INSERT
void testTemplateBinarytree_Insert(){
    //RBtreeNode<string,double> tree("hello",3.14);
    //tree.showDetail();

//    cout<<tree<<endl;
    ///cout<<tree<<endl; ///@bug 应该是显示层的bug
    srand((unsigned)time(NULL));
    RBtree<int,string> rb;
    for(int i = 0;i<100;i++){
        int x = rand()%1000+1;
        //cout<<x<<endl;
        bool tmp=true;
        tmp = rb.insert(x, "ok");
        if(!tmp){
            cout<<"insert Error ! "<<x<<endl;
            //break;
        }
    }
    rb.showInorderTraversal();
}
void testTemplateRBtree_Insert(){
//    testTemplateBinarytree_Insert();
    ///@bug 出现bug
    RBtree<int,string> rb;
    rb.insert(2,"3");
    rb.insert(4,"2");
    rb.insert(1,"1");
    rb.insert(1,"2");
    rb.insert(0,"0");
    rb.insert(-1,"-1");
    rb.insert(5,"5");
    rb.showInorderTraversal();
}
void testTemplateRBtree_Final(){
    RBtree<double,string> rb;
    for(int i = 0;i<100;i++){
        int tmpx = i;
        rb.insert((double)tmpx+0.01*i,"hello "+to_string(i));
    }
#ifdef SHOW_INSERT
    rb.showInorderTraversal();
    rb.clear();
    cout<<"clear test successful"<<endl;
    rb.showInorderTraversal();
#endif
    RBtree<int,string> ra;
    for(int i = 0;i<100;i++){
        ra.insert(i,"ok");
    }
    ra.insert(111,"tianmao");
    cout<<ra[1]<<endl;
    cout<<ra[111]<<endl;
    ra[1] = "no";
    cout<<ra[1]<<endl;
    for(int i = 0;i<20;i++){
        ra.erase(i+10);
    }
    ra.showInorderTraversal();

}
int main() {
    //testTemplateBinarytree_Insert();
//    testTemplateRBtree_Insert();
    testTemplateRBtree_Final();

    return 0;
}