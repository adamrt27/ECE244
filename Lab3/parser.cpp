//
//  parser.cpp
//  lab3
//
//  Modified by Tarek Abdelrahman on 2020-10-04.
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018-2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.


#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "Shape.h"

void value_input(stringstream& lineStream, int& val, bool& error, const bool last_arg);
int name_input(stringstream& lineStream, string& val, bool& error, const bool create, const bool drawdelete, const bool last_arg);
void type_input(stringstream& lineStream, string& val, bool& error, const bool last_arg);
void loc_size_input(stringstream& lineStream, int& val, bool& error, const bool last_arg);
void size_same(const int size1, const int size2, const string shapetype, bool& error);
void angle_input(stringstream& lineStream, int& val, bool& error, const bool last_arg);
void check_name(const string val, bool& error);
void shape_arr_full(int index, int max, bool& error);
void too_many_args(stringstream& lineStream, bool& error);
void too_few_args(stringstream& lineStream, bool& error);
void too_args(stringstream& lineStream, bool& error, const bool last_arg);

// This is the shape array, to be dynamically allocated
Shape** shapesArray;

// The number of shapes in the database, to be incremented 
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes = 0;

// invalid names
string invalid_names[10] = {"maxShapes", "create", "move", "rotate", "draw", "delete", "ellipse", "circle", "rectangle", "triangle"};

// ECE244 Student: you may want to add the prototype of
// helper functions you write here

int main() {

    string line;
    string command;
    
    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input

    // initialize argument storing variables
    string name;
    string type;
    int loc1;
    int loc2;
    int size1;
    int size2;
    int value;
    int angle;
    bool error = false;

    // initialize variable to count where you are in shapesArray
    int ind = 0;

    while (!cin.eof()) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream (line);
        
        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;

        error = false;

        // Check for the command and act accordingly
        // ECE244 Student: Insert your code here
        if (command == "maxShapes"){
            // check if we have to delete original array
            if (max_shapes != 0) {
                for (int i =0 ; i < max_shapes; i ++){
                        delete shapesArray[i];
                }
                delete shapesArray;
            }


            value_input(lineStream, max_shapes, error, true);

            if (!error) {
                if (!lineStream.eof()){

                }
                shapesArray = new Shape*[max_shapes];

                shapeCount = 0;

                cout << "New database: max shapes is " << max_shapes << endl;
            }
        
        
        } else if (command == "create"){

            // name
            name_input(lineStream, name, error, true, false, false);

            // type
            type_input(lineStream, type, error, false);
            // loc
            loc_size_input(lineStream, loc1, error, false);

            loc_size_input(lineStream, loc2, error, false);
            // size
            loc_size_input(lineStream, size1, error, false);
            loc_size_input(lineStream, size2, error, true);

            // check error
            size_same(size1, size2, type, error);

            // check too many args
            too_many_args(lineStream, error);

            // check array full
            if(shapeCount == max_shapes){
                error = true;
                cout << "Error: shape array is full" << endl;
            }

            if (!error){
                shapesArray[shapeCount] = new Shape(name, type, loc1, size1, loc2, size2);
                cout << "Created ";
                shapesArray[shapeCount]->draw();
                cout << endl;
                shapeCount ++;
            }

        } else if (command == "move"){
            int ind = name_input(lineStream, name, error, false, false, false);

            loc_size_input(lineStream, loc1, error, false);
            loc_size_input(lineStream, loc2, error, true);

            too_many_args(lineStream, error);

            if (!error){
                shapesArray[ind]->setXlocation(loc1);
                shapesArray[ind]->setYlocation(loc2);
                cout << "Moved " << name << " to " << loc1 << " " << loc2 << endl;
            }
        } else if (command == "rotate"){
            int ind = name_input(lineStream, name, error, false, false, false);
            angle_input(lineStream, angle, error, true);

            too_many_args(lineStream, error);

            if(!error){
                shapesArray[ind]->setRotate(angle);
                cout << "Rotated " << name << " by " << angle << " degrees" << endl;
            }
        } else if (command == "draw"){
            too_few_args(lineStream, error);

            int ind = name_input(lineStream, name, error, false, true, true);

            too_many_args(lineStream, error);

            if(!error){
                if (ind == -1){
                    cout << "Drew all shapes ";
                    for(int i = 0; i < shapeCount; i ++){
                        if(shapesArray[i] !=  NULL) {
                            shapesArray[i]->draw();
                            cout << " ";
                        }
                    }
                    cout << endl;
                }
                else {
                    cout << "Drew ";
                    shapesArray[ind]->draw();
                    cout << endl;
                }
            }
            
        } else if (command == "delete"){
            int ind = name_input(lineStream, name, error, false, true, false);

            too_many_args(lineStream, error);

            if(!error){
                if (ind == -1){
                    cout << "Deleted: all shapes" << endl;
                    for(int i = 0; i < max_shapes; i ++){
                        delete (shapesArray[i]);
                    }
                    shapeCount = 0;
                } else {
                    cout << "Deleted shape " << name << endl;
                    delete shapesArray[ind];
                  //  shapesArray[ind] = NULL;
                    for(int i = ind; i < max_shapes - 1; i ++){
                        shapesArray[i] = shapesArray[i + 1];
                    }
                    shapeCount --;
                }
            }
        } else {
            cout << "Error: invalid command" << endl;
        }

        // get parameters

        // check errors
        
        
        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);
        
    }  // End input loop until EOF.
    
    return 0;
}

