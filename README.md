# Troll Bridge
The world's first trivia-based security system.

Authors: 
- Colby Heaton
- Tyler Amey
- Thomas Martinez

## How To Use
1. Connect the Arduino to the PC
   1. Assuming the Arduino is properly constructed and installed with the Troll Bridge program...
   2. Connect the Arduino to the device that will run the Node.js server via USB cable.

2. Get the Node.js Server in Working Order
    1. Add your OpenAI API key to a .env file in the "server" directory, on the same level as package.json. The variable name should be ```API_KEY```
    2. Additionally, add another variable to the .env called ```USB_PATH```. For its value, set it to the filepath of the USB port that the Arduino is connected to. This can be found in the Arduino IDE - for Windows devices, the USB path will probably be ```COM3``` or some other number.
    3. Open a terminal in the "server" directory.
    4. Run the command `npm i` to install the required packages.
    5. Run the command `npm start` to initialize the server.
  
3. Using Troll Bridge
    1. Now, the Arduino and the node server will be talking with each other. The Arduino makes a request to the node server for trivia questions - the server will then make a request to the OpenAI API for questions, and it will send the results back to the Arduino.
    2. You will be seeing output in the terminal window - this is coming from the Arduino. In our original scope, this text would be printed to an LCD screen.
    3. When you see the message ```armed``` in the console, that means the lock is ready to be opened. Press any button on the Arduino to make a question appear.
    4. Once you have been prompted with a question, you have unlimited time to give a ```true``` or ```false``` answer. Since there are two buttons on the Arduino, one button corresponds to ```true``` and the other one means ```false```.
    5. If you provide an incorrect answer, the lock will remain ```armed```, and a new question will appear in the console.
    6. If you answer correctly, the lock will disarm temporarily, and it will automatically re-arm after a few seconds, starting the cycle over again.
    7. The Arduino stores five questions and answers at a time - once its bank of questions is used up, it makes a request to the node server for more questions.


      
![image](https://people.rit.edu/mc2682/235/selfie.jpg)

Maxwell Chu
Another Satisfied Customer
