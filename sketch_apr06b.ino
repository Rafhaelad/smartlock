#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Keypad.h> // keys on keypad

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C
#define passwordLengt 5
#define startPos 0



int ledPin = 13;                // the pin that the LED is atteched to
int pirPin = 10;                 // sensor Output
int pirStat = 0;                // variable to store the sensor status (value)


char data[passwordLengt];
char master[passwordLengt] = {"1234"};  // The password for the keypad is 1234
byte dataCount = 0; 
char customKey;
const byte ROWS = 4; // four rows 
const byte COLS = 4; // three columns
char hexaKeys[ROWS][COLS] = { // keys on keypad
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};  //keypad rows 
byte colPins[COLS] = {5, 4, 3, 2};  //keypad column

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(9600);         // initialize serial
  pinMode(pirPin, INPUT);      // Initalize motion detector
  pinMode(ledPin, OUTPUT);    // initalize LED as an output
  pinMode(12, OUTPUT);       // init pin for piezo
  digitalWrite(12, LOW);     // initalize for motion 


  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  } 

   // display.display();
   //delay(100); // Pause for 0,5 seconds
  display.clearDisplay();

}


void loop() {
  checkmovement();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Please    Enter Your Code:");
  display.display();

  customKey = customKeypad.getKey();

  if (customKey) {
    data[dataCount] = customKey;
    
    if (dataCount == 0) {
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.println("*");
      display.display();
    } else if (dataCount == 1) {
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.println("**");
      display.display();
    } else if (dataCount == 2) {
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.println("***");
      display.display();
    } else if (dataCount == 3) {
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.println("****");
      display.display();
    }
    delay(100);
    dataCount++;
  }

  if (dataCount == passwordLengt - 1) {

    display.clearDisplay();
    display.setCursor(0, 0);

    if (!strcmp(data, master)) {

      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.println("Correct!");
      display.display();
      delay(100);
      display.clearDisplay();
    }
    else
    {
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.println("Incorrect!");
      display.display();
      delay(100);
      display.clearDisplay();
      playsound();
    }
    clearData();
  }
}


void checkmovement() {
  Serial.println(digitalRead(pirPin)); //
  pirStat = digitalRead(pirPin);  // read sensor value
  delay(500);                     // delay 0,5 seconds
  if (pirStat == HIGH) {          // check if the sensor is HIGH
    digitalWrite(ledPin, HIGH);   // turn LED ON
    Serial.println("MOTION DETECTED"); //Writes in seriall monitor "motion detected" when motion is detected
  }
  else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    Serial.println("NO MOTION DETECTED"); //Writes in seriall monitor "No motion detected" when no motion is detected
  }
}






void clearData() {

  while (dataCount != 0) {

    data[dataCount--] = 0;
  }
  return;
}


// Alarm Tone
void playsound() {
  tone(12, 261); //Middle C
  delay(100);

  tone(12, 277); //C#
  delay(100);

  tone(12, 294); //D
  delay(100);

  noTone(12);

}
