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

int fanSpeedIn = 5;

// ANALOG OUT - 2
int fanPin = 2;

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
}
void loop()
{
  // Get temp from temperature sensor
  int tempAnalog = analogRead(tempPin);

  int fanSpeed = analogRead(fanSpeedIn);

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

  // Display Temperature in C
  // lcd.print("Temp:        C  ");
  
  // Display Temperature in F
  lcd.print("Temp:        F  ");

  // Sets LCD to print on first line, offset by 6
  lcd.setCursor(6, 0);
  
  // Display Temperature in C
  // lcd.print(tempC);

  // Display Temperature in F
  lcd.print(tempF);

  // Set LCD to print on second line
  lcd.setCursor(0, 1);  

  // Display Fan Speed message
  lcd.print("Fan:         RPM");
//  lcd.print(fanSpeed);

   lcd.setCursor(6, 1);
   unsigned int rpms = fan.getSpeed(); // Send the command to get RPM
   lcd.print(rpms);
   
  if (tempF > 85) {
//    lcd.print("HIGH");
//    analogWrite(fanPin, 255);
    digitalWrite(PWM_PIN, HIGH);
  } else {
//    lcd.print("LOW ");
//    analogWrite(fanPin, 0);
    digitalWrite(PWM_PIN, LOW);
  }
 
  delay(500);
}
