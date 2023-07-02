//
//  parser.cpp
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//  Copyright © 2020 Tarek Abdelrahman.
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
#include "ShapeNode.h"
#include "GroupNode.h"
#include "ShapeList.h"
#include "GroupList.h"

// This is a pointer to the groups list
// The list itseld must be allocated
GroupList* gList;

// ECE244 Student: you may want to add the prototype of
// helper functions you write here
//command functions
void create(stringstream &lineStream);
void group(stringstream &lineStream);
void move(stringstream &lineStream);
void toDelete(stringstream &lineStream);
void draw(stringstream &lineStream);

//helper functions
int nameExists(string name);
void errorResponse(int errorCode, string name);

int main() {
    // Create the groups list
    gList = new GroupList();
    
    // Create the poo group and add it to the group list
    GroupNode* poolGroup = new GroupNode(keyWordsList[NUM_KEYWORDS-1]);
    gList->insert(poolGroup);
    
    string line;
    string command;
    
    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input

    while ( !cin.eof () ) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream (line);
        
        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;

        // Check for the command and act accordingly
        // ECE244 Student: Insert your code here
        if (command == keyWordsList[0]) create(lineStream);
        else if (command == keyWordsList[1]) group(lineStream);
        else if (command == keyWordsList[2]) move(lineStream);
        else if (command == keyWordsList[3]) toDelete(lineStream);
        else if (command == keyWordsList[4]) draw(lineStream);
        
        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);
        
    }  // End input loop until EOF.
    
    return 0;
}

void create(stringstream &lineStream){
    string name, type;
    int locx, locy, sizex, sizey;
    lineStream >> name;
    int nameExistsCode = nameExists(name);
    if (nameExistsCode == 0){
        lineStream >> type;
        lineStream >> locx;
        lineStream >> locy;
        lineStream >> sizex;
        lineStream >> sizey;
        Shape* newShape = new Shape(name, type, locx, locy, sizex, sizey);  //all good create shape
        ShapeNode* newShapeNode = new ShapeNode;//allocate the new shape node
        newShapeNode->setShape(newShape);  //insert the shape in the shape node
        gList->getHead()->getShapeList()->insert(newShapeNode); //insert into the pool
        newShape->draw();//print created node
    } else if (nameExistsCode == 3) errorResponse(0, name); // name is a reserve keyword
    else errorResponse(1, name); // name is either already a shape or a group
}

void group(stringstream &lineStream){
    string name;
    lineStream >> name;
    int nameExistsCode = nameExists(name);
    if (nameExistsCode == 0) { // name is okay to use
        GroupNode* newGroupNode = new GroupNode(name); // allocate the new groupnode
        gList->insert(newGroupNode); // insert it into the list
        cout << name << ": group" << endl; // print the requirement
    } else if (nameExistsCode == 3) errorResponse(0, name); // name is a reserve keyword
    else errorResponse(1, name); // name is either already a shape or a group
}

void move(stringstream &lineStream){
    string name, groupName;
    lineStream >> name;
    int nameExistsCode = nameExists(name);
    if (nameExistsCode == 1){ //if name exists
        lineStream >> groupName;
        nameExistsCode = nameExists(groupName);
        if (nameExistsCode == 2){ //if the group exists
            GroupNode* groupTraverser = gList -> getHead();
            bool shapeFound = false;
            GroupNode* addtoGroup;
            ShapeNode* toMoveShape;
            while (groupTraverser != nullptr) { //find the group we want to the move the shape to
                if (groupTraverser -> getName() == groupName) {
                    addtoGroup = groupTraverser;
                }
                ShapeList* sList = groupTraverser->getShapeList();
                if (sList != nullptr && !shapeFound) { // find the shape we want to move
                    toMoveShape = sList->remove(name); //returns the shape we want to further add to the given group
                    if (toMoveShape != nullptr) { //if the shapefound if not a nullptr then we have found the shape
                        shapeFound = true; //shape is found no need to loop through anymore
                    }
                }
                groupTraverser = groupTraverser -> getNext();
            }
            addtoGroup->getShapeList()->insert(toMoveShape);
            cout << "moved " << name << " to " << groupName << endl;
        } else if (nameExistsCode == 3) errorResponse(0, groupName); // groupname is a reserve keyword
        else errorResponse(3, groupName); // groupname does not exist
    }else if (nameExistsCode == 3) errorResponse(0, name); // name is a reserve keyword
    else errorResponse(2, name); // name does not exist
}

void toDelete(stringstream &lineStream){
    string name;
    lineStream >> name;
    int nameExistsCode = nameExists(name);
    if (nameExistsCode == 3) errorResponse(0, name);
    else if (nameExistsCode == 0) errorResponse(2, name);
    else if (nameExistsCode == 2) { //delete group
        GroupNode* toDelete = gList->remove(name); //returns the head of the 
        gList->getHead()->getShapeList()->insert(toDelete->getShapeList()->getHead());
        //loop until deletion head points to null (trick valgrind)
        while (toDelete->getShapeList()->getHead() != nullptr) {
            toDelete->getShapeList()->setHead(toDelete->getShapeList()->getHead()->getNext());
        }
        delete toDelete;
        toDelete = nullptr;
        cout << name << ": deleted" << endl;
    } else if (nameExistsCode == 1) {
        GroupNode* groupTraverser = gList->getHead();
        while (groupTraverser != nullptr) {//find the name in the each group node
            ShapeList* sList = groupTraverser->getShapeList(); 
            if (sList != nullptr) {
            ShapeNode* toDeleteShape = sList->remove(name); // find and return the shape to be delete
                if (toDeleteShape != nullptr) {
                    cout << toDeleteShape->getShape()->getName() << ": deleted" << endl; //output error message
                    delete toDeleteShape;
                }
            }
            groupTraverser = groupTraverser -> getNext();
        }
    }
}

void draw(stringstream &lineStream){
    cout << "drawing:" << endl;
    gList -> print();
}

/*
Returns 0 when name not found as a group or shape name
Returns 1 when name is a already a shape
Returns 2 when name is a group
Returns 3 when name is a reserve keyword
*/
int nameExists(string name) {
    //check key words
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (name == keyWordsList[i]) {
            return 3; //reserve keyword
        }
    }
    //check type words
    for (int i = 0; i < NUM_TYPES; i++) {
        if (name == shapeTypesList[i]) {
            return 3; //reserve keyword
        }
    }
    //check group names
    GroupNode* traverseG = gList -> getHead();
    while (traverseG != nullptr) {
        if (name == traverseG -> getName()) {
            return 2; //name is a group name
        }
        else {
            //check shape names
            ShapeList* sList = traverseG->getShapeList();
            if (sList != nullptr) {
                if (sList->find(name) != nullptr) {
                    return 1; //name is already a shape name
                }
            }
            traverseG = traverseG -> getNext(); //goto the next groupnode
        }
    }
    return 0; //all checked; this name is availible for use
}

/*switch case statements to print error statements based on predecided numeric code input
3: Group "name" not found
2: Shape "Name" not found
1: name "name" exists 
0: Invalid Name
*/
void errorResponse(int errorCode, string name) {
    string output = "error: ";
    switch (errorCode) {
        case 0: //invalid name
            output += "invalid name";
            break;
        case 1: // name exists
            output += "name " + name + " exists";
            break;
        case 2: // shape not found
            output += "shape " + name + " not found";
            break;
        case 3: // group not found
            output += "group " + name + " not found";
            break;
        default: // should never happen
            output = "?";
            break;
    }

    cout << output << endl;
}