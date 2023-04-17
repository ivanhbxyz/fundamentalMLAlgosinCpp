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
	
    a naive bayes implementation in cpp.
	
*/

vector <double> calcPrioriVec(vector <int> target)
{
    vector<double> result = {0,0};

    for(int i = 0; i < target.size(); i++){

        if(target.at(i) == 0)
            result.at(0) += 1;
        if(target.at(i) == 1)
            result.at(1) += 1;
    }

        result.at(0) /= target.size();
        result.at(1) /= target.size();

        return result; // <target?FALSE, target?TRUE>

}

vector <int> countTagetSplit(vector <int> target)
{
    vector<int> result = {0,0};

    for(int i = 0; i < target.size(); i++){

        if(target.at(i) == 0)
            result.at(0) += 1;
        if(target.at(i) == 1)
            result.at(1) += 1;
    }
     return result; // <target?FALSE, target?TRUE>

}

vector <double> calc_error(vector<int> labels, vector<double> prob_vector){
    vector <double> error_vec;
    for(int i = 0; i < prob_vector.size(); i++){
        error_vec.push_back(labels.at(i) - prob_vector.at(i));
    }
    return error_vec;
}

vector <double> probCalc(vector <double> predictedVect) {
    // What does sigmoid take in as an argument?
    vector <double> result;
    for (int i = 0; i < predictedVect.size(); i++){
        result.push_back(exp(predictedVect.at(i)) / (1 + exp(predictedVect.at(i))));
    }
    return result;
}


double getMean(vector <int> target, vector<double> predictor, int factor_val){ 
    double mean = 0;
    int j = 0;
    for(int i = 0; i < target.size(); i++) {

        if(target.at(i) == factor_val){
            mean += predictor.at(i);
            j++;
        }
    }
    mean = mean/j;
    return mean;
}

double getVar(vector <int> target, vector<double> predictor, int factor_val, double mean){
    
    double variance = 0;
    int j=0;

    for(int i = 0; i < target.size(); i++) {
        if(target.at(i) == factor_val) {
            variance += (predictor.at(i) - mean)*(predictor.at(i) - mean);
            j++;
        }
    }
    variance/=j;
    return variance;
}

double calc_age_lh(double target_value, double mean, double variance)
{
    double stdDev = sqrt(variance);

    double probDensity = 0;
    probDensity = (1 / ( stdDev * sqrt(2 * M_PI))) * exp((-0.5)*( ((target_value - mean)/stdDev) * ((target_value - mean)/stdDev) ));
    return probDensity;
}

