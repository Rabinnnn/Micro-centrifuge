#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int potPin = A0;        // Potentiometer connected to analog pin A0
int motorEnablePin = 9; // PWM pin for motor speed control
int motorIN1 = 5;       // Motor driver input pin 1
int motorIN2 = 4;       // Motor driver input pin 2
int potValue = 0;
int motorSpeed = 0;

const int numReadings = 10;
int readings[numReadings];
int index = 0;
int total = 0;
// Threshold for stopping the motor
const int stopThreshold = 0;

void setup() {
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  // Set up motor control pins
  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);

  // Initialize readings array
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }

  // Initialize Serial communication for debugging
  Serial.begin(9600);
}

void loop() {
// Read and filter potentiometer value
  int rawPotValue = analogRead(potPin);
  total = total - readings[index];
  readings[index] = rawPotValue;
  total = total + readings[index];
  index = (index + 1) % numReadings;
  int filteredPotValue = total / numReadings;

  // Map potentiometer value to motor speed (0-255)
  motorSpeed = map(filteredPotValue, 0, 1023, 0, 255);

 
  



  // Calculate RPM (you may need to calibrate this)
  int rpm = map(motorSpeed, 0, 255, 0, 4500); // Adjust the range based on your motor specs

  // Clear the display and display RPM and motor state
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("RPM: ");
  display.println(rpm);
  display.display();

  // Print RPM and motor state to the Serial Monitor for debugging
  Serial.print("RPM: ");
  Serial.println(rpm);
 

  delay(100); // Adjust the delay as needed

    // Control motor direction based on motorSpeed
  if (motorSpeed > 0) {
    analogWrite(motorEnablePin, motorSpeed);
    digitalWrite(motorIN1, HIGH); // Set direction
    digitalWrite(motorIN2, LOW);
  } else {
    analogWrite(motorEnablePin,0);
    digitalWrite(motorIN1, LOW);  // Set direction
    digitalWrite(motorIN2, LOW);
  }
}
