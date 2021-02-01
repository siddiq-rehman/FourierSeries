# FourierSeries


To run on emscripten step into cpp directory then run the below command, that will generate main.js, main.wasm and main.worker.js files into root directory


  em++ -I . main.cpp trapzInt.cpp -std=c++1z -s WASM=3 -s EXPORTED_FUNCTIONS="['_setFunctionData','_main','_getLength', '_getFourierCoefficientRealPart','_getFourierCoefficientImgPart', '_doThreadingForCoeff', '_doThreadingForCoeffThread']" -s EXTRA_EXPORTED_RUNTIME_METHODS="['cwrap','getValue','setValue']" -s TOTAL_MEMORY=64MB  
  -s SAFE_HEAP=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD -s PTHREAD_POOL_SIZE=50 -o ../main.js
