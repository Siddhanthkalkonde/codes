/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */


// Encoder ticks 60 ticks per revolution in Y
// Encoder ticks 80 ticks per revolution in X

#include <Encoder.h>

#include "Wire.h"
#include <MPU6050_light.h>

#include<math.h>


MPU6050 mpu(Wire);
unsigned long timer = 0;
long oldPositionX  = -999;
long oldPositionY  = -999;
int diameter = 6 ; 
const float pi= 3.1416 ;


float globalX = 0 ;
float globalY = 0 ;

float prevglobalX ;
float prevglobalY ;

float xdistance;
float ydistance;
double distance = 0 ;

Encoder myEnc1(2, 4);
Encoder myEnc2(3, 5);

int yaw ;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);

  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
  Serial.println("Basic Encoder Test:");
}


void loop() {
  mpu.update();
  // Serial.print("\tZ : ");
	// Serial.println(mpu.getAngleZ());
  if((millis()-timer)>10){ // print data every 10ms
	yaw = mpu.getAngleZ();
	timer = millis();  
  
  }


  long newPositionX = myEnc1.read();
  long newPositionY = myEnc2.read();
  if ((newPositionX != oldPositionX) || (newPositionY != oldPositionY)) {
    oldPositionX = newPositionX;
    oldPositionY = newPositionY;
    xdistance = pi*diameter*newPositionX/80;
    ydistance = -pi*diameter*newPositionY/60;
    myEnc1.write(0);
    myEnc2.write(0);


    float z = (yaw*pi)/180 ;

    float sine = sin(z);
    float cose = cos(z);
    globalX = globalX +  xdistance*cose + ydistance*sine;
    globalY = globalY +  sine + ydistance*cose;

    prevglobalX =globalX ;
    prevglobalY = gloablY ;
  }

  

  
  
  Serial.print("  X : ");
  Serial.print(globalX);
  Serial.print("  Y : ");
  Serial.print(globalY);
  Serial.println("") ;
  // Serial.print("  Yaw : ");
  // Serial.println(yaw);

  // Serial.print("  X : ");
  // Serial.print(xdistance);
  // Serial.print("  Y : ");
  // Serial.print(ydistance);
  // Serial.print("  Yaw : ");
  // Serial.println(yaw);


  distance = sqrt(pow(globalX - prevglobalX , 2) + pow(globalY- prevglobalY , 2 )) ;
  

  Serial.print("\t DISTANCE BETWEEN 2 POINTS =  ") ;
  Serial.print(distance,2);

}