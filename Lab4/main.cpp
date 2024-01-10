#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes 

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join


// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);
void checkDepart(bool end);
void checkQueue();
void checkSingle();
Customer* get_ind(int index);
bool compareArrays(int a, int b);
void swap(int& a, int& b);
void sortArrays(int array1[], int array2[], int size);
void checkDepartMulti();
double max_wait_time();
double avg_wait_time();
double std_dev_wait_time();


// Global variables
RegisterList *registerList; // holding the list of registers
QueueList *doneList; // holding the list of customers served
QueueList *singleQueue; // holding customers in a single virtual queue
double expTimeElapsed; // time elapsed since the beginning of the simulation
int numDeparted; // number of customers departed
double lastTimeElapsed;

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;
  numDeparted = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }

  cout << endl << "Finished at time " << expTimeElapsed << endl << "Statistics:" << endl;

  cout << "\tMaximum wait time: " << max_wait_time() << endl;

  cout << "\tAverage wait time: " << avg_wait_time() << endl;

  cout << "\tStandard Deviation of wait time: " << std_dev_wait_time() << endl;

  delete doneList;
  delete registerList;
  delete singleQueue;

  // You have to make sure all dynamically allocated memory is freed 
  // before return 0
  return 0;
}

double max_wait_time(){
  
  double max = 0;
  double temp_wait;
  
  Customer* ptr = doneList->get_head();
  
  while (ptr != nullptr){
    temp_wait = ptr->get_departureTime() - ptr->get_arrivalTime();
    if (temp_wait > max){
      max = temp_wait;
    }
    ptr = ptr->get_next();
  }

  return max;
}

double avg_wait_time(){

  double total = 0;
  double num = 0;
  double temp_wait;
  
  Customer* ptr = doneList->get_head();
  
  while (ptr != nullptr){
    temp_wait = ptr->get_departureTime() - ptr->get_arrivalTime();
    total += temp_wait;
    num ++;
    ptr = ptr->get_next();
  }

  return total/num;
}

double std_dev_wait_time(){

  double total = 0;
  double num = 0;
  double temp_wait;
  double avg_wait = avg_wait_time();

  Customer* ptr = doneList->get_head();
  
  while (ptr != nullptr){
    temp_wait = ptr->get_departureTime() - ptr->get_arrivalTime();
    total += ((temp_wait - avg_wait) * (temp_wait - avg_wait));
    num ++;
    ptr = ptr->get_next();
  }

  total = total/num;

  total = pow(total, 0.5);

  return total;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }
  // Depending on the mode of the simulation (single or multiple),
  // add the customer to the single queue or to the register with
  // fewest items

  lastTimeElapsed = expTimeElapsed;
  expTimeElapsed += timeElapsed;

  if (mode == "single"){
    checkSingle();
  } else {
    checkDepartMulti();
  }

  // things to do: check for customers to depart

  cout << "A customer entered" << endl;

  // create customer
  Customer* cust = new Customer(expTimeElapsed, items);

  // first do single mode
  if(mode == "single"){
    // check if there are any free registers
    Register* free = registerList->get_free_register();

    singleQueue->enqueue(cust);

    if (free == nullptr){
      cout << "No free registers" << endl;
    } else {
      free->get_queue_list()->enqueue(singleQueue->dequeue());
      cout << "Queued a customer with free register " << free->get_ID() << endl;
    }
    return;
  } else { // do multi mode
    Register* free = registerList->get_min_items_register();

    free->get_queue_list()->enqueue(cust);
    cout << "Queued a customer with quickest register " << free->get_ID() << endl;
    return;
  }
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  // If we were simulating a single queue, 
  // and there were customers in line, then 
  // assign a customer to the new register

  // check if register w id is open, use foundregister
  if(registerList->foundRegister(ID)){
    cout << "Error: register " << ID << " is already open" << endl;
    return;
  }

  lastTimeElapsed = expTimeElapsed;
  expTimeElapsed += timeElapsed;

  if (mode == "single"){
    checkSingle();
  } else {
    checkDepartMulti();
  }

  // open register
  // create new register
  Register* reg = new Register(ID, secPerItem,setupTime, expTimeElapsed);
  cout << "Opened register " << ID << endl;
  // if single queue, add customer to new register
  if(mode == "single"){
    if (singleQueue->get_head() != nullptr){
      reg->get_queue_list()->enqueue(singleQueue->dequeue());
      cout << "Queued a customer with free register " << ID << endl;
    }
  }
  registerList->enqueue(reg);

}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free it's memory
  // Otherwise, print an error message 

  // check if register is open
  if(!registerList->foundRegister(ID)){
    cout << "Error: register " << ID << " is not open" << endl;
    return;
  }

  lastTimeElapsed = expTimeElapsed;
  expTimeElapsed += timeElapsed;

  if (mode == "single"){
    checkSingle();
  } else {
    checkDepartMulti();
  }

  // dequeue
  Register* del = registerList->dequeue(ID);
  delete del;

  cout << "Closed register " << ID << endl;

}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}

