/*
 * BrewMation arudino controller
*/

// Load the libraries
#include <LiquidCrystal.h>
#include <ArduinoJson.h>
#include <Time.h>

// Initialize LCD
LiquidCrystal lcd(12,11,5,4,3,2);

//PINS
const int temperaturePin = 0;
const int downButton = 7;
const int upButton = 6; //
const int goodTempRangeLED = 13; //Indicate when we are at the right temp

//REFRESH RATES
const int buttonReadUpdate = 200; //miliseconds between button reads
const int tempUpdate = 10000; //temperature read refresh

unsigned long lastButtonRead = buttonReadUpdate;
unsigned long lastTempUpdate = tempUpdate;

//TIME MESSAGES
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

//MISC
const String degreesCelsius  = String((char)223) + "C"; // ºC symbol

bool manualMode = true; //Are we in manual or automatic mode
float desiredTemp = 40.00; //What is the ideas temp?

void setup()
{
  //Setup buttons
  pinMode(downButton, INPUT);
  pinMode(upButton, INPUT);

  //Setup LEDs
  pinMode(goodTempRangeLED, OUTPUT);  
  
  //Open serial port
  Serial.begin(9600);
  Serial.println("Started Arduino COM");
  
  //Setup LCd & Clear
  lcd.begin(16, 2);
  lcd.clear();

  setupLCDforTemp();
}

//Read commands from node host
String content = "";
char character;
char escapeChar = '!'; //Use the exclamation mark to end a command

//===================================================START OF MAIN LOOP==============================================
void loop()
{
  float currentTemp;

  //Check for Serial Data
  while(Serial.available()) {
    //Read
    character = Serial.read();

    //While we don't hit the escape character concatenate
    if(character != escapeChar) {
      content.concat(character);
    }
    else //If we get an escape character then findout what to do with the instruction!
    {
      processInstructionMessage(content);
      content = ""; //Now empty the messageholder
    }
  }


//  //Adjust desired temp if necessary
//  if (manualMode) {
//    if ((millis() - lastButtonRead) >= buttonReadUpdate) {
//      desiredTemp += getButtonState(digitalRead(downButton), digitalRead(upButton));
//      //refreshLCDforDTemp();
//      lastButtonRead = millis();
//    }
//  }
//  
//  //Get Current Temp (C. Temp)
//  currentTemp = getTemp(temperaturePin);
//
//  
//    //Update screen if it´s time
//    if ((millis() - lastTempUpdate) >= tempUpdate) {
//      //refreshLCDforCTemp(currentTemp); //LCD
//      writeToSerial(currentTemp); //Serial
//      lastTempUpdate = millis();
//    }
//
//    //Is it in the "Good" range?
//    if (inTempRange(currentTemp)) {
//      digitalWrite(goodTempRangeLED, HIGH);
//    }
//    else {
//      digitalWrite(goodTempRangeLED, LOW);
//    }

//===================================================END OF MAIN LOOP==============================================
}

float getTemp(int pin)
{
  float voltage = (analogRead(pin) * 0.004882814);
  return ((voltage - 0.5) * 100.0);
}

int getButtonState(int downButtonState, int upButtonState)
{
  if (((upButtonState == LOW) || (downButtonState == LOW))  && 
    !((downButtonState == LOW) && (upButtonState == LOW)))
    {
      if(downButtonState == LOW) {
          return (-1);
        }
        else if(upButtonState == LOW) {
            return (1);
        }
    }
    else
    {
      return (0); //No button pressed
    }
}

bool inTempRange (float cTemp)
{
  //Check temp difference
  if (abs(desiredTemp - cTemp) <= 1) {
      return true;
    }
    else {
      return false;
    }
}

void writeToSerial (float cTemp) 
{
  StaticJsonBuffer<200> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  root["ctemp"] = cTemp;
  root["time"] = now();
 
  root.printTo(Serial);
}

void refreshLCDforCTemp (float cTemp)
{
  lcd.setCursor(9,0); //Position Cursor
  lcd.print(cTemp);  //Print current temp to LCD
}

void refreshLCDforDTemp ()
{
  lcd.setCursor(9,1); //Position Cursor
  lcd.print(desiredTemp);  //Print desired temp to LCD
}

void setupLCDforTemp ()
{
  lcd.print("C. Temp:");

  lcd.setCursor(14,0);   // Position Cursor
  lcd.print(degreesCelsius); //Print ºC symbol

  lcd.setCursor(0,1);   // Position Cursor
  lcd.print("D. Temp:");

  lcd.setCursor(14,1);   // Position Cursor
  lcd.print(degreesCelsius); //Print ºC 
}

//TIME ROUTINES
String digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void processInstructionMessage(String message) {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(message.substring(0,3) == "SET") {
    //Remove set instruction and convert to long
    char tString[(message.length() -2)];
    message.substring(3,message.length()).toCharArray(tString, sizeof(tString));
    setTime(atoi(tString));
    
    for (int i = 0; i < sizeof(tString); i++) {
      Serial.print(tString[i]);
    }
    Serial.println(year());
  }
  else {
    // if nothing matches, what the f**K?
    Serial.println("Unknown Command");
  }
}

