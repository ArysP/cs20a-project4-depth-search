#ifndef LIST_H
#define LIST_H

#include<iostream>
#include<assert.h>
using namespace std;
// Linked List object that maintains both head and tail pointers
// and the size of the list.  Note that you have to keep the head,
// tail and size consistent with the intended state of the List 
// otherwise very bad things happen. 
template<typename Type>
class List {
public:

	List();

	List(const List<Type>& other);

	List<Type>& operator=(const List<Type>& other);

	~List();

	void	print() const;
	bool	empty() const;

	void	push_front(const Type& item);
	void	push_back(const Type& item);
	void	add(int index, const Type& item);

	// Note that the user must first ensure the list is not empty
	// prior to calling these functions.
	Type	front() const;
	Type	rear() const;
	Type	get_at(int index) const;

	int		size() const;
	int		find(const Type& item) const;

	bool	pop_front();
	bool	pop_rear();
	bool	pop_at(int index);

	// Forward declare the nodes for our List.
// Will be implemented along with list's
// member functions
	class Node;

	// We'll have both head and tail points for 
	// Fast insertion/deletion from both ends.
	Node* m_head;
	Node* m_tail;

	// Keep track of size of list
	int		m_size;

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif

};


// List Implementation
//
//  Since List is a template class (which is not an actual 
//  class yet, not until we actually instantiate the list) 
//  we need to keep the implementation together with 
//  the definition.  There are ways to simulate having 
//  separate "implementation/definition" with templates, 
//  but they aren't necessary and can be confusing.
//

// Node definition
//		Already implemented, nothing to do here but to use it.
template<typename Type>
class List<Type>::Node {
public:
	Node() :next(nullptr), prev(nullptr) {}
	Node(Type item, Node* p, Node* n) : item(item), next(n), prev(p) {}

	Type  item;
	Node* next;
	Node* prev;
};



// List default constructor
//		Set head and tail pointer to point to nothing, size is zero
//		Already implemented, nothing to do.
template<typename Type>
List<Type>::List() :m_head(nullptr), m_tail(nullptr), m_size(0) {
}


// Copy constructor  
//		This should create an entirely new linked list with the same 
//		number of Nodes and the Values stored these Nodes in the same 
//		order as seen the other list’s Nodes.  This should not result 
//		in any memory leaks or aliasing.  
template<typename Type>
List<Type>::List(const List<Type>& other) {
	if (other.m_head == nullptr) {
		m_head = m_tail = nullptr;
	}
	else {
		Node* p = other.m_head;
		m_size = other.m_size;
		Node* next = nullptr;
		Node* prev = nullptr;
		for (int i = 0; i < m_size; i++)
		{
			Node* n = new Node;
			n->item = p->item;
			if (m_head == nullptr)
			{
				m_head = n;
				m_tail = m_head;
			}
			else
			{
				m_tail->next = n;
				m_tail = n;
			}
			n->prev = prev;
			prev = n;
			p = p->next;
		}
		m_tail->next = nullptr;
	}
}

// Overloaded assignment operator 
//		Causes the already existing linked list to be identical to the 
//		other linked list without causing any memory leaks or aliasing.
template<typename Type>
List<Type>& List<Type>::operator=(const List<Type>& other) {
	if (this == &other) {
		return *this;
	}
	else {
		Node* p = m_head;
		while (p != nullptr) {
			Node* n = p->next;
			delete p;
			p = n;
		}
		m_head = nullptr;
		m_tail = nullptr;
		m_size = NULL;
		if (other.m_head == nullptr) {
			m_head = m_tail = nullptr;
		}
		else {
			Node* p = other.m_head;
			m_size = other.m_size;
			Node* next = nullptr;
			Node* prev = nullptr;
			for (int i = 0; i < m_size; i++)
			{
				Node* n = new Node;
				n->item = p->item;
				if (m_head == nullptr)
				{
					m_head = n;
					m_tail = m_head;
				}
				else
				{
					m_tail->next = n;
					m_tail = n;
				}
				n->prev = prev;
				prev = n;
				p = p->next;
			}
			m_tail->next = nullptr;
		}
		return *this;
	}
}


// List destructor
//		Traverses the nodes and deletes them without causing memory leaks
template<typename Type>
List<Type>::~List() {
	Node* p = m_head;
	while (p != nullptr) {
		Node* n = p->next;
		delete p;
		p = n;
	}
	m_head = nullptr;
	m_tail = nullptr;
	m_size = NULL;
}

// List print
//		Traverses list and prints items in list to console
//		Assumes << is overloaded for object Type.
//		

template<typename Type>
void List<Type>::print() const {
	Node* p = m_head;
	if (p != NULL) {
		cout << p->item;
		p = p->next;
		while (p != NULL) {
			cout << ' ' << p->item;
			p = p->next;
		}
		cout << endl;
	}
}

// List empty
//		returns true if there are no items in the list
//		otherwise returns false.
template<typename Type>
bool List<Type>::empty() const {
	if (m_head == nullptr) {
		return true;
	}
	else {
		return false;
	}
}


