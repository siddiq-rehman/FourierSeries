#include "iostream"
#include <trapzInt.h>
#include <chrono>
#include <math.h>
#include <functional>

typedef double number;

//using namespace std;
using namespace trapzInt;

//using intFn = std::function<number(trapzInt::linspaceStruct*)>;

number *realNumberPointer;
number *imgNumberPointer;
unsigned int realNumberLength;
unsigned int imgNumberLength;

number pi = 2 * asin(1.0);

number myFunction(trapzInt::linspaceStruct *x)
{
    return 4 * (1.0 / (1 + pow(x->value, 2)));
}

extern "C"
{
    void setFunctionData(number *valueBufferPtr, unsigned int length, int a)
    {
        if (a == 0)
        {
            realNumberPointer = valueBufferPtr;
            realNumberLength = length;
        }
        else
        {
            imgNumberPointer = valueBufferPtr;
            imgNumberLength = length;
        }

        //cout<<valueBufferPtr[0]<<endl;
    }

    void getLength()
    {
        int count = 0;
        for (int i = 0; i < realNumberLength; i++)
        {
            count++;
        }
        std::cout << count << std::endl;
    }

    number getFourierCoefficientRealPart(number TimePeriod, int k)
    {
        number T = 62.5 * 3 * TimePeriod;
        number g2 = trapzInt::trapIntegration([T, k](trapzInt::linspaceStruct *x) {
            return static_cast<number>(static_cast<number>(1.0 * realNumberPointer[x->index] * cos(static_cast<number>(k * (x->value) * (2.0) * (M_PI) / T))) + static_cast<number>(1.0 * imgNumberPointer[x->index] * sin(static_cast<number>(k * (x->value) * (2.0) * (M_PI) / T))));
        },
                                              0, T, 11186);
        return static_cast<number>(g2 * (1.0 / T));
    }

    number getFourierCoefficientImgPart(number TimePeriod, int k)
    {
        number T = 62.5 * 3 * TimePeriod;
        number g2 = trapzInt::trapIntegration([T, k](trapzInt::linspaceStruct *x) {
            return (static_cast<number>(1.0 * imgNumberPointer[x->index] * cos(static_cast<number>(k * (x->value) * (2.0) * (M_PI) / T))) - static_cast<number>(1.0 * realNumberPointer[x->index] * sin(static_cast<number>(k * (x->value) * (2.0) * (M_PI) / T))));
        },
                                              0, T, 11186);
        return static_cast<number>(g2 * (1.0 / T));
    }

    void getFourierCoefficientRealPartTh(number TimePeriod, int intK, int finalK, number *start, number *end)
    {
        number T = 62.5 * 3 * TimePeriod;
        auto it = start;
        while (it <= end && intK <= finalK)
        {
            int k = intK;
            number g2 = trapzInt::trapIntegration([T, k](trapzInt::linspaceStruct *x) {
                return static_cast<number>(static_cast<number>(1.0 * realNumberPointer[x->index] * cos(static_cast<number>(k * (x->value) * (2.0) * (M_PI) / T))) + static_cast<number>(1.0 * imgNumberPointer[x->index] * sin(static_cast<number>(k * (x->value) * (2.0) * (M_PI) / T))));
            },
                                                  0, T, 11186);
            *it = static_cast<number>(g2 * (1.0 / T));
            // std::cout<<"DD"<< intK<<std::endl;
            it += 2;
            intK++;
        }
    }

    void getFourierCoefficientImgPartTh(number TimePeriod, int intK, int finalK, number *start, number *end)
    {

        number T = 62.5 * 3 * TimePeriod;
        auto it = start;
        while (it <= end && intK <= finalK)
        {
            int k = intK;
            number g2 = trapzInt::trapIntegration([T, k](trapzInt::linspaceStruct *x) {
                return (static_cast<number>(1.0 * imgNumberPointer[x->index] * cos(static_cast<number>(k * (x->value) * (2.0) * (M_PI) / T))) - static_cast<number>(1.0 * realNumberPointer[x->index] * sin(static_cast<number>(k * (x->value) * (2.0) * (M_PI) / T))));
            },
                                                  0, T, 11186);
            *it = static_cast<number>(g2 * (1.0 / T));
            it += 2;
            intK++;
        }

        // number T = 62.5*3*TimePeriod;
        //  number g2 = trapzInt::trapIntegration([T, k](trapzInt::linspaceStruct* x){
        //         return (static_cast<number>(1.0 * imgNumberPointer[x->index] * cos(static_cast<number>(k*(x->value)*(2.0)*(M_PI)/T) ))
        //     - static_cast<number>(1.0 * realNumberPointer[x->index] * sin(static_cast<number>(k*(x->value)*(2.0)*(M_PI)/T) )));
        //  }, 0, T, 11186);
        //  resp = static_cast<number>(g2*(1.0/T));
    }
}

long worker(long f, long e)
{
    long sum = 0;
    for (long i = 0; i < 10000000; i++)
    {
        sum += i;
    }
    return sum;
}

void doThreadingWithAsync()
{
    long total = 0;
    std::vector<std::future<long>> futuresV(500);
    for (auto it = std::begin(futuresV); it != std::end(futuresV); ++it)
    {

        *it = std::async(std::launch::async, worker, 0, 10000000);
    }

    for (auto &&i : futuresV)
    {
        //  cout<< "join" <<endl;
        long a = i.get();
        total += a;
        //cout<<a<<endl;
    }

    std::cout << total << std::endl;
}

