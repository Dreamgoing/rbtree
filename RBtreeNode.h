//
// Created by 王若璇 on 16/11/9.
//

#ifndef RBTREE_RBTREE_H
#define RBTREE_RBTREE_H


/**
 * @brief 红黑树性质
 * @author ruoxuanwang
 * @brief node有5个属:color,key,left,right,value
 * 如果一个节点没有子结点，或者父结点，则对应指为NIL
 * @note 性质
 * + 每个结点是红色或者黑色
 * + 根结点是黑色的
 * + 每个叶结点是黑色的
 * + 如果一个结点是红色的，则它的两个子结点都是黑色的
 * + 对于每个结点，从该结点到其所有后代叶结点的简单路径上面，均包含相同数目的黑色结点
 * + 新增结点必为红色
 * @param nil
 * */
#include <iostream>
#include <algorithm>

//#define RBtreeNode RBtreeNode
using namespace std;

///@todo 思考是不是可以在不同的文件中定义两个类
template<class K,class T> class RBtree;

template<class K, class T>
class RBtreeNode{
    ///@brief 模版类中声明为友元。
    ///@note wrong friend template <class K,class T> class RBtree;
    ///@todo 学习声明为友元类的作用
    friend class RBtree<K,T>;

public:
    ///@brief 枚举量color声明为private
    enum Color{
        RED = 0,BLACK = 1
    }color;
    K key;
    T value;
    RBtreeNode<K,T> *rightSon;
    RBtreeNode<K,T> *leftSon;
    RBtreeNode<K,T> *parent;

public:


    ///@brief explicit 声明构造函数防止被隐式转换
    explicit RBtreeNode(const K& _key,const T& _value):key(_key),value(_value){
        color = RED;
        leftSon = rightSon = parent = nullptr;
    }
    explicit RBtreeNode(){
        color = RED;
        leftSon = rightSon = parent = nullptr;
    }
    explicit RBtreeNode(const Color& _color){
        color = _color;
        leftSon = rightSon = parent = nullptr;
    }
    explicit RBtreeNode(const K& _key,const T& _value,const Color& _color):key(_key),value(_value){
        color = _color;
        leftSon = rightSon = parent = nullptr;
    }

    ///@note 正确书写模版编程
    RBtreeNode<K,T>& operator=(const RBtreeNode<K,T>& rhs){
        ///@brief identity test 证同测试
        if(this==&rhs){
            return *this;
        } else{
            key(rhs.key);
            value(rhs.value);
            color = rhs.color;
            this->parent = rhs.parent;
            this->leftSon = rhs.leftSon;
            this->rightSon = rhs.rightSon;
            return *this;
        }

    }
    friend ostream& operator<<(ostream& os,const RBtreeNode<K,T>& rbtreeNode) {
        os<<"key = "<<rbtreeNode.key<<"  value = "<<rbtreeNode.value;
        return os;
    }
    bool operator<(const RBtreeNode<K,T> rhs)const{
        return this->key<rhs.key;
    }
    //  void showDetail();
};






#endif //RBTREE_RBTREE_H
