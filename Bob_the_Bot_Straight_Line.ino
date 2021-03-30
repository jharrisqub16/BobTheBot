//Motors
int motor1A = 4; //Left motor reverse
int motor1B = 5; //Left motor forward
int motor2A = 3; //Right motor forward
int motor2B = 2; //Right motor reverse

byte power = 100; //Max 255, use byte to save space

//Ultrasonic sensors
//Forward
int rVcc = 37; //Supply pin
int rTrig = 39; //Trigger pin
int rEcho = 41; //Echo pin
//Right
int fVcc = 44; //Supply pin
int fTrig = 46; //Trigger pin
int fEcho = 48; //Echo pin

long fDuration, fDistance, rDuration, rDistance;

//Encoders
int interruptLeft = 19; //Left encoder interrupt pin(blue wire);
int interruptRight = 21; //Right encoder interrupt pin(blue wire);

long pulseLeft = 0; //Variable to count left motor encoder
long pulseRight = 0; //Variable to count left motor encoder

//Values
int left90 = 480; //Pulse for 90 degree rev
int right90 = 540; //Pulse for 90 degree rev
int fullRevLeft = 1920; //Pulse for full rev == 24cm
int fullRevRight = 2160; //Pulse for full rev == 24cm

const double SOUND = 0.0343; //Constant speed of sound, cm/us

void setup() {
  //Set motors to output
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  
  //Set up encoders
  pinMode(interruptLeft, INPUT_PULLUP);
  pinMode(interruptRight, INPUT_PULLUP);
  
  //Set up interrupt
  attachInterrupt(digitalPinToInterrupt(interruptLeft), countLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptRight), countRight, CHANGE);

  //Set up Ultrasonic sensors
  pinMode(fVcc, OUTPUT);
  pinMode(fTrig, OUTPUT);
  pinMode(fEcho, INPUT);
  pinMode(rVcc, OUTPUT);
  pinMode(rTrig, OUTPUT);
  pinMode(rEcho, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  
  forward(fullRevLeft, fullRevRight);
}

void triggerForward()
{
  digitalWrite(fVcc, 1); //Apply power to forward sensor
  digitalWrite(fTrig, 0); //Pull trig low to reset
  
  delayMicroseconds(2); //Stop program for 2us
  digitalWrite(fTrig, 1); //Turn trig on to generate pulse
  delayMicroseconds(10); //Stop program for 10us
  digitalWrite(fTrig, 0); //Turn trig off

  fDuration = pulseIn(fEcho, 1); //Measures the time(in micro-seconds) between the rising and falling edges of signal on echo
  fDistance = (fDuration*SOUND)/2; //(time x speed of sound) ÷ 2 = distance(cm) --- ÷2 as it's an echo
}

void triggerRight()
{
  digitalWrite(rVcc, 1); //Apply power to right sensor
  digitalWrite(rTrig, 0); //Pull trig low to reset
  
  delayMicroseconds(2); //Stop program for 2us
  digitalWrite(rTrig, 1); //Turn trig on to generate pulse
  delayMicroseconds(10); //Stop program for 10us
  digitalWrite(rTrig, 0); //Turn trig off

  rDuration = pulseIn(rEcho, 1); //Measures the time(in micro-seconds) between the rising and falling edges of signal on echo
  rDistance = (rDuration*SOUND)/2; //(time x speed of sound) ÷ 2 = distance(cm) --- ÷2 as it's an echo
}

void countLeft()
{
  //Increment up pulseLeft variable
  pulseLeft++;
}

void countRight()
{
  //Increment up pulseRight variable
  pulseRight++;
}

void backward(int revLeft, int revRight)
{
  //Reset pulse variables
  pulseReset();

  //While both motors haven't turned 1 full revolution
  while(pulseLeft < revLeft && pulseRight < revRight)
  {
  Serial.println("Backwards");
    //If left motor hasn't turned 1 full revolution
    if(pulseLeft < revLeft)
    {
      analogWrite(motor1A, 0);
      analogWrite(motor1B, power); //Left forward
    }
    //If right motor hasn't turned 1 full revolution
    if(pulseRight < revRight)
    {
      analogWrite(motor2A, power); //Right forward
      analogWrite(motor2B, 0);
    }
  }
  
  //Turn motors off
  motorStop();
  //Reset pulse variables
  pulseReset();
}

void forward(int revLeft, int revRight)
{
  //Reset pulse variables
  pulseReset();

  //While both motors haven't turned 1 full revolution
  while(pulseLeft < revLeft && pulseRight < revRight)
  {
  Serial.println("Forward");
    //If left motor hasn't turned 1 full revolution
    if(pulseLeft < revLeft)
    {
      analogWrite(motor1A, power); //Left reverse
      analogWrite(motor1B, 0);
    }
    //If right motor hasn't turned 1 full revolution
    if(pulseRight < revRight)
    {
      analogWrite(motor2A, 0);
      analogWrite(motor2B, power); //Right reverse
    }
  }
  //Reset pulse variables
  pulseReset();
}

void left(int revLeft, int revRight)
{
  //Reset pulse variables
  pulseReset();

  //While both motors haven't turned 90 degrees
  while(pulseLeft < revLeft && pulseRight < revRight)
  {
    //If left motor hasn't turned 90 degrees
    if(pulseLeft < revLeft)
    {
      analogWrite(motor1B, power); //Left forward
      analogWrite(motor1A, 0);
    }
    //If right motor hasn't turned 90 degrees
    if(pulseRight < revRight)
    {
      analogWrite(motor2B, power); //Right reverse
      analogWrite(motor2A, 0);
    }
  }

  //Turn motors off
  motorStop();
  //Reset pulse variables
  pulseReset();
}

void right()
{
  //Reset pulse variables
  pulseReset();
  
  //While both motors haven't turned 90 degrees
  while(pulseLeft < left90 && pulseRight < right90)
  {
    //If left motor hasn't turned 90 degrees
    if(pulseLeft < left90)
    {
      analogWrite(motor1A, power); //Left reverse
      analogWrite(motor1B, 0);
    }
  
    //If right motor hasn't turned 90 degrees
    if(pulseRight < right90)
    {
      analogWrite(motor2A, power); //Right forward
      analogWrite(motor2B, 0);
    }
  }
  //Turn motors off
  motorStop();
  //Reset pulse variables
  pulseReset();
}

//Turn off motors
void motorStop()
{
  analogWrite(motor1B, 0);
  analogWrite(motor2A, 0);
  analogWrite(motor1A, 0);
  analogWrite(motor2B, 0);
}

//Set pulse variables to 0
void pulseReset()
{
  pulseLeft = 0;
  pulseRight = 0;
}