void doThreading()
{
    long total = 0;
    std::vector<std::future<long>> futuresV(500);
    for (auto it = std::begin(futuresV); it != std::end(futuresV); ++it)
    {

        *it = std::async(std::launch::async, worker, 0, 10000000);
    }

    for (auto &&i : futuresV)
    {
        //  cout<< "join" <<endl;
        long a = i.get();
        total += a;
        //cout<<a<<endl;
    }

    //  std::cout<<total<<std::endl;
}

void smallFunction(int a, int lastValue, number *itu)
{
}

extern "C"
{

    void doThreadingForCoeffThread(int a, number *valueBufferPtr)
    {

        auto tempP = valueBufferPtr;

        number totalfPtr = 4 * a + 2; //42

        std::cout << "futures " << totalfPtr << std::endl;

        int threadNumber = 10;

        std::vector<std::thread> threadPointers(threadNumber + 1);

        int ws = ((4 * a + 2) / (threadNumber * 2)) * 2; //4;   //5 //10

        std::cout << "window size" << ws << std::endl;



        //i = -100,-99,-98,-97
        std::cout << "END FOR VEC" << std::endl;
        auto itr = std::end(threadPointers);
        std::cout << &(*itr) << std::endl;
        int lastCalculated;
        for (auto [it, i] = std::tuple<std::vector<std::thread>::iterator, int>{std::begin(threadPointers), a * -1}; it != std::end(threadPointers) - 1; it += 2, i += (ws))
        {
            *it = std::thread(getFourierCoefficientRealPartTh, 1, i, i + (ws - 1), valueBufferPtr, valueBufferPtr + (2 * ws - 1));

            *(it + 1) = std::thread(getFourierCoefficientImgPartTh, 1, i, i + (ws - 1), valueBufferPtr + 1, valueBufferPtr + (2 * ws - 1));
            valueBufferPtr += (2 * ws);

            lastCalculated = i + (ws - 1);
        }

        //valueBufferPtr

        if (lastCalculated < a)
        {

            smallFunction(a, lastCalculated, valueBufferPtr);

            //  while(lastCalculated<a){
            //              lastCalculated++;
            //              std::cout<< "IN while" << getFourierCoefficientRealPart(1, lastCalculated) << std::endl;
            //              *valueBufferPtr = static_cast<number>(getFourierCoefficientRealPart(1, lastCalculated));

            //              std::cout<<"captured" << *valueBufferPtr<<std::endl;
            //             // *(it+1) = getFourierCoefficientImgPart(1, lastValue);
            //              valueBufferPtr+=2;
            //         }

            threadPointers.back() = std::thread([](int a, int lastValue, number *it) {
                while (lastValue < a)
                {
                    lastValue++;
                    std::cout << "IN while" << getFourierCoefficientRealPart(1, lastValue) << std::endl;
                    *it = static_cast<number>(getFourierCoefficientRealPart(1, lastValue));
                    *(it + 1) = getFourierCoefficientImgPart(1, lastValue);
                    if (lastValue != a)
                    {
                        it += 2;
                    }
                }
            },
                                                a, lastCalculated, valueBufferPtr);
            //lastCalculated++;
        }

        std::cout << "GOT OUTSIDE" << std::endl;
        std::cout << "last i " << lastCalculated << std::endl;

        int count = 0;

        //     threadPointers.at(0).join();
        //     threadPointers.at(1).join();
        //     threadPointers.at(2).join();
        //     threadPointers.at(3).join();
        //     threadPointers.at(4).join();
        //     threadPointers.at(5).join();
        //    // threadPointers.at(6).join();

        std::cout << "ALl threads joined back" << std::endl;

        for (auto &&i : threadPointers)
        {
            //  cout<< "join" <<endl;
            //count++;
            i.join();
            //total+=a;
            //printf(" %.9lf\n", a);
        }
    }

    //static
    void doThreadingForCoeff(int a, number *valueBufferPtr)
    {

        number totalfPtr = 4 * a + 2;

        std::cout << "futures " << totalfPtr << std::endl;
        std::vector<std::future<number>> futuresV(totalfPtr);

        for (int i = -i * a; i <= a; i++)
        {
            // let a = getFourierCoefficientRealPart(1, i);
            // let b = getFourierCoefficientImgPart(1, i);
            // Coefficients.push([a, b]);
            // console.log(i);
        }

        //int incValue =
        for (auto [it, i] = std::tuple<std::vector<std::future<number>>::iterator, int>{std::begin(futuresV), a * -1}; it != std::end(futuresV); it += 2, i++)
        {
            std::cout << "putting for" << i << std::endl;
            *it = std::async(std::launch::async, getFourierCoefficientRealPart, 1, i);
            *(it + 1) = std::async(std::launch::async, getFourierCoefficientImgPart, 1, i);
        }

        int count = 0;

        for (auto &&i : futuresV)
        {
            //  cout<< "join" <<endl;
            count++;
            number a = i.get();
            *valueBufferPtr = a;
            valueBufferPtr++;
            std::cout << count << std::endl;
            //total+=a;
            //printf(" %.9lf\n", a);
        }
    }
}

int main()
{

    std::cout << "MAIN STRTED" << std::endl;

    using namespace std::chrono;

    int a = 10 / 100;

    std::cout << a << std::endl;

    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // cout<<"Doing calculations" <<endl;
    // number g2 = trapzInt::trapIntegration(*myFunction, 0, 1, 11187);
    // cout<<"value " << g2 <<endl;
    // printf(" %.9lf\n", g2);
    // cout<<"DONEh " <<endl;
    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[ns]" << std::endl;

    //number aa = getFourierCoefficientRealPart(1,0);

    //cout<< aa<<endl;
    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // doThreading();
    // long sum;
    // for(int i=0; i< 500; i++){
    //   long a=  worker(0, 10000000);
    //   sum+=a;
    // }
    // cout<< sum<<endl;
    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[sec]" << std::endl;

    return 0;
}
