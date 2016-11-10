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
    RBtreeNode<K,T>* minimum(){
        RBtreeNode<K,T>* tmp = root;
        while (tmp->leftSon!= nullptr){
            tmp = tmp->leftSon;
        }
        return tmp;
    }
    //template <class K,class T>
    RBtreeNode<K,T>* maximum(){
        RBtreeNode<K,T>* tmp = root;
        while (tmp->rightSon!= nullptr){
            tmp = tmp->rightSon;
        }
        return tmp;
    }
    void deleteFixup(RBtreeNode<K,T>* x){

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

};










#endif //RBTREE_RBTREETEMP_H
