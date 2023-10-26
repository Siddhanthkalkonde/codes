
#include <PulseSensorPlayground.h>
#include <Adafruit_SSD1306.h>

PulseSensorPlayground ps;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int pulse ;


void setup() {
  Serial.begin(9600); 
  ps.begin(); 

  // Initialize the OLED display
  if(!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display(); // Display initialization
  delay(1000); 
  display.clearDisplay();
}

void loop() {
 
 bpm();
 

}


void bpm(){

  pulse = ps.getBeatsPerMinute();

  if (ps.sawStartOfBeat()){
    Serial.println("BPM: ");
    Serial.print(pulse);
    
    displaymeter(); // calling the  oled  display function 

  }
  delay(100);
}



void displayneter(){

  display.clearDisplay();
  display.setTextSize(3);      // Set text size
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("BPM: ");
  display.print(pulse);
  display.display();
}
