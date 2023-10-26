#include <LiquidCrystal.h>

LiquidCrystal lcd(7,6,5,4,3,2);  // Initialize the LCD

// Pin definitions for buttons
const int hour = 9;
const int min = 10;
const int set = 11;
const int start = 12;
const int buz = 13 ;
const int s4 =8 ;

// definning the led/ buttons pins


// Variables to store time and alarm
int hours = 0;
int minutes = 0;
int seconds = 0;
int alarmHours = 0;
int alarmMinutes = 0;
bool isClockRunning = false;
bool isSettingAlarm = false;
bool alrmstate = 1;

bool buttonstate = 0 ;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Manual Clock");
  delay(1000);
  lcd.clear();
  pinMode(hour, INPUT_PULLUP);
  pinMode(min, INPUT_PULLUP);
  pinMode(set, INPUT_PULLUP);
  pinMode(start, INPUT_PULLUP);


  pinMode(buz, OUTPUT);

  pinMode(s4, INPUT_PULLUP);
  Serial.begin(9600);

  displayTime();
}

void loop() {
   //int in = digitalRead(s4) ;
  if (!isClockRunning) {
    if (isSettingAlarm) {
      sets();
    } else {
      setClock();
    }

  } else {
    displayTime();


    int alarmstate = digitalRead(s4);

    if(alarmstate == LOW){
      digitalWrite(buz, LOW);

    }
  }
 
  
}

void setClock() {
  lcd.clear();
  lcd.print("Set Time:");
  lcd.setCursor(0, 1);
  lcd.print(String(hours) + ":" + String(minutes) + ":" + String(seconds));

  if (digitalRead(hour) == LOW) {
    hours = (hours + 1) % 24;
    delay(200);
  }
  if (digitalRead(min) == LOW) { 
    minutes = (minutes + 1) % 60;
    delay(200);
  }

  if (digitalRead(set) == LOW) {
    isSettingAlarm = true;
    lcd.clear();
    lcd.print("Set Alarm:");
    lcd.setCursor(0, 1);
    lcd.print(String(alarmHours) + ":" + String(alarmMinutes));
    delay(200);
  }

  if (digitalRead(start) == LOW) {
    isClockRunning = true;
    lcd.clear();
    displayTime();
  }
}

void sets() {
  lcd.clear();
  lcd.print("Set Alarm:");
  lcd.setCursor(0, 1);
  lcd.print(String(alarmHours) + ":" + String(alarmMinutes));

  if (digitalRead(hour) == LOW) {
    alarmHours = (alarmHours + 1) % 24;
    delay(200);
  }
  if (digitalRead(min) == LOW) {
    alarmMinutes = (alarmMinutes + 1) % 60;
    delay(200);
  }

  if (digitalRead(set) == LOW) {
    isSettingAlarm = false;
    lcd.clear();
    displayTime();
    delay(200);
  }

  if (digitalRead(start) == LOW) {
    isClockRunning = true;
    lcd.clear();
    displayTime();
  }
 

}

void displayTime() {
  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.setCursor(0, 1);
  lcd.print(String(hours, DEC) + ":" + String(minutes, DEC) + ":" + String(seconds, DEC));

  if (isClockRunning) {
    delay(1000);  // Update time every second
    seconds++;
    if (seconds == 60) {
      seconds = 0;
      minutes++;
      if (minutes == 60) {
        minutes = 0;
        hours++;
        if (hours == 24) {
          hours = 0;
        }
      }
    }
    
    if (hours == alarmHours && minutes == alarmMinutes) {
      // Alarm triggered. Stop the clock.
      //isClockRunning = false;
      lcd.clear();
      lcd.print("Alarm Triggered!");
      digitalWrite(buz, HIGH);
     
      //buzAlarm();
      delay(700);
      lcd.clear();
       digitalWrite(buz,1);
      displayTime();

      delay(200);
       //digitalWrite(buz, buttonstate);
      //  int e = digitalRead(s4);

      //  if(e == 0){
      //   digitalWrite(buz, LOW);
      //   Serial.println(e);
      //   delay(200);
      //  }    

      

       
      
      // Delay for one minute (60,000 milliseconds)
     // delay(60000);
      
      
    }
//     else{
// // Start the clock again.
//       isClockRunning = true;
//     }
  }
}
