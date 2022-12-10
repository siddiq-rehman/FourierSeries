import { data } from "./imageData.js";
import * as Comlink from "./comlink.js";

console.log(location.search.split("=")[1]);
let Coefficients = [];
let threadedCoefficients = [];
let numberCount = parseInt(location.search.split("=")[1]);

document.getElementById("NVALUE").innerHTML = "N = " + numberCount;

// const acc = data.reduce((acc, v, i) => {
//   if (i < 3) acc = acc + v[0];
//   //acc[1] = acc[1] + v[1];
//   return acc;
// }, 0);

// console.log(acc);

window.myDONE = () => {
  console.log("Hello");
};

Module["onRuntimeInitialized"] = () => {
  console.log("generated");
  const setFunctionData = Module.cwrap("setFunctionData", null, [
    "number",
    "number",
    "number",
  ]);


  var nByte = 8;
  var length = data.length;

  const setDataIntegrationLength = Module.cwrap("setDataIntegrationLength", null, ["number"])
  setDataIntegrationLength(length);

  var realBuffer = Module._malloc(length * nByte);
  var imgBuffer = Module._malloc(length * nByte);

  console.log(realBuffer);
  console.log(imgBuffer);

  for (var i = 0; i < length; i++) {
    // console.log(Module.getValue(buffer + i * nByte, 'i32'));
    Module.setValue(realBuffer + i * nByte, data[i][0], "double");
    Module.setValue(imgBuffer + i * nByte, data[i][1], "double");
    //buffer+=buffer + i * nByte;
  }

  console.log(Module.getValue(realBuffer + 0 * nByte, "double"));

  setFunctionData(realBuffer, length, 0); //setting real value of the function
  setFunctionData(imgBuffer, length, 1);
  ccall("getLength");
  console.log("calling trigger");

  const getFourierCoefficientRealPart = Module.cwrap(
    "getFourierCoefficientRealPart",
    "number",
    ["number", "number"]
  );

  const getFourierCoefficientImgPart = Module.cwrap(
    "getFourierCoefficientImgPart",
    "number",
    ["number", "number"]
  );

  console.time("test");
  // for (i = -numberCount; i <= numberCount; i++) {
  //   let a = getFourierCoefficientRealPart(1, i);
  //   let b = getFourierCoefficientImgPart(1, i);
  //   Coefficients.push([a, b]);
  //   console.log(i);
  // }
  console.timeEnd("test");

  console.table(Coefficients);

  console.log("Threaded");

  const doThreadingForCoeff = Module.cwrap("doThreadingForCoeffThread", "", [
    "number",
    "number",
  ]);

  var coeffBuffer = Module._malloc((4 * numberCount + 2) * nByte);

  var sharesMemory = new SharedArrayBuffer((4 * numberCount + 2) * nByte);

  const sharedBuffer = new SharedArrayBuffer(Int32Array.BYTES_PER_ELEMENT * 4);

  const sharedArray = new Int32Array(sharedBuffer);

  sharedArray[0] = () => {
    console.log("Ia m func");
  };

  console.log(sharedArray[0]);

  for (let i = 0; i < 4; i++) {
    console.log(i && sharedArray[i - 1] + 2);
    sharedArray[i] = i && sharedArray[i - 1] + 2;
  }

  function createWorker() {
    var wrapper = Comlink.wrap(new Worker("myWorker.js"));

    (async () => {
      console.log(asmLibraryArg);
      await wrapper("HELLo");
    })();

    //const fetchResult = await fetch("./main.wasm");
    //const bytes = await fetchResult.arrayBuffer()
  }

  const imports = {
    env: {
      consoleLog: console.log,
      stringLog: (offset, length) => {
        const sliced = new Int8Array(
          wasm.instance.exports.memory.buffer.slice(offset, offset + length)
        );
        let utf8decoder = new TextDecoder();
        console.log(utf8decoder.decode(sliced));
      },
    },
  };

  // fetch("check.wasm")
  //   .then((fetchResult) => {
  //     return fetchResult.arrayBuffer();
  //   })
  //   .then((module) => {
  //     WebAssembly.instantiate(module, {

  //     });
  //     // worker.postMessage(module);
  //   })
  //   .then((d) => console.log(d));
  //worker.postMessage(); // start the worker.

  console.time("testThread");
  doThreadingForCoeff(numberCount, coeffBuffer);

  // Module.getValue(coeffBuffer + 0 * nByte, "double");
  console.timeEnd("testThread");

  for (var i = 0; i < 4 * numberCount + 2; i += 2) {
    let a = [
      Module.getValue(coeffBuffer + i * nByte, "double"),
      Module.getValue(coeffBuffer + (i + 1) * nByte, "double"),
    ];
    //console.log(Module.getValue(coeffBuffer + i * nByte, "double"));
    threadedCoefficients.push(a);
  }

  //console.table(threadedCoefficients);
  Coefficients = threadedCoefficients;

  alert("PAINT");
  TriggerPaint();
};

function TriggerPaint() {
  console.log("painted");
  mainF();
}

function mainF() {
  const cb = (count) => async () => {
    for (let i = 0; i <= numberCount; i++) {
      let coeff = Coefficients[-i + numberCount];
      var { x, y } = await getFunctionDraw(
        x,
        y,
        -i,
        count,
        coeff[0],
        coeff[1],
        i == 0 ? "I" : ""
      );

      if (i) {
        coeff = Coefficients[i + numberCount];
        var { x, y } = await getFunctionDraw(
          x,
          y,
          i,
          count,
          coeff[0],
          coeff[1],
          i !== numberCount ? "" : "F"
        );
      }
    }

    count++;
    console.log(cb)
    window.dispatchEvent(new CustomEvent("buildPoint", { detail: { x, y } }));
    window.requestAnimationFrame(cb(count));
  };

  window.requestAnimationFrame(cb(0));
}

console.log("SUPPJS");
