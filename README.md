# 红黑树
>一种有特殊平衡条件的平衡二叉树

## 红黑树性质
 * 每个结点是红色或者黑色
 * 根结点是黑色的
 * 每个叶结点是黑色的
 * 如果一个结点是红色的，则它的两个子结点都是黑色的
 * 对于每个结点，从该结点到其所有后代叶结点的简单路径上面，均包含相同数目的黑色结点
 * 新增结点必为红色
 
 
## 实现
+ c++ template 泛型编程实现
+ doxygen 文档注释，便于阅读
+ 实现功能，基本插入，删除，查找，重载了[]运算符,并保证了每个叶结点的黑高相等。


## 参考文档
[mit红黑树ppt](https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-046j-introduction-to-algorithms-sma-5503-fall-2005/video-lectures/lecture-10-red-black-trees-rotations-insertions-deletions/lec10.pdf)

[红黑树伪代码实现](https://www.cs.auckland.ac.nz/software/AlgAnim/red_black.html)

[red black tree](http://www.stolerman.net/studies/cs521/red_black_trees.pdf)

《算法导论》

[可视化算法学习网站](https://www.cs.usfca.edu/~galles/visualization/RedBlack.html)


