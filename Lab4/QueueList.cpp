#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {
  if (head != nullptr){
    delete head;
  }  
}

Customer* QueueList::get_head() { 
  return head;
}

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head

  Customer* ptr = head;

  if(head == nullptr){
    head = customer;
    head->set_next(nullptr);
  } else{
    while(ptr->get_next() != nullptr){
      ptr = ptr->get_next();
    }
    ptr->set_next(customer);
    ptr = ptr->get_next();
    ptr->set_next(nullptr);
  } 
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue 
  // and return a pointer to it
  
  if(head == nullptr){
    return nullptr;
  }
  Customer* ptr = head;
  head = head->get_next();
  return ptr;
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
  int count = 0;
  Customer* ptr = head;
  while(ptr != nullptr){
    count += ptr->get_numOfItems();
    ptr = ptr->get_next();
  }
  return count;
}

bool QueueList::empty_queue() {
  // if the queue is empty, return false
  // if the queue is not empty, delete all customers
  // and return true
  if(head == nullptr){
    return false;
  }

  delete head;
  return true;
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}