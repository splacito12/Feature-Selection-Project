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
vector<vector<double>> dataSet;

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

        dataSet.push_back(features);
    }

    //here we will let the user know that we have loaded the data
    //we also have to output the number of instances and features
    cout << "The Data has been loaded successfully." << endl;
    cout << "It has " << dataSet.size() << " instances and ";
    cout << dataSet[0].size() - 1 << " features." << endl;
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
    for(int i = 0; i < dataSet.size(); i++){
        double minDistance = numeric_limits<double>::max();
        int nearestClass = -1;

        //now we will loop through the other instances
        //we do this so that we can find the nearest neighbor
        for(int j = 0; j < dataSet.size(); j++){
            //if the instance are the same skip.
            if(i == j){
                continue;
            }else{
                double distance = calDistance(dataSet[i], dataSet[j], selectedFeatures);

                //But, if the distance is less than the min, update the min and nearest class
                //nearest class is updated with the class of the first instance (or column)
                if(distance < minDistance){
                    minDistance = distance;
                    nearestClass = dataSet[j][0]; 
                }
            }
        }

        //check if the nearest class is the same as the actual
        if(nearestClass == dataSet[i][0]){
            correctClass++;
        }

    }

    //now we calculate the accuracy
    double accuracy = (double)correctClass / dataSet.size() * 100.0;

    return accuracy;
}


//this will be our forward selection algorithm
//we will start with an empty set of features and add one feature at a time
void forwardSelection(){
    vector<int> currFeatures;

    vector<int> bestOverallFeatures;
    double bestOverallAccuracy = 0.0;

    //loop through the features, starting with an empty set
    for(int i = 1; i <= (dataSet[0].size() - 1); i++){
        int bestFeature = -1;
        double bestAccuracy = 0.0;

        //second loop for the features that haven't been selected
        for(int j = 1; j <= (dataSet[0].size() - 1); j++){
            //if already selected, skip
            if(find(currFeatures.begin(), currFeatures.end(),j) != currFeatures.end()){
                continue;
            }

            //create temp to hold curr features
            vector<int> tmpFeatures = currFeatures;
            tmpFeatures.push_back(j);

            //calculate the accuracy
            double currAccuracy = nearestNeighbor(tmpFeatures);

            //if the curr accuracy is better than our current best, update the best
            if(currAccuracy > bestAccuracy){
                bestAccuracy = currAccuracy;
                bestFeature = j;   //also update the best feature
            }

            //output the features and the accuracy
            cout << "Using feature(s) { ";
            for(int x = 0; x < tmpFeatures.size(); x++){
                cout << tmpFeatures[x] << " ";
            }
            cout << "}.";
            cout <<  " Accuracy is: " << currAccuracy << "%" << endl;
        }

        //add the best feature to our current features
        //add the best feature to our current features
        currFeatures.push_back(bestFeature);

        //now, we will output the best feature and accuracy
        cout << "\nThe best feature at this level is: { ";
        for(int k = 0; k < currFeatures.size(); k++){
            cout << currFeatures[k] << " ";
        }
        cout << "}.";
        cout << "Accuracy is: " << bestAccuracy << "%" << endl << endl;

        //update the best overall features and accuracy
        if(bestAccuracy > bestOverallAccuracy){
            bestOverallAccuracy = bestAccuracy;
            bestOverallFeatures = currFeatures;
        }
    }

    //output the best overall 
    cout << "\nFinished Search!!" << endl;
    cout << "The best feature subset is: {";
    for (int i = 0; i < bestOverallFeatures.size(); i++){
        cout << bestOverallFeatures[i] << " ";
    }
    cout << "}.";
    cout << "With an accuracy of: " << bestOverallAccuracy << "%" << endl;
}


