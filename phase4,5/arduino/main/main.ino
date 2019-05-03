#include <LiquidCrystal.h>
#include <FanController.h>

#define SENSOR_PIN 2
#define SENSOR_THRESHOLD 1000

// PWM pin
#define PWM_PIN 5

/*
 * The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 */

// Initialize library
FanController fan(SENSOR_PIN, SENSOR_THRESHOLD, PWM_PIN);
 
// ANALOG IN - A0
int tempPin = 0;

// ANALOG IN - A3
int voltagePin = A3; 

// ANALOG IN - A4
int currentPin = A4;

int fanSpeedIn = 5;

// ANALOG OUT - 2
int fanPin = 2;

// Switch between first screen (temp and fan) and second screen (current and voltage)
boolean firstScreen = false;

// Count the number of loops the program goes through
int delayCount = 0;

//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup()
{
  // Initializing LCD to be 16x2
  lcd.begin(16, 2);
  
  // Start up the library
  fan.begin();

  // Setting PWM pin to output
  pinMode(PWM_PIN, OUTPUT);

  // Initialize Serial Monitor
  Serial.begin(9600);
}
void loop()
{
  
  if (delayCount >= 10) {
      // Switch screen
      firstScreen = !firstScreen;

      // Reset delayCount
      delayCount = 0;
  } else {
      // Increase delayCount
      delayCount++;
  }
  
  // Get temp from temperature sensor
  int tempAnalog = analogRead(tempPin);

  int fanSpeed = analogRead(fanSpeedIn);

  // Get current from currentPin
  double currentRead = analogRead(currentPin);

  // Get voltage from voltagePin
  double voltageRead = analogRead(voltagePin);

  // Calculate voltage
  // (x / 1023) * 5 --> (analog/1023) = (voltage/5v)
  double voltage = (voltageRead / 1023) * 5 * 101;

  // Calculate voltage for the current
  double voltageCurrent = (currentRead / 1023) * 5;

  // Calculate current
  double current = (voltageCurrent * 1000) / 2000;

  // Initial calculation of temp in Kelvin
  double tempK = log ( 10000.0 * ( ( 1024.0 / tempAnalog - 1 ) ) );

  //  Temp Kelvin
  tempK = 1 / ( 0.001129148 + ( 0.000234125 + ( 0.0000000876741 * tempK * tempK ) ) * tempK );

  // Convert Kelvin to Celcius
  float tempC = tempK - 273.15;

  // Convert Celcius to Fahrenheit
  float tempF = (tempC * 9.0) / 5.0 + 32.0; 

  // Set LCD to print on first line
  lcd.setCursor(0, 0);

   if (firstScreen) {
      // Display Temperature in C
      // lcd.print("Temp:        C  ");
  
      // Display Temperature in F
         lcd.print("Temp:        F  ");
      
    } else {
       // Display Voltage         
         lcd.print("Volt:        V  ");
    }

  // Sets LCD to print on first line, offset by 6
  lcd.setCursor(6, 0);

  if (firstScreen) {
     // Display Temperature in C
     // lcd.print(tempC);

     // Display Temperature in F
        lcd.print(tempF);
    } else {
     // Display Voltage
        lcd.print(voltage);
    }
  
  // Set LCD to print on second line
  lcd.setCursor(0, 1);  

   if (firstScreen) {
      // Display Fan Speed message
         lcd.print("Fan:         RPM");
    } else {
      // Display Current in amps
         lcd.print("Cur:         A  ");
    }

//  lcd.print(fanSpeed);

   lcd.setCursor(6, 1);

   if (firstScreen) {
     // Get RPM using FanController library
        unsigned int rpms = fan.getSpeed(); // Send the command to get RPM
     
     // Display fan speed in RPM        
        lcd.print(rpms);
    } else {
      
     // Display current
        lcd.print(current);
    }

 // Turn on fan if tempF is greater than 85
  if (tempF > 85) {
//    lcd.print("HIGH");
//    analogWrite(fanPin, 255);
    digitalWrite(PWM_PIN, HIGH);
  } else {
//    lcd.print("LOW ");
//    analogWrite(fanPin, 0);
    digitalWrite(PWM_PIN, LOW);
  }

//  Serial.println(currentRead);
   
  delay(1000);
}
