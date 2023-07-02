//
//  GroupList.cpp
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student:
//  Write the implementation (and only the implementation) of the GroupList class below
#include "GroupList.h"
#include "GroupNode.h"
#include "ShapeList.h"
#include "ShapeNode.h"
#include "Shape.h"
#include <iostream>
#include <string>

GroupList::GroupList() {
    head = nullptr;
}

GroupList::~GroupList() {
    GroupNode* ptr;
    while (head != nullptr) {
        ptr = head;
        head = ptr -> getNext();
        delete ptr;
    }
}

GroupNode* GroupList::getHead() const { return head; }
void GroupList::setHead(GroupNode* ptr) { head = ptr; }

void GroupList::insert(GroupNode* gnode) {
    if (head == nullptr) { 
        head = gnode; //inserting at the front
    } else { 
        GroupNode* ptr = head;
        while (ptr -> getNext() != nullptr) { //traverse till the end
            ptr = ptr -> getNext(); //get the last node
        }
        ptr -> setNext(gnode); //append to the last node
    }
}
GroupNode* GroupList::remove(string name) {
    GroupNode* previous = nullptr;
    GroupNode* current = head;
    while (current != nullptr) { //traverse till the end
        GroupNode* next = current -> getNext();
        if (current -> getName() == name) {
            if (previous == nullptr) {//prev is null which means curr is head so we remove head 
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
void GroupList::print() const { //grouplist's print is called THEN we call groupnode's print THEN we 
//call shapelist's print THEN we call shapenode's print THEN we call shapenode's print then we call shape's draw() [FINALLY LOL]
    GroupNode* ptr = head;
    while (ptr != nullptr) {
        ptr -> print(); //call the group nodes's print command
        ptr = ptr -> getNext();
    }
}




