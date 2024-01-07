#include <bits/stdc++.h>

using namespace std;

vector<char> generateChild(vector<char>& par) {
    vector<char> child(par.size());
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution dist(1.0/(int)par.size());

    for (int64_t i = 0; i < par.size(); i++) {
        if (dist(gen)) { // flip with probability p
            child[i] = 1-par[i];
        } else {
            child[i] = par[i];
        }
    }
    return child;
}


long double getFitness(vector<char> &point, map<vector<char>, long double> &fitness, long double p, int64_t distortionCutoff) {
    if (fitness.count(point)==0) {
        random_device rd;
        mt19937 gen(rd());
        bernoulli_distribution dis(p); // distort with probability
        if (dis(gen)) { // distorted point
            random_device rd2;
            mt19937 gen2(rd2());
            uniform_real_distribution<> dist2(0, distortionCutoff);
            double distortion = dist2(gen2);
            /*
            exponential_distribution<double> dist2(0.4);
            double distortion = dist2(gen2);
            while (distortion > distortionCutoff) { // condition on sampling <= distortionCutoff
                distortion = dist2(gen2);
            }
            */
            //cout << distortion << endl;
            fitness[point] = count(point.begin(), point.end(), 1) + distortion;
        } else { // not distorted point
            fitness[point] = count(point.begin(), point.end(), 1);
        }
    }
    return fitness[point];
}

int64_t simulate(vector<char> currPoint, map<vector<char>,long double> &fitness, long double p, int64_t cutoff, int64_t lambda, long double k, bool isElitary, int64_t distortionCutoff) {
    int64_t genCounter = 1;
    while (getFitness(currPoint, fitness, p, distortionCutoff) < (int64_t)currPoint.size()-k && genCounter < cutoff) {

        genCounter++;
        vector<char> fittestChild = generateChild(currPoint);

        for (int64_t i = 0; i < lambda-1; i++) {
            vector<char> child = generateChild(currPoint);

            if (getFitness(child, fitness, p, distortionCutoff) > getFitness(fittestChild, fitness, p, distortionCutoff)) {
                fittestChild = child;
            }
        }
        if (isElitary) {
            if (getFitness(fittestChild, fitness, p, distortionCutoff) > getFitness(currPoint, fitness, p, distortionCutoff)) {
                currPoint = fittestChild;
            }
        } else {
            currPoint = fittestChild;
        }
    }
    //cout << genCounter << " " << getFitness(currPoint, fitness, p) << " " << count(currPoint.begin(), currPoint.end(), 1) << endl;
    return genCounter;

}

int main()
{
    // Compare 1+lambda and 1,lambda for
    int64_t numIter = 100;
    int64_t from =  70; // smallest n to benchmark
    int64_t to = 200; // biggest n to benchmark
    int64_t stepSize = 10; // step size of n
    int64_t cutoff = 50'000;



    string filename = "simulateWithDistortionCutoff_unifDiscrete_iter=" + to_string(numIter) + "_from=" + to_string(from) + "_to=" + to_string(to) + "_steps=" +to_string(stepSize)+ "_cutoff=" +to_string(cutoff) + ".txt";
    ofstream outFile(filename);
    streambuf* coutBuffer = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());


    for (int64_t distortionCutoff = 1; distortionCutoff <= 20; distortionCutoff *= 2) {
        for (long double pFac = 1/8.0; pFac <= 2.0; pFac *= 2.0) {
            cout << distortionCutoff << " " << pFac << endl;
            vector<long double> plusMeans, commaMeans;
            for (int64_t n = from; n <= to; n += stepSize) {
                long double plusSum = 0;
                long double commaSum = 0;
                long double p = pFac*0.3/(sqrt(n)); // distortion probability
                long double k = pow(n, 0.15);
                for (int64_t iter = 0; iter < numIter; iter++) {
                    int64_t lambda = round( (exp(1)/(exp(1)-1)) * log(n));
                    map<vector<char>,long double> fitness;
                    vector<char> initSearchPoint(n, 0);
                    plusSum += simulate(initSearchPoint, fitness, p, cutoff, lambda, k, true, distortionCutoff);
                    commaSum += simulate(initSearchPoint, fitness, p, cutoff, lambda, k, false, distortionCutoff);
                }
                plusMeans.push_back((long double) plusSum / numIter);
                commaMeans.push_back((long double) commaSum / numIter);
            }
            for (int i = 0; i < plusMeans.size(); i++) {
                cout << plusMeans[i] << " ";
            }
            cout << endl;
            for (int i = 0; i < commaMeans.size(); i++) {
                cout << commaMeans[i] << " ";
            }
            cout << endl << endl;
        }
    }

    return 0;
}
