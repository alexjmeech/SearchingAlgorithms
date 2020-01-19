#ifndef LINKEDLISTITERATOR_H_
#define LINKEDLISTITERATOR_H_

#include "ListNode.h"

template<typename T>
class LinkedList;

template<typename T>
class LinkedListIterator
{
private:
	ListNode<T>* cur;
public:
	LinkedListIterator(ListNode<T>* = nullptr);

	LinkedListIterator<T>& operator++();   // pre-increment
	LinkedListIterator<T> operator++(int); // post-increment

	LinkedListIterator<T>& operator--();
	LinkedListIterator<T> operator--(int);

	LinkedListIterator<T>& operator+=(unsigned long);
	LinkedListIterator<T>& operator-=(unsigned long);

	bool operator==(const LinkedListIterator<T>&) const;
	bool operator!=(const LinkedListIterator<T>&) const;

	T& operator*();

	bool hasPrev();
	bool hasNext();

	friend class LinkedList<T>;
};

template<typename T>
LinkedListIterator<T>::LinkedListIterator(ListNode<T>* node)
{
	cur = node;
}

template<typename T>
LinkedListIterator<T>& LinkedListIterator<T>::operator++()
{
	if (cur != nullptr)
	{
		cur = cur->getNext();
	}

	return *this;
}

template<typename T>
LinkedListIterator<T> LinkedListIterator<T>::operator++(int dummy)
{
	LinkedListIterator<T> output(cur);

	if (cur != nullptr)
	{
		cur = cur->getNext();
	}

	return output;
}

template<typename T>
LinkedListIterator<T>& LinkedListIterator<T>::operator--()
{
	if (cur != nullptr)
	{
		cur = cur->getPrev();
	}

	return *this;
}

template<typename T>
LinkedListIterator<T> LinkedListIterator<T>::operator--(int dummy)
{
	LinkedListIterator<T> output(cur);

	if (cur != nullptr)
	{
		cur = cur->getPrev();
	}

	return output;
}

template<typename T>
LinkedListIterator<T>& LinkedListIterator<T>::operator+=(unsigned long increment)
{
	for (unsigned long i = 0; i < increment && cur != nullptr; i++)
	{
		cur = cur->getNext();
	}

	return *this;
}

template<typename T>
LinkedListIterator<T>& LinkedListIterator<T>::operator-=(unsigned long decrement)
{
	for (unsigned long i = 0; i < decrement && cur != nullptr; i++)
	{
		cur = cur->getPrev();
	}

	return *this;
}

template<typename T>
bool LinkedListIterator<T>::operator==(const LinkedListIterator<T>& other) const
{
	return cur == other.cur;
}

template<typename T>
bool LinkedListIterator<T>::operator!=(const LinkedListIterator<T>& other) const
{
	return cur != other.cur;
}

template<typename T>
T& LinkedListIterator<T>::operator*()
{
	return cur->getData();
}

template<typename T>
bool LinkedListIterator<T>::hasPrev()
{
	if (cur == nullptr)
	{
		return false;
	}

	return cur->getPrev() != nullptr;
}

template<typename T>
bool LinkedListIterator<T>::hasNext()
{
	if (cur == nullptr)
	{
		return false;
	}

	return cur->getNext() != nullptr;
}

#endif /* LINKEDLISTITERATOR_H_ */
