#include <bits/stdc++.h>

using namespace std;

vector<char> generateChild(vector<char>& par) {
    vector<char> child(par.size());
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution dist(1.0/(int)par.size());

    for (int64_t i = 0; i < par.size(); i++) {
        if (dist(gen)) {
            child[i] = 1-par[i];
        } else {
            child[i] = par[i];
        }
    }
    return child;
}

bool isEqual(vector<char> &v1, vector<char> &v2) {
    for (int i = 0; i < v1.size(); i++) {
        if(v1[i] != v2[i]) return false;
    }
    return true;
}

long double getFitness(vector<char> &point, map<vector<char>, long double> &fitness, long double p) {
    if (fitness.count(point)==0) {
        random_device rd;
        mt19937 gen(rd());
        bernoulli_distribution dis(p); // distort with probability
        if (dis(gen)) { // distorted point
            random_device rd2;
            mt19937 gen2(rd2());
            //exponential_distribution<double> dist2(0.4);
           // cauchy_distribution<double> dist2(0, 0.3);
            uniform_real_distribution<> dist2(0, 8);
            double distortion = abs(dist2(gen2));
            fitness[point] = count(point.begin(), point.end(), 1) + distortion;
        } else { // not distorted point
            fitness[point] = count(point.begin(), point.end(), 1);
        }
    }
    return fitness[point];
}

int64_t simulate(vector<char> currPoint, map<vector<char>,long double> &fitness, long double p, int64_t cutoff, int64_t lambda, long double k, bool isElitary) {
    int64_t genCounter = 1;
    while (getFitness(currPoint, fitness, p) < (int64_t)currPoint.size()-k && genCounter < cutoff) {
        genCounter++;
        vector<char> fittestChild = generateChild(currPoint);

        for (int64_t i = 0; i < lambda-1; i++) {
            vector<char> child = generateChild(currPoint);

            if (getFitness(child, fitness, p) > getFitness(fittestChild, fitness, p)) {
                fittestChild = child;
            }
        }
        if (isElitary) {
            if (getFitness(fittestChild, fitness, p) > getFitness(currPoint, fitness, p)) {
                currPoint = fittestChild;
               // cout << genCounter << " " << count(currPoint.begin(), currPoint.end(), 1) << " " << (getFitness(currPoint, fitness, p)-count(currPoint.begin(), currPoint.end(), 1)) << "\n";
            }
        } else {
            if (!isEqual(currPoint, fittestChild)) {
                //cout << genCounter << " " << count(fittestChild.begin(), fittestChild.end(), 1) << " " << (getFitness(fittestChild, fitness, p)-count(fittestChild.begin(), fittestChild.end(), 1)) << "\n";
            }
            currPoint = fittestChild;


        }
    }
    //cout << genCounter << " " << getFitness(currPoint, fitness, p) << " " << count(currPoint.begin(), currPoint.end(), 1) << endl;
    return genCounter;

}

int main()
{
    int64_t n= 500;
    int64_t numIter = 49;
    long double k = pow(n, 0.15);
    int64_t lambda = round( (exp(1)/(exp(1)-1)) * log(n));
    int64_t cutoff = 1'000'000'000;

    string filename = "DifferentP_Uniform8_n="+to_string(n)+"_numIter=" +to_string(numIter) + "_k=" + to_string(k) + "_lambda=" + to_string(lambda) +  ".txt";
    ofstream outFile(filename);
    streambuf* coutBuffer = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());
    for (long double p = 1.0; p >= 0.0001; p /= sqrt(2)) {



        cout << p << endl;
        int64_t iterSum = 0;
        for (int i = 0; i < numIter; i++) {
            map<vector<char>,long double> fitness;
            vector<char> initSearchPoint(n, 0);
            int64_t numGen =simulate(initSearchPoint, fitness, p, cutoff, lambda, k, true);
            if (numGen == cutoff) return 0;
            iterSum += numGen;
            cout << numGen << " ";
        }
        cout << "Mean: " << iterSum / (long double) numIter << endl;
        cout << endl << endl;
    }
    return 0;
}


