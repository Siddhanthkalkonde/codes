#include<Wire.h>
#include<LiquidCrystal_I2C.h>


#define #define I2C_ADDR 0x27  // Change this address based on your specific LCD
LiquidCrystal_I2C lcd(I2C_ADDR);

// LiquidCrystal_I2C lcd(0x27);



const int tbsensor = A0;  
const int phsensor = A1;
int turbidityValue;       

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);  
}

void loop() {
 float read= turbudity();
 float ph= phlevel();

 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("turbudity =  ");
 lcd.print(read);


 lcd.setCursor(0,1);
 lcd.print("ph : ")
 lcd.print(ph);

 delay(500);
 
 
}


float turbudity(){
  float val = analogRead(tbsesnor);
  delay(500);

  return val ;
}


float phlevel(){

  float ph = analogRead(phsensor)*(5.0/1023.0);
  delay(500);

  return ph ;
}