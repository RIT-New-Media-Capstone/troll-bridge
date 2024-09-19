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

// state machine
enum states {awaitingResponse, armed, gettingQuestionAnswer, unlocked};
states state;

void setup()
{
  Serial.begin(9600);
  Serial.println("begin");

  servo_10.attach(10, 500, 2500);

  // setup LED pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  //btn pins
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  rearm();
}

void loop()
{
  trueButtonState = digitalRead(2);
  falseButtonState = digitalRead(3);

  switch (state) {
    case awaitingResponse: // wait for answer from user
      if (trueButtonState == HIGH) {
        Serial.println("input: TRUE");
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
        break;
      }
      else if (falseButtonState == HIGH) {
        // if FALSE is the correct answer unlock door
        Serial.println("input: FALSE");
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
        break;
      }
      else {
        break;
      }
    
    case armed:
      if (falseButtonState == HIGH || trueButtonState == HIGH) {
        state = gettingQuestionAnswer;
        getQuestionAnswer();
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
}

void getQuestionAnswer() {
  Serial.println("getting Q/A");

  question = "Is Mount Everest the tallest mountain";
  answer = true;

  // this function will take time in the future when API is implemented
  delay(1000);

  // once q/a has been recieved prompt user
  askQuestion();
}

void askQuestion() {
  Serial.println("asking question");

  // output question
  Serial.print(question);
  Serial.println("?");

  // update state
  state = awaitingResponse;
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

  Serial.println("servo set to 0");
  servo_10.write(0);
}

//unlock door
void unlock()
{
  Serial.println("unlocking");

  // update LEDs
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);

  Serial.println("servo set to 90");
  servo_10.write(90);

  delay(unlocktime);

  rearm();
}