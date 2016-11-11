//
// Created by 王若璇 on 16/11/10.
//
#ifndef RBTREE_RBTREETEMP_H
#define RBTREE_RBTREETEMP_H


#include "RBtreeNode.h"
#include <iostream>
#include <cassert>
using namespace std;



template <class K,class T>
class RBtree {
    ///@brief 代码的艺术性重命名
    ///@todo 研究protected成员变量
    ///@note 为了解决边界问题，使用nil指针

private:
    RBtreeNode<K,T> *nil;
    RBtreeNode<K,T> *root;
    size_t _size = 0;
public:
    ///@note
    /// can't separate the implementation and header of a template class anyway.
    ///@todo 以后测试完全之后可以改成返回pair类型
    //template <class K,class T>
    RBtree<K,T>(){
        nil = new RBtreeNode<K,T>(RBtreeNode<K,T>::BLACK);
        nil->rightSon = nil->leftSon = nil->parent = nil;
        root = nil;
        _size = 0;
        //root = nullptr;

    }
    bool empty() const{
        return _size==0;
    }
    pair<K,T> findPair(const K& key)const{
        RBtreeNode<K,T>* res = treeSearch(key);
        if(res== nullptr||res==nil){
            return make_pair(key,nil->value);
        } else{
            return make_pair(key,res->value);
        }
    };
    T& find(const K& key)const {
        RBtreeNode<K,T>* res = treeSearch(key);
        return res->value;
    }
    bool change(const K& key, const T& value){
        RBtreeNode<K,T>* res = treeSearch(key);
        if(res== nullptr||res==nil){
            return false;
        } else{
            res->value = value;
            return true;
        }
    }
    size_t size() const {
        return _size;
    }
    void showInorderTraversal(){
        inorderTraversal(root);
    }

    ///@brief 函数的多态
    bool insert(const K& key,const T& value){
        RBtreeNode<K,T> insertTerm(key,value);
        pair<RBtreeNode<K,T>*,bool> pos = treeInsert(insertTerm);
        if(pos.second){
            //cout<<"pos "<<*pos.first<<endl;
            insertFixup(pos.first);
        }
        /*cout<<"res"<<endl;
        showInorderTraversal();*/
        return pos.second;

    }

    bool insert(const pair<K,T>& p){
        return insert(p.first,p.second);
    }
    bool erase(const K& key){
        pair<RBtreeNode<K,T>*,bool> pos =  treeDelete(key);
        return pos.second;
    }
    ~RBtree(){
        treeDestory(root);
        delete nil;
    }
    void clear(){
        ///@brief 释放内存空间
        treeDestory(root);
        root = nil;
        nil->rightSon = nil->leftSon = nil->parent = nil;
        nil->color = RBtreeNode<K,T>::BLACK;
    }
    const T& operator[](const K& key) const{
        return find(key);
    }
    T& operator[](const K& key){
        return const_cast<T&>(
                dynamic_cast<const RBtree&>(*this)[key]);
    }




private:
    /**
     * @brief 内置二叉树的插入
     *        tree prefix 都是基于二叉树实现的内部函数
     * @return 插入成功返回，插入的位置的指针，true
     *         插入失败，则返回nullptr false；
     * */
    pair<RBtreeNode<K,T>*,bool> treeInsert(const RBtreeNode<K,T>& node){
        RBtreeNode<K,T>* tmp = root;
        RBtreeNode<K,T>* par = root;
        while (tmp!= nil){
            par = tmp;
            if(node.key>tmp->key){
                tmp = tmp->rightSon;
            }else if(node.key<tmp->key){
                tmp = tmp->leftSon;
            } else{
                return make_pair(nullptr, false);
            }
        }
        RBtreeNode<K,T>* add = new RBtreeNode<K,T>(node);
        add->rightSon = add->leftSon = add->parent = nil;
        ///@brief 插入结点是根结点
        if(tmp==root){
            root = add;
            root->color = RBtreeNode<K,T>::BLACK;
            root->parent = nil;
            tmp = root;
        } else{
            if(node.key>par->key){
                par->rightSon = add;
                //cout<<"rightSon "<<*par->rightSon<<endl;
            } else{
                par->leftSon = add;
            }
            add->parent = par;
            tmp = add;
        }
        _size++;
        return make_pair(tmp,true);
    }

    /** @brief 插入修正有三种情况
     *  @def x 为当前结点
     * + case1: x的叔结点为红色
     * + case2: x的叔结点为黑色,x是一个左儿子
     * + case3: x的叔结点为黑色,x是一个右儿子
     */

