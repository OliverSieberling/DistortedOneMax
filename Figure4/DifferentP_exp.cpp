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

// simulates optimizing distorted OneMax on (1+lambda)-EA (isElitary = true) or (1-lambda)-EA (isElitary = false).
int64_t simulate(vector<char> currPoint, map<vector<char>,long double> &fitness, long double p, int64_t lambda, long double k, bool isElitary) {
    int64_t genCounter = 1;
    long double currentPointFitness = getFitness(currPoint, fitness, p);
    while (currentPointFitness < (int64_t)currPoint.size()-k) {
        genCounter++;
        long double fittestChildFitness = -1;
        vector<char> fittestChild;

        for (int64_t i = 0; i < lambda; i++) {
            vector<char> child = generateChild(currPoint);
            long double childFitness = getFitness(child, fitness, p);

            if (childFitness > fittestChildFitness) { // uniform tie break done by order
                fittestChildFitness = childFitness;
                fittestChild = child;
            }
        }

        long double fittestChildDistortion = fittestChildFitness - count(fittestChild.begin(), fittestChild.end(), 1);

        if (isElitary) {
            if (fittestChildFitness >= currentPointFitness) {
                currPoint = fittestChild;
                currentPointFitness = fittestChildFitness;
            }
        } else {
            currPoint = fittestChild;
            currentPointFitness = fittestChildFitness;
        }
    }
    return genCounter;
}

int main()
{
    int64_t n = 300;
    int64_t numIter = 499;
    long double k = pow(n, 0.15);
    int64_t lambda = round(1.5* log(n));

    string filename = "DifferentP_uniform8_n="+to_string(n)+"_numIter=" +to_string(numIter) + "_k=" + to_string(k) + "_lambda=" + to_string(lambda) +  ".txt";
    ofstream outFile(filename);
    streambuf* coutBuffer = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());
    for (long double p = 1.0; p >= 0.088; p /= sqrt(2)) {
        cout << p << endl;
        for (int i = 0; i < numIter; i++) {
            // choose initial search point uniformly at random
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, 1);
            vector<char> initSearchPoint(n);
            for (int i = 0; i < n; i++) {
                initSearchPoint[i] = dis(gen);
            }
            
            map<vector<char>,long double> fitness;

            cout << simulate(initSearchPoint, fitness, p, lambda, k, true) << " ";
        }
        cout << endl;
    }
    return 0;
}

