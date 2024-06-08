int motb_pin1 = 11;
int motb_pin2 = 3;

int mota_pin1 = 9;
int mota_pin2 = 10;

int buttonPin = 2;

int trigPin = 4;
int echoPin = 5;


void setup() {
  // motor B
  pinMode(motb_pin1, OUTPUT);
  pinMode(motb_pin2, OUTPUT);
  
  // motor A
  pinMode(mota_pin1, OUTPUT);
  pinMode(mota_pin2, OUTPUT);
  
  // button setup
  pinMode(buttonPin, INPUT_PULLUP);
  
  //ultrasonic pin
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
  
}


/*
                              FUNCTIONS                                   FUNCTIONS                                         FUNCTIONS
*/


  // sends out and recieves sound
  // returns distance in cm
int ultrasonic(){
  
  long time;
  float distance;
  
  // trigger a sound
  // send out trigger signal
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);
  
  
  // a sound has gone out
  // wait for sound to come back
  
  time = pulseIn(echoPin, HIGH);
  
  // calculate distance in cm
   distance = 0.01715 * time;
   
   return distance;
}


void moveToWall(int speedWA, int speedWB) {
  // motor A
  analogWrite(mota_pin1,speedWA);
  analogWrite(mota_pin2, 0);
  
  // motor B
  analogWrite(motb_pin1, speedWB);
  analogWrite(motb_pin2, 0);
  
  // stop when you hit wall
  int distance = ultrasonic();
  
  while (distance > 10){
    // do nothing except check dist
    distance = ultrasonic();
  }
  
  fullStop();
  moveBackward(255,255,10);
  // while motor A goes forward, B goes back
  analogWrite(mota_pin2, 0);
  analogWrite(mota_pin1, speedWA);
  analogWrite(motb_pin2, speedWB);
  analogWrite(motb_pin1, 0);
  delay(500);
  
  moveForward(255,255,5);
  
  
  
  
}

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





/*
                              LOOP                                   LOOP                                         LOOP
*/

void loop() {
    
    Serial.println(ultrasonic());
    delay(100);
 
  if (digitalRead(buttonPin)==LOW){
    
    while (true) {
    moveToWall(255,255);
    }
    
  }
  else{
    fullStop();
  }

/*

1. create ultrasonic function that returns distance in cm DONE
2. create move to wall function DONE
3. when you stop, back up and turn around DONE
4. once you see a wall, stop, back up turn around and keep looking for walls (infinite loop)



*/



  }
  


/* Challenges
1. start towards a cone, make a U-turn around it, then go back to original spot DONE
2. travel around a box until back to start location

*/