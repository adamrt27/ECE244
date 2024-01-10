//
//  Triangle.h
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

#ifndef Triangle_h
#define Triangle_h

#include <iostream>
#include <string>
using namespace std;


// This class implements the triangle Shape objects.
// ECE244 Student: Write the definition of the class here.


#include "Shape.h"

class Triangle : public Shape {
private: 
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;
public:
    // Constructor 
    Triangle (string n, float x1, float y1, float x_2, float y_2, float x_3, float y_3);

    // Destructor
    virtual ~Triangle();

    // Accessors
    float get_x1() const;
    float get_y1() const;
    float get_x2() const;
    float get_y2() const;
    float get_x3() const;
    float get_y3() const;

    // Mutators
    void set_x1(float x_1);
    void set_y1(float y_1);
    void set_x2(float x_2);
    void set_y2(float y_2);
    void set_x3(float x_3);
    void set_y3(float y_3);

    // Utility methods
    virtual void draw() const;

    virtual float computeArea() const;

    virtual Shape* clone() const;
};

#endif /* Triangle_h */