// List push_front
//		Adds a new item to the front of the list
//		updates head, tail, and size accordingly.
//		
//		Two cases to consider: If the list is empty 
//		and if the list has items already it.
//
template<typename Type>
void List<Type>::push_front(const Type& item) {
	//for no items in list
	if (m_head == nullptr) {
		Node* p = new Node;
		p->item = item;
		p->next = nullptr;
		p->prev = nullptr;
		m_head = p;
		m_tail = m_head;
		m_size++;
	}
	// for items already in list
	else {
		Node* p = new Node;
		p->item = item;
		p->next = m_head;
		m_head->prev = p;
		p->prev = nullptr;
		m_head = p;
		m_size++;
	}
}

// List push_back
//		Adds a new item to the rear of the list
//		updates head, tail, and size accordingly.
//
//		Two cases to consider: If the list is empty 
//		and if the list has items already it.
//
template<typename Type>
void List<Type>::push_back(const Type& item) {
	if (m_head == nullptr) {
		push_front(item);
	}
	else {
		Node* p = new Node;
		p->item = item;
		m_tail->next = p;
		p->next = nullptr;
		p->prev = m_tail;
		m_tail = p;
		m_size++;
	}
}

// List add
//		Adds a new item at the index indicated
//		updates head, tail, and size accordingly.
//
//		If the index less than or equal to zero add to the front.
//		If the index is greater than or equal to
//			the size of the list, add to the rear.
//		All other cases, add the item to the appropriate
//			location
//
template<typename Type>
void List<Type>::add(int index, const Type& item) {
	if (index <= 0) {
		push_front(item);
	}
	else if (index >= m_size) {
		push_back(item);
	}
	//test this more
	else {
		Node* t = m_head;
		Node* p = new Node;
		for (int i = 0; i < index - 1; i++) {
			t = t->next;
		}
		p->item = item;
		p->next = t->next;
		p->prev = t;
		t->next = p;
		t = t->next->next;
		t->prev = p;
		m_size++;
	}
}

// 8. List front
//		returns the first item in the list.
//
template<typename Type>
Type List<Type>::front() const {
	// Force check that the head is pointing to a Node
	// Typical solution for deployement code is to throw
	// exceptions, but since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(m_head != nullptr);
	Type item;
	return m_head->item;
}

// List rear
//		returns the last item in the list
//
template<typename Type>
Type List<Type>::rear() const {

	// Force check that the tail is pointing to a Node
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(m_tail != nullptr);
	Type item;
	return m_tail->item;
}

// List get_at
//		returns the item at index
//
template<typename Type>
Type List<Type>::get_at(int index) const {

	// Force index to be correct before getting the Item
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(index >= 0 && index < m_size);
	if (index <= 0) {
		return front();
	}
	else if (index == m_size) {
		return rear();
	}
	else {
		int i = 0;
		Node* p = m_head;
		while (i < index) {
			p = p->next;
			i++;
		}
		Type item;
		return p->item;
	}
}

// List size
//		return the size of the list
template<typename Type>
int List<Type>::size() const {
	return m_size;
}


// List find
//		Traverses the list to see if the item is in the list
//		if it is return the index of that item, otherwise
//		return -1. Assumes the == operator is overloaded for
//		object Type.
template<typename Type>
int List<Type>::find(const Type& item) const {
	Node* p = m_head;
	int i = 0;
	while (p != nullptr) {
		if (p->item == item) {
			return i;
		}
		else {
			p = p->next;
			i++;
		}
	}
	if (i >= m_size) {
		return -1;
	}
}

// List pop_front
//		Removes the first item in the list returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly.
//
//		Must consider at least two cases where the list: is empty, 
//		has one or more items.
//		
template<typename Type>
bool List<Type>::pop_front() {
	if (m_head == nullptr) {
		return false;
	}
	else if (m_size <= 1) {
		Node* d = m_head;
		m_head = d->next;
		m_tail = m_head;
		d->prev = nullptr;
		d->next = nullptr;
		delete d;
		m_size--;
		return true;
	}
	else {
		Node* d = m_head;
		m_head = d->next;
		m_head->prev = nullptr;
		d->prev = nullptr;
		d->next = nullptr;
		delete d;
		m_size--;
		return true;
	}
}
// List pop_rear
//		Removes the last item in the list returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly.
//
//		Must consider at least three cases where the list: is empty, 
//		has one item, has two or more items.
//	
template<typename Type>
bool List<Type>::pop_rear() {
	if (m_head == nullptr) {
		return false;
	}
	else if (m_size == 1) {
		pop_front();
	}
	else {
		Node* d = m_tail;
		m_tail = d->prev;
		m_tail->next = nullptr;
		delete d;
		m_size--;
		return true;
	}

}

// List pop_at
//		Removes the item at the index returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly. If the index is
//		out of bounds return false.
//
template<typename Type>
bool List<Type>::pop_at(int index) {
	if (index >= 0 && index < m_size) {
		if (index == 0) {
			pop_front();
		}
		else if (index == m_size - 1) {
			Node* d = m_tail;
			m_tail = d->prev;
			m_tail->next = nullptr;
			delete d;
			m_size--;
			return true;
		}
		else {
			int i = 0;
			Node* p = m_head;
			while (i < index - 1) {
				p = p->next;
				i++;
			}
			if (p != nullptr) {
				Node* d = p->next;
				p->next = d->next;
				p = p->next;
				p->prev = d->prev;
				delete d;
				m_size--;
				return true;
			}
			return false;
		}
	}
	else {
		return false;
	}
}

#endif