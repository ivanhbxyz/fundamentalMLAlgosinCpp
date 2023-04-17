#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>
using namespace std;

/*
    Name: ivanhb
    Course: Machine Learning
    
    a logistic regression implementation in cpp
    
*/

vector <double> calc_prob_vector(vector <double> dataV1, vector <double> dataV2, vector <double> weights){
    vector <double> prob_vector;
    double temp = 0;
    for(int i = 0; i < dataV1.size(); i++){

        for(int j =0; j < weights.size(); j++){
        temp = dataV1.at(i)*weights.at(j) + dataV2.at(i)*weights.at(j);
        }
        prob_vector.push_back(temp);
    }
    return prob_vector;
}

vector <double> zVec(vector <double> dataV1, vector <double> dataV2, vector <double> weights)
{
    vector <double> prob_vector;
    double temp = 0;
    for(int i = 0; i < dataV1.size(); i++){

        
        temp = dataV1.at(i)*weights.at(0) + dataV2.at(i)*weights.at(1);
        

        prob_vector.push_back(temp);
    }

    return prob_vector;
}

vector <double> sigmoid(vector <double> z) {
    // What does sigmoid take in as an argument?
    vector <double> result;
    for (int i = 0; i < z.size(); i++){
        result.push_back(1.0 / (1 + exp(- (z.at(i)))));
    }
    return result;
}

vector <double> calc_error(vector<int> labels, vector<double> prob_vector){
    vector <double> error_vec;
    for(int i = 0; i < prob_vector.size(); i++){
        error_vec.push_back(labels.at(i) - prob_vector.at(i));
    }
    return error_vec;
}

vector<double> computeWs(vector <double>dataV1, vector <double> dataV2, vector<int> labels, int observs)
{
    vector <double> prob_vector;
    vector <double> weights{1,1};
    vector <double> errors;
    double learning_rate = 0.001;

    vector <double> dataTerror{0,0};
    double temp1;
    double temp2;

    //////
    for(int i = 0; i < 500; i++){


        prob_vector = sigmoid( zVec(dataV1, dataV2, weights) );
        errors = calc_error(labels, prob_vector);


        // t(data_matrix) %*% error
        for(int j = 0; j < dataV1.size(); j++){
                    dataTerror.at(0) += dataV1.at(j) * errors.at(j);
                    dataTerror.at(1) += dataV2.at(j) * errors.at(j);
        }

        dataTerror.at(0)= learning_rate * dataTerror.at(0);
        dataTerror.at(1)= learning_rate * dataTerror.at(1);

        weights.at(0)= weights.at(0) + dataTerror.at(0);
        weights.at(1)= weights.at(1) + dataTerror.at(1);

    }
    return weights;
}



vector <double> probCalc(vector <double> predictedVect) {
    // What does sigmoid take in as an argument?
    vector <double> result;
    for (int i = 0; i < predictedVect.size(); i++){
        result.push_back(exp(predictedVect.at(i)) / (1 + exp(predictedVect.at(i))));
    }
    return result;
}

vector <int> probClassification(vector <double> probVec) {
    // What does sigmoid take in as an argument?
    vector <int> result;
    for (int i = 0; i < probVec.size(); i++){
        if(probVec.at(i) < 0.5)
            result.push_back(0);
        if(probVec.at(i) >= 0.5)
            result.push_back(1);
    }
    return result;
}

vector<vector<int>> genTable(vector <int> pred, vector<int> target_arr)
{
    vector<vector<int>> table = {{0,0},
                                {0,0}};

       for(int i = 0; i < target_arr.size(); i++){
           
           if(target_arr.at(i)==1 && pred.at(i) == 1){
                table.at(0).at(0)+=1;
           } else if(target_arr.at(i)==0 && pred.at(i) == 1){
                table.at(0).at(1)+=1;
           } else if(target_arr.at(i)==1 && pred.at(i) == 0){
                table.at(1).at(0)+=1;
           } else if(target_arr.at(i)==0 && pred.at(i) == 0){
                table.at(1).at(1)+=1;
           }
        }

    return table;
}




