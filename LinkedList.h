#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "ListNode.h"
#include "LinkedListIterator.h"

template<typename T>
class LinkedList
{
private:
	ListNode<T>* head;
	ListNode<T>* tail;
	unsigned long nodes;

	void wipeNodes();
public:
	LinkedList();
	LinkedList(const LinkedList<T>&);
	LinkedList<T>& operator=(const LinkedList<T>&);
	~LinkedList();

	unsigned long size();
	bool isEmpty();

	void push_front(T);
	void push_back(T);

	T pop_front();
	T pop_back();

	T& peek_front();
	T& peek_back();

	void insert(T, LinkedListIterator<T>);
	void remove(LinkedListIterator<T>);

	void clear();

	LinkedListIterator<T> begin();
	LinkedListIterator<T> end();
	LinkedListIterator<T> tail_pos();
};

template<typename T>
LinkedList<T>::LinkedList()
{
	head = nullptr;
	tail = nullptr;
	nodes = 0;
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& copy)
{
	if (copy.nodes == 0)
	{
		head = nullptr;
		tail = nullptr;
		nodes = 0;
	}
	else
	{
		head = copy.head->deepCopy();
		tail = head;

		while (tail->getNext() != nullptr)
		{
			tail = tail->getNext();
		}

		nodes = copy.nodes;
	}
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& copy)
{
	if (&copy != this)
	{
		wipeNodes();

		if (copy.nodes > 0)
		{
			head = copy.head->deepCopy();
			tail = head;

			while (tail->getNext() != nullptr)
			{
				tail = tail->getNext();
			}

			nodes = copy.nodes;
		}
	}

	return *this;
}

template<typename T>
LinkedList<T>::~LinkedList()
{
	wipeNodes();
}

template<typename T>
void LinkedList<T>::wipeNodes()
{
	while (head != nullptr)
	{
		ListNode<T>* cur = head;
		head = head->getNext();
		delete cur;
	}

	tail = nullptr;
	nodes = 0;
}

template<typename T>
unsigned long LinkedList<T>::size()
{
	return nodes;
}

template<typename T>
bool LinkedList<T>::isEmpty()
{
	return nodes == 0;
}

template<typename T>
void LinkedList<T>::push_front(T element)
{
	head = new ListNode<T>(element, nullptr, head);

	if (tail == nullptr)
	{
		tail = head;
	}

	if (head->getNext() != nullptr)
	{
		head->getNext()->getPrev() = head;
	}

	nodes++;
}

template<typename T>
void LinkedList<T>::push_back(T element)
{
	if (head == nullptr)
	{
		push_front(element);
		return;
	}

	tail = new ListNode<T>(element, tail);
	tail->getPrev()->getNext() = tail;
	nodes++;
}

template<typename T>
T LinkedList<T>::pop_front()
{
	T data = head->getData();
	ListNode<T>* next = head->getNext();
	delete head;
	head = next;

	if (head != nullptr)
	{
		head->getPrev() = nullptr;
	}
	else
	{
		tail = nullptr;
	}

	nodes--;

	return data;
}

template<typename T>
T LinkedList<T>::pop_back()
{
	T data = tail->getData();
	ListNode<T>* prev = tail->getPrev();
	delete tail;
	tail = prev;

	if (tail != nullptr)
	{
		tail->getNext() = nullptr;
	}
	else
	{
		head = nullptr;
	}

	nodes--;

	return data;
}

template<typename T>
T& LinkedList<T>::peek_front()
{
	return head->getData();
}

template<typename T>
T& LinkedList<T>::peek_back()
{
	return tail->getData();
}

template<typename T>
void LinkedList<T>::insert(T element, LinkedListIterator<T> position)
{
	if (nodes == 0 || position.cur == head)
	{
		push_front(element);
	}
	else if (position.cur == nullptr)
	{
		push_back(element);
	}
	else
	{
		position.cur->getPrev()->getNext() = new ListNode<T>(element, position.cur->getPrev(), position.cur);
		position.cur->getPrev() = position.cur->getPrev()->getNext();
		nodes++;
	}
}

template<typename T>
void LinkedList<T>::remove(LinkedListIterator<T> position)
{
	if (position.cur == nullptr)
	{
		return;
	}

	if (position.cur == head)
	{
		pop_front();
	}
	else if (position.cur == tail)
	{
		pop_back();
	}
	else
	{
		position.cur->getPrev()->getNext() = position.cur->getNext();
		position.cur->getNext()->getPrev() = position.cur->getPrev();
		delete position.cur;
		nodes--;
	}
}

template<typename T>
void LinkedList<T>::clear()
{
	wipeNodes();
}

template<typename T>
LinkedListIterator<T> LinkedList<T>::begin()
{
	return LinkedListIterator<T>(head);
}

template<typename T>
LinkedListIterator<T> LinkedList<T>::end()
{
	return LinkedListIterator<T>();
}

template<typename T>
LinkedListIterator<T> LinkedList<T>::tail_pos()
{
	return LinkedListIterator<T>(tail);
}

#endif /* LINKEDLIST_H_ */
