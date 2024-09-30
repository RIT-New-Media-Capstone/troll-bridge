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
int unlocktime = 3000;

String question = "";
boolean answer = false;

int qIndex = 0;

String questions[10];
bool answers[10];

// state machine
enum states {awaitingResponse, armed, gettingQuestionAnswer, unlocked};
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

  getQuestions();
  rearm();
}

void loop()
{
  //Serial.println(qIndex);
  trueButtonState = digitalRead(2);
  falseButtonState = digitalRead(3);

  switch (state) {
    case gettingQuestionAnswer:

      nextQuestion();
      
      delay(1000);

      // update state
      state = awaitingResponse;
      // ask question
      askQuestion();

      break;
    case awaitingResponse: // wait for answer from user
      if (trueButtonState == HIGH) {
        //Serial.println("input: TRUE");
        // if TRUE is the correct answer unlock door
        if(answer == true) {
          Serial.println("CORRECT!!!! :D");
          state = unlocked;
          nextQuestion();
          unlock();
        }
        else {
          Serial.println("WRONG!!!! >:(");
          flashIncorrect();
          nextQuestion();
          askQuestion();
        }
        break;
      }
      else if (falseButtonState == HIGH) {
        // if FALSE is the correct answer unlock door
        //Serial.println("input: FALSE");
        if(answer == false) {
          Serial.println("CORRECT!!!! :D");
          state = unlocked;
          nextQuestion();
          unlock();
        }
        else{
          Serial.println("WRONG!!!! >:(");
          flashIncorrect();
          nextQuestion();
          askQuestion();
        }
        break;
      }
      else {
        break;
      }
    
    case armed:
      if (falseButtonState == HIGH || trueButtonState == HIGH) {
        state = gettingQuestionAnswer;
      }
      break;

    default:
      break;
  }

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

  if (qIndex > 10) {
    getQuestions();
  }
}

void nextQuestion() {
  if (qIndex > 9) {
    getQuestions();
    qIndex = 0;
  }
  // temporary fake data retrieval 
  question = questions[qIndex];
  answer = answers[qIndex];
  qIndex++;
}

void askQuestion() {
  //Serial.println("asking question");

  // output question
  Serial.print(question);
  Serial.println("?");
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
    // Assign values to questions
    questions[0] = "Is the Earth round";
    questions[1] = "Do dolphins breathe air";
    questions[2] = "Is fire a chemical reaction";
    questions[3] = "Do humans have 3 lungs";
    questions[4] = "Can birds fly";
    questions[5] = "Is the Great Wall of China visible from space";
    questions[6] = "Is the human body mostly water";
    questions[7] = "Do snakes have legs";
    questions[8] = "Is 2 a prime number";
    questions[9] = "Do spiders have six legs";

    // Assign values to answers
    answers[0] = true;
    answers[1] = true;
    answers[2] = true;
    answers[3] = false;
    answers[4] = true;
    answers[5] = false;
    answers[6] = true;
    answers[7] = false;
    answers[8] = true;
    answers[9] = false;
}