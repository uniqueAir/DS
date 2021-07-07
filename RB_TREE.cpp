/***
 *   1.ʵ���˺�����Ĳ������
 *   2.ʵ���˺������ɾ������
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

// �������
void adjust(rbTree* &root) {
	// ��1��rootΪ��

	if (root->parent == nullptr) {  
		root->color = BLACK;
		return;
	}

	// ��2��root�ĸ��ڵ�Ϊ��

	if (root->parent->color == BLACK)
		return;

	// (3)root�ĸ����Ϊ��

	if (root->parent->color == RED) {
		auto uncle = root->parent->parent->left == root->parent ? root->parent->parent->right : root->parent->parent->left;
		//��3��root�ĸ���Ժ�
		if (uncle->color == RED) {
			root->parent->color = BLACK;
			uncle->color = BLACK;
			root->parent->parent->color = RED;
			adjust(root->parent->parent);
		}
	
		else if (uncle->color == BLACK) {
			// (4)root���������rootΪ����
			// ������������ 

			if (root == root->parent->right) {  
				auto tp_node4 = root->parent;
				leftRoate(root->parent);
				adjust(tp_node4);
			}
			// (4)root���������rootΪ����
			// ����ү��ү���� 

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

//root �� reference����root���ָ������ĵ�ַ 
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

// ɾ������
void deleteFixUpBlackNode(rbTree*& xnode) {
	if (xnode->parent == nullptr)  //��1��xָ���
		return;
	if (xnode->color == RED)       //��2��xΪ�� R+B
		return;
	//  XΪ�� B+B
	auto parent = xnode->parent;
	auto bro_node = xnode == parent->left ? parent->right : parent->left;
	//  ��3��X���ֵܺ�
	if (bro_node->color == RED) {
		bro_node->color = BLACK;
		parent->color = RED;
		leftRoate(parent);
		deleteFixUpBlackNode(xnode);       //��������X���ֵܽ��
	}
    // X���ֵܺ�
	if (bro_node->color == BLACK) {
		// X���ֵܺڣ��ֵ����Ӻ�
		if ((bro_node->right != nullptr && bro_node->right->color == BLACK)|| bro_node->right == nullptr){
			//��4��X���ֵܺڣ��ֵܺ���ȫ��
			if ((bro_node->left != nullptr && bro_node->left->color == BLACK) || bro_node->left == nullptr) {
				bro_node->color = RED;
				deleteFixUpBlackNode(parent);
			}
			//  ��5��X���ֵܺڣ��ֵ��Һ��� �󺢺�
			else{	
				bro_node->left->color = BLACK;
				bro_node->color = RED;
				rightRoate(bro_node);
				deleteFixUpBlackNode(xnode);  // ��������X���ֵܽ��
			}

		 }
		// ��6��X���ֵܺڣ��ֵ��Һ��� ������
		else {
			bro_node->color = parent->color;
			parent->color = BLACK;
			bro_node->right->color = BLACK;
			leftRoate(parent);
			// ����XΪ����㣬Ŀ��������ѭ��
			while (xnode->parent != nullptr)
				xnode = xnode->parent;
			deleteFixUpBlackNode(xnode);
		}
		

		
	}
}
void deleteRB(rbTree* &root) {
	//���ҽԿ�
	if (root->left == nullptr && root->right == nullptr) {
		auto parent = root->parent;
		delete root;
		if (root->color == BLACK && parent!=nullptr)     // ��ɾ���Ϊ���Ҳ�Ϊ������ɾ�����ϵ���
			deleteFixUpBlackNode(parent);
	}
	//��� �� �ҿ�
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
		 
			delete root;                 // ��ɾ�����Ϊ�� ֱ��ɾ��
			if(root->color==BLACK) {     // ��ɾ�����Ϊ��,��ɾ���������ϵ���
				auto x_node = child;
				deleteFixUpBlackNode(x_node);
			}
		}
		else {            // ��ɾ�����Ϊ����㣬rootֻ����Ϊ��
			rbTree* node = root;
			root = child;
			root->color = BLACK;
			delete node;
	
		}
		//if (root->color == RED) {            // ��ɾ�����Ϊ��  
		//	if (parent != nullptr) {
		//		if (parent->left == root) 
		//			parent->left = child;
		//		else 
		//			parent->right = child;
		//		child->parent = parent;
		//		delete root;
		//	}
		//	else {            // ��ɾ�����Ϊ�����
		//		rbTree* node = root;
		//		root = child;
		//		delete node;
		//	}
		//}
		//else {                        // ��ɾ�����Ϊ�� 
		////	auto bro_node = 
		//}
	}

	// ���ҷǿ�
	else {
		auto successor_node = root->right; 
		while (successor_node->left != nullptr) {   // ��ȡroot���
			successor_node = successor_node->left;
		}
		//auto succ_parent = successor_node->parent;
		//auto succ_right = successor_node->right;
		//if (root->parent != nullptr) {                   // �������
		//	if (root == root->parent->left)
		//		root->parent->left = successor_node;
		//	else 
		//		root->parent->right = successor_node;
		//	successor_node->parent = root;
		//}
		//else {                                         // ��ɾ���Ϊ�����
		//}
		//successor_node->left = root->left;
		//successor_node->right = root->right;
		auto root_color = root->color;
		root->data = successor_node->data;                  // ����ֵ
		root->color = successor_node->color;
		successor_node->color = root_color;
		deleteRB(successor_node);                           // ɾ�����
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
	rbTree* root = nullptr;            // ����root is a	null pointer;
	insertRB(nullptr,root,8);         // �ں����У���ı����root��ֵ�������Ҫ��reference
	insertRB(nullptr, root, 6);
	insertRB(nullptr, root, 10);
	insertRB(nullptr, root, 9);
	cout <<root->data<< endl;
	cout << endl;
}