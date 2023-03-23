unsigned long pulseStart1 = 0; // variable to store the time when the pulse starts
unsigned long distance1 = 0; // variable to measure the time between two edges.
unsigned long distance1a = 0;
unsigned long distance1b = 5000;
unsigned long distance1bPrev = 5000;
unsigned long pulseEnd1 = 0; // variable to store the time when the pulse ends
unsigned long pulseStart2 = 0; // variable to store the time when the pulse starts
unsigned long distance2 = 0; // variable to measure the time between two edges.
unsigned long distance2a = 0;
unsigned long distance2b = 0;
unsigned long distance2bPrev = 0;
unsigned long pulseEnd2 = 0; // variable to store the time when the pulse ends

unsigned long currentMillis;
long previousMillis = 0;    // set up timers
int diff;
long distance;

int flag1;
int flag2;

//filter

// Define the maximum and minimum values for the variable
const int MIN_VALUE1 = 0;
const int MAX_VALUE1 = 8000;
// Define the initial value for the variable
float value1 = 0;
// Define the target value for the variable
int targetValue1 = 0;
// Define the previous time
unsigned long previousTime1 = 0;
// Define the speed of the interpolation
const float SPEED1 = 0.2;


// Define the maximum and minimum values for the variable
const int MIN_VALUE2 = 0;
const int MAX_VALUE2 = 8000;
// Define the initial value for the variable
float value2 = 0;
// Define the target value for the variable
int targetValue2 = 0;
// Define the previous time
unsigned long previousTime2 = 0;
// Define the speed of the interpolation
const float SPEED2 = 0.2;

int syncIn = 18;
int usr1Echo = 2;
int usr2Echo = 3;
int Usr1Trig = 7;
int Usr2Trig = 8;
void setup() {
  
  pinMode(usr1Echo, INPUT);      // Ultrasonic Sensor 1 echo in
  pinMode(usr2Echo, INPUT);      // Ultrasonic Sensor 2 echo in
  pinMode(syncIn, INPUT);     // IR trigger in
  pinMode(Usr1Trig, OUTPUT);     // Ultrasonic Sensor 1 trigger out
  pinMode(Usr2Trig, OUTPUT);     // Ultrasonic Sensor 2 trigger out
  Serial.begin(115200);

  // set the interrupt pin as an input and enable the pull-up resistor
  pinMode(usr1Echo, INPUT);

  // attach the   function to the interrupt pin
  attachInterrupt(digitalPinToInterrupt(usr1Echo), pulseCounter1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(usr2Echo), pulseCounter2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(syncIn), receiver, CHANGE);

}

void loop() {

  currentMillis = millis();
  if (currentMillis - previousMillis >= 20) {  // start timed event
   
        previousMillis = currentMillis;
       
        if (distance1 > 0 && distance1 < 8000) {
          distance1a = distance1;       
        }
        targetValue1 = distance1a;
        updateValue1();
        previousTime1 = millis(); 

        if (distance2 > 0 && distance1 < 8000) {
          distance2a = distance2;
        }   
        targetValue2 = distance2a;
        updateValue2();
        previousTime2 = millis(); 

        Serial.print(3000);
        Serial.print(" , ");
        Serial.print(9000);
        Serial.print(" , ");
        Serial.print(value1);
        Serial.print(" , ");
        Serial.println(value2);

  }

}

// interrupt function to count the pulse width - left receiver
void pulseCounter1() {
  
      if (digitalRead(usr1Echo) == HIGH) {    
          pulseStart1 = micros();
      }
      else if (digitalRead(usr1Echo) == LOW) {
          pulseEnd1 = micros();
          distance1 = pulseEnd1 - pulseStart1;             // distance between rising and falling edge
              
      }      
}

// interrupt function to count the pulse width - right receiver
void pulseCounter2() {
  
      if (digitalRead(usr2Echo) == HIGH) {    
          pulseStart2 = micros();
      }
      else if (digitalRead(usr2Echo) == LOW) {
          pulseEnd2 = micros();
          distance2 = pulseEnd2 - pulseStart2;             // distance between rising and falling edge
              
      }      
}


void receiver () {
  // trigger UltrasonicS
  if (digitalRead(syncIn) == LOW) {
      digitalWrite(Usr1Trig, LOW);
      digitalWrite(Usr1Trig, HIGH);
      delayMicroseconds(15);
      digitalWrite(Usr1Trig, LOW);
  }

  else if(digitalRead(syncIn) == HIGH) {
      digitalWrite(Usr2Trig, LOW);
      digitalWrite(Usr2Trig, HIGH);
      delayMicroseconds(15);
      digitalWrite(Usr2Trig, LOW);
  }
}

void updateValue1() {
  // Calculate the new value for the variable using a cubic spline interpolation
  value1 += (targetValue1 - value1) * SPEED1;

  // Constrain the value to the minimum and maximum values
  value1 = constrain(value1, MIN_VALUE1, MAX_VALUE1);
}

void updateValue2() {
  // Calculate the new value for the variable using a cubic spline interpolation
  value2 += (targetValue2 - value2) * SPEED2;

  // Constrain the value to the minimum and maximum values
  value2 = constrain(value2, MIN_VALUE2, MAX_VALUE2);
}



