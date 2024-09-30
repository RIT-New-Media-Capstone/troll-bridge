const { SerialPort } = require('serialport')
const { ReadlineParser } = require('@serialport/parser-readline')
const axios = require('axios');
require('dotenv').config();

// Adjust the arduino path
if (!process.env.USB_PATH) {
  console.log("ERROR: NO USB PATH FOUND. RUN npm find-ports");
  return;
}

let port;

function setUpSerial() {
  port = new SerialPort({ path: process.env.USB_PATH, baudRate: 9600 });
  const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

  // Open serialport and handle errors
  port.on('open', () => {
    console.log('Serial port opened.');
  });

  // Error handling
  port.on('error', (err) => {
    console.log('Error: ', err.message);
  });

  // Listen for data from Arduino (requesting more questions)
  parser.on('data', async (data) => {

    if (data.trim() == "GETQUESTIONS") {
      // Make a request for questions
      //console.log('Making a call to /trivia endpoint...');

      try {
        const response = await axios.get('http://localhost:3000/trivia');

        if (response) {
          //console.log(response.data);

          let questionString = "";
          let answerString = "";

          response.data.forEach(element => {
            questionString += `${element.question},`;
            answerString += `${element.answer},`;
          });

          writeToSerial(questionString);
          writeToSerial(answerString);
        }
      } catch (error) {
        console.error('Error calling /trivia endpoint:', error.message);
      }
    }
    else {
      console.log(data.trim());
    }

  });
}

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

module.exports = { setUpSerial };