//here we will have our backward elimination algorithm
//should be slightly different from forward selection
//we will start with all the features and remove one 
// feature at a time
void backwardElimination(){
    vector<int> currFeatures;
    vector<int> bestOverallFeatures;
    double bestOverallAccuracy = 0.0;

    //first, we start with all the features
    for(int i = 1; i <= (dataSet[0].size() - 1); i++){
        currFeatures.push_back(i);
    }

    bestOverallAccuracy = nearestNeighbor(currFeatures);
    bestOverallFeatures = currFeatures;

    //loop through the remaining features
    while(currFeatures.size() > 1){
        int worstFeature = -1;
        double bestAccuracy = 0.0;
        
        //we will loop through features, again
        //remove one feature at a time
        for(int x = 0; x < currFeatures.size(); x++){
            vector<int> tmpFeatures = currFeatures;

            //remove the feature at index x
            tmpFeatures.erase(tmpFeatures.begin() + x);

            double currAccuracy = nearestNeighbor(tmpFeatures);

            //if curr accuracy is better than our current best, update the best
            if(currAccuracy > bestAccuracy){
                bestAccuracy = currAccuracy;
                worstFeature = x;   //also update the worst feature
            }

            //output the features and the accuracy
            cout << "Using feature(s) { ";
            for(int k = 0; k < tmpFeatures.size(); k++){
                cout << tmpFeatures[k] << " ";
            }
            cout << "}.";
            cout <<  " Accuracy is: " << currAccuracy << "%" << endl;
        }

        //remove the worst feature from our current features
        int removedFeature = currFeatures[worstFeature];
        cout << "\nRemoved feature: " << removedFeature << endl;

        currFeatures.erase(currFeatures.begin() + worstFeature);

        //now, we will output the best feature and accuracy
        cout << "\nThe best feature at this level is: { ";
        for(int k = 0; k < currFeatures.size(); k++){
            cout << currFeatures[k] << " ";
        }
        cout << "}.";
        cout << "Accuracy is: " << bestAccuracy << "%" << endl << endl;

        //update the best overall features and accuracy
        if(bestAccuracy > bestOverallAccuracy){
            bestOverallAccuracy = bestAccuracy;
            bestOverallFeatures = currFeatures;
        }
    }

    //output the best overall 
    cout << "\nFinished Search!!" << endl;
    cout << "The best feature subset is: {";
    for (int i = 0; i < bestOverallFeatures.size(); i++){
        cout << bestOverallFeatures[i] << " ";
    }
    cout << "}.";
    cout << "With an accuracy of: " << bestOverallAccuracy << "%" << endl << endl;

}


//main function will go here
//make sure to use a switch case
int main(){
    string filename;

    cout << "Welcome to Sam's Feature Selection Algorithm!" << endl;

    //ask for the file name
    cout << "\nBefore we start, please enter one of the two filename:" << endl;
    cout << "      1. CS170_Large_DataSet__29.txt" << endl;
    cout << "      2. CS170_Small_DataSet__119.txt" << endl;
    cin >> filename;
    cout << endl;

    //read the dataset
    readDataset(filename);

    //have to include a small output for all the features and the accuracy of using all the features
    //for the data graph
    vector<int> allFeatures;
    for(int i = 1; i <= (dataSet[0].size() - 1); i++){
        allFeatures.push_back(i);
    }

    double allFeaturesAccuracy = nearestNeighbor(allFeatures);
    
    cout << "Using all Features { ";
    for(int i = 0; i < allFeatures.size(); i++){
        cout << allFeatures[i] << " ";
    }
    cout << "}.";
    cout << " Accuracy is: " << allFeaturesAccuracy << "%" << endl << endl;

    //now, ask which algorithm they want to use
    int algorithm;
    cout << "Now, which algorithm do you want to run?" << endl;
    cout << "   1. Forward Selection" << endl;
    cout << "   2. Backward Elimination" << endl;
    cin >> algorithm;
    cout << endl;

    //if it isn't 1 or 2, let them try again
    while(algorithm != 1 && algorithm != 2){

        cout << "I'm sorry, that wasn't a valid choice :(" << endl;
        cout << "Please try again :D!" << endl;
        cin >> algorithm;
        cout << endl;
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