void value_input(stringstream& lineStream, int& val, bool& error, const bool last_arg){

    if (!error){

        // check for no arg found
        if (lineStream.eof() && !last_arg){
            error = true;
            cout << "Error: too few arguments" << endl;
            return;
        }

        lineStream >> val;

        // check for invalid arg
        if(lineStream.fail() || lineStream.peek() == '.'){
            lineStream.ignore(1000, '\n');
            lineStream.clear();
            cout << "Error: invalid argument" << endl;
            error = true;
            return;
        }

        // check for invalid value
        if (val < 0){
            cout << "Error: invalid value" << endl;
            error = true;
            return;
        }

        // check for too few/many args
        too_args(lineStream, error, last_arg);
        return;
    }
    return;
}


int name_input(stringstream& lineStream, string& val, bool& error, const bool create, const bool drawdelete, const bool last_arg){

    if (!error) {
    
        // check for no arg found
        if (lineStream.eof() && !last_arg){
            error = true;
            cout << "Error: too few arguments" << endl;
            return 0;
        }

        lineStream >> val;

        if (drawdelete){
            if (val == "all") {
                return -1;
            }
        }

        // check for invalid arg
        if(lineStream.fail()){
            lineStream.ignore(1000, '\n');
            lineStream.clear();
            cout << "Error: invalid argument" << endl;
            error = true;
            return 0;
        }

        if (create){
            // check for invalid shape name    
            for (int i = 0; i < 10; i ++){
                if (invalid_names[i] == val){
                    cout << "Error: invalid shape name" << endl;
                    error = true;
                    return 0;
                }
            }

            // check for name already created
            for (int i = 0; i < shapeCount; i ++){
                if (shapesArray[i]->getName() == val){
                    cout << "Error: shape " << val << " exists" << endl;
                    error = true;
                    return 0;
                }
            }
        } else {
            // shape name not found
            bool namefound = false;
            int ind = 0;
            for (int i = 0; i < shapeCount; i ++){
                if(shapesArray[i]->getName() == val){
                    namefound = true;
                    ind = i;
                }
            }
            if (namefound == false){
                cout << "Error: shape " << val << " not found" << endl;
                error = true;
                return 0;
            } else{
                return ind;
            }
        }

        // check for too few/many args
        too_args(lineStream, error, last_arg);
    }
    return 0;
}

