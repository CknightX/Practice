#ifndef _RBTREE_TEST_H
#define _RBTREE_TEST_H
#include <climits>
#include<iostream>
#include<cstdio>
#include<queue>
using namespace std;
template<class T>
struct Node
{
	Node(T _data, Node<T>* left, Node<T>* right) :data(_data), left(left), right(right){}
	T data;
	Node<T>* left;
	Node<T>* right;
};

template<class T>
class RBTree
{
public:
	RBTree():root(nullptr){}
	void add_node(const T& value);
	void del_node(const T& value);
	bool is_balance(){ return _is_balance(root); }
	void print_depression(){ _print_child_tree(root, 0); }
	void print_tree(){ _print_as_tree(root); }
private:
	void _add_node(const T& value);
	void single_rotation(Node<T>*& pointer_root,Node<T>* root,bool is_leftRotation);
	void double_rotation(Node<T>*& pointer_root, Node<T>* root, bool is_leftRotation);
	int _height(Node<T>* root,int h);
	bool _is_balance(Node<T>* root);
	bool _is_leaf(Node<T>* root){ return (!root->left&&!root->right); }
	void _print_child_tree(Node<T>* root, int floor);
	void _print_as_tree(Node<T>* root);

	Node<T>* root; 
};

template<class T>
void RBTree<T>::_print_as_tree(Node<T>* root)
{
	queue<Node<T>* > q;
	if (root)
	{
		q.push(root);
	}
	bool is_complete = false;
	int num = 0;
	int pre_num=1;
	int bak_pre = 1;
	int floor = _height(root,0);
	for (int i = 0; i < floor; ++i)
		cout << " ";
	while (!q.empty())
	{
		auto tmp = q.front();
		q.pop();
		printf("%d", tmp->data);
		for (int i = 0; i < 2 * (floor - 1) + 1; ++i)
			cout << ' ';

		pre_num--;
		if (pre_num == 0) //已输出完一层
		{
			cout << endl;
			floor--;
			for (int i = 0; i < floor; ++i)
				cout << " ";
		}
		if (tmp->left)
		{
			q.push(tmp->left);
			++num;
		}
		if (tmp->right)
		{
			q.push(tmp->right);
			++num;
		}
		if (pre_num == 0)
		{
			bak_pre = num;
			pre_num = num;
			num = 0;
		}

	}

}
template<class T>
void RBTree<T>::add_node(const T& value)
{
	_add_node(value);


	if (_is_balance(root))
		return;

	bool is_leftRotation = false;
	bool is_singleRotarion = false;
	Node<T>* imbalance_pos = root;
	Node<T>* bak = nullptr;

	while (true) //找到最深的不平衡节点
	{
		Node<T>* tmp = (_height(imbalance_pos->left, 0) > _height(imbalance_pos->right, 0) ? imbalance_pos->left : imbalance_pos->right);
		if (_is_balance(tmp))
			break;
		bak = imbalance_pos;//最深不平衡节点的父节点
		imbalance_pos = tmp; //最深的不平衡节点
	}
	Node<T>*& pointer = (bak ? (bak->left == imbalance_pos ? bak->left : bak->right) : root); //指向最底层不平衡节点的指针

	is_leftRotation = (_height(imbalance_pos->left, 0) > _height(imbalance_pos->right, 0) ? true : false); //确定左侧还是右侧旋转

	Node<T>* next = (is_leftRotation ? imbalance_pos->left : imbalance_pos->right); //由左右旋转确定为最底层不平衡节点的左右子树
	is_singleRotarion = (_height(next->left, 0) > _height(next->right,0) ? true : false);  //确定next节点左右子树哪个深度大
	is_singleRotarion = (is_leftRotation && is_singleRotarion) || (!is_leftRotation&&!is_singleRotarion); //确定单双旋转

	if (is_singleRotarion)
		single_rotation(pointer, imbalance_pos, is_leftRotation);
	else
		double_rotation(pointer, imbalance_pos, is_leftRotation);
}
template<class T>
void RBTree<T>::single_rotation(Node<T>*& pointer_root, Node<T>* root,bool is_leftRoatation)
{
	if (is_leftRoatation)  //左侧单旋转
	{
		Node<T>* k1 = root->left;
		Node<T>* k2 = root;
		Node<T>* B = k1->right;

		pointer_root = k1;
		k2->left = B;
		k1->right = k2;
	}
	else //右侧单旋转
	{
		Node<T>* k1 = root->right;
		Node<T>* k2 = root;
		Node<T>* B = k1->left;

		pointer_root = k1;
		k2->right = B;
		k1->left = k2;
	}

}
template<class T>
void RBTree<T>::double_rotation(Node<T>*& pointer_root, Node<T>* root,bool is_leftRotation)
{
	if (is_leftRotation) //左侧双旋转
	{

		Node<T>* k1 = root->left;
		Node<T>* k2= k1->right;
		Node<T>* k3 = root;
		single_rotation(k3->left, k1, false);
		single_rotation(pointer_root, k3, true);

	}
	else //右侧双旋转
	{
		Node<T>* k1 = root->right;
		Node<T>* k2 = k1->left;
		Node<T>* k3 = root;
		single_rotation(k3->right, k1, true);
		single_rotation(pointer_root, k3, false);

	}
}
template<class T>
int RBTree<T>::_height(Node<T>* root, int h)
{
	if (!root)
		return h;
	int left = _height(root->left, h + 1);
	int right = _height(root->right, h + 1);
	return (left > right ? left : right);
}

