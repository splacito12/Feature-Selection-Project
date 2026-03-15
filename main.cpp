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
void readDataset(string filename){
    ifstream file(filename);
    string line;

    //add an error message
    if(!file.is_open()){
        cerr << "Error: Could not open the file " << filename << endl;
        exit(1);
    }

    //store the data into a vector
    while(getline(file, line)){
        stringstream ss(line);

        double value;
        vector<double> features;

        //we have to read from the file
        while(ss >> value){
            features.push_back(value);
        }

        data.push_back(features);
    }

    //here we will let the user know that we have loaded the data
    //we also have to output the number of instances and features
    cout << "The Data has been loaded successfully." << endl;
    cout << "It has " << data.size() << " instances and ";
    cout << data[0].size() - 1 << " features." << endl;
}


//now we will need to create a function to calculate the distance between the two instances
//we will basically be using only selected features
//needed for nearest neighbor
//we will be using the equation from the notes
double calDistance(const vector<double>& inst1, const vector<double>& inst2, const vector<int> selectedFeatures){
    double distance = 0.0;

    //loop through the selected features 
    for(int i = 0; i < selectedFeatures.size(); i++){
        int indx = selectedFeatures[i];

        //calculate the distance
        //we will be using euclidean
        distance += pow(inst1[indx] - inst2[indx], 2);
    }
    return sqrt(distance);
}



//here we will have the nearest neighbor classifier
//it will use leave one out cross validation to calculate the accuracy of the classifier
double nearestNeighbor(vector<int> selectedFeatures){
    //count the number of correct classifications
    int correctClass = 0;

    //loop through each instance in the dataset
    for(int i = 0; i < data.size(); i++){
        double minDistance = numeric_limits<double>::max();
        int nearestClass = -1;

        //now we will loop through the other instances
        //we do this so that we can find the nearest neighbor
        for(int j = 0; j < data.size(); j++){
            //if the instance are the same skip.
            if(i == j){
                continue;
            }else{
                double distance = calDistance(data[i], data[j], selectedFeatures);

                //But, if the distance is less than the min, update the min and nearest class
                //nearest class is updated with the class of the first instance (or column)
                if(distance < minDistance){
                    minDistance = distance;
                    nearestClass = data[j][0]; 
                }
            }
        }

        //check if the nearest class is the same as the actual
        if(nearestClass == data[i][0]){
            correctClass++;
        }

    }

    //now we calculate the accuracy
    double accuracy = (double)correctClass / data.size() * 100.0;

    return accuracy;
}


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

    //have to include a small output for all the features and the accuracy of using all the features
    //for the data graph
    vector<int> allFeatures;
    for(int i = 1; i <= (data[0].size() - 1); i++){
        allFeatures.push_back(i);
    }

    double allFeaturesAccuracy = nearestNeighbor(allFeatures);
    
    cout << "Using all Features { ";
    for(int i = 0; i < allFeatures.size(); i++){
        cout << allFeatures[i] << " ";
    }
    cout << " }.";
    cout << " Accuracy is: " << allFeaturesAccuracy << "%" << endl;

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