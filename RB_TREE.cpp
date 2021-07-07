/***
 *   1.实现了红黑树的插入操作
 *   2.实现了红黑树的删除操作
 *   @author: air 
 ***/

#include<vector>
#include <iostream>
using namespace std;

enum Color {
	RED = 0,
	BLACK = 1
};
struct rbTree {
	int data;
	//int key;
	Color color;
	rbTree* left, * right, * parent;
	rbTree(int data = 0,Color color = RED,rbTree* left = nullptr, rbTree* right = nullptr, rbTree* parent = nullptr) :data(data),color(color),left(left),right(right),parent(parent){}
};
void leftRoate(rbTree* &root) {

	auto parent = root->parent;
	auto tp_node = root->right;             
	if (parent!=nullptr)             // if parent is not none,then let root's right child be parent's child
		if (root == parent->left) {    // if root is parent's left child
			parent->left = tp_node;
			tp_node->parent = parent;
		}
		else {                               // if root is parent's right child
			parent->right = tp_node;
			tp_node->parent = parent;
		}
	if (tp_node->left != nullptr) {
		root->right = tp_node->left;  // root's right child links to root's right-child's left child   
		root->right->parent = root;
	}

	tp_node->left = root;                  // original root-right-child's left links to root 
	root->parent = tp_node;
}
void rightRoate(rbTree* &root) {
	auto parent = root->parent;
	auto tp_node = root->left;
	if (parent != nullptr)           // if parent is not none,then let root's left child be parent's child
		if (root == parent->left) {    // if root is parent's left child
			parent->left = tp_node;
			tp_node->parent = parent;
		}
		else {                               // if root is parent's right child
		    parent->right = tp_node;
			tp_node->parent = parent;
		}
	if (tp_node->right != nullptr) {
		root->left = tp_node->right;       // root's left child links to root's left-child's right child   
		root->left->parent = root;
	}


	tp_node->right = root;                // original root-left-child's right links to root 
	root->parent = tp_node;
}

// 插入调整
void adjust(rbTree* &root) {
	// （1）root为根

	if (root->parent == nullptr) {  
		root->color = BLACK;
		return;
	}

	// （2）root的父节点为黑

	if (root->parent->color == BLACK)
		return;

	// (3)root的父结点为红

	if (root->parent->color == RED) {
		auto uncle = root->parent->parent->left == root->parent ? root->parent->parent->right : root->parent->parent->left;
		//（3）root的父叔皆红
		if (uncle->color == RED) {
			root->parent->color = BLACK;
			uncle->color = BLACK;
			root->parent->parent->color = RED;
			adjust(root->parent->parent);
		}
	
		else if (uncle->color == BLACK) {
			// (4)root父红叔黑且root为右子
			// 父左旋调整父 

			if (root == root->parent->right) {  
				auto tp_node4 = root->parent;
				leftRoate(root->parent);
				adjust(tp_node4);
			}
			// (4)root父红叔黑且root为左子
			// 父黑爷红爷右旋 

			if (root == root->parent->right) {
				auto tp_node5 = root->parent;
				root->parent->color = BLACK;
				root->parent->parent->color = RED;
				rightRoate(root->parent->parent);
			//	adjust(tp_node5);
			}
		}
	}

}

//root 的 reference含有root这个指针变量的地址 
void insertRB(rbTree* pa,rbTree* &root, int data) {
	if (root == nullptr) {
		root = new rbTree(data);
		root->parent = pa;
		adjust(root);
		return ;
	}
	if (data < root->data)
		insertRB(root,root->left, data);
	else if(data>root->data){
		insertRB(root,root->right, data);
	}
	return;
}

