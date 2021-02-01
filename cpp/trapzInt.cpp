#include <iostream>
#include <math.h>
#include <chrono>
#include <trapzInt.h>
using namespace std;

typedef double number;


number vectorAccumulator(vector<number>* v){
    number sum=0.0;
    for (auto it = (*v).begin(); it != (*v).end(); it++)
         sum+=(*it);
    return sum;
}


template <typename T>
vector<T> trapzInt::linspace(number start, number end, number num)
{
    vector<T> linspaced;
    int index = 0;

    if (0 != num)
    {
        if (1 == num)
        {
            linspaced.push_back(static_cast<T>(new trapzInt::linspaceStruct(start, index)));
        }
        else
        {
            number delta = (end - start) / (num - 1);

            for (auto i = 0; i < (num - 1); ++i)
            {
                linspaced.push_back(static_cast<T>(new trapzInt::linspaceStruct(start + delta * i, index)));
                index++;
            }
            // ensure that start and end are exactly the same as the input
            linspaced.push_back(static_cast<T>(new trapzInt::linspaceStruct(end, index)));
        }
    }
    return linspaced;
}

number trapzInt::trapIntegration(intFn f, number a, number b, number N)
{
    number dx = (number)(b - a) / N;
    vector<linspaceStruct*> g1 = linspace<linspaceStruct*>(a, b, N+1);

    vector<number> tempVector;
   
    auto it = g1.begin();
    while(it <( g1.end() -1)){
        number value=(f((*it)) + (f((*++it))));
        tempVector.push_back(static_cast<number>(value));
        delete *(it-1);
    }
    //delete *it;
    //vector<linspaceStruct*>().swap(g1);

    g1.clear();

    tempVector.resize(N-1);

    number resp= vectorAccumulator(&tempVector) * (dx /2.0000000);

    tempVector.clear();
    
    return resp;
}
