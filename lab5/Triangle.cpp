//
//  Triangle.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student: Write the implementation of the class Rectangle here

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#include "Shape.h"
#include "Triangle.h"


//Constructor
Triangle::Triangle (string n, float xcent, float ycent, float x1, float y1, float x2, float y2, float x3, float y3) : Shape(n, xcent, ycent) {
    cx1 = x1;
    cx2 = x2;
    cx3 = x3;
    cy1 = y1;
    cy2 = y2;
    cy3 = y3;
}

//Destructor
Triangle::~Triangle() {
    //Nothing
}

//Accessors
float Triangle::getx1() const { return cx1; }
float Triangle::getx2() const { return cx2; }
float Triangle::getx3() const { return cx3; }
float Triangle::gety1() const { return cy1; }
float Triangle::gety2() const { return cy2; }
float Triangle::gety3() const { return cy3; }

//Mutators
void Triangle::setx1(float x) { cx1 = x; }
void Triangle::setx2(float x) { cx2 = x; }
void Triangle::setx3(float x) { cx3 = x; }
void Triangle::sety1(float y) { cy1 = y; }
void Triangle::sety2(float y) { cy2 = y; }
void Triangle::sety3(float y) { cy3 = y; }

//Utility Methods
void Triangle::draw() const {
    cout << std::fixed;
    cout << std::setprecision(2);

    cout << "triangle: " << name << " "
         << x_centre << " " << y_centre
         << " " << cx1 << " " << cy1
         << " " << cx2 << " " << cy2
         << " " << cx3 << " " << cy3
         << " " << computeArea() << endl;
}

float Triangle::computeArea() const {
    float a = 0.5 * (cx1 * (cy2 - cy3) + cx2 * (cy3 - cy1) + cx3 * (cy1 - cy2));
    return (a >= 0.0) ? a : -a; //test case triangle area can't be negative
}

Shape* Triangle::clone() const {
    return (new Triangle(*this));
}