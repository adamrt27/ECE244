
// Created by Salma Emara on 2023-06-02.
#include "Register.h"

#include <iostream>
using namespace std;

Register::Register(int id, double timePerItem, double overhead,
                   double entryTime) {
  ID = id;
  secPerItem = timePerItem;
  overheadPerCustomer = overhead;
  availableTime = entryTime;
  next = nullptr;           // no other registers yet
  queue = new QueueList();  // no customers in line yet, but has to initialize a
                            // queue
}

Register::~Register() {
  if(queue != nullptr){
    delete queue;
  }
  if(next != nullptr){
    delete next;
  }
}

QueueList* Register::get_queue_list() { 
  return queue;
}

Register* Register::get_next() { 
  return next;
}

int Register::get_ID() { 
  return ID;
}

double Register::get_secPerItem() {  
  return secPerItem;
}

double Register::get_overheadPerCustomer() {  
  return overheadPerCustomer;
}

double Register::get_availableTime() {  
  return availableTime;
}

void Register::set_next(Register* nextRegister) {  
  next = nextRegister;
}


void Register::set_availableTime(double availableSince) {
  availableTime = availableSince;
}

double Register::calculateDepartTime() {
  // Get the departure time of the first customer in the queue
  // returns -1 if no customer is in the queue
  if(queue->get_head() == nullptr){
    return -1;
  }

  double t;

  // if the time the customer came to register is later than when they arrive, ie they had to wait
  if(availableTime > queue->get_head()->get_arrivalTime()){ 
    t = availableTime;
  } else { // if the queue was empty when they arrived
    t = queue->get_head()->get_arrivalTime();
  }

  double time;
  time = (secPerItem * queue->get_head()->get_numOfItems()) + overheadPerCustomer + t;
  return time;
}

void Register::departCustomer(QueueList* doneList) {
  // dequeue the head, set last dequeue time, add to doneList,
  // update availableTime of the register
  double depart_time = calculateDepartTime();
  queue->get_head()->set_departureTime(depart_time);
  Customer* done = queue->dequeue();
  done->set_next(nullptr);
  if (done == nullptr){
    return;
  } else {
    //done->set_departureTime(depart_time);
    doneList->enqueue(done);
    //cout << "Departed a customer at register ID " << ID << " at " << depart_time << endl;
  }

  availableTime = depart_time;
  //cout << "set avail time for reg " << ID << " to " << availableTime << endl;
}

void Register::print() {
  std::cout << "Register ID: " << ID << std::endl;
  std::cout << "Time per item: " << secPerItem << std::endl;
  std::cout << "Overhead per customer: " << overheadPerCustomer << std::endl;
  if (queue->get_head() != nullptr) {
    std::cout << "Queue has customers: \n";
    queue->print();
  }
}
