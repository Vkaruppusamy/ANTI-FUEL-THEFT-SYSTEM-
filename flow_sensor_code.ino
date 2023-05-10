

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

float vol = 0.0,l_minute;
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
unsigned long flow_frequency;



void flow () // Interrupt function to increment flow
{
   flow_frequency++;
}



void setup()
{
   Serial.begin(9600);
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
   
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
   display.clearDisplay();
   //display.setCursor(0,0);
   display.print("PETROL FLOWMETER");                                  
   //display.setCursor(0,1);
   display.print("   sensing... ");
   delay(500);
   currentTime = millis();
   cloopTime = currentTime;
}

void loop ()
{
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0)
    {
      
       l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      display.clearDisplay();
      //display.setCursor(0,0);
      display.print("Rate: ");
      display.print(l_minute);
      display.print(" L/M");
      l_minute = l_minute/60;
      //display.setCursor(0,1);
      vol = vol +l_minute;
      display.print("Vol:");
      display.print(vol);
      display.print(" L");
      flow_frequency = 0; // Reset Counter
      Serial.print(l_minute, DEC); // Print litres/hour
      Serial.println(" L/Sec");
    }
    else {
      Serial.println(" flow rate = 0 ");
      display.clearDisplay();
      //display.setCursor(0,0);
      display.print("Rate: ");
      display.print( flow_frequency );
      display.print(" L/M");
      //display.setCursor(0,1);
      display.print("Vol:");
      display.print(vol);
      display.print(" L");
    }
   }
}
