var sp = require("serialport");

//Consts
var portName = "COM3";
var dateNow = (new Date).getTime();


var serialport = new sp.SerialPort(portName, {
    baudrate: 9600,
    // defaults for Arduino serial communication
     dataBits: 8,
     parity: 'none',
     stopBits: 1,
     flowControl: false
});

serialport.on('open', function(error){
  if (error) {
    console.log("Error opening " + portName)
  }
  else {
    console.log('Serial Port Opened');
    console.log('Syncing...');

    setTimeout(function() {
      serialport.write("SET"+ dateNow +"!");
      console.log("Sent time data");
    }, 5000);

    serialport.on('data', function(data){
        console.log(data.toString());
    });
  }
});
