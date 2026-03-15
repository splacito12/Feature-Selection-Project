//Project 2: Feature Selection with Nearest Neighbor
//Name: Samantha Placito Melendrez
//Due: March 16th, 2026

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

//we will store the data of our dataset into a 2D vector
vector<vector<double>> data;

//here will go the function where we read the data
//from the dataset file the user types in 
void readDataset(string filename){}


//now we will need to create a function to calculate the distance between the two instances
//we will basically be using only selected features
//needed for nearest neighbor
//we will be using the equation from the notes
double calDistance(const vector<double> inst1, vector<double> inst2, const vector<int> selectedFeatures){}



//here we will have the nearest neighbor classifier
//it will use leave one out cross validation to calculate the accuracy of the classifier
double nearestNeighbor(vector<int> selectedFeatures){}


//this will be our forward selection algorithm
//we will start with an empty set of features and add one feature at a time
void forwardSelection(){}


//here we will have our backward elimination algorithm
//we will start with all the features and remove one feature at a time
void backwardElimination(){}


//main function will go here
//make sure to use a switch case
int main(){
    string filename;

    cout << "Welcome to Sam's Feature Selection Algorithm!" << endl;

    //ask for the file name
    cout << "Before we start, please enter one of the two filename:" << endl;
    cout << "1. CS170_Large_DataSet__29.txt" << endl;
    cout << "2. CS170_Small_DataSet__119.txt" << endl;
    cin >> filename;

    //read the dataset
    readDataSet(filename);

    //now, ask which algorithm they want to use
    int algorithm;
    cout << "Now, which algorithm do you want to run?" << endl;
    cout << "1. Forward Selection" << endl;
    cout << "2. Backward Elimination" << endl;
    cin >> algorithm;

    //if it isn't 1 or 2, let them try again
    while(algorithm != 1 && algorithm != 2){

        cout << "I'm sorry, that wasn't a valid choice :(" << endl;
        cout << "Please try again :D!" << endl;
        cin >> algorithm;
    }

    switch(algorithm){
        case 1:
            forwardSelection();
            break;
        case 2:
            backwardElimination();
            break;
    }

    return 0;
}