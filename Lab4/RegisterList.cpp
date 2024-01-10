#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  if(head != nullptr){
    delete head;
  }
}

Register* RegisterList::get_head() { return head; }

int RegisterList::get_size() { 
  return size;
}


Register* RegisterList::get_min_items_register() {
  // loop all registers to find the register with least number of items
  Register* ptr = head;
  int min = -1;
  Register* min_reg = nullptr;
  int temp;
  while(ptr != nullptr){
    temp = ptr->get_queue_list()->get_items();
    if(min == -1 || temp < min){
      min = temp;
      min_reg = ptr;
    }
    ptr = ptr->get_next();
  }
  return min_reg;
}

Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr

  Register* ptr = head;
  while(ptr != nullptr){
    if(ptr->get_queue_list()->get_head() == nullptr){
      return ptr;
    }
    ptr = ptr->get_next();
  }
  return nullptr;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size 
  
  Register* ptr = head;

  if(ptr == nullptr){

    head = newRegister;

  } else {

    while(ptr->get_next() != nullptr){
      ptr = ptr->get_next();
    }
    ptr->set_next(newRegister);
    ptr = ptr->get_next();
    ptr->set_next(nullptr);
  }
  size++;
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise
  Register* ptr = head;
  while(ptr != nullptr){
   // cout << ptr->get_ID() << endl;
    if(ptr->get_ID() == ID){
      return true;
    }
    ptr = ptr->get_next();
  }
  return false;
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID
  // return the dequeued register
  // return nullptr if register was not found
  if(foundRegister(ID)){
    Register* ptr = head;
    Register* prev = nullptr;
    Register* res;
    if(head->get_ID() == ID){
      res = head;
      head = head->get_next();
      size --;
      res->set_next(nullptr);
      return res;
    }
    while(ptr != nullptr){
      if(ptr->get_ID() == ID){
        res = ptr;
        prev->set_next(ptr->get_next());
        size --;
        res->set_next(nullptr);
        return res;
      }
      prev = ptr;
      ptr = ptr->get_next();
    }
    return nullptr;
  } else{
    return nullptr;
  }
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr

  Register* ptr = head;
  double min = -1;
  Register* res = nullptr;
  while(ptr != nullptr){
    if(ptr->get_queue_list()->get_head() != nullptr){
      //ptr->set_availableTime(expTimeElapsed);
      if(ptr->calculateDepartTime() < min || min == -1){
        min = ptr->calculateDepartTime();
        res = ptr;
      }
    }
    ptr = ptr->get_next(); 
    }
  return res;
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
