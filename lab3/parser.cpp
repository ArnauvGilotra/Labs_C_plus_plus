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

// This is the shape array, to be dynamically allocated
Shape** shapesArray;

// The number of shapes in the database, to be incremented 
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes;

// ECE244 Student: you may want to add the prototype of
// helper functions you write here
// command functions. 1 for each
void maxShapes (stringstream &line);
void create (stringstream &line);
void move (stringstream &line);
void rotate (stringstream &line);
void draw (stringstream &line);
void deleteShapes (stringstream &line);

// Primary helper functions declarations:
// Helper functions
int shapeIndex (string name);
void deleteAllShapes ();
void deleteOneShape (string name);
void drawAllShapes ();
bool shapeNameExists (string name);
bool isSoftwareKeyword(string name);
bool invalidType (string type);
bool tooFewOrInvalid (stringstream &line);
bool invalidPosition (int pos);
bool invalidRotation (int angle);
bool tooManyArgs (stringstream &line);
bool isfloat (float ly, int leny);

//Error Code print functions declaration:
void typeInvalid ();
void commandInvalid ();
void nameExists (string name);
void nameNF (string name);
void argInvalid ();
void nameInvalid ();
void arrFull ();
void valInvalid ();
void tooManyArguments ();
void tooFewArguments ();

//End----------------------------------------------------------------------------------------------------------------------------------------

int main() {

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
        bool validCommand = false;
        for (int i = 0; i < 7; i++){ //check if command is valid
            if (command == keyWordsList[i]){
                validCommand = true;
            }
        }
        if (validCommand)
        {
            if (command == "maxShapes") maxShapes(lineStream);
            else if (command == "create") create(lineStream);
            else if (command == "move") move(lineStream);
            else if (command == "rotate") rotate(lineStream);
            else if (command == "draw") draw(lineStream);
            else if (command == "delete") deleteShapes(lineStream);
        } else { //command not valid
            commandInvalid(); //print error code
        }
        // Once the command has been processed, prompt for the
        // next command
        cout << "> "; // Prompt for input
        getline(cin, line);
    }  // End input loop until EOF.
    
    return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------
//command function for dynamically allocating a shapes pointer array
void maxShapes (stringstream &line){
    
    if (max_shapes > 0){
        for (int i=0; i < max_shapes; i++) //iterate till shape counter - 1
        {
            if (shapesArray[i] != NULL) { //not NULL
                delete shapesArray[i];
                shapesArray[i] = NULL;
            }
        }
        if (shapesArray != NULL){
            delete shapesArray;
            shapesArray = NULL;
        }
        shapeCount = 0;
    }
    float ms;
    line >> ms;
    max_shapes = (int)ms;
    if (tooFewOrInvalid(line)) return;
    if (isfloat(ms, max_shapes)) return;
    shapesArray = new Shape*[max_shapes];
    for (int i=0; i < max_shapes; i++) {
        shapesArray[i] = NULL;
    }
    cout << "New database: max shapes is " << max_shapes << endl;
}

//command function for deleteing shapes
void deleteShapes (stringstream &line){
    string argNext;
    line >> argNext;
    if (tooFewOrInvalid(line)) return; //Invalid Input
    if (argNext == keyWordsList[0]){ //Commanded to delete "ALL"
        if (tooManyArgs(line)) return; //Invalid Input
        else deleteAllShapes();
    } else {//is a single shape name
        if (!shapeNameExists(argNext)){ //shape name not found
            nameNF(argNext); // print error 
            return;
        } else { //found name
            if (tooManyArgs(line))  return; //incorrect input command syntax
            else deleteOneShape(argNext);
        }
    }
}

//command function for setting shape rotation
void rotate (stringstream &line){
    string name; //local variables
    int angle;
    float fa;
    line >> name;
    if (tooFewOrInvalid(line)) return; //incorrect input command syntax
    if (!shapeNameExists(name)){ //shape is not found 
        nameNF(name); // print error
        return;
    }
    line >> fa;
    angle = (int)fa;
    if (tooFewOrInvalid(line)) return; //incorrect input command syntax
    if (isfloat(fa, angle)) return;
    if (invalidRotation(angle)) return; //check 0<angle<360
    if (tooManyArgs(line)) return; //incorrect input command syntax
    shapesArray[shapeIndex(name)]->setRotate(angle); //call member func "setRotate" to set the rotation angle attribute of the shape
    cout << "Rotated " << name << " by " << angle << " degrees " << endl; //print reqd statement
}

