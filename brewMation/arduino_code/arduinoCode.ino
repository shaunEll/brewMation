/*
*/

// Load the LiquidCrystal library, which will give us
// commands to interface to the LCD:

#include <LiquidCrystal.h>
#include <ArduinoJson.h>

// Initialize the library with the pins we're using.
// (Note that you can use different pins if needed.)
// See http://arduino.cc/en/Reference/LiquidCrystal
// for more information:

LiquidCrystal lcd(12,11,5,4,3,2);

//PINS
const int ambientTemperaturePin = 0;
const int downButton = 7;
const int upButton = 6; //
const int goodTempRangeLED = 13; //Indicate when we are at the right temp

//REFRESH RATES
const int buttonReadUpdate = 200; //miliseconds between button reads
const int tempUpdate = 2000; //temperature read refresh

unsigned long lastButtonRead = buttonReadUpdate;
unsigned long lastTempUpdate = tempUpdate;

//MISC
const String degreesCelsius  = String((char)223) + "C"; // ºC symbol

bool manualMode = true; //Are we in manual or automatic mode
float desiredTemp = 40.00; //What is the idead temp?

void setup()
{
  //Setup buttons
  pinMode(downButton, INPUT);
  pinMode(upButton, INPUT);

  //Setup LEDs
  pinMode(goodTempRangeLED, OUTPUT);  
  
  //Open serial port
  Serial.begin(9600);
  Serial.println("Starting COMM");
  
  //Setup LCd & Clear
  lcd.begin(16, 2);
  lcd.clear();

  setupLCDforTemp();
}

//===================================================START OF MAIN LOOP==============================================
void loop()
{
  float currentAmbientTemp;

  //Check for Serial Data
  if(Serial.available()){ // only send data back if data has been sent
    char inByte = Serial.read(); // read the incoming data
  }

  //Adjust desired temp if necessary
  if (manualMode) {
    if ((millis() - lastButtonRead) >= buttonReadUpdate) {
      desiredTemp += getButtonState(digitalRead(downButton), digitalRead(upButton));
      refreshLCDforDTemp();
      lastButtonRead = millis();
    }
  }
  
  //Get Current Temp (C. Temp)
  currentAmbientTemp = getAmbientTemp(ambientTemperaturePin);

  
    //Update screen if it´s time
    if ((millis() - lastTempUpdate) >= tempUpdate) {
      refreshLCDforCTemp(currentAmbientTemp); //LCD
      writeToSerial(currentAmbientTemp); //Serial
      lastTempUpdate = millis();
    }

    //Is it in the "Good" range?
    if (inTempRange(currentAmbientTemp)) {
      digitalWrite(goodTempRangeLED, HIGH);
    }
    else {
      digitalWrite(goodTempRangeLED, LOW);
    }

//===================================================END OF MAIN LOOP==============================================
}

float getAmbientTemp(int pin)
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

bool inTempRange (float currentTemp)
{
  //Check temp difference
  if (abs(desiredTemp - currentTemp) <= 1) {
      return true;
    }
    else {
      return false;
    }
}

void writeToSerial (float currentTemp) 
{
  Serial.print("{ \"ctemp\" : \"");
  Serial.print(currentTemp); //Print current temp to serial
  Serial.print("\", \"dtemp\" : \"");
  Serial.print(desiredTemp); //Print desired temp to serial
  Serial.println("\"}");
}

void refreshLCDforCTemp (float currentTemp)
{
  lcd.setCursor(9,0); //Position Cursor
  lcd.print(currentTemp);  //Print current temp to LCD
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
