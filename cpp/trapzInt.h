#pragma once

#include <vector>
#include <functional>
#include <thread>
#include "mutex"
#include <future>

typedef double number;



namespace trapzInt
{
    class linspaceStruct{     
    public:
        number value;
        int index;
        linspaceStruct(number value, int index) {
            this->value = value;
            this->index = index;
        };
        ~linspaceStruct(){
           // std::cout<<"delete" <<std::endl;
        };
    };

    using intFn = std::function<number(linspaceStruct*)>;



    number trapIntegration(intFn f, number a, number b, number N);

    template <typename T>
    std::vector<T> linspace(number start, number end, number num);
}