    void insertFixup(RBtreeNode<K,T>* node){
        ///@brief 1.在普通插入之后进行修正
        ///@note 当前结点的颜色为RED
        ///@def node 为当前结点，y结点为当前结点的叔结点
        RBtreeNode<K,T>* y;
        while (node!=root&&node->parent->color==RBtreeNode<K,T>::RED){
            ///@note 如果node的父结点为左结点，则y(叔结点)为右结点
            if(node->parent==node->parent->parent->leftSon){
                //cout<<*node<<" node"<<endl;
                y = node->parent->parent->rightSon;
                ///@note 边界问题，用nil处理
                assert(y!= nullptr);
                if(y->color==RBtreeNode<K,T>::RED){
                    ///@brief 叔结点y为红色 case1: 修改颜色，让当前结点node 上升
                    node->parent->color = RBtreeNode<K,T>::BLACK;
                    y->color = RBtreeNode<K,T>::BLACK;
                    node->parent->parent->color = RBtreeNode<K,T>::RED;
                    node = node->parent->parent;
                } else{
                    ///@brief 叔结点y为黑色 case2 to case3 or  case3.
                    if(node==node->parent->rightSon){
                        ///node为右儿子 case2: 使当前结点node上升，左旋
                        node = node->parent;
                        leftRotate(node);
                    }
                    ///case3: 改变颜色，右旋
                    node->parent->color = RBtreeNode<K,T>::BLACK;
                    node->parent->parent->color = RBtreeNode<K,T>::RED;
                    rightRotate(node->parent->parent);
                }
            } else{
                ///@brief 和上述一样，left=>right right=>left 交换
                y = node->parent->parent->leftSon;
                if(y->color==RBtreeNode<K,T>::RED){
                    ///case1:
                    node->parent->color = RBtreeNode<K,T>::BLACK;
                    y->color = RBtreeNode<K,T>::BLACK;
                    node->parent->parent->color = RBtreeNode<K,T>::RED;
                    node = node->parent->parent;
                } else{
                    ///case2 or case3
                    if(node==node->parent->leftSon){
                        node = node->parent;
                        rightRotate(node);
                    }
                    node->parent->color = RBtreeNode<K,T>::BLACK;
                    node->parent->parent->color = RBtreeNode<K,T>::RED;
                    leftRotate(node->parent->parent);
                }
            }

        }
        root->color = RBtreeNode<K,T>::BLACK;
    }
    RBtreeNode<K,T>* treeSuccessor(RBtreeNode<K,T>* node){
        /***@brief 如果右子结点不为nil则，后继结点为右子结点的最小结点
         *         否则，向上求后继结点
         * */
        if(node->rightSon!=nil){
            return minimum(node->rightSon);
        }
        ///@brief 向上求后继结点
        RBtreeNode<K,T>* y = node->parent;
        while (y!=nil&&node==y->rightSon){
            node = y;
            y = y->parent;
        }
        return y;
    };
    pair<RBtreeNode<K,T>*,bool> treeDelete(const K& key){
        ///@brief 删除策略找到应该删除的位置，替换删除点的值和位置点的值
        RBtreeNode<K,T>* node = treeSearch(key);
        RBtreeNode<K,T>* z = nullptr;
        if(node==nil){
            ///@brief 没有找到
            cerr<<"Error : can't find the key to be deleted"<<endl;
            return make_pair(nullptr,false);
        }
        _size--;
        if(node->leftSon==nil||node->rightSon==nil){
            z = node;
        }else{
            ///@brief 如果z有两个结点，则设z为其后继结点
            z = treeSuccessor(node);
        }
        RBtreeNode<K,T>* y = nullptr;
        if(z->leftSon!=nil){
            y = z->leftSon;
        } else{
            ///@brief
            y = z->rightSon;
        }
        y->parent = z->parent;
        if(z->parent==nil){
            ///删除为根
            root = y;
        }else if(z==z->parent->leftSon){
            z->parent->leftSon = y;
        } else{
            z->parent->rightSon = y;
        }
        if(node!=z){
            node->key = z->key;
            node->value = z->value;
        }
        if(z->color==RBtreeNode<K,T>::BLACK){
            deleteFixup(y);
        }
        delete z;///删除z
        nil->parent = nil;
        return make_pair(y,true);
    }
    void deleteFixup(RBtreeNode<K,T>* x){
        while(x != root && x->color == RBtreeNode<K,T>::BLACK)
        {
            RBtreeNode<K,T>* w = NULL;
            if(x == x->parent->leftSon)//x是左孩子时
            {
                w = x->parent->rightSon;
                if(w->color == RBtreeNode<K,T>::RED)//当x的兄弟是红色的时候，通过旋转将其兄弟转换为黑色
                {
                    w->color = RBtreeNode<K,T>::BLACK;
                    x->parent->color = RBtreeNode<K,T>::RED;
                    leftRotate(x->parent);
                }
                //当x的兄弟是黑色时
                //当w的两个儿子都是黑色时
                if(w->leftSon->color == RBtreeNode<K,T>::BLACK && w->rightSon->color == RBtreeNode<K,T>::BLACK)
                {
                    w->color = RBtreeNode<K,T>::RED;//w颜色减去黑色
                    x = w->parent;//x上移
                }
                    //当w的左儿子是红色，右儿子是黑色时,把右儿子转换为红色
                else if(w->rightSon->color == RBtreeNode<K,T>::BLACK && w->leftSon->color == RBtreeNode<K,T>::RED)
                {
                    w->leftSon->color = RBtreeNode<K,T>::BLACK;
                    w->color = RBtreeNode<K,T>::RED;
                    rightRotate(w);
                    w = w->parent;
                }
                //w的右儿子是红色
                if(w->rightSon->color == RBtreeNode<K,T>::RED)
                {
                    w->color = w->parent->color;
                    w->parent->color = RBtreeNode<K,T>::BLACK;
                    w->rightSon->color = RBtreeNode<K,T>::BLACK;
                    leftRotate(w->parent);
                    x = root;
                }
            }//if(x == x->parent->leftSon)
            else//x是右孩子时
            {
                w = x->parent->leftSon;
                if(w->color == RBtreeNode<K,T>::RED)//当x的兄弟是红色的时候，通过旋转将其兄弟转换为黑色
                {
                    w->color = RBtreeNode<K,T>::BLACK;
                    x->parent->color = RBtreeNode<K,T>::RED;
                    rightRotate(x->parent);
                }
                //当x的兄弟是黑色时
                //当w的两个儿子都是黑色时
                if(w->leftSon->color == RBtreeNode<K,T>::BLACK && w->rightSon->color == RBtreeNode<K,T>::BLACK)
                {
                    w->color = RBtreeNode<K,T>::RED;//w颜色减去黑色
                    x = w->parent;//x上移
                }
                    //当w的右儿子是红色，左儿子是黑色时,把左儿子转换为红色
                else if(w->leftSon->color == RBtreeNode<K,T>::BLACK && w->rightSon->color == RBtreeNode<K,T>::RED)
                {
                    w->rightSon->color = RBtreeNode<K,T>::BLACK;
                    w->color = RBtreeNode<K,T>::RED;
                    leftRotate(w);
                    w = w->parent;
                }
                //w的左儿子是红色
                if(w->leftSon->color == RBtreeNode<K,T>::RED)
                {
                    w->color = w->parent->color;
                    w->parent->color = RBtreeNode<K,T>::BLACK;
                    w->leftSon->color = RBtreeNode<K,T>::BLACK;
                    rightRotate(w->parent);
                    x = root;
                }
            }//else
        }//while
        x->color = RBtreeNode<K,T>::BLACK;
        nil->color = RBtreeNode<K,T>::BLACK;//一开始x为nill时，会在第一次上移前把nill颜色设置为红色
    }
    