// === main ===
int main(int argc, char* argv[]) {

ifstream inFS; // Input file stream
string headings;
string target_str, predictor_str;

const int MAX_LEN = 1050;// Knowing that the data set consists of 506 rows
vector<double> predictor_arr(MAX_LEN);
vector<int> target_arr(MAX_LEN);

string targetArg;
string predictorArg;

// Try to open file
//inFS.open(argv[1]);
inFS.open(argv[1]);
//inFS.open("titanic_project_short.csv");
if (!inFS.is_open()) {
    cout << "Could not open file " << argv[1] << endl;
    return 1; // 1 indicates error
}

cout << "Reading Lines" << endl << endl;
getline(inFS, headings); //assuming heading
cout << "Headings: " << headings << endl;

int numObservs = 0;

string singleLine;

while (inFS.good()) { // S:READ FILE

    getline(inFS, singleLine, ',');
    getline(inFS, predictor_str, ',');
    getline(inFS, target_str, '\n');
    // Hard coded, but can you make it a better script?

    target_arr.at(numObservs) = stof(target_str);
    predictor_arr.at(numObservs) = stof(predictor_str);
    numObservs++;
    } // E:READ FILE
    inFS.close();
    cout << "Values loaded.\nClosing file titanic_project.csv. \n" << endl;

    // // create the test/train split
    target_arr.resize(numObservs) ; // S: DATA WRANGLING
    predictor_arr.resize (numObservs) ;
    int trainCount = 900;
    int testCount = target_arr.size() - trainCount;
    // training vectors    
    vector<double> trainArr_pred;
    vector<int> trainArr_target;
    // test vectors
    vector<double> testArr_pred;
    vector<int> testArr_target;

    for(int i = 0; i < numObservs ; i++){ // train/test split

        if(i < trainCount){
        trainArr_pred.push_back(predictor_arr.at(i));
        trainArr_target.push_back(target_arr.at(i));
        }
        if(i >= trainCount){
        testArr_pred.push_back(predictor_arr.at(i));
        testArr_target.push_back(target_arr.at(i));
        }
    }
    trainArr_target.resize(trainCount) ;
    trainArr_pred.resize(trainCount);

    testArr_target.resize (testCount);
    testArr_pred.resize(testCount);

    // S: MALLOC FOR DATA
    vector <double> foundWeights;

    // create dataV1
    vector <double> dataV1; // the required the 1s vector
    for(int i = 0; i < trainCount; i++){
        dataV1.push_back(1);
    }

    // generate 1s vector for testing. 
    vector <double> test_dataV1; // the required 1s
    for(int i = 0; i < testCount; i++){
        test_dataV1.push_back(1);
    }

    double accuracy;
    double sensitivity;
    double specificity;

    vector <double> prob_vec;
    vector <double> predicted_vec;
    vector <int> predictions;
    vector <vector<int>> table;

    clockid_t start, end;
    start = clock(); // start modeling timer

    foundWeights = computeWs(dataV1, trainArr_pred, trainArr_target, trainCount);

    //  NOW TEST
    
    predicted_vec = zVec(test_dataV1, testArr_pred, foundWeights); // CHANGE THE NAME
    prob_vec = probCalc(predicted_vec);

    // compute classification
    predictions = probClassification(prob_vec);

    // calculate the mean correct predictions
    table = genTable(predictions, testArr_target);

    accuracy = (double)(table.at(0).at(0) + table.at(1).at(1)) / (double)( table.at(0).at(0) + table.at(0).at(1) + table.at(1).at(0) + table.at(1).at(1) );
    sensitivity = (double)(table.at(0).at(0) ) / (double)( table.at(0).at(0) + table.at(1).at(0));
    specificity = (double)(table.at(1).at(1)) / (double)( table.at(0).at(1) + table.at(1).at(1) );

    end = clock();
    cout << "Coefficients: " << endl;
    cout << "\tWeight Estimates:" << endl;
    cout << "\t   (Intercept):: " << foundWeights.at(0) << endl;
    cout << "\t   pClass:: " << foundWeights.at(1) << endl;

    cout << "\n" << endl;
    cout << "Model Metrics:" << endl ;
    cout << "\taccuracy:: " << accuracy << endl;
    cout << "\tsensitivity:: " << sensitivity << endl;
    cout << "\tspecificity:: " << specificity << endl << endl;

    // TIME OUTPUT

    // Calculating total time taken by the program.
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Modeling Time:: " << fixed 
         << time_taken << setprecision(3);
    cout << " sec " << endl;

   return 0;
}