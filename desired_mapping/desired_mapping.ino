#include <Encoder.h>
#include "Wire.h"
#include <MPU6050_light.h>
#include <CytronMotorDriver.h> // Include the Cytron motor driver library

MPU6050 mpu(Wire);
CytronMD motor1, motor2; // Define two motor objects

unsigned long timer = 0;
long oldPositionX = -999;
long oldPositionY = -999;
int diameter = 6;

const float pi = 3.1416;

float globalX = 0;
float globalY = 0;

float prevglobalX = 0;
float prevglobalY = 0;

float xdistance = 0;
float ydistance = 0;
double distance = 0;

Encoder myEnc1(2, 4);
Encoder myEnc2(3, 5);
int yaw;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) { } // stop everything if could not connect to MPU6050
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
  Serial.println("Basic Encoder Test:");
  motor1.begin(10, 11); // Initialize motor driver pins
  motor2.begin(12, 13); // Adjust pin numbers as needed
}

void loop() {
  mpu.update();
  if ((millis() - timer) > 10) {
    yaw = mpu.getAngleZ();
    timer = millis();
  }
  long newPositionX = myEnc1.read();
  long newPositionY = myEnc2.read();
  if ((newPositionX != oldPositionX) || (newPositionY != oldPositionY)) {
    oldPositionX = newPositionX;
    oldPositionY = newPositionY;
    xdistance = pi * diameter * newPositionX / 80;
    ydistance = -pi * diameter * newPositionY / 60;
    myEnc1.write(0);
    myEnc2.write(0);
    float z = (yaw * pi) / 180;
    float sine = sin(z);
    float cose = cos(z);
    globalX = globalX + xdistance * cose + ydistance * sine;
    globalY = globalY + sine + ydistance * cose;
    prevglobalX = globalX;
    prevglobalY = globalY;
  }
  // Serial.print("  X : ");
  // Serial.print(globalX);
  // Serial.print("  Y : ");
  // Serial.print(globalY);
  // Serial.println("");
  distance = sqrt(pow(globalX - prevglobalX, 2) + pow(globalY - prevglobalY, 2));
  Serial.print("\t DISTANCE BETWEEN 2 POINTS =  ");
  Serial.print(distance, 2);


 
  float tX = 10.0; 
  float tY = 10.0; 
  float eX = tX - globalX;
  float eY = tY - globalY;

  
  float motorSpeed1 = eX + eY; 
  float motorSpeed2 = eX - eY; 
  motor1.setSpeed(motorSpeed1);
  motor2.setSpeed(motorSpeed2);

  
}