// 删除调整
void deleteFixUpBlackNode(rbTree*& xnode) {
	if (xnode->parent == nullptr)  //（1）x指向根
		return;
	if (xnode->color == RED)       //（2）x为红 R+B
		return;
	//  X为黑 B+B
	auto parent = xnode->parent;
	auto bro_node = xnode == parent->left ? parent->right : parent->left;
	//  （3）X黑兄弟红
	if (bro_node->color == RED) {
		bro_node->color = BLACK;
		parent->color = RED;
		leftRoate(parent);
		deleteFixUpBlackNode(xnode);       //重新设置X的兄弟结点
	}
    // X黑兄弟黑
	if (bro_node->color == BLACK) {
		// X黑兄弟黑，兄弟右子黑
		if ((bro_node->right != nullptr && bro_node->right->color == BLACK)|| bro_node->right == nullptr){
			//（4）X黑兄弟黑，兄弟孩子全黑
			if ((bro_node->left != nullptr && bro_node->left->color == BLACK) || bro_node->left == nullptr) {
				bro_node->color = RED;
				deleteFixUpBlackNode(parent);
			}
			//  （5）X黑兄弟黑，兄弟右孩黑 左孩红
			else{	
				bro_node->left->color = BLACK;
				bro_node->color = RED;
				rightRoate(bro_node);
				deleteFixUpBlackNode(xnode);  // 重新设置X的兄弟结点
			}

		 }
		// （6）X黑兄弟黑，兄弟右孩红 左孩任意
		else {
			bro_node->color = parent->color;
			parent->color = BLACK;
			bro_node->right->color = BLACK;
			leftRoate(parent);
			// 设置X为根结点，目的是跳出循环
			while (xnode->parent != nullptr)
				xnode = xnode->parent;
			deleteFixUpBlackNode(xnode);
		}
		

		
	}
}
void deleteRB(rbTree* &root) {
	//左右皆空
	if (root->left == nullptr && root->right == nullptr) {
		auto parent = root->parent;
		delete root;
		if (root->color == BLACK && parent!=nullptr)     // 被删结点为黑且不为根，先删再向上调整
			deleteFixUpBlackNode(parent);
	}
	//左空 或 右空
	else if (root->left == nullptr || root->right == nullptr) {
		auto tp_node = root;
		auto child = root->left == nullptr ? root->right : root->left;
		auto parent = root->parent;
		if (parent != nullptr) {
			if (parent->left == root)
				parent->left = child;
			else
				parent->right = child;
			child->parent = parent;
		 
			delete root;                 // 待删除结点为红 直接删除
			if(root->color==BLACK) {     // 待删除结点为黑,先删除，再向上调整
				auto x_node = child;
				deleteFixUpBlackNode(x_node);
			}
		}
		else {            // 待删除结点为根结点，root只可能为黑
			rbTree* node = root;
			root = child;
			root->color = BLACK;
			delete node;
	
		}
		//if (root->color == RED) {            // 待删除结点为红  
		//	if (parent != nullptr) {
		//		if (parent->left == root) 
		//			parent->left = child;
		//		else 
		//			parent->right = child;
		//		child->parent = parent;
		//		delete root;
		//	}
		//	else {            // 待删除结点为根结点
		//		rbTree* node = root;
		//		root = child;
		//		delete node;
		//	}
		//}
		//else {                        // 待删除结点为黑 
		////	auto bro_node = 
		//}
	}

	// 左右非空
	else {
		auto successor_node = root->right; 
		while (successor_node->left != nullptr) {   // 获取root后继
			successor_node = successor_node->left;
		}
		//auto succ_parent = successor_node->parent;
		//auto succ_right = successor_node->right;
		//if (root->parent != nullptr) {                   // 交换结点
		//	if (root == root->parent->left)
		//		root->parent->left = successor_node;
		//	else 
		//		root->parent->right = successor_node;
		//	successor_node->parent = root;
		//}
		//else {                                         // 被删结点为根结点
		//}
		//successor_node->left = root->left;
		//successor_node->right = root->right;
		auto root_color = root->color;
		root->data = successor_node->data;                  // 交换值
		root->color = successor_node->color;
		successor_node->color = root_color;
		deleteRB(successor_node);                           // 删除后继
	}

}
void deleteRB(rbTree*& root, int val) {
	if (root == nullptr) {
		return;
	}
	if (root->data == val) {
		deleteRB(root);
		return;
	}
	if (root->data > val)
		deleteRB(root->left, val);
	else
		deleteRB(root->right, val);
}
int main() {
	rbTree* root = nullptr;            // 变量root is a	null pointer;
	insertRB(nullptr,root,8);         // 在函数中，会改变变量root的值，因此需要传reference
	insertRB(nullptr, root, 6);
	insertRB(nullptr, root, 10);
	insertRB(nullptr, root, 9);
	cout <<root->data<< endl;
	cout << endl;
}