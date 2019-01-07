#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/*
Author: Akshatkumar Patel
Date: October 15, 2018
*/

#include <algorithm>

template <typename T>
class LinkedList {
	struct Node {
		T data_;
		unsigned int access_;
		Node *next_;
		Node *prev_;

		Node(const T& d = T{ }, Node* next = nullptr, Node* prev = nullptr) {
			data_ = d;
			access_ = 0;
			next_ = next;
			prev_ = prev;
		}
	};

	Node *head_;
	Node *tail_;
	size_t size_;

	//Initialization
	void init();

public:
	class const_iterator {
	protected:
		const LinkedList<T> *theList;
		Node *current;
		const_iterator(Node *p, const LinkedList<T>* list) : current(p), theList(list) {}
		friend class LinkedList<T>;
	public:

		//Default constructor - intializer
		const_iterator() {
			current = nullptr;
			theList = nullptr;
		}

		//++x
		const_iterator operator++() {
			//Ensures operator does not go out of bounds
			if (this->current->next_ != nullptr) {
				current = current->next_;
			}
			return *this;
		}

		//x--
		const_iterator operator++(int) {
			const_iterator old = *this;
			++(*this);
			return old;
		}

		//--x
		const_iterator operator--() {
			//Ensures operator does not go out of bounds
			if (current->prev_ != nullptr) {
				current = current->prev_;
			}
			return *this;
		}

		//x--
		const_iterator operator--(int) {
			const_iterator old = *this;
			--(*this);
			return old;
		}

		//Compares rhs and current object to see if they refer to the same node
		bool operator== (const const_iterator & rhs) const {
			return (current == rhs.current);
		}

		//Compares rhs and current object to see if they DO NOT refer to the same node
		bool operator!= (const const_iterator & rhs) const {
			return !(*this == rhs);
		}

		//Returns a const reference to data in the node referred to by the iterator
		const T& operator* () const {
			return this->current->data_;
		}
	};

	class iterator : public const_iterator {
	protected:
		iterator(Node *p, const LinkedList<T>* lst) : const_iterator(p, lst) { }
		friend class LinkedList<T>;
	public:

		iterator() :const_iterator() { }

		//++x
		iterator operator++() {
			//Ensures operator does not go out of bounds
			if (this->current->next_ != nullptr) {
				this->current = this->current->next_;
			}
			return *this;
		}

		//x++
		iterator operator++(int) {
			iterator old = *this;
			++(*this);
			return old;
		}

		//--x
		iterator operator--() {
			//Ensures operator does not go out of bounds
			if (this->current->prev_ != nullptr) {
				this->current = this->current->prev_;
			}
			return  *this;
		}

		//x--
		iterator operator--(int) {
			iterator old = *this;
			--(*this);
			return old;
		}

		//Returns a reference to data in the node referred to by the iterator.
		T& operator*() {
			return this->current->data_;
		}

		//Returns a const reference to data in the node referred to by the iterator.
		const T & operator*() const {
			return const_iterator::operator*();
		}

	};

	LinkedList();
	~LinkedList();
	LinkedList(const LinkedList& rhs);
	LinkedList& operator=(const LinkedList& rhs);
	LinkedList(LinkedList&& rhs);
	LinkedList& operator=(LinkedList&& rhs);

	//Iterators
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

	void insert(const T& data);
	iterator search(const T& data);
	iterator erase(iterator it);
	iterator erase(iterator first, iterator last);
	bool empty() const;
	int size() const;
};



//Returns a iterator which points to the beginning of the list (Node after dummy list)
template <typename T>
typename LinkedList<T>::iterator begin() {
	return iterator(head_->next_, this);
}

//Returns a constant iterator pointing to the beginning of the list
template <typename T>
typename LinkedList<T>::const_iterator cbegin(){
	return const_iterator(head_->next_, this);
}

//Returns a iterator pointing to the end of the list(Tail_ dummy node)
template <typename T>
typename LinkedList<T>::iterator end() {
	return iterator(tail_, this);
}

//Returns a constant iterator pointing to the end of the list
template <typename T>
typename LinkedList<T>::const_iterator cend(){
	return const_iterator(tail_, this);
}



//Initializes member variables, reduces redundancy in constructors
template <typename T>
void LinkedList<T>::init() {
	size_ = 0;
	head_ = new Node();
	tail_ = new Node();
	head_->next_ = tail_;
	tail_->prev_ = head_;
}



