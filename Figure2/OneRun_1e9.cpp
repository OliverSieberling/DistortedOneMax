#include <bits/stdc++.h>
using namespace std;

// returns a copy of the parent, where each bit is flipped with probability 1/n
vector<char> generateChild(vector<char>& par) {
    vector<char> child(par);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, par.size());
    for (int64_t i = 0; i < child.size(); i++) {
        if (dis(gen) == 0) {
            child[i] = 1-child[i];
        }
    }
    return child;
}

// returns if v1 and v2 are equal
bool isEqual(vector<char> &v1, vector<char> &v2) {
    for (int i = 0; i < v1.size(); i++) {
        if(v1[i] != v2[i]) return false;
    }
    return true;
}


// returns fitness of a point; when called for the first time on this point samples distortion and computes fitness
long double getFitness(vector<char> &point, map<vector<char>, long double> &fitness, long double p) {
    if (fitness.count(point)==0) {
        random_device rd;
        mt19937 gen(rd());
        bernoulli_distribution dis(p); // distort with probability p
        if (dis(gen)) {
            exponential_distribution<double> dis2(0.4);
            double distortion = dis2(gen);
            fitness[point] = count(point.begin(), point.end(), 1) + distortion;
        } else {
            fitness[point] = count(point.begin(), point.end(), 1);
        }
    }
    return fitness[point];
}

// simulates optimizing distorted OneMax on (1+lambda)-EA (isElitary = true) or (1-lambda)-EA (isElitary = false). Outputs whenever a different point is accepted.
int64_t simulate(vector<char> currPoint, map<vector<char>,long double> &fitness, long double p, int64_t cutoff, int64_t lambda, long double k, bool isElitary) {
    int64_t genCounter = 1;
    long double currentPointFitness = getFitness(currPoint, fitness, p);
    cout  << setw(13) << left << genCounter << setw(13) << left << currentPointFitness - count(currPoint.begin(), currPoint.end(), 1)  << setw(13) << left << currentPointFitness << endl;
    while (currentPointFitness < (int64_t)currPoint.size()-k && genCounter < cutoff) {
        genCounter++;
        if (genCounter%10000000 == 0) cout << genCounter << endl;
        long double fittestChildFitness = -1;
        vector<vector<char>> fittestChildren;

        for (int64_t i = 0; i < lambda; i++) {
            vector<char> child = generateChild(currPoint);
            long double childFitness = getFitness(child, fitness, p);

            if (childFitness > fittestChildFitness) {
                fittestChildren.clear();
                fittestChildFitness = childFitness;
                fittestChildren.push_back(child);
            } else if (childFitness == fittestChildFitness) {
                fittestChildren.push_back(child);
            }
        }

        // tie brake uniformly at random between offspring of equal fitness
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, fittestChildren.size()-1);
        vector<char> fittestChild = fittestChildren[dis(gen)];

        long double fittestChildDistortion = fittestChildFitness - count(fittestChild.begin(), fittestChild.end(), 1);

        if (isElitary) {
            if (fittestChildFitness >= currentPointFitness) {
                if (!isEqual(currPoint, fittestChild)) { // do not print if fittestChild is copy of parent
                    cout  << setw(13) << left << genCounter << setw(13) << left <<  fittestChildDistortion << setw(13) << left << fittestChildFitness << endl;
                }
                currPoint = fittestChild;
                currentPointFitness = fittestChildFitness;
            }
        } else {
            if (!isEqual(currPoint, fittestChild)) { // do not print if fittestChild is copy of parent
                cout  << setw(13) << left << genCounter << setw(13) << left <<  fittestChildDistortion << setw(13) << left << fittestChildFitness << endl;
            }
            currPoint = fittestChild;
            currentPointFitness = fittestChildFitness;
        }
    }
    return genCounter;
}

int main()
{
    int64_t n = 150;
    long double p = 0.3/(sqrt(n));
    long double k = pow(n, 0.15);
    int64_t lambda = round((exp(1)/(exp(1)-1)) * log(n));
    int64_t cutoff = 1'000'000'000;

    // output to file
    
    string filename = "oneRun_exp04_n=" + to_string(n) + "_cutoff=" + to_string(cutoff) + ".txt";
    ofstream outFile(filename);
    streambuf* coutBuffer = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());


    // choose initial search point uniformly at random
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);
    vector<char> initSearchPoint(n);
    for (int i = 0; i < n; i++) {
        initSearchPoint[i] = dis(gen);
    }

    map<vector<char>,long double> fitness;
    cout << "target: " << n-k << endl;
    simulate(initSearchPoint, fitness, p, cutoff, lambda, k, false);
    simulate(initSearchPoint, fitness, p, cutoff, lambda, k, true);

    return 0;
}
