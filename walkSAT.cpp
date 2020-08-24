#include<cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream> 
#include <chrono>
#include <vector>
#include <sstream> 
#include <algorithm>
#include <unordered_set>

using namespace std;

vector<vector<int>> totalclauses;   // Holds all clauses in a given CNF
vector<int> totalliterals;  // Holds all the literals in a given CNF
static int numClauses;
static int numLiterals;

class Model 
{
    int numTrue = 0;

public:
    vector<int> assignment; // The Model's model = assignment of true/false to all variables
    void random_assignment ();  // Assigns a random value of 0 (false) or 1(true) to all literals of all clauses
    int satisfiability (vector<int>);  // Tests a model's assignment and returns how many clauses it satisfies
    void flip_literal(int);     // Flips a literal
    void max_truths(vector<int>);  // Find the literal that maximizes satisfiability upon flipping
};



void Model::random_assignment()
{
    for(int i =0;i<numLiterals;i++){
        int value = rand() % 2;

        if(value == 0){
            assignment.push_back(-totalliterals[i]);
        }
        else{
            assignment.push_back(totalliterals[i]);
        }
    }
}

int Model::satisfiability(vector<int> model)
{
    // bool satisfied = false;
    numTrue = 0;
    vector< vector<int> >::const_iterator row; 
    vector<int>::const_iterator col; 

    for (row = totalclauses.begin(); row != totalclauses.end(); row++){
         for (col = row->begin(); col != row->end(); col++){ 

            if(find(model.begin(), model.end(), *col)!= model.end()){
                numTrue++;
                break;
            }

         } 
    } 
    return numTrue;

}

void Model::flip_literal(int var){
    // cout<<"Flipping at index"<< var<<endl;
    assignment[var] = -assignment[var];
}

// Test the flip of every literal and record the clauses made true, return index of literal with highest count
void Model::max_truths(vector<int> clause){
    vector<int>truths_count;


    for(int i =0; i<clause.size();i++){
        vector<int> test(assignment);

        for(int j=0;j<test.size();j++){
            if(abs(test[j]) == abs(clause[i])){
                test[j] = -test[j];
                break;
            }
        }

        truths_count.push_back(satisfiability(test));

    }

    auto largest=max_element(truths_count.begin(),truths_count.end());
    int var = distance(truths_count.begin(), largest);

            for(int i=0; i<numLiterals;i++){
                if(abs(assignment[i]) == abs(clause[var])){
                    flip_literal(i);
                    break;
                }
            }

}

// Determines on probability p returns True or False
bool probability(float p){
    if(rand() % 2 == 0){
        return true;
    }
    return false;
    // return rand()/(RAND_MAX+1.0) < p;
}

// Walksat implementation, it takes a prob -> 0.5 chance to choose random literal or optmized literal
// and maxflips -> 1000, algorithm will search until maxflips reached
bool walksat(float prob, float maxflips)
{
    srand(time(NULL));

    Model initial;
    initial.random_assignment();


    for(int i =0; i<maxflips; i++){
        int trueclauses = initial.satisfiability(initial.assignment);

        if(trueclauses == numClauses){
            return true;
        }

        // Choose a clause at random
        vector<int> rand_clause = totalclauses[rand() % (numClauses-1-0 + 1)];
        vector<int>::iterator it;


        // based on probability p, will choose either random var or maximized true clauses var
        if(probability(prob)){
            int index = rand()%((rand_clause.size())-1-0 + 1);
            initial.flip_literal(index);

            
        }
        else{
            initial.max_truths(rand_clause);            

        }
    }

    return false;
}

int main(int argc, char** argv)
{
    ifstream myfile;
    myfile.open(argv[1]);
    // int totalClauses =0;

    string line;
    int i=0;

    while(!myfile.eof()){
        getline(myfile, line);

        if(line[0] == 'c'|| line[0]=='p'|| line == ""){
            // ignore line
        }
        else{
                vector<int> clause;
                istringstream ss(line);

                int x =0;


                while (ss >> x){

                    if(x != 0){

                        clause.push_back(x);
                        totalliterals.push_back(abs(x));
                    }
                }
                
                totalclauses.push_back(clause);
            }
    }

    // Create vector of literals
    unordered_set<int> s;
    for (int i : totalliterals){
        s.insert(i);
    }
    totalliterals.assign( s.begin(), s.end() );
    numClauses = totalclauses.size(); 
    numLiterals = totalliterals.size();


    
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    walksat(0.4,10000);
    bool satisfied = walksat(0.4, 10000);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    if(satisfied){
        cout << "SATSIFAIABLE" <<endl;
    }
    else{
        cout<< "UNSATISFIABLE"<<endl;
    }

    cout << "Time: = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << endl;
    return 0;
}