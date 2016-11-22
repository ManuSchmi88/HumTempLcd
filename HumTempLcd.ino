#include <DS3231.h>
#include <LiquidCrystal.h>
#include <SimpleDHT.h>

// for DHT11, Define Pins:
//      VCC: 5V
//      GND: GND
//      DATA: 7

// For LCD, define Pins:
//      VSS: 5V
//      VDD: GND
//      V0 : 10K Poti goes to +5V and GND
//      RS : 12
//      E  : 11
//      D4 : 5
//      D5 : 4
//      D6 : 3
//      D7 : 2
//      A  : +5V
//      K  : GND


//Initialize DHT11 Temp and Moisture Sensor
int pinDHT11 = 7;
SimpleDHT11 dht11;
int ledPin   = 10;

//Initialize LCD Display at the Pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//RTC
DS3231 rtc(SDA,SCL);

void setup() {
  Serial.begin(9600);

  //Initialize the RTC
  rtc.begin();
  //rtc.setDOW(WEDNESDAY);
  //rtc.setTime(17,37,0);
  //rtc.setDate(22,11,2016);
  
  //Set the internal LED to LOW to save Energy
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  //Blink red LED Shortly to see its working
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,HIGH);
  delay(1000);
  digitalWrite(ledPin,LOW);

  //Set up LCD Number of columns and rows
  lcd.begin(16,2);
  //Print initial Message to the LCD
  lcd.print("Hello Handsome!");
  delay(2000);
  lcd.clear();
  
  }

void loop() {

  // Set up variables containing temperature and humidity.
  byte temperature = 0;
  byte humidity = 0;
  // Read in variables from DHT11
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) {
    Serial.print("Woops something went wrong manu.");
    return;
  }
  // Light up a red light if humidity goes over 55%
  if (humidity > 55) {
    digitalWrite(ledPin,HIGH);
  } else {
    digitalWrite(ledPin,LOW);
  }
  // Display Values on LCD
  lcd.setCursor(0,0);
  lcd.print("Room Climate");
  lcd.setCursor(0,1);
  lcd.print((int)temperature);lcd.print("*C   ");
  lcd.print((int)humidity); lcd.print("%");
  delay(4000);
  lcd.clear();
  
  //Display Date and Time
  lcd.setCursor(0,0);
  lcd.print(rtc.getDateStr());
  lcd.setCursor(0,1);
  lcd.print(rtc.getTimeStr());
  delay(2000);
  lcd.clear();
  
  //Do the serial printing, mostly for buxfixing.
  Serial.print("Time: ");Serial.print(rtc.getTimeStr());Serial.print("  ");
  Serial.print(rtc.getDateStr());
  Serial.print("Sample is OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" %");
}