//Creates an empty list and initializes the member variables to safe empty state
template <typename T>
LinkedList<T>::LinkedList() {
	init();
}



//Deallocates the memory allocated
template <typename T>
LinkedList<T>::~LinkedList() {
	//iterates through the list, deleting each Node (Nodes aren't stored in memory sequencially)
	erase(begin(), end());
	delete head_;
	delete tail_;
}



//Copy constructor - Calls the copy assignment operator
template <typename T>
LinkedList<T>::LinkedList(const LinkedList& rhs) {
	init();
	*this = rhs;
}



/* Copy Assignment operator
   Assigns current object the values of the object passed through parameter
   Returns the current object */
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& rhs) {

	//Check if the parameter is not the object itself
	if (this != &rhs) {

		//Delete all of the Nodes in the current List
		erase(begin(), end());

		if (!rhs.empty()) {

			//Copy the data from cbegin() to cend()
			for (const_iterator it = rhs.cbegin(); it != rhs.cend(); it++) {
				insert(it.current->data_);
				tail_->prev_->access_ = it.current->access_;
			}
			size_ = rhs.size_;
		}
	}
	return *this;
}



// Move Constructor - Calls the Move Assignment operator
template <typename T>
LinkedList<T>::LinkedList(LinkedList&& rhs) {
	init();
	*this = std::move(rhs);
}



/* Move Assignment operator
   Swaps the member variables (Head, Tail Nodes) of both of the objects
   Returns the current object */
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& rhs) {

	//Check for self move
	if (this != &rhs) {
		std::swap(rhs.head_, this->head_);
		std::swap(rhs.tail_, this->tail_);
		std::swap(rhs.size_, this->size_);
	}

	return *this;
}



/*
Inserts a node previous to the tail (dummy node)
New node will be accessed 0 times
*/
template <typename T>
void LinkedList<T>::insert(const T& data) {

	Node *newNode = new Node(data, tail_, tail_->prev_);
	tail_->prev_->next_ = newNode;
	tail_->prev_ = newNode;
	size_++;
}



/* Searches for data(T), and updates the access_ & the list order accordingly
   Returns an iterator pointing to the found Node or end if not found */
template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::search(const T& data) {

	iterator it = begin();
	iterator itEnd = end();

	Node* dataIndex = nullptr;
	Node* newIndex = nullptr;

	// Iterates through the list & if found updates the list
	for (it; it != itEnd; it++) {

		if (*it == data) {

			it.current->access_++;
			dataIndex = it.current;

			for (iterator change = begin(); change != itEnd; change++) {

				if (it.current->access_ >= change.current->access_) {
					newIndex = change.current;

					// it & change point to location, hence does not have to be moved
					if (it == change) {
						return it;
					}
					else {
						// Unlink and relink Node
						dataIndex->prev_->next_ = dataIndex->next_;
						dataIndex->next_->prev_ = dataIndex->prev_;

						dataIndex->next_ = newIndex;
						dataIndex->prev_ = newIndex->prev_;

						newIndex->prev_->next_ = dataIndex;
						newIndex->prev_ = dataIndex;

						return iterator(dataIndex, this);
					}
				}
			}
		}
	}

	return end();
}



/* Deletes the node at location(it)
   Returns an iterator which points to the next_ value of the deleted nodes */
template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::erase(iterator it) {

	// Unlink and relink the Nodes in the list and delete Node at it
	Node* temp = it.current;
	iterator val(temp->next_, this);
	temp->prev_->next_ = temp->next_;
	temp->next_->prev_ = temp->prev_;
	delete temp;
	size_--;

	return val;
}



/* Erases Node(s) from first to last. First is included in delete, last is excluded.
   Returns the iterator to the next (last which is excluded from delete) */
template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::erase(iterator first, iterator last) {

	for (iterator start = first; start != last;) {
		start = erase(start);
	}

	return last;
}



/* Checks if the function is empty
   Returns a bool */
template <typename T>
bool LinkedList<T>::empty() const {
	//List is empty when the two dummy nodes point to each other
	return (head_->next_ == tail_);
}



/* Counts the number of Nodes within the list and returns count
   Returns an int representing size */
template <typename T>
int LinkedList<T>::size() const {
	return size_;
}



#endif