vector<vector<int>> genTable(vector<int> target_arr, vector<double> pred_t, vector<double> pred_f)
{
    vector<vector<int>> table = {{0,0},
                                {0,0}};

       for(int i = 0; i < target_arr.size(); i++){
           
           if(target_arr.at(i)==1 && pred_t.at(i) > 0.5){
                table.at(0).at(0)+=1;
           } else if(target_arr.at(i)==0 && pred_t.at(i) > 0.5){
                table.at(0).at(1)+=1;
           } else if(target_arr.at(i)==1 && pred_t.at(i) <= 0.5){
                table.at(1).at(0)+=1;
           } else if(target_arr.at(i)==0 && pred_t.at(i) <= 0.5){
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
vector<double> predictor1_arr(MAX_LEN); // pclass
vector<double> predictor2_arr(MAX_LEN); // sex
vector<double> predictor3_arr(MAX_LEN); // age

vector<int> target_arr(MAX_LEN);

// Try to open file
inFS.open(argv[1]);
if (!inFS.is_open()) {
    cout << "Could not open file " << argv[1] << endl;
    return 1; // 1 indicates error
}
cout << "Reading Lines" << endl << endl;
getline(inFS, headings); //assuming heading
cout << "Headings: " << headings << endl;

clockid_t start, end;
int numObservs = 0;
string singleLine;
while (inFS.good()) { // READ IN FILE / VALUES
    getline(inFS, singleLine, ',');
    getline(inFS, predictor_str, ','); // pclass
    predictor1_arr.at(numObservs) = stof(predictor_str);
    
    getline(inFS, target_str, ','); // survived target loaded
    target_arr.at(numObservs) = stof(target_str);

    getline(inFS, predictor_str, ','); // sex
    predictor2_arr.at(numObservs) = stof(predictor_str);

    getline(inFS, predictor_str, '\n'); // age
    predictor3_arr.at(numObservs) = stof(predictor_str);
    
    numObservs++;
    }

    target_arr.resize(numObservs); // SURVIVED
    predictor1_arr.resize (numObservs); // PCLASS
    predictor2_arr.resize (numObservs); // SEX
    predictor3_arr.resize (numObservs); // AGE

    cout << "Values loaded.\nClosing file titanic_project.csv. \n" << endl;

    inFS.close(); // Done with file, so close it

    // START TRAIN/TEST SPLIT
    int trainCount = 900;
    int testCount = target_arr.size() - trainCount;

    vector<double> train_pclass;
    vector<double> train_sex;
    vector<double> train_age;

    vector<int> train_targetArr;

    vector<double> test_pclass;
    vector<double> test_sex;
    vector<double> test_age;

    vector<int> test_targetArr;

    for(int i = 0; i < numObservs ; i++){

        if(i < trainCount){
        train_pclass.push_back(predictor1_arr.at(i)); // PCLASS
        train_sex.push_back(predictor2_arr.at(i)); // SEX
        train_age.push_back(predictor3_arr.at(i)); // AGE
        train_targetArr.push_back(target_arr.at(i));
        }

        if(i >= trainCount){
        test_pclass.push_back(predictor1_arr.at(i));
        test_sex.push_back(predictor2_arr.at(i));
        test_age.push_back(predictor3_arr.at(i));
        test_targetArr.push_back(target_arr.at(i));
        }
    }
    // END TRAIN/TEST SLIPT

    // START MALLOC FOR THE DATA
    vector <double> prioriVect;
    vector < vector <double>> lh_pclass = {{0,0,0}, // survived?NO ~ class 1, 2, 3
                                            {0,0,0}}; // survived?YES ~ class 1, 2, 3

    vector < vector <double>> lh_sex = {{0,0}, // survived?NO ~  sex [F, M]
                                        {0,0}}; //survived?YES ~ sex [F, M]

    vector <double> age_mean_vec = {0,0}; // where {survived?FALSE, survivded?TRUE}
    vector <double> age_var_vec = {0,0}; // where {survived?FALSE, survivded?TRUE}
    
    vector <int> countTargetSp;
    vector <vector<int>> table; // 2 x 2
    double accuracy;
    double sensitivity;
    double specificity;
    double num_s =0;
    double num_p = 0;
    double denominator = 0;

    vector <double> prob_survived;
    vector <double> prob_perished;

    // END MALLOC FOR THE DATA

    // START MODELING
    start = clock();
    countTargetSp = countTagetSplit(train_targetArr);

    prioriVect = calcPrioriVec(train_targetArr); // CALCULATE PRIORS

    // CALCULATE THE LIKELIHOOD FOR SURVIVED~PCLASS MATRIX
    for(int i = 0; i < train_targetArr.size(); i++){

        if(train_targetArr.at(i) == 0 && predictor1_arr.at(i) == 1){
            lh_pclass.at(0).at(0) += 1;
        }

        if(train_targetArr.at(i) == 0 && predictor1_arr.at(i) == 2){
            lh_pclass.at(0).at(1) += 1;
        }

        if(train_targetArr.at(i) == 0 && predictor1_arr.at(i) == 3){
            lh_pclass.at(0).at(2) += 1;
        }

        if(train_targetArr.at(i) == 1 && predictor1_arr.at(i) == 1){
            lh_pclass.at(1).at(0) += 1;
        }

        if(train_targetArr.at(i) == 1 && predictor1_arr.at(i) == 2){
            lh_pclass.at(1).at(1) += 1;
        }

        if(train_targetArr.at(i) == 1 && predictor1_arr.at(i) == 3){
            lh_pclass.at(1).at(2) += 1;
        }
    }

    lh_pclass.at(0).at(0) /= countTargetSp.at(0);
    lh_pclass.at(0).at(1) /= countTargetSp.at(0);
    lh_pclass.at(0).at(2) /= countTargetSp.at(0);
    lh_pclass.at(1).at(0) /= countTargetSp.at(1);
    lh_pclass.at(1).at(1) /= countTargetSp.at(1);
    lh_pclass.at(1).at(2) /= countTargetSp.at(1);

    // CALCULATE THE LIKELIHOOD FOR SURVIVED~SEX
    for(int i = 0; i < train_targetArr.size(); i++){

        if(train_targetArr.at(i) == 0 && predictor2_arr.at(i) == 0){// perished & female
         lh_sex.at(0).at(0) += 1;
        }

        if(train_targetArr.at(i) == 0 && predictor2_arr.at(i) == 1){ // perished & male
         lh_sex.at(0).at(1) += 1;
        }

        if(train_targetArr.at(i) == 1 && predictor2_arr.at(i) == 0){// survived & female
         lh_sex.at(1).at(0) += 1;
        }

        if(train_targetArr.at(i) == 1 && predictor2_arr.at(i) == 1){ // survived & male
         lh_sex.at(1).at(1) += 1;
        }
    }

    lh_sex.at(0).at(0) /= countTargetSp.at(0);
    lh_sex.at(0).at(1) /= countTargetSp.at(0);
    lh_sex.at(1).at(0) /= countTargetSp.at(1);
    lh_sex.at(1).at(1) /= countTargetSp.at(1);

    countTargetSp = countTagetSplit(test_targetArr); // TESTING

    // CALCULATE LIKELIHOOD FOR QUANTITATIVE DATA
    age_mean_vec.at(0) = getMean(train_targetArr, train_age, 0);
    age_mean_vec.at(1) = getMean(train_targetArr, train_age, 1);

    age_var_vec.at(0) = getVar(train_targetArr, train_age, 0, age_mean_vec.at(0));
    age_var_vec.at(1) = getVar(train_targetArr, train_age, 1, age_mean_vec.at(1));

    // CALCULATE PROBABILITIES
    
    for(int i = 0; i < test_targetArr.size(); i++) {

        num_s = lh_pclass.at(1).at(test_pclass.at(i)-1) * lh_sex.at(1).at(test_sex.at(i)) * prioriVect.at(1) * calc_age_lh(test_age.at(i), age_mean_vec.at(1), age_var_vec.at(1));
        num_p = lh_pclass.at(0).at(test_pclass.at(i)-1) * lh_sex.at(0).at(test_sex.at(i)) * prioriVect.at(0) * calc_age_lh(test_age.at(i), age_mean_vec.at(0), age_var_vec.at(0));

        denominator = (lh_pclass.at(1).at(test_pclass.at(i)-1) * lh_sex.at(1).at(test_sex.at(i)) * prioriVect.at(1) * calc_age_lh(test_age.at(i), age_mean_vec.at(1), age_var_vec.at(1)))
                        + (lh_pclass.at(0).at(test_pclass.at(i)-1) * lh_sex.at(0).at(test_sex.at(i)) * prioriVect.at(0) * calc_age_lh(test_age.at(i), age_mean_vec.at(0), age_var_vec.at(0)));

        prob_survived.push_back(num_s/denominator);
        prob_perished.push_back(num_p/denominator);    
    }

    table = genTable(test_targetArr, prob_survived, prob_perished);

    accuracy = (double)(table.at(0).at(0) + table.at(1).at(1)) / (double)( table.at(0).at(0) + table.at(0).at(1) + table.at(1).at(0) + table.at(1).at(1) );
    sensitivity = (double)(table.at(0).at(0) ) / (double)( table.at(0).at(0) + table.at(1).at(0));
    specificity = (double)(table.at(1).at(1)) / (double)( table.at(0).at(1) + table.at(1).at(1) );


    end = clock();
    // END OF MODELING

    // Calculating total time taken by the program.
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "\nModeling Time:: " << fixed << time_taken << setprecision(3);
    cout << " sec\n" << endl;


    // PRINT OUT RESULTS
    cout << "Naive Bayes Classifiers for Discrete Predictors" << endl << endl;    
    cout << "A-priori probabilities:" << endl;
    cout << " PERISHED  SURVIVED" << endl;
    cout << "   " << prioriVect.at(0) << "     " << prioriVect.at(1) << endl;

    cout << "\nConditional probabilities:" << endl;
    cout << "*where ROW[1]: PERISHED"<< endl << "   and ROW[2]: SURVIVED" << endl;
    cout << "\n   pclass:" << endl;
    cout << "\t  1 \t  2    \t 3" << endl;

    for(int i = 0; i < lh_pclass.size(); i++){
         cout << "\t";
         for(int j = 0; j < lh_pclass.at(1).size(); j++){
             cout << lh_pclass.at(i).at(j) << "   ";
         }
         cout << endl;
     }

    cout << "\n   sex:" << endl;
    cout << "\tmale\tfemale" << endl;
    for(int i = 0; i < lh_sex.size(); i++){
        cout << "\t";
         for(int j = 0; j < lh_sex.at(1).size(); j++){
             cout << lh_sex.at(i).at(j) << "   ";
         }
         cout << endl;
     }

    cout << "\n   age:" << endl;
    cout << "\t mean\t std.dev." << endl;
    cout <<"\t"<< age_mean_vec.at(0) << "   " <<sqrt(age_var_vec.at(0)) << endl;
    cout << "\t"<<age_mean_vec.at(1) << "   " <<sqrt(age_var_vec.at(1)) << endl;

    cout << "\n" << endl;
    cout << "Model Metrics:" << endl ;
    cout << "\taccuracy:: " << accuracy << endl;
    cout << "\tsensitivity:: " << sensitivity << endl;
    cout << "\tspecificity:: " << specificity << endl << endl;

    // print out probabilitiess
    cout << "\n\n     PROB_PERISHED     " << "PROB_SURVIVED " << endl;
    for(int i = 0; i < test_targetArr.size(); i++){

        cout <<"["<<i+1<< "]" << "    "<<prob_perished.at(i) << "             " << prob_survived.at(i) << endl;  
    }

   return 0;
}