//command function for drawing shapes
void draw (stringstream &line){ //same as delete but calling draw func rather than delete
    string nextArg;
    line >> nextArg;
    if (tooFewOrInvalid(line)) return; //Invalid Input
    if (nextArg == keyWordsList[0]){ //check if command is all
        if (tooManyArgs(line)) return; //incorrect input command syntax
        else drawAllShapes();  
    } else { //command seems to be a name
        if (!shapeNameExists(nextArg)){ // name is not found
            nameNF(nextArg); // print error
            return;
        } else {
            if (tooManyArgs(line)) return; //incorrect input command syntax
            cout << "Drew " ; //reqd statement
            shapesArray[shapeIndex(nextArg)]->draw(); //call the member func "draw" for the rest of the print requirements
        }
    }
}
//Move command func
void move (stringstream &line)
{
    int x, y; 
    float fx, fy; //pull these from the ss one by one
    string name;
    line >> name;

    if (tooFewOrInvalid(line)) return; //Invalid Input
    if (!shapeNameExists(name)){ //shape isnt in the array
        nameNF(name); //print EC
        return;
    }
    line >> fx;
    x = (int)fx;
    if (tooFewOrInvalid(line)) return; //Invalid Input
    if (isfloat(fx, x)) return;
    if (invalidPosition(x)) return; //x has be to a appropriate number

    line >> fy;
    y = (int)fy;
    if (tooFewOrInvalid(line)) return;
    if (isfloat(fy, y)) return; //do same for y
    if (invalidPosition(y)) return;

    if (tooManyArgs(line)) return; //incorrect input command syntax
    shapesArray[shapeIndex(name)]->setXlocation(x); //set x & y using member functions
    shapesArray[shapeIndex(name)]->setYlocation(y);
    cout << "Moved " << name << " to " << x << " " << y << endl; // print reqd statement
}
//The CREATE command func
void create (stringstream &line)
{
    string name, type; //pull these from the ss one by one
    int lenx, leny,x, y;
    float lx, ly, fx, fy;
    
    line >> name;
    if (tooFewOrInvalid(line)) return; //Invalid Input
    if (isSoftwareKeyword(name)) return; //name is a software reserve keyword
    if (shapeNameExists(name)){ //name is already taken
        nameExists(name); //print EC
        return;
    }

    line >> type;
    if (tooFewOrInvalid(line)) return;
    if (invalidType(type)) return; // invalid type
    
    line >> fx;
    x = (int)fx;
    if (tooFewOrInvalid(line)) return;
    if (isfloat(fx, x)) return;
    if (invalidPosition(x)) return;

    line >> fy;
    y = (int)fy;
    if (tooFewOrInvalid(line)) return;
    if (isfloat(fy, y)) return;
    if (invalidPosition(y)) return; // y has to be +ve

    line >> lx;
    lenx = (int)lx;
    if (tooFewOrInvalid(line)) return;
    if (isfloat(lx, lenx)) return;
    if (invalidPosition(lenx)) return; //lenght in x has to be a +ve numbers

    line >> ly;
    leny = (int)ly;
    if (tooFewOrInvalid(line)) return;
    if (isfloat(ly, leny)) return;
    if (invalidPosition(leny)) return;

    if (type == shapeTypesList[0] && leny != lenx){
        valInvalid ();
        return;
    }

    if (tooManyArgs(line)) return; //incorrect input command syntax
    if (max_shapes == shapeCount) { //arr is full
        arrFull();
        return;
    }
    shapesArray[shapeCount] = new Shape(name, type, x, lenx, y, leny); //call shape constructor
    cout << "Created " << name << ": " << type << " " << x << " " << y << " " << lenx << " " << leny << endl;
    shapeCount++;
}
//-----------------------------------------------------------------------------------------------------------------------------------
//Helper Function Declarations Below: 
//Function has the general code to find the index of a shape in the array
int shapeIndex (string name){
    for (int i=0; i < max_shapes; i++){
        if (shapesArray != NULL){
            if (shapesArray[i] != NULL){
                if (shapesArray[i]->getName() == name){
                return i;
                }
            }   
        }
    }
    return -1;
}

