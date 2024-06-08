// Colors of the lines
// format: {rmin, rmax, gmin, gmax, bmin, bmax}
int outsideBorder[] = {0,10,100,255,0,10}; // green rgb
int gateZone[] = {0,10,0,10,100,255}; // blue rgb
int targetPoint[] = {100,255,0,10,0,10}; // red rgb

// dist to detect is about half a box, so it it seen from the center of box
float minBoundaryDist = 20;


// Motor pins
const int motb_pin1 = 11;
const int motb_pin2 = 3;
const int mota_pin1 = 9;
const int mota_pin2 = 10;

// Color sensing pins
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
int frequency = 0;
  
// Other pins
const int buttonPin = 24; // digital, not PWM
const int photoresistorPin = A0;

// defines ultrasound pins numbers
const int trigPin = 2;
const int echoPin = 12;

// defines ultrasound variables
long duration;
int distance;

// defines whether our program is running
bool programStarted = false;

////////////////////////////////////////////////////////////////////// Setup
void setup() {
  /////////////////////////// Set up motors
   // motor B
  pinMode(motb_pin1, OUTPUT);
  pinMode(motb_pin2, OUTPUT);
  
  // motor A
  pinMode(mota_pin1, OUTPUT);
  pinMode(mota_pin2, OUTPUT);

  // other pins
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);

  /////////////////////////// Set up color sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Setting frequency-scaling to 20% (for color sensor)
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
}

///////////////////////////////////////////////////////////////////// Start drive motor functions
void moveForward(int speedFA, int speedFB, int inchesF) {
  // motor A
  analogWrite(mota_pin2, 0);
  analogWrite(mota_pin1, speedFA);
  
  // motor B
  analogWrite(motb_pin2, 0);
  analogWrite(motb_pin1, speedFB);
  
  // move forward the distance in inches
  
  int myDelayF;
  myDelayF = inchesF*55;
  
  delay(myDelayF);
  
  
  //stop
  fullStop();
}


void moveBackward(int speedBA, int speedBB, int inchesB) {
  // motor A
  analogWrite(mota_pin2, speedBA);
  analogWrite(mota_pin1, 0);
  
  // motor B
  analogWrite(motb_pin2, speedBB);
  analogWrite(motb_pin1, 0);
  
    // move forward the distance in inches
  
  int myDelayB;
  myDelayB = inchesB*55;
  
  delay(myDelayB);
  
  
  //stop
  fullStop();
  
}

void slowStop() {
  // motor A
  analogWrite(mota_pin1, 0);
  analogWrite(mota_pin2, 0);
  
  // motor B
  analogWrite(motb_pin1, 0);
  analogWrite(motb_pin2, 0);
}

void fullStop() {
  analogWrite(mota_pin1, 255);
  analogWrite(mota_pin2, 255);
  
  // motor B
  analogWrite(motb_pin1, 255);
  analogWrite(motb_pin2, 255);
}

void turnRight() {
  // motor A is stopped
  analogWrite(mota_pin1, 0);
  analogWrite(mota_pin2, 0);
  // motor B is moving forward
  analogWrite(motb_pin1, 255);
  analogWrite(motb_pin2, 0);
  // for .7 second
  delay(575);
  // then stop motor A
  analogWrite(mota_pin1, 0);
  analogWrite(mota_pin2, 0);
}

void turnLeft() {
  // motor B is stopped
  analogWrite(motb_pin1, 0);
  analogWrite(motb_pin2, 0);
  // motor A is moving forward
  analogWrite(mota_pin2, 0);
  analogWrite(mota_pin1, 255);
  // for .7 second
  delay(600);
  // then stop motor B
  analogWrite(motb_pin1, 0);
  analogWrite(motb_pin2, 0);
}

void do180() {
  // while motor A goes forward, B goes back
  analogWrite(mota_pin2, 0);
  analogWrite(mota_pin1, 255);
  analogWrite(motb_pin2, 255);
  analogWrite(motb_pin1, 0);
  delay(500);
}

/////////////////////////////////////////////////////////////////////////////// End drive motor functions

float readDist() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  return distance;
}

int getRedValue() {
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  return frequency;
}

int getGreenValue() {
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  return frequency;
}

int getBlueValue() {
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  return frequency;
}

// Returns true if detects a color in the user-provided RGB range.
bool isColor(int rmin, int rmax, int gmin, int gmax, int bmin, int bmax) {
  int rval = getRedValue();
  int gval = getGreenValue();
  int bval = getBlueValue();

// checks the proper values for r,g,and b as stated on the very top
  return (rval>=rmin && rval<=rmax 
    && gval>=gmin && gval<=gmax 
    && bval>=bmin && bval<=bmax);
}


bool atBorder() {
  // is robot at border? (green)
  return isColor(outsideBorder[0], outsideBorder[1], outsideBorder[2], outsideBorder[3], outsideBorder[4], outsideBorder[5]);
}

bool atGate() {
  // is robot at gate? (blue)
  return isColor(gateZone[0], gateZone[1], gateZone[2], gateZone[3], gateZone[4], gateZone[5]);
}

bool atTarget() {
  // is robot at target (red), stop if here
  return isColor(targetPoint[0], targetPoint[1], targetPoint[2], targetPoint[3], targetPoint[4], targetPoint[5]);
}

bool atWall() {
  // is robot at wall? (distance from wall)
  return readDist() <= minBoundaryDist;
}

////////////////////////////////////////////////////////////////////////////////// Main loop

void loop() {
  if (programStarted) {

    /* If cannot use color sensor:
    // Read the light intensity
    int val = analogRead(photoresistorPin);
    Serial.print("Photoresistor intensity proportional to ");
    Serial.println(val);
    //*/

    
    // Read distance with ultrasonic
    if (atWall || atBorder) {
      // moveBackward just a little
      turnRight();
      if (atWall || atBorder) {
      do180(); // do NOT turn right only once, you'd go backwards
      }
    } else if (atTarget) {
      fullStop();
    } else {
      // moveForward one half square (to detect boundaries)
      moveForward(255,255, 10) //using 10 as an approximate for 25 cm (half box)
    }

    
  } else {
    // Check if button pressed to start whole program
    
    if (digitalRead(buttonPin) == LOW) {
      programStarted = true;
      Serial.println("Starting program..");
      
    }
  }
  
}