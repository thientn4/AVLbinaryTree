#include "runtimeexcept.hpp"
#include <string>
#include <vector>
#include <algorithm>

class ElementNotFoundException : public RuntimeException 
{
public:
	ElementNotFoundException(const std::string & err) : RuntimeException(err) {}
};


template<typename Key, typename Value>
class MyAVLTree
{
private:
	// fill in private member data here
	// If you need to declare private functions, do so here too.
	struct tNode{
		tNode* tLeft=nullptr;
		tNode* tRight=nullptr;
		tNode* tParent=nullptr;
		int tHeight=0;
		Key tKey;
		Value tValue;
		tNode(Key newKey, Value newValue, tNode*newParent=nullptr){
			tParent = newParent;
			tKey=newKey;
			tValue=newValue;
		}
	};
	unsigned tSize=0;
	tNode* tRoot=nullptr;
	
	// Function to support balancing as in ZyBook
	void updateHeight(tNode* currentNode){
		int leftHeight=(currentNode->tLeft!=nullptr)?(currentNode->tLeft->tHeight+1):0;
		int rightHeight=(currentNode->tRight!=nullptr)?(currentNode->tRight->tHeight+1):0;
		currentNode->tHeight=std::max(leftHeight,rightHeight);
	}
	int getBalance(tNode* currentNode){
		int leftHeight=(currentNode->tLeft!=nullptr)?(currentNode->tLeft->tHeight+1):0;
		int rightHeight=(currentNode->tRight!=nullptr)?(currentNode->tRight->tHeight+1):0;
		return leftHeight-rightHeight;
	}
	void setChild(tNode* currentParent,std::string tDirection,tNode* newChild){
		if(tDirection=="left")currentParent->tLeft=newChild;
		else if(tDirection=="right")currentParent->tRight=newChild;
		if(newChild!=nullptr)newChild->tParent=currentParent;
		updateHeight(currentParent);
	}
	void replaceChild(tNode* currentParent,tNode* currentChild, tNode* newChild){
		if(currentParent->tLeft==currentChild)setChild(currentParent,"left",newChild);
		else if(currentParent->tRight==currentChild)setChild(currentParent,"right",newChild);
	}
	void rotateRight(tNode* currentNode){
		tNode* leftRightChild = currentNode->tLeft->tRight;
   		if (currentNode->tParent != nullptr) replaceChild(currentNode->tParent, currentNode, currentNode->tLeft);
   		else {
      		tRoot = currentNode->tLeft;
      		tRoot->tParent = nullptr;
  		}
   		setChild(currentNode->tLeft, "right", currentNode);
   		setChild(currentNode, "left", leftRightChild);
		
	}
	void rotateLeft(tNode* currentNode){
		tNode* rightLeftChild = currentNode->tRight->tLeft;
   		if (currentNode->tParent != nullptr)replaceChild(currentNode->tParent, currentNode, currentNode->tRight);
   		else {
      		tRoot = currentNode->tRight;
      		tRoot->tParent = nullptr;
  		}
   		setChild(currentNode->tRight, "left", currentNode);
   		setChild(currentNode, "right", rightLeftChild);

	}
	void rebalance(tNode* currentNode){
		updateHeight(currentNode);
   		if (getBalance(currentNode) == -2) {
      		if (getBalance(currentNode->tRight) == 1)rotateRight(currentNode->tRight);
      		return rotateLeft(currentNode);
   		}
   		else if (getBalance(currentNode) == 2) {
      		if (getBalance(currentNode->tLeft) == -1)rotateLeft(currentNode->tLeft);
      		return rotateRight(currentNode);
   		}   
	}
	
	//functions for tree traversal
	void orderRecurse(std::vector<Key>&returnOrder,tNode*currentNode,std::string orderType)const{
		if(currentNode==nullptr)return;
			if(orderType=="pre")returnOrder.push_back(currentNode->tKey);
		orderRecurse(returnOrder,currentNode->tLeft,orderType);
			if(orderType=="in")returnOrder.push_back(currentNode->tKey);
		orderRecurse(returnOrder,currentNode->tRight,orderType);
			if(orderType=="post")returnOrder.push_back(currentNode->tKey);
	}
	void destructorRecurse(tNode*currentNode){
		if(currentNode==nullptr)return;
		destructorRecurse(currentNode->tLeft);
		destructorRecurse(currentNode->tRight);
		delete currentNode;
	}
	
	////////////////////////FOR TESTING PURPOSE ONLY///////////////////////////
	void sortIntoHeight(std::vector<std::vector<Key>>& heightSort,tNode*currentNode,int level=0){
		if(currentNode==nullptr)return;
		sortIntoHeight(heightSort,currentNode->tLeft,level+1);
		heightSort[level].push_back(currentNode->tKey);
		sortIntoHeight(heightSort,currentNode->tRight,level+1);
	}
public:
	MyAVLTree();


	// The destructor is, however, required. 
	~MyAVLTree()
	{
		destructorRecurse(tRoot);
	}

	// size() returns the number of distinct keys in the tree.
	size_t size() const noexcept;

	// isEmpty() returns true if and only if the tree has no values in it. 
	bool isEmpty() const noexcept;