// Function Deletes Everything
void deleteAllShapes (){
    for (int i=0; i < max_shapes; i++) //iterate till shape counter - 1
    {
        if (shapesArray[i] != NULL) { //not NULL
            delete shapesArray[i];
            shapesArray[i] = NULL;
        }
    }
    
    shapeCount = 0;
    cout << "Deleted: all shapes" << endl;
    
}

// Function deletes one shape whoes name has been inputed
void deleteOneShape (string name){
    for (int i = 0; i < max_shapes; i++) {
        if (shapesArray[i]!=NULL){
            if (name == shapesArray[i]->getName()) {
                cout << "Deleted shape " << name << endl;
                delete shapesArray[i];
                shapesArray[i] = NULL;
                break;
            }
        }
    }
}

// Function outputs the required statement for draw and also calls the draw member function for each shape
void drawAllShapes (){
    cout << "Drew all shapes" << endl;
    for (int i=0; i < shapeCount; i++)
    {
        if (shapesArray != NULL){
            if (shapesArray[i] != NULL){
                shapesArray[i]->draw();
            }
        }
    }
}

// Functions checks if the name already exists in the shape array 
bool shapeNameExists (string name){
    for (int i = 0; i < max_shapes; i++)
    {
        if (shapesArray != NULL){
            if (shapesArray[i] != NULL){
                if (name == shapesArray[i]->getName()) return true;
            }
        }
    }
    return false;
}

// Function checks that for any input shape name matching an reserved type name of the defined arrays in globals.h
bool isSoftwareKeyword (string name){
    for (int i = 0; i < 3; i++)
    {
        // Checks if it is a shape name
        if (name == shapeTypesList[i])
        {
            nameInvalid();
            return true;
        }
    }
    
    for (int i = 0; i < 7; i++)
    {
        // Checks if it is a command/key word
        if (name == keyWordsList[i])
        {
            nameInvalid();
            return true;
        }
    }
    return false;
}

// Functions checks if the shape type is supported from the defined shape type array. If there is no match then we call Invalid Type Error Function
bool invalidType (string type){

    for (int i = 0; i < 4; i++)
    {
        if (type == shapeTypesList[i]) return false;
    }
    typeInvalid();
    return true;
}

// Checks for the eof character and if its not found then calls the Invalid Argument error function
bool tooFewOrInvalid (stringstream &line){
    if (line.fail())
    {
        if (line.eof()) tooFewArguments();
        else argInvalid();
            return true;
    }
    return false;
}

// Function checks if the input position is less than zero. Calls the Invalid Value error function if appropriate
bool invalidPosition (int pos){
    if (pos < 0)
    {
        valInvalid();
        return true;
    }
    else return false;
}

//Function checks whether angle is between 0 - 360 Degress. Calls the error function if appropriate
bool invalidRotation (int angle){
    if (angle < 0 || angle > 360)
    {
        valInvalid();
        return true;
    }
    else return false;
}

// Function will check if there is anything further in the stream and if there then return true
bool tooManyArgs(stringstream &line){
    string test;
    
    line >> test;
    if (!line.fail())
    {
         tooManyArguments();
         return true;
    }
    return false;
}

bool isfloat (float ly, int leny){
    float a = ly - leny;
    if (a != 0){
        argInvalid();
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//error functions which are called to print the appropriate error codes
void typeInvalid () { cout << "Error: invalid shape type" << endl; }
void commandInvalid () { cout << "Error: invalid command" << endl; }
void nameExists (string name) { cout << "Error: shape " << name << " exists" << endl; }
void nameNF (string name) { cout << "Error: shape " << name << " not found" << endl;}
void argInvalid () { cout << "Error: invalid argument" << endl; }
void nameInvalid () { cout << "Error: invalid shape name" << endl; }
void arrFull () { cout << "Error: shape array is full" << endl; }
void valInvalid () { cout << "Error: invalid value" << endl; }
void tooManyArguments () { cout << "Error: too many arguments" << endl; }
void tooFewArguments () { cout << "Error: too few arguments" << endl; }