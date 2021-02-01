console.log("started");

var earth = new Image();

const event = new Event("finalValues");

canvasWidth = 1650;
canvasHeight = 3000;

window.addEventListener(
  "buildPoint",
  function (e) {
    var drawCtx = document.getElementById("mycanvasDraw").getContext("2d");

    drawCtx.lineWidth = 1.5;
    drawCtx.strokeStyle = "white";
    drawCtx.quadraticCurveTo(e.detail.x, e.detail.y, e.detail.x, e.detail.y);
    //drawCtx.lineTo(e.detail.x, e.detail.y);
    drawCtx.stroke();
  },
  false
);

var twoPi = 2 * Math.PI;

function getFunctionDraw(
  x_center = 0,
  y_center = 0,
  k,
  count = 0,
  realCoeff,
  imgCoeff,
  intialFunction = "I"
) {
  var timePeriod = 1;
  deltaT = 0.1;
  var radius = Math.sqrt(Math.pow(realCoeff, 2) + Math.pow(imgCoeff, 2)) * 5;

  var ctx = document.getElementById("mycanvas").getContext("2d");
  var t = 0 + count * deltaT;

  function x_value(t) {
    return (
      x_center +
      (realCoeff * Math.cos(twoPi * k * (1 / (timePeriod * 62.5 * 3)) * t) -
        imgCoeff * Math.sin(twoPi * k * (1 / (timePeriod * 62.5 * 3)) * t)) *
        5
    );
  }

  function y_value(t) {
    return (
      y_center +
      (realCoeff * Math.sin(twoPi * k * (1 / (timePeriod * 62.5 * 3)) * t) +
        imgCoeff * Math.cos(twoPi * k * (1 / (timePeriod * 62.5 * 3)) * t)) *
        5
    );
  }

  return new Promise((resolve, reject) => {
    var x = x_value(t);
    var y = y_value(t);

    ctx.globalCompositeOperation = "destination-over";
    if (intialFunction === "I") {
      ctx.clearRect(0, 0, canvasWidth, canvasHeight); // clear canvas
    }

    ctx.fillStyle = "rgba(255, 255, 255, 0.5)";
    ctx.strokeStyle = "rgba(255,255, 255, 0.5)";
    ctx.save();

    ctx.beginPath();
    ctx.arc(x_center, y_center, radius, 0, Math.PI * 2);
    ctx.stroke();

    ctx.beginPath();
    ctx.fillStyle = intialFunction === "F" ? "red" : "white";
    ctx.arc(x_center, y_center, 1, 0, 2 * Math.PI);
    ctx.stroke();
    ctx.moveTo(x_center, y_center);
    ctx.lineTo(x, y);
    ctx.stroke();
    ctx.beginPath();
    ctx.arc(x, y, intialFunction === "F" ? 10 : 1, 0, 2 * Math.PI, true);
    ctx.fill();
    t += deltaT;
    // cb(0, x, y);
    resolve({ x, y });
  });

  // if (intialFunction === "F") {
  //   window.dispatchEvent(new CustomEvent("buildPoint", { detail: { x, y } }));
  // }
  // if (intialFunction === "I") {
  //   requestAnimationFrame(draw);
  // }
}

function getFunctionDraw_cpy(
  x_center,
  y_center,
  radius,
  timePeriod = 1,
  deltaT = 1,
  count = 0,
  intialFunction = "I",
  cb = () => {}
) {
  var ctx = document.getElementById("mycanvas").getContext("2d");
  var t = 0 + count * deltaT;

  function x_value(t) {
    return (
      x_center + radius * Math.cos(twoPi * (1 / (timePeriod * 62.5 * 3)) * t)
    );
  }

  function y_value(t) {
    return (
      y_center + radius * Math.sin(twoPi * (1 / (timePeriod * 62.5 * 3)) * t)
    );
  }

  return new Promise((resolve, reject) => {
    var x = x_value(t);
    var y = y_value(t);

    ctx.globalCompositeOperation = "destination-over";
    if (intialFunction === "I") {
      ctx.clearRect(0, 0, 700, 700); // clear canvas
    }

    ctx.fillStyle = "rgba(0, 0, 0, 0.4)";
    ctx.strokeStyle = "rgba(0,0, 0)";
    ctx.save();

    ctx.beginPath();
    ctx.arc(x_center, y_center, radius, 0, Math.PI * 2);
    ctx.stroke();

    ctx.beginPath();
    ctx.fillStyle = "black";
    ctx.arc(x_center, y_center, 1, 0, 2 * Math.PI);
    ctx.stroke();
    ctx.moveTo(x_center, y_center);
    ctx.lineTo(x, y);
    ctx.stroke();
    ctx.beginPath();
    ctx.arc(x, y, 2, 0, 2 * Math.PI, true);
    ctx.fill();
    t += deltaT;
    // cb(0, x, y);
    resolve({ x, y });
  });

  // if (intialFunction === "F") {
  //   window.dispatchEvent(new CustomEvent("buildPoint", { detail: { x, y } }));
  // }
  // if (intialFunction === "I") {
  //   requestAnimationFrame(draw);
  // }
}