/* Single mode 
  what to do on each command before performing the action:
    1. update system time
    2. check if you can depart customer at head of register
      a. find register with min depart time with calculateMinDepartTimeRegister()
      b. if null, all registers are free, so go to step 3
      c. if not null, see if the departure time of the customer at that register is earlier than
         elapsed time
      d. if so, depart customer and add next customer in singleQueue if any
      e. else go to step 5
    3. check if you can add in anyone else to head of queue
      a. check if anyone in single queue
      b. if not go to step 2
      c. if there is, check for free register by using get_free_register()
      d. go to step 2
    4. repeat steps 2 and 3 until you cant add or depart anyone else
    5. run rest of command

    IMPLEMENT THIS IN checkDepartQueue()
*/

void checkDepartMulti(){
  Register* reg = registerList->calculateMinDepartTimeRegister(lastTimeElapsed);

  if(registerList->get_head() == nullptr){
    return;
  } else if (reg == nullptr){
    return;
  } else {
    double dep_time = reg->calculateDepartTime();
    //cout << dep_time << ", " << expTimeElapsed << endl;
    if(dep_time <= expTimeElapsed && dep_time != -1){
      cout << "Departed a customer at register ID " << reg->get_ID() << " at " << dep_time << endl;
      reg->departCustomer(doneList);
      checkDepartMulti();
      return;
    } else {
      return;
    }
  }
}

void checkDepart(bool end){
  //registerList->print();
  Register* reg = registerList->calculateMinDepartTimeRegister(lastTimeElapsed);
 // if (reg != nullptr){
  //  cout << reg->get_ID() << endl;
 // }
 // cout << end << endl;
  if(registerList->get_head() == nullptr){
    return;
  } else if (reg == nullptr){
    if (!end){
      checkQueue();
    }
    return;
  } else{
    double dep_time = reg->calculateDepartTime();
   // cout << dep_time << ", " << expTimeElapsed << endl;
    if(dep_time <= expTimeElapsed && dep_time != -1){
      cout << "Departed a customer at register ID " << reg->get_ID() << " at " << dep_time << endl;
      reg->departCustomer(doneList);
      if(!end){
        checkQueue();
      } else {
        checkDepart(true);
      }
      return;
    } else {
      return;
    }
  }
}

void checkQueue(){
  if(singleQueue->get_head() == nullptr){
    checkDepart(true);
    return;
  } else{
    Register* reg = registerList->get_free_register();
    if(reg == nullptr){
      checkDepart(false);
      return;
    } else {
      Customer* temp = singleQueue->dequeue();
      reg->get_queue_list()->enqueue(temp);
      reg->get_queue_list()->get_head()->set_departureTime(reg->calculateDepartTime());
      cout << "Queued a customer with free register " << reg->get_ID() << endl;
      checkDepart(false);
      return;
    }
  }
}

void checkSingle(){
  checkDepart(false);
  return;
  
}

/*
void checkDepart(){
  // set time to timeElapsed
  Register* reg = registerList->get_head();

  // keep track of how many we got rid of
  int num = 0;

  // keep track of ids for the ones we got rid of
  int id_dep[registerList->get_size()];

  // keep track of depart times, same index as id_dep
  int dep_tim_arr[registerList->get_size()];

  while(reg != nullptr){

    // calculate depart time
    double dep_time = reg->calculateDepartTime();
    reg->set_availableTime(expTimeElapsed);

    while(dep_time <= expTimeElapsed && dep_time != -1){
      id_dep[num] = reg->get_ID();
      dep_tim_arr[num] = dep_time;
      reg->departCustomer(doneList);
      dep_time = reg->calculateDepartTime();
      num ++;
      numDeparted ++;
    }

    reg = reg->get_next();
  }

  // sort arrays with respect to depart time
  sortArrays(dep_tim_arr, id_dep, num);

  for(int i = 0; i < num; i ++){
    cout << "Departed a customer at register ID " << id_dep[i] << " at " << dep_tim_arr[i] << endl;
  }
} */

Customer* get_ind(int index){
  Customer* ptr = doneList->get_head();
  for(int i = 0; i < index; i ++){
    ptr = ptr->get_next();
  }
  return ptr;
}

// Custom comparison function for sorting
bool compareArrays(int a, int b) {
    return a < b;
}

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void sortArrays(int array1[], int array2[], int size) {
    // Bubble sort based on the values from the first array
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (!compareArrays(array1[j], array1[j + 1])) {
                // Swap the values in both arrays
                swap(array1[j], array1[j + 1]);
                swap(array2[j], array2[j + 1]);
            }
        }
    }
}