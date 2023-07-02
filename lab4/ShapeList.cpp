//
//  ShapeList.cpp
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student:
//  Write the implementation (and only the implementation) of the ShapeList class below


#include "ShapeList.h"
#include "ShapeNode.h"
#include "Shape.h"
#include <iostream>
#include <string>

ShapeList::ShapeList() {
    head = NULL;
}

ShapeList::~ShapeList() {
    ShapeNode* ptr;
    while (head != nullptr) { //traverse till the end
        ptr = head; //ptr is the first node
        head = ptr -> getNext(); //make head the next node
        delete ptr; //delete the "previously" head node
    }
}

ShapeNode* ShapeList::getHead() const { return head; }
void ShapeList::setHead(ShapeNode* ptr) { head = ptr;}

ShapeNode* ShapeList::find(string name) const {
    ShapeNode* current = head;
    while (current != nullptr) { //traverse till the end
        if (current -> getShape() -> getName() == name) {
            return current;
        } else {
            current = current -> getNext();
        }
    }
    return nullptr;
}

void ShapeList::insert(ShapeNode* snode) {
    if (head == nullptr) {
        head = snode;//inserting at the front
    }
    else {
        ShapeNode* ptr = head;
        while (ptr -> getNext() != nullptr) { //traverse till the end
            ptr = ptr -> getNext(); //get the last node
        }
        ptr -> setNext(snode); //append to the last node
    }
}

ShapeNode* ShapeList::remove(string name) {
    ShapeNode* previous = nullptr;
    ShapeNode* current = head;
    while (current != nullptr) { //traverse till the end
        ShapeNode* next = current -> getNext();
        if (current -> getShape() -> getName() == name) {
            if (previous == nullptr) { //prev is null which means curr is head so we remove head 
                head = next;//set the new head to the next link
            } else {
                previous -> setNext(next); // make the prvious link point to the node right after the removed node
            }
            current -> setNext(nullptr);
            return current;
        }
        else {
            previous = current;
            current = next;
            next = (next != nullptr) ? next -> getNext() : nullptr; //set Next to the next node's link only it is already isnt null
        }
    }
    return nullptr; //nthing found
}

void ShapeList::print() const {
    ShapeNode* ptr = head;
    while (ptr != nullptr) { //traverse until end
        ptr -> print();
        ptr = ptr -> getNext();
    }
}