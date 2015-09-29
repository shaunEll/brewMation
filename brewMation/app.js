﻿var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var temporal = require("temporal");
var five = require("johnny-five");

var routes = require('./routes/index');
var users = require('./routes/users');

var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

// uncomment after placing your favicon in /public
app.use(favicon(__dirname + '/public/favicon.ico'));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(require('stylus').middleware(path.join(__dirname, 'public')));
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', routes);
app.use('/users', users);

// catch 404 and forward to error handler
app.use(function (req, res, next) {
    var err = new Error('Not Found');
    err.status = 404;
    next(err);
});

// error handlers

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
    app.use(function (err, req, res, next) {
        res.status(err.status || 500);
        res.render('error', {
            message: err.message,
            error: err
        });
    });
}

// production error handler
// no stacktraces leaked to user
app.use(function (err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
        message: err.message,
        error: {}
    });
});

var server = app.listen(3000, function () {
  var host = server.address().address;
  var port = server.address().port;

  console.log('brewMation listening at http://%s:%s', host, port);
});

//var socket = require('socket.io')(server);

// ******** JOHNNY-FIVE CODE ****************************
// var board = new five.Board();
// board.on("ready", function() {
//   var temperature = new five.Temperature({
//     controller: "TMP36",
//     pin: "A0"
//   });
//
//   temperature.on("change", function() {
//       console.log(this.celsius + "°C", this.fahrenheit + "°F");
//   });
// });

//****** NODE-SERIALPORT CODE *********************
// var sp = require("serialport");
//
// //let's see which ports are available
// sp.list(function (err, ports) {
//   console.log("Listing available serial ports: ");
//
//   if(ports.length === 0) {
//     console.log("No ports discovered!");
//   }
//
//   ports.forEach(function(port) {
//     console.log(port.comName + " " + port.pnpId);
//   });
// });
//
// var serialPort = new sp.SerialPort("COM3", {
//   baudrate: 9600
// }, false); // this is the openImmediately flag [default is true]
//
// serialPort.open(function (error) {
//   if ( error ) {
//     console.log('failed to open: '+error);
//   } else {
//     console.log('open');
//     serialPort.on('data', function(data) {
//       console.log('data received: ' + data);
//     });
//     serialPort.write("ls\n", function(err, results) {
//       console.log('err ' + err);
//       console.log('results ' + results);
//     });
//   }
// });

module.exports = app;