void type_input(stringstream& lineStream, string& val, bool& error, const bool last_arg){

    if (!error) {
    
        // check for no arg found
        if (lineStream.eof() && !last_arg){
            error = true;
            cout << "Error: too few arguments" << endl;
            return;
        }

        lineStream >> val;

        // check for invalid arg
        if(lineStream.fail()){
            lineStream.ignore(1000, '\n');
            lineStream.clear();
            cout << "Error: invalid argument" << endl;
            error = true;
            return;
        }

        // check for invalid shape name    
        string valid_names[4] = {"circle","ellipse","rectangle","triangle"};
        bool inarr = false;
        for (int i = 0; i < 4; i ++){
            if (valid_names[i] == val){
                inarr = true;
            }
        }
        if(!inarr){
            cout << "Error: invalid shape type" << endl;
            error = true;
            return;
        }

        // check for too few/many args
        too_args(lineStream, error, last_arg);
    }
    return;
}

void loc_size_input(stringstream& lineStream, int& val, bool& error, const bool last_arg){
    // just check invalid arg, invalid value
    if (!error) {

        // check for no arg found
        if (lineStream.eof() && !last_arg){
            error = true;
            cout << "Error: too few arguments" << endl;
            return;
        }
    
        lineStream >> val;

        // check for invalid arg
        if(lineStream.fail() || (last_arg && lineStream.peek() == '.')){
            lineStream.ignore(1000, '\n');
            lineStream.clear();
            cout << "Error: invalid argument" << endl;
            error = true;
            return;
        }

        // check for invalid value
        if (val < 0){
            cout << "Error: invalid value" << endl;
            error = true;
            return;
        }

        // check for too few/many args
        too_args(lineStream, error, last_arg);
    }
    return;
}

void size_same(const int size1, const int size2, const string shapetype, bool& error){
    if (!error){
        if(shapetype == "circle" && size1 != size2){
            cout << "Error: invalid value" << endl;
            error = true;
            return;
        }
    }
    return;
}

void angle_input(stringstream& lineStream, int& val, bool& error, const bool last_arg){
    // just check invalid arg, invalid value
    if (!error) {

        // check for no arg found
        if (lineStream.eof() && !last_arg){
            error = true;
            cout << "Error: too few arguments" << endl;
            return;
        }
    
        lineStream >> val;

        // check for invalid arg
        if(lineStream.fail()){
            lineStream.ignore(1000, '\n');
            lineStream.clear();
            cout << "Error: invalid argument" << endl;
            error = true;
            return;
        }

        // check for invalid value
        if (val < 0 || val > 360){
            cout << "Error: invalid value" << endl;
            error = true;
            return;
        }

        // check for too few/many args
        too_args(lineStream, error, last_arg);
    }
    return;
}

void check_name(const string val, bool& error) {

    if (!error){
        // check for name not created
        for (int i = 0; i < max_shapes; i ++){
            if (shapesArray[i]->getName() == val){
                return;
            }
        }
        cout << "Error: shape " << val << " not found" << endl;
        error = true;
        return;
    }
    return;
}

void shape_arr_full(int index, int max, bool& error){

    if (!error){

        if (index >= max){
            cout << "Error: shape array is full" << endl;
            error = true;
            return;
        }
    }
    return;
}

void too_many_args(stringstream& lineStream, bool& error){
    if (!error && lineStream.eof() == 0){
        char temp;
        while(!lineStream.eof()) {
            lineStream >> temp;
            if (temp != ' ' && !lineStream.eof()){
                error = true;
                cout << "Error: too many arguments" << endl;
                return;
            }
        }
    }
}

void too_few_args(stringstream& lineStream, bool& error){
    if (!error && lineStream.eof() == 1){
        error = true;
        cout << "Error: too few arguments" << endl;
        return;
    }
}

void too_args(stringstream& lineStream, bool& error, const bool last_arg){
    if(!error){
        if (last_arg){
           // too_many_args(lineStream, error);
        } else{
            too_few_args(lineStream, error);
        }
    }
}