template<class T>
bool RBTree<T>::_is_balance(Node<T>* root)
{
	if (!root)
		return true;
	if (abs(_height(root->left, 0) - _height(root->right, 0)) <= 1)
		return _is_balance(root->left) && _is_balance(root->right);
	else
		return false;
}
template<class T>
void RBTree<T>::_print_child_tree(Node<T>* root, int floor)
{
	for (int i = 0; i < floor; ++i)
		cout << "--";
	if (!root)
	{
		cout << '&' << endl;
		return;
	}
	cout << root->data << endl;
	_print_child_tree(root->left, floor + 1);
	_print_child_tree(root->right, floor + 1);
}

template<class T>
void RBTree<T>::del_node(const T& value)
{
	Node<T>* pos = root;
	Node<T>* pre_pos = nullptr;
	while (pos&&pos->data != value)
	{
		pre_pos = pos;
		if (value > pos->data)
			pos = pos->right;
		else
			pos = pos->left;
	}
	Node<T>*& pointer = (pre_pos ? (pre_pos->left == pos ? pre_pos->left : pre_pos->right) : root);
	if (pos->data != value) //can't find
		return;
	if (pos->left&&pos->right)
	{
		Node<T>* child_node = pos->right;
		Node<T>* pre_child_node = pos;
		while (child_node->left)
		{
			pre_child_node = child_node;
			child_node = child_node->left;
		}
		pre_child_node->left = nullptr;
		pointer = child_node;
		child_node->left = pos->left;
		child_node->right = pos->right;
	}
	else
	{
		if (pos->left)
			pointer = pos->left;
		else if (pos->right)
			pointer = pos->right;
		else
			pointer = nullptr;
	}
	delete pos;
}

template<class T>
void RBTree<T>::_add_node(const T& value)
{
	Node<T>* insert_node = new Node<T>(value, nullptr, nullptr);
	if (!root)
		root = insert_node;
	else
	{
		Node<T>* pos = root;
		while (true)
		{
			if (value > pos->data)
			{
				if (!pos->right)
				{
					pos->right = insert_node;
					break;
				}
				pos = pos->right;
			}
			else
			{
				if (!pos->left)
				{
					pos->left = insert_node;
					break;
				}
				pos = pos->left;
			}
		}
	}
}
#endif 