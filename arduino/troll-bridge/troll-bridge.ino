/*
  Troll Bridge Arduino Interface
  by Thomas Martinez
  
  controls the troll bridge lock mechanism
*/

#include <Servo.h>
int pos = 0;
Servo servo_10;

int RED_LED_PIN = 9;
int GREEN_LED_PIN = 8;

int TRUE_BTN_PIN = 2;
int FALSE_BTN_PIN = 3;

int trueButtonState = 0;
int falseButtonState = 0;

// delays
int unlocktime = 5000;

String question = "";
boolean answer = false;

String incomingStr = "";

int qIndex = 0;

String questions[5];
bool answers[5];

// state machine
enum states {initialized, awaitingQuestions, parsingQuestions, awaitingResponse, armed, unlocked};
states state;

void setup()
{
  Serial.begin(9600);
  Serial.println("-----------------");

  servo_10.attach(10, 500, 2500);

  // setup LED pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  //btn pins
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  state = initialized;
  
  getQuestions();
}

void loop()
{
  //Serial.println(qIndex);
  trueButtonState = digitalRead(2);
  falseButtonState = digitalRead(3);

  switch (state) {
    case awaitingQuestions:
      if (Serial.available() > 0) {
        // read the incoming byte:
        incomingStr = Serial.readString();

        //Serial.print("Recieved: ");
        //Serial.println(incomingStr);

        delay(1000);

        parseQuestions(incomingStr);

        //state = parsingQuestions;
      }
      break;
    case parsingQuestions:
      parseQuestions(incomingStr);
      break;
    case awaitingResponse: // wait for answer from user
      if (trueButtonState == HIGH) {
        //Serial.println("input: TRUE");
        // if TRUE is the correct answer unlock door
        if(answer == true) {
          Serial.println("CORRECT!!!! :D");
          state = unlocked;
          unlock();
        }
        else {
          Serial.println("WRONG!!!! >:(");
          flashIncorrect();
          askQuestion();
        }
        if (qIndex > 5) {
          getQuestions();
          qIndex = 0;
        }
        break;
      }
      else if (falseButtonState == HIGH) {
        // if FALSE is the correct answer unlock door
        //Serial.println("input: FALSE");
        if(answer == false) {
          Serial.println("CORRECT!!!! :D");
          state = unlocked;
          unlock();
        }
        else{
          Serial.println("WRONG!!!! >:(");
          flashIncorrect();
          askQuestion();
        }
        if (qIndex > 5) {
          getQuestions();
          qIndex = 0;
        }
        break;
      }
      else {
        break;
      }
    
    case armed:
      if (falseButtonState == HIGH || trueButtonState == HIGH) {
        askQuestion();
      }
      break;

    default:
      break;
  }

  //Serial.println(state);

  delay(10);
}

// flashes red light (also creates delay for an incorrect button press)
void flashIncorrect() {
  for(int i = 0; i < 4; i++) {
    digitalWrite(RED_LED_PIN, LOW);
    delay(200);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(200);
  }
}

void nextQuestion() {
  // temporary fake data retrieval 
  question = questions[qIndex];
  answer = answers[qIndex];
  qIndex++;
}

void askQuestion() {
  //Serial.println("asking question");
  nextQuestion();
  delay(1000);
  // update state
  state = awaitingResponse;

  // output question
  Serial.println(question);
}

// lock door
void rearm()
{
  Serial.println("arming");

  // update LEDs
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);

  // update state
  state = armed;

  //Serial.println("servo set to 0");
  servo_10.write(0);
}

//unlock door
void unlock()
{
  //Serial.println("unlocking");

  // update LEDs
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);

  Serial.println("servo set to 90");
  servo_10.write(90);

  delay(unlocktime);

  rearm();
}

// Function to initialize the arrays
void getQuestions() {
  if (state != awaitingQuestions) {
    Serial.println("GETQUESTIONS");
    state = awaitingQuestions;
  }
}

void parseQuestions(String rawStr) {
  Serial.println("entering parseQuestions");
  // Convert the String to a char array to work with strtok()
  char inputCharArray[rawStr.length() + 1];
  rawStr.toCharArray(inputCharArray, rawStr.length() + 1);

  // Use strtok() to split the input string by commas
  char* token = strtok(inputCharArray, ",");

  int index = 0;
  bool q = true;
  while (token != NULL) {
    Serial.print(token);
    Serial.print(" -> ");
    // Store the token into the array
    if (String(token).equals("NEXTARRAY")) {
      q = false;
      index = 0;
      Serial.println("REMOVED (HOPEFULLY)");
      token = strtok(NULL, ",");
      continue;
    }
    if (q) {
      questions[index] = String(token);
      //Serial.println(String(token));
    }
    else {
      answers[index] = (String(token).equals("true"));
      //Serial.println(answers[index]);
    }
    index++;
    token = strtok(NULL, ","); // Continue splitting
    if (!q && index == 5) {
      token = strtok(NULL, ","); // Continue splitting
    }
  }

  // Print out the parsed strings
Serial.println("Parsed questions:");
for (int i = 0; i < 5; i++) {
  Serial.print(questions[i]);
  Serial.print(" = ");
  Serial.println(answers[i]);
}

  //arm
  rearm();
}
