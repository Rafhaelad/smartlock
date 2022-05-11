#include <Adafruit_SSD1306.h>  //libary for keyboard
#include <Wire.h>  // libary for keyboard
#include <Keypad.h> // library for keyboard

#define SCREEN_WIDTH 128   // OLED display width, in pixels
#define SCREEN_HEIGHT 64   //  OLED display height, in pixels
#define OLED_RESET 4   // Reset pin 
#define SCREEN_ADDRESS 0x3C   // Display text on the screen 
#define passwordLengt 5  // The length of the password
#define startPos 0  // The position of the start 



int ledPin = 13;                // the pin that the LED is attached to
int pirPin = 2;                 // sensor Output
int pirStat = 0;                // variable to store the sensor status (value)
int state = LOW;             // by default, no motion detected



char data[passwordLengt]; // Character to hold password input
char master[passwordLengt] = {"1254"};  // The password for the keypad is 1234
byte dataCount = 0; //Counter for character entries
char customKey;    // Character to hold key input
const byte ROWS = 4; // four rows
const byte COLS = 4; // three columns
char hexaKeys[ROWS][COLS] = { // // Array to represent keys on keypad
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};  //keypad rows/ Connections to Arduino
byte colPins[COLS] = {5, 4, 3, 2};  //keypad column/ Connections to Arduino

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); // Create keypad object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(9600);         // initialize serial
  pinMode(pirPin, INPUT);     // Initalize motion detector
  pinMode(ledPin, OUTPUT);    // initalize LED as an output
  pinMode(12, OUTPUT);       // init pin for piezo
  digitalWrite(12, LOW);     // initalize for motion

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);   // Don't proceed, loop forever
  }

  display.clearDisplay(); //clears display

}

void loop() {
  checkmovement(); 
  display.setTextSize(2); //text size 
  display.setTextColor(WHITE); //text color
  display.setCursor(0, 0);  //placement of the text
  display.println("Please    Enter Your Code:"); // what to write on the screen 
  display.display();  // Updates the screen so it comes up on the screen

  customKey = customKeypad.getKey(); // Look for keypress

  if (customKey) {     // Enter keypress into array and increment counter
    data[dataCount] = customKey; //variable for character/number enter
    if (dataCount == 0) { //when you enter one more character/number on keypad
      display.setTextSize(2); //size of text
      display.setTextColor(WHITE); //color of the text
      display.println("*");  //writes on the screen
      display.display();    // Updates the screen so it comes up on the screen
    } else if (dataCount == 1) { //when you enter one more character/number on keypad
      display.setTextSize(2); //size of text
      display.setTextColor(WHITE); //color of the text
      display.println("**"); //writes on the screen
      display.display();    // Updates the screen so it comes up on the screen
    } else if (dataCount == 2) { //when you enter one more character/number on keypad
      display.setTextSize(2); //size of text
      display.setTextColor(WHITE); //color of the text
      display.println("***"); //writes on the screen
      display.display();  // Updates the screen so it comes up on the screen
    } else if (dataCount == 3) { //when you enter one more character/number on keypad
      display.setTextSize(2); //size of text
      display.setTextColor(WHITE); //color of the text
      display.println("****"); //writes on the screen
      display.display();  // Updates the screen so it comes up on the screen
    }
    dataCount++; //increase so that you can enter one more character
  }

  if (dataCount == passwordLengt - 1) {  // See if we have reached the password length

    display.clearDisplay();  //clears display
    display.setCursor(0, 0); //where the text should be

    if (!strcmp(data, master)) {    // If password is correctit it will write on the screen

      display.setTextSize(2);  //size of text
      display.setTextColor(WHITE); //color of the text
      display.println("Correct!"); //writes on the screen
      display.display(); // Updates the screen so it comes up on the screen
      delay(100);  // A delay/pause 
      display.clearDisplay(); //clears the screen/display
    }
    else
         // If password is incorrect it will write on the screen password is incorrect
    {
      display.setTextSize(2);  //size of text
      display.setTextColor(WHITE); //color of the text
      display.println("Incorrect!"); //writes on the screen
      display.display(); // Updates the screen so it comes up on the screen
      delay(100); // A delay/pause 
      display.clearDisplay(); //clears the screen/display
      playsound(); //plays alarm 
    }
    clearData(); // clears data
  }
}


void checkmovement() {
  Serial.println(digitalRead(pirPin)); //Writes in value of sensor
  pirStat = digitalRead(pirPin);  // read sensor value
  delay(100);                // delay 100 milliseconds
  if (pirStat == HIGH) {          // check if the sensor is HIGH
    digitalWrite(ledPin, HIGH);   // turn LED ON
    Serial.println("MOTION DETECTED"); //Writes in seriall monitor "motion detected" when motion is detected


    if (state == LOW) {  //check if the sensor is LOW
      Serial.println("Motion detected!");
      state = HIGH;     // update variable state to HIGH
    }
  }
  else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    Serial.println("NO MOTION DETECTED"); //Writes in seriall monitor "No motion detected" when no motion is detected

    if (state == HIGH) { //check id sensor is high
      Serial.println("Motion stopped!"); //Writes "Motion stopped"
      state = LOW;       // update variable state to LOW

    }
  }
}



// Go through array and clear data
// when it is not empty it clears the screen 
void clearData() {

  while (dataCount != 0) {  //if it is not empty on the screen

    data[dataCount--] = 0;  //clear screen
  }
  return; 
}


// Alarm Tone
void playsound() {
  tone(12, 261); //melodi tone:Middle C 
  delay(100); // A delay/pause 

  tone(12, 277); //melodi C#
  delay(100); // A delay/pause 

  tone(12, 294); //melodi D
  delay(100); // A delay/pause 

  noTone(12); //stops alaram

}
