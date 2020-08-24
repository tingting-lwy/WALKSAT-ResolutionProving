#include<cstdio>
#include <cstdlib>
#include <iostream> 
#include <fstream>
#include <sstream> 
#include <chrono>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

vector<vector<int>> totalclauses;   // Holds all clauses in a given CNF PLUS the ones inferred
vector<vector<int>> newclauses;     // Holds all newly inferred clauses for one loop of the totalclauses
vector<int> single_literals;     // Holds all inferred clauses that are single literals


// Infers a new clause and updates newclauses from inference
void infer(vector<int> clause1, vector<int> clause2,int index1, int index2)
{
    vector <int> newclause;

    // Iterate through both clauses to add everything other than literal and its opposite
    for(int i =0; i<clause1.size();i++){
            if(clause1[i] != clause1[index1]){
                newclause.push_back(clause1[i]);
            }
    }
    for(int j =0; j<clause2.size();j++){
            if(clause2[j] != clause2[index2]){
                newclause.push_back(clause2[j]);
            }
    }

    // Check for duplicates and also remove any literals with opposites 
    unordered_set<int> s;
    for (int i : newclause){
        bool duplicate = false;

        for(int j=i+1; j<newclause.size();j++){

            if(newclause[i] == -newclause[j]){
                duplicate = true;
            }
        }
        if(!duplicate){
            s.insert(i);
        }
    }
    // Adding to newclauses
    newclause.assign( s.begin(), s.end() );
    newclauses.push_back(newclause);

    // If clause is a single literal add to single_literals
    if(newclause.size() ==1 ){
        single_literals.push_back(newclause[0]);
    }
}


// all literals of a clause pair to find an opposite literal to any literal in the clause. 
void find_opposite(vector<int> clause1,vector<int> clause2)
{

    // Iterate through the clause 1 then clause 2 and compare
    for (int i=0; i<clause1.size();i++){

            for (int j = 0;j<clause2.size(); j++){ 
                if (clause1[i] == -clause2[j]){

                    infer(clause1,clause2,i,j);
                    // stop = true;
                    break;
                }

            } 

    }
}

// Will proves entailment through use of CNF and inference
bool resolution_proving()
{
    bool entails = true;

    // Initial check that there is no pre-existing empty clauses
    for(int i=0;i<single_literals.size();i++){

        // empty clause found
        if(find(single_literals.begin()+i, single_literals.end(), -single_literals[i]) != single_literals.end()){
            cout<<"immediate empty case"<<endl;
            entails = false;
            return entails;
        }

    }


    // Check pairs of clauses
    // vector<int> clause1;
    // vector<int> clause2;

    vector< vector<int> >::const_iterator row; 
    vector<int>::const_iterator col; 

    // Infinite loop until empty clause or no clauses can be added.
    while(true){

        // iterate through all clauses
        for (int i=0; i < totalclauses.size(); i++){
            for (int j = i+1;j<totalclauses.size(); j++){ 

                find_opposite(totalclauses[i], totalclauses[j]);

                    // Check for empty clause
                    for(int i=0;i<single_literals.size();i++){

                        // empty clause found
                        if(find(single_literals.begin()+i, single_literals.end(), -single_literals[i]) != single_literals.end()){
                            entails = false;
                            return entails;
                        }

                    } 

                }
            }

        // sort(newclauses.begin(), newclauses.end(), [](const vector<int> & a, const vector<int> & b){ return a.size() < b.size(); });

        // Checks if no new clauses added
        if(newclauses.size() == 0){
            entails = true;
            return entails;
        }

        totalclauses=newclauses;
        newclauses.clear();

        // sort(totalclauses.begin(), totalclauses.end(), [](const vector<int> & a, const vector<int> & b){ return a.size() < b.size(); });

    }

}

int main(int argc, char** argv)
{
    ifstream myfile;
    myfile.open(argv[1]);

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
                while(ss>>x){
                    if(x != 0){

                        clause.push_back(x);

                    }
                }
                totalclauses.push_back(clause);
                if(clause.size() == 1){
                    single_literals.push_back(clause[0]);
                }
            }
    }


    // Sort the total clauses by size
    // sort(totalclauses.begin(), totalclauses.end(), [](const vector<int> & a, const vector<int> & b){ return a.size() < b.size(); });

    
    chrono::steady_clock::time_point begin  = chrono::steady_clock::now();
    bool satisfied = resolution_proving();
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    if(satisfied){
        cout << "SATSIFAIABLE - no entailment" <<endl;
    }
    else{
        cout<< "UNSATISFIABLE - entailment can be determined"<<endl;
    }

    cout << "Time: = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << endl;
    return 0;
}