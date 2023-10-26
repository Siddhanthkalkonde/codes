#include <PS3BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
PS3BT *PS3[2]; // We will use this pointer to store the two instance, you can easily make it larger if you like, but it will use a lot of RAM!
const uint8_t length = sizeof(PS3) / sizeof(PS3[0]); // Get the lenght of the array
bool printAngle[length];
bool oldControllerState[length];




#include<stdio.h>
#include "CytronMotorDriver.h"
#include<math.h>

double power;
double theta;
double turn;

int val1,val2,val3,val4;

CytronMD motor1(PWM_DIR, 4, 14);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor2(PWM_DIR, 5, 15); // PWM 2 = Pin 9, DIR 2 = Pin 10.
CytronMD motor3(PWM_DIR, 6, 16);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor4(PWM_DIR, 7, 17); // PWM 2 = Pin 9, DIR 2 = Pin 10.

int maxspeed=150;




#define supportlifter1 27
#define mainlifter1 28
#define lifter1_pusher 29
#define shooter1_pusher 30




bool triangle_state = 0;
bool circle_state = 0;
bool square_state = 0;
bool cross_state = 0;



#include <Servo.h>

#define servograb1 8
#define servograb2 44
#define servodrop1 45
#define servodrop2 46

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;





struct DATA
{
  byte joylx;
  byte joyly;
  byte joyrx;
  byte joyry;
  byte l2;
  byte r2;
  bool triangle;
  bool circle;
  bool cross;
  bool square;
  bool up;
  bool right;
  bool down;
  bool left;
  bool l1;
  bool l3;
  bool r1;
  bool r3;

};

DATA data[length]; //STRUCTURE VARIABLE


void setup()
{
  for (uint8_t i = 0; i < length; i++) {
    PS3[i] = new PS3BT(&Btd); // Create the instances
    PS3[i]->attachOnInit(onInit); // onInit() is called upon a new connection - you can call the function whatever you like
  }

  Serial.begin(115200);
  #if !defined(_MIPSEL_)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));


  resetdata(0);
  resetdata(1);
  //pin definations
  pinMode(mainlifter1,OUTPUT);
  pinMode(supportlifter1,OUTPUT);
  pinMode(lifter1_pusher,OUTPUT);
  pinMode(shooter1_pusher,OUTPUT);
  digitalWrite(mainlifter1,LOW);
  digitalWrite(supportlifter1,LOW);
  digitalWrite(lifter1_pusher,LOW);
  digitalWrite(shooter1_pusher,LOW);
  
  triangle_state = 0;
  circle_state = 0;
  square_state = 0;
  cross_state = 0;

  servo1.attach(servograb1);
  servo2.attach(servograb2);
  servo3.attach(servodrop1);
  servo4.attach(servodrop2);
  
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);  

}


void loop() 
{
  Usb.Task();

  for (uint8_t i = 0; i < length; i++) 
  {
    if (PS3[i]->PS3Connected || PS3[i]->PS3NavigationConnected) 
    {     
      data[i].joylx = PS3[i]->getAnalogHat(LeftHatX);
      data[i].joyly = PS3[i]->getAnalogHat(LeftHatY);
      data[i].joyrx = PS3[i]->getAnalogHat(RightHatX);
      data[i].joyry = PS3[i]->getAnalogHat(RightHatY);
      data[i].l2 = PS3[i]->getAnalogButton(L2);
      data[i].r2 = PS3[i]->getAnalogButton(R2);
      data[i].triangle = PS3[i]->getButtonClick(TRIANGLE);
      data[i].circle = PS3[i]->getButtonClick(CIRCLE);
      data[i].cross = PS3[i]->getButtonClick(CROSS);
      data[i].square = PS3[i]->getButtonClick(SQUARE);
      data[i].up = PS3[i]->getButtonClick(UP);
      data[i].right = PS3[i]->getButtonClick(RIGHT);
      data[i].down = PS3[i]->getButtonClick(DOWN);
      data[i].left = PS3[i]->getButtonClick(LEFT);
      data[i].l1 = PS3[i]->getButtonClick(L1);
      data[i].l3 = PS3[i]->getButtonClick(L3);
      data[i].r1 = PS3[i]->getButtonClick(R1);
      data[i].r3 = PS3[i]->getButtonClick(R3); 
        
    }
     else
    {
      resetdata(i);    
    }
  }

    if (data[1].triangle)
  {
    if(triangle_state == 0)
    {
      Serial.println(triangle_state); 
      digitalWrite(mainlifter1,HIGH);
      delay(500);
      digitalWrite(supportlifter1,HIGH);       
      triangle_state = 1; 
    }
    else
    {
      Serial.println(triangle_state); 
      digitalWrite(mainlifter1,LOW);
      delay(500);
      digitalWrite(supportlifter1,LOW);       
      triangle_state = 0;
    }
  }

  if (data[1].cross)
  {
    if(cross_state == 0)
    {
      Serial.println(cross_state); 
      
      cross_state = 1; 
    }
    
    else
    {
      Serial.println(cross_state); 
      
      cross_state = 0; 
    }
    
  }
  

  if (data[1].circle)
  {
    if(circle_state == 0)
    {
      Serial.println(circle_state); 
      digitalWrite(lifter1_pusher,HIGH);   
      circle_state = 1; 
    }
    else
    {
      Serial.println(circle_state); 
      digitalWrite(lifter1_pusher,LOW); 
      circle_state = 0; 
    }
    
  }

    if (data[1].square)
  {
    if(square_state == 0)
    {
      Serial.println(square_state); 
      digitalWrite(shooter1_pusher,HIGH);   
      square_state = 1; 
    }
    else
    {
      Serial.println(square_state); 
      digitalWrite(shooter1_pusher,LOW); 
      square_state = 0; 
    }
    
  }


  int RX = map(data[0].joyrx,0,255,-maxspeed,maxspeed);
  int RY = map(data[0].joyry,0,255,maxspeed,-maxspeed);
  turn = map(data[0].joylx,0,255,maxspeed,-maxspeed);

  theta = atan2(RY,RX);
  power = hypot(RX,RY);  
  double sine = sin(theta - 3.1413/4);
  double cose = cos(theta - 3.1413/4);
  double max = max(abs(sine),abs(cose));  

  val1 = (power*cose/max + turn); 
  val2 = (power*sine/max - turn);
  val3 = (power*cose/max - turn); 
  val4 = (power*sine/max + turn);

  motor1.setSpeed(val1);
  motor2.setSpeed(val2);
  motor3.setSpeed(val3);
  motor4.setSpeed(val4);

  // motorvals();

}


void onInit() {
  for (uint8_t i = 0; i < length; i++) {
    if ((PS3[i]->PS3Connected || PS3[i]->PS3NavigationConnected) && !oldControllerState[i]) {
      oldControllerState[i] = true; // Used to check which is the new controller
      PS3[i]->setLedOn((LEDEnum)(i + 1)); // Cast directly to LEDEnum - see: "controllerEnums.h"
    }
  }
}


void resetdata(uint8_t i)
{
  data[i].joylx = 128;
  data[i].joyly = 128;
  data[i].joyrx = 128;
  data[i].joyry = 128;
  data[i].l2 = 0;
  data[i].r2 = 0;
  data[i].triangle = 0;
  data[i].circle = 0;
  data[i].cross = 0;
  data[i].square = 0;
  data[i].up = 0;
  data[i].right = 0;
  data[i].down = 0;
  data[i].left = 0;
  data[i].l1 = 0;
  data[i].l3 = 0;
  data[i].r1 = 0;
  data[i].r3 = 0;    
}


void motorvals()
{
  Serial.print("MOTOR1: ");
  Serial.print(val1);
  Serial.print("\tMOTOR2: ");
  Serial.print(val2);
  Serial.print("\tMOTOR3: ");
  Serial.print(val3);
  Serial.print("\tMOTOR4: ");
  Serial.println(val4);
}