// libraries for servo and IR
#include <IRremote.h>
#include <Servo.h>

int serPin1 = 12; // pin for the servo
int serPin2 = 13; // pin for the servo
int IRpin = 11; // pin for the IR sensor
int pos1 = 0;    // variable to store the servo position

boolean vacant1 = true;
boolean vacant2 = true;
boolean vacant3 = true;

// stepper motor (1) platform
int dirpin1 = 7;
int steppin1 = 8;

// stepper motor (2) base
int dirpin2 = 9;
int steppin2 = 10;


// SETTING THE NUMBER OF STEPS IN Y DIRECTION
int ySTEPS = 13000;


Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
IRrecv irrecv(IRpin); //create IR object. IRpin is input signal
decode_results results; // variable for readings of IR receiver

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  // stepper motor
  pinMode(dirpin1, OUTPUT);
  pinMode(steppin1, OUTPUT);
  pinMode(dirpin2, OUTPUT);
  pinMode(steppin2, OUTPUT);
  
  myservo1.attach(serPin2);
  myservo2.attach(serPin1);
}

void loop() {
    if (irrecv.decode(&results)) {
      irrecv.resume();   // Receive the next value
    }

    // first cuby - storing an item
    if (results.value == 16724175) {  // if 1 is pressed
      if (vacant1 == true) {
        motion(8120, true);
        results.value = 0;
      } else {
        motion(8120, false);
        results.value = 0;
      }
      vacant1 = !vacant1;
    }

    // second cuby - storing an item
    if (results.value == 16718055) {  // if 2 is pressed
      if (vacant2 == true) {
        motion(16170, true);
        results.value = 0;
      } else {
        motion(16170, false);
        results.value = 0;
      }
      vacant2 = !vacant2;
    }

    // third cuby - storing an item
    if (results.value == 16743045) {  // if 3 is pressed
      if (vacant3 == true) {
        motion(23900, true);
        results.value = 0;
      } else {
        motion(23900, false);
        results.value = 0;
      }
      vacant3  = !vacant3;
    } 
}
void motion(int stepsNum, boolean toStore) {
    int i;

// SEQUANCE FOR STORING
  if (toStore == true) {
    myservo1.write(0);
    myservo2.write(180);
    fromCubyY();
    goForward(stepsNum);
    toCubyY();
    goBackward(stepsNum+150);

// SEQUENCE FOR RETRIEVING
  } else {
    goForward(stepsNum);
    fromCubyY();
    goBackward(stepsNum+150);
    toCubyY();
  }
}


// moving the platform "forward" along X (using DOUBLESTEPS)
void goForward(int xStepsNum) {
    digitalWrite(dirpin2, HIGH);     // Set the direction.
    int i;
    delay(100);
    for (i = 0; i<xStepsNum; i++)       // Iterate for 4000 microsteps.
    {
      digitalWrite(steppin2, HIGH);  // This LOW to HIGH change is what creates the
      digitalWrite(steppin2, LOW); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(50);      // This delay time is close to top speed for this
    }                              // particular motor. Any faster the motor stalls.

    digitalWrite(dirpin2, HIGH);    
    //delay(100);
    
    for (i = 0; i<xStepsNum; i++)       // Iterate for 4000 microsteps.
    {
      digitalWrite(steppin2, HIGH);  // This LOW to HIGH change is what creates the
      digitalWrite(steppin2, LOW); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(50);      // This delay time is close to top speed for this
    }  
}


// moving the platform "backward" along X
void goBackward(int xStepsNum) {
    digitalWrite(dirpin2, LOW);    // Change direction.
    delay(100);                   // WAITING TIME 
    int i;
    for (i = 0; i<xStepsNum; i++)       // Iterate for 10000 microsteps
    {
      digitalWrite(steppin2, HIGH);  // This LOW to HIGH change is what creates the
      digitalWrite(steppin2, LOW); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(50);      // This delay time is close to top speed for this
    }                              // particular motor. Any faster the motor stalls.

    digitalWrite(dirpin2, LOW);    // Change direction.
    //delay(100);
    for (i = 0; i<xStepsNum; i++)       // Iterate for 10000 microsteps
    {
      digitalWrite(steppin2, HIGH);  // This LOW to HIGH change is what creates the
      digitalWrite(steppin2, LOW); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(50);      // This delay time is close to top speed for this
    }    
}


// taking an item from the cuby (motion in Y direction)
void fromCubyY() {
  movePlatformIn();
  closeDoors();
  movePlatformOut();
  openDoors();
}

// inserting an item to the cuby (motion in Y direction)
void toCubyY() {
  closeDoors();
  movePlatformIn();
  openDoors();
  movePlatformOut();
  closeDoors();
  movePlatformInFinish();
  openDoors();
  movePlatformOutFinish();
}


void closeDoors() {
    for (pos1 = 0; pos1 <= 90; pos1 += 1) { 
       // in steps of 1 degree
       myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
       myservo2.write(180-pos1);              // tell servo to go to position in variable 'pos'
       delay(15);                       // waits 15ms for the servo to reach the position
    }
}

void openDoors() {
    for (pos1 = 90; pos1 >= 0; pos1 -= 1) { // goes from 180 degrees to 0 degrees
       myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
       myservo2.write(180-pos1);              // tell servo to go to position in variable 'pos'
       delay(15);                       // waits 15ms for the servo to reach the position
    }
}

void movePlatformIn() {
    digitalWrite(dirpin1, LOW);     // Set the direction.
    delay(100);
    int i;
    for (i = 0; i<ySTEPS; i++)       // Iterate for 4000 microsteps.
    {
      digitalWrite(steppin1, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(steppin1, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(50);      // This delay time is close to top speed for this
    }                              // particular motor. Any faster the motor stalls.
}

void movePlatformInFinish() {
    digitalWrite(dirpin1, LOW);     // Set the direction.
    delay(100);
    int i;
    for (i = 0; i<2500; i++)       // Iterate for 4000 microsteps.
    {
      digitalWrite(steppin1, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(steppin1, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(50);      // This delay time is close to top speed for this
    }                              // particular motor. Any faster the motor stalls.
}

void movePlatformOut() {
    digitalWrite(dirpin1, HIGH);    // Change direction.
    delay(100);                   // WAITING TIME 
    int i;
    for (i = 0; i<(ySTEPS+100); i++)       // Iterate for 10000 microsteps
    {
      digitalWrite(steppin1, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(steppin1, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(50);      // This delay time is close to top speed for this
    }                              // particular motor. Any faster the motor stalls.
}

void movePlatformOutFinish() {
    digitalWrite(dirpin1, HIGH);    // Change direction.
    delay(100);                   // WAITING TIME 
    int i;
    for (i = 0; i<(2500+100); i++)       // Iterate for 10000 microsteps
    {
      digitalWrite(steppin1, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(steppin1, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(50);      // This delay time is close to top speed for this
    }                              // particular motor. Any faster the motor stalls.
}