	//  contains() returns true if and only if there is a (key, value) pair in the tree that has the given key as its key.
	bool contains(const Key & k) const; 

	// find returns the value associated with the given key
	// If !contains(k), this will throw an ElementNotFoundException
	// There needs to be a version for const and non-const MyAVLTrees.
	Value & find(const Key & k);
	const Value & find(const Key & k) const;

	// Inserts the given key-value pair into 
	// the tree and performs the AVL re-balance
	// operation, as described in lecture. 
	void insert(const Key & k, const Value & v);
	
	std::vector<Key> inOrder() const;
	std::vector<Key> preOrder() const;
	std::vector<Key> postOrder() const;

	//for testing purpose only
	std::vector<std::vector<Key>>getByHeight();
};


template<typename Key, typename Value>
MyAVLTree<Key,Value>::MyAVLTree()
{

}

template<typename Key, typename Value>
size_t MyAVLTree<Key, Value>::size() const noexcept
{
	return tSize; // stub
}

template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::isEmpty() const noexcept
{
	return tSize==0; // stub
}

template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::contains(const Key &k) const
{
	////////////////////////CHECK IF CONTAIN - NO NEED FOR BALANCING///////////////////////////////////
	if(tRoot==nullptr)
		return false;
	else{
		tNode* currentNode=tRoot;
		while(currentNode!=nullptr){
			if(k<currentNode->tKey){
				if(currentNode->tKey==k){
					return true;
					currentNode=nullptr;
				}else
					currentNode=currentNode->tLeft;
			}else{
				if(currentNode->tKey==k){
					return true;
					currentNode=nullptr;
				}else
					currentNode=currentNode->tRight;
			}
		}
	}
	return false; // stub
}

template<typename Key, typename Value>
Value & MyAVLTree<Key, Value>::find(const Key & k)
{
	////////////////////////FIND VALUE - NO NEED FOR BALANCING///////////////////////////////////
	//int count=-1;//////////////////////////balance checking
	if(tRoot==nullptr)
		throw ElementNotFoundException("key not found");
	else{
		tNode* currentNode=tRoot;
		while(currentNode!=nullptr){
			//count++;//////////////////////balance checking
			if(k<currentNode->tKey){
				if(currentNode->tKey==k){
					//std::cout<<"After "<<count<<" steps and we get ";///////////////////////balance checking
					return currentNode->tValue;
					currentNode=nullptr;
				}else
					currentNode=currentNode->tLeft;
			}else{
				if(currentNode->tKey==k){
					//std::cout<<"After "<<count<<" steps and we get ";///////////////////////balance checking
					return currentNode->tValue;
					currentNode=nullptr;
				}else
					currentNode=currentNode->tRight;
			}
		}
	}
	throw ElementNotFoundException("key not found");
}

template<typename Key, typename Value>
const Value & MyAVLTree<Key, Value>::find(const Key & k) const
{
	////////////////////////FIND VALUE - NO NEED FOR BALANCING///////////////////////////////////
	if(tRoot==nullptr)
		throw ElementNotFoundException("key not found");
	else{
		tNode* currentNode=tRoot;
		while(currentNode!=nullptr){
			if(k<currentNode->tKey){
				if(currentNode->tKey==k){
					return currentNode->tValue;
					currentNode=nullptr;
				}else
					currentNode=currentNode->tLeft;
			}else{
				if(currentNode->tKey==k){
					return currentNode->tValue;
					currentNode=nullptr;
				}else
					currentNode=currentNode->tRight;
			}
		}
	}
	throw ElementNotFoundException("key not found");
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::insert(const Key & k, const Value & v)
{
	////////////////////////////INSERT WITHOUT BALANCE///////////////////////////
	tSize++;
	if(tRoot==nullptr)
		tRoot=new tNode(k,v);
	else{
		tNode* currentNode=tRoot;
		while(true){
			if(k<currentNode->tKey){
				if(currentNode->tLeft==nullptr){
					currentNode->tLeft=new tNode(k,v,currentNode);
					break;
				}else
					currentNode=currentNode->tLeft;
			}else{
				if(currentNode->tRight==nullptr){
					currentNode->tRight=new tNode(k,v,currentNode);
					break;
				}else
					currentNode=currentNode->tRight;
			}
		}
		//////////////////////////BALANCING HERE/////////////////////////////
		while(currentNode!=nullptr){
			rebalance(currentNode);
			currentNode=currentNode->tParent;
		}
	}	
}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::inOrder() const
{
	std::vector<Key> foo;
	orderRecurse(foo,tRoot,"in");
	return foo; 
}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::preOrder() const
{
	std::vector<Key> foo;
	orderRecurse(foo,tRoot,"pre");
	return foo; 
}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::postOrder() const
{
	std::vector<Key> foo;
	orderRecurse(foo,tRoot,"post");
	return foo; 
}


/////////////////////////////////FOR TESTING PURPOSE ONLY////////////////////////////
template<typename Key, typename Value>
std::vector<std::vector<Key>>MyAVLTree<Key, Value>::getByHeight(){
	std::vector<std::vector<Key>>toReturn;
	for(int i=0; i<=tRoot->tHeight; i++)
		toReturn.push_back({});
	sortIntoHeight(toReturn,tRoot);
	return toReturn;
}
