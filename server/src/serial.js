const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

// Adjust the path
const port = new SerialPort('/dev/ttyUSB0', { baudRate: 9600 });
const parser = port.pipe(new Readline({ delimiter: '\n' }));

// Open port and handle errors
port.on('open', () => {
  console.log('Serial port opened.');
});

// Error handling
port.on('error', (err) => {
  console.log('Error: ', err.message);
});

// Listen for data from Arduino (requesting more questions)
parser.on('data', (data) => {
  console.log('Received from Arduino:', data.trim());

  // Make a request for questions
});

// Exposed function to write data to the serial port
function writeToSerial(data) {
  port.write(data + '\n', (err) => {
    if (err) {
      console.log('Error on write:', err.message);
    } else {
      console.log('Message sent to Arduino:', data);
    }
  });
}

// Export the function so it can be used in other files
module.exports = { writeToSerial };