    void inorderTraversal(RBtreeNode<K,T>* node){
        if(node== nil){
            return;
        }
        inorderTraversal(node->leftSon);
        cout<<*node<<endl;
        inorderTraversal(node->rightSon);
    }

    ///@brief 左旋
    ///@param x为旋转轴点，也为y的父亲结点

    void leftRotate(RBtreeNode<K,T>* x){
        ///@brief 1.将y的左子树放到x的右子树上面
        RBtreeNode<K,T> *y = x->rightSon;
        x->rightSon = y->leftSon;
        if(y->leftSon!= nil){
            y->leftSon->parent = x;
        }
        ///@brief 2. y的新父亲结点为原来x的父亲结点
        y->parent = x->parent;
        ///@brief 3. 设置原来x的父亲结点的的儿子结点为y，而不是x (y替代x位置)

        if(x->parent== nil){
            root = y;
        } else{
            if(x==(x->parent->leftSon)){
                x->parent->leftSon = y;
            }else{
                ///@note x 肯定在右儿子结点
                x->parent->rightSon = y;
            }
        }
        ///@brief 4.让x在y的左儿子结点，设置x的父亲结点为y
        y->leftSon = x;
        x->parent = y;
        nil->parent = nil;

    }
    void rightRotate(RBtreeNode<K,T>* x){
        RBtreeNode<K,T> *y = x->leftSon;
        x->leftSon = y->rightSon;
        if(y->rightSon!= nil){
            y->rightSon->parent = x;
        }
        y->parent = x->parent;
        if(x->parent== nil){
            root = y;
        } else{
            if(x==(x->parent->rightSon)){
                x->parent->rightSon = y;
            } else{
                x->parent->leftSon = y;
            }
        }
        y->rightSon = x;
        x->parent = y;
        nil->parent = nil;

    }
    RBtreeNode<K,T>* minimum(RBtreeNode<K,T>* node){
        RBtreeNode<K,T>* tmp = node;
        while (tmp->leftSon!= nil){
            tmp = tmp->leftSon;
        }
        return tmp;
    }
    //template <class K,class T>
    RBtreeNode<K,T>* maximum(RBtreeNode<K,T>* node){
        RBtreeNode<K,T>* tmp = node;
        while (tmp->rightSon!= nil){
            tmp = tmp->rightSon;
        }
        return tmp;
    }

    RBtreeNode<K,T>* treeSearch(const K& dkey) const {
        RBtreeNode<K,T>* tmp = root;
        while (tmp!=nil&&dkey!=tmp->key){
            if(dkey>tmp->key){
                tmp = tmp->rightSon;
            }else if(dkey<tmp->key){
                tmp = tmp->leftSon;
            } else{
                break;
            }
        }
        return tmp;
    };
    void treeDestory(RBtreeNode<K,T>* node){
        if(node==nil||node== nullptr){
            return;
        }
        treeDestory(node->leftSon);
        treeDestory(node->rightSon);
        delete node;

    }

};










#endif //RBTREE_RBTREETEMP_H
