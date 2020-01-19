#ifndef LISTNODE_H_
#define LISTNODE_H_

template<typename T>
class ListNode
{
private:
	T data;
	ListNode<T>* prev;
	ListNode<T>* next;
public:
	ListNode(T, ListNode<T>* = nullptr, ListNode<T>* = nullptr);

	T& getData();
	ListNode<T>*& getPrev();
	ListNode<T>*& getNext();

	ListNode<T>* deepCopy(ListNode<T>* = nullptr);
};

template<typename T>
ListNode<T>::ListNode(T data, ListNode<T>* prev, ListNode<T>* next)
{
	this->data = data;
	this->prev = prev;
	this->next = next;
}

template<typename T>
T& ListNode<T>::getData()
{
	return data;
}

template<typename T>
ListNode<T>*& ListNode<T>::getPrev()
{
	return prev;
}

template<typename T>
ListNode<T>*& ListNode<T>::getNext()
{
	return next;
}

template<typename T>
ListNode<T>* ListNode<T>::deepCopy(ListNode<T>* prev)
{
	ListNode<T>* deep = new ListNode<T>(data, prev);

	if (next != nullptr)
	{
		deep->getNext() = next->deepCopy(deep);
	}

	return deep;
}

#endif /* LISTNODE_H_ */
