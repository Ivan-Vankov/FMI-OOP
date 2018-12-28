#include "List.h"

template <typename T>
T& List<T>::Iterator::operator*(){
	return iter->value;
}

template <typename T>
const T& List<T>::Iterator::operator*()const{
	return iter->value;
}

template <typename T>
/*iterator<T>*/ typename List<T>::Iterator List<T>::Iterator::operator++(int){
	Iterator temp = iter;
	iter = iter->next;
	return temp;
}

template <typename T>
/*iterator<T>&*/typename List<T>::Iterator& List<T>::Iterator::operator++(){
	iter = iter->next;
	return *this;
}

template <typename T>
/*iterator<T>*/ typename List<T>::Iterator List<T>::Iterator::operator--(int){
	Iterator temp = iter;
	iter = iter->prev;
	return temp;
}

template <typename T>
/*iterator<T>&*/ typename List<T>::Iterator& List<T>::Iterator::operator--(){
	iter = iter->prev;
	return *this;
}

template <typename T>
bool List<T>::Iterator::operator==(const Iterator& rhs)const{
	return &(iter->value) == &(rhs.iter->value);
}

template <typename T>
bool List<T>::Iterator::operator!=(const Iterator& rhs)const{
	return !(*this == rhs);
}

template <typename T>
const T& List<T>::constIterator::operator*()const{
	return iter->value;
}

template <typename T>
typename List<T>::constIterator List<T>::constIterator::operator--(int){
	Iterator temp = iter;
	iter = iter->prev;
	return temp;
}

template <typename T>
typename List<T>::constIterator& List<T>::constIterator::operator--(){
	iter = iter->prev;
	return *this;
}

template <typename T>
typename List<T>::constIterator List<T>::constIterator::operator++(int){
	Iterator temp = iter;
	iter = iter->next;
	return temp;
}

template <typename T>
typename List<T>::constIterator& List<T>::constIterator::operator++(){
	iter = iter->next;
	return *this;
}

template <typename T>
bool List<T>::constIterator::operator==(const constIterator& rhs)const{
	return &(iter->value) == &(rhs.iter->value);
}

template <typename T>
bool List<T>::constIterator::operator!=(const constIterator& rhs)const{
	return !(*this == rhs);
}

template <typename T>
List<T>::List() :size(0){
	dummy.prev = &dummy;
	dummy.next = &dummy;
}

template <typename T>
List<T>::List(const T& newVal){
	Node* newNode = new Node;
	newNode->value = newVal;
	dummy.prev = newNode;
	dummy.next = newNode;
	newNode->next = &dummy;
	newNode->prev = &dummy;
	size = 1;
}

template <typename T>
List<T>::List(const List& rhs) :size(0){
	dummy.prev = &dummy;
	dummy.next = &dummy;
	append(rhs);
}


template <typename T>
List<T>& List<T>::operator=(const List& rhs){
	if (this != &rhs){
		free();
		append(rhs);
	}
	return *this;
}

template <typename T>
List<T>::~List(){
	free();
}

template <typename T>
void List<T>::append(const List& rhs){
	if (rhs.size){
		Node* currNode = rhs.dummy.next;
		for (int i = 0; i < rhs.size; currNode = currNode->next, ++i){
			pushBack(currNode->value);
		}
	}
}

template <typename T>
void List<T>::free(){
	while (!isEmpty()){
		popFront();
	}
}

template <typename T>
/*iterator<T>*/ typename List<T>::Iterator List<T>::begin(){
	return dummy.next;
}

template <typename T>
/*iterator<T>*/ typename List<T>::Iterator List<T>::end(){
	return &dummy;
}

template <typename T>
typename List<T>::constIterator List<T>::begin()const{
	return dummy.next;
}

template <typename T>
typename List<T>::constIterator List<T>::end()const{
	return &dummy;
}

template <typename T>
const T& List<T>::front()const{
	return dummy.next->value;
}

template <typename T>
const T& List<T>::back()const{
	return dummy.prev->value;
}

template <typename T>
bool List<T>::isEmpty()const{
	return !size;
}

template <typename T>
int List<T>::getSize()const{
	return size;
}

template <typename T>
void List<T>::pushFront(const T& newVal){
	insertBefore(dummy.next, newVal);
}

template <typename T>
void List<T>::pushBack(const T& newVal){
	insertAfter(dummy.prev, newVal);
}

template <typename T>
void List<T>::popFront(){
	deleteNode(dummy.next);
}

template <typename T>
void List<T>::popBack(){
	deleteNode(dummy.prev);
}

template <typename T>
void List<T>::pop(Iterator& iter){
	deleteNode(iter.iter);
}

template <typename T>
void List<T>::insertAfter(Iterator& iter, const T& newVal){
	insertAfter(iter.iter, newVal);
}

template <typename T>
void List<T>::insertBefore(Iterator& iter, const T& newVal){
	insertBefore(iter.iter, newVal);
}

template <typename T>
void List<T>::insertAfter(Node* curr, const T& newVal){
	Node* temp = new Node;
	temp->value = newVal;
	curr->next->prev = temp;
	temp->next = curr->next;
	curr->next = temp;
	temp->prev = curr;
	++size;
}

template <typename T>
void List<T>::insertBefore(Node* curr, const T& newVal){
	insertAfter(curr->prev, newVal);
}

template <typename T>
void List<T>::deleteNode(Node* curr){
	curr->prev->next = curr->next;
	curr->next->prev = curr->prev;
	--size;
	delete curr;
}