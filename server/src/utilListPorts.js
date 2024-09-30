const SerialPort = require('serialport');

// List all available ports
SerialPort.list().then(
  ports => ports.forEach(port => console.log(port)),
  err => console.error('Error listing ports', err)
);