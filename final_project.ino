//need to fix the light circuit - can it go through the arduino safely? need two strips to work.
//figure out voltage divider

//humidity sensor on pin 2(power on 5v on the side); ir sensor on A1(power on 5v on the side);
//moistureLED data on A0(power through vin and the voltage regulator, make sure there is a ground connected to the double pin on the LED);
//humidity LED input still needs to be defined(same power input as moisture LED);

#include <LiquidCrystal.h>
#include <SPI.h>
#include <DHT.h>
#define DHTPIN 6
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PINMOISTURE       9
#define PINHUMIDITY       10
#define NUMPIXELS         10



LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int buttonPin = 7;     // the number of the pushbutton pin
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int buttonState = 0;  // variable for reading the pushbutton status
int menuSelector;
int pushButton = 0;
int cactusON = 0;
int cactusButton;
int orchidON = 0;
int orchidButton;
int plant3ON = 0;
int plant3Button;





int moistureIndicator, humidityIndicator, humidtySensor;
boolean activateCircuit = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PINMOISTURE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsHumidty = Adafruit_NeoPixel(NUMPIXELS, PINHUMIDITY, NEO_GRB + NEO_KHZ800);

//orchid values
int moistureSensor;
float humiditySensor;
int maxHumidity;
int minHumidity;
int maxMoisture;
int minMoisture;


void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixelsHumidty.begin();
}

void loop() {
  menu();
  orchid();
  cactus();
  plant3();
  irSensorFunction();
}


void menu () {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  sensorValue = analogRead(sensorPin);
  menuSelector = map(sensorValue, 0, 1010, 0, 3);
  // read the value from the potentiometer:
  switch (menuSelector) {
    case 0:
      lcd.clear();
      cactusON = 0;
      orchidON = 0;
      plant3ON = 0;
      lcd.begin(16, 2);
      lcd.println("Welcome!");
      if (buttonState == HIGH) {
        pushButton = 1;
      }
      delay(200);
      break;
    case 1:
      pushButton = 0;
      lcd.clear();
      cactusON = 1;
      orchidON = 0;
      plant3ON = 0;
      lcd.print("Cactus");
      delay(200);
      if (buttonState == HIGH) {
        cactusButton = 1;
        orchidButton = 0;
        plant3Button = 0;
        //         Serial.println("cactusButton");
        //         Serial.println(cactusButton);
        //         Serial.println("orchidButton");
        //         Serial.println(orchidButton);
        //         Serial.println("plant3Button");
        //         Serial.println(plant3Button);
      }
      break;
    case 2:
      lcd.clear();
      cactusON = 0;
      orchidON = 1;
      plant3ON = 0;
      lcd.print("Orchid");
      if (buttonState == HIGH) {

        cactusButton = 0;
        orchidButton = 1;
        plant3Button = 0;
        //         Serial.println("cactusButton");
        //         Serial.println(cactusButton);
        //         Serial.println("orchidButton");
        //         Serial.println(orchidButton);
        //         Serial.println("plant3Button");
        //         Serial.println(plant3Button);
      }
      delay(200);
      break;
    case 3:
      pushButton = 0;
      lcd.clear();
      cactusON = 0;
      orchidON = 0;
      plant3ON = 1;
      lcd.print("Arboricola");
      if (buttonState == HIGH) {
        cactusButton = 0;
        orchidButton = 0;
        plant3Button = 1;
        //         Serial.println("cactusButton");
        //         Serial.println(cactusButton);
        //         Serial.println("orchidButton");
        //         Serial.println(orchidButton);
        //         Serial.println("plant3Button");
        //         Serial.println(plant3Button);
      }
      delay(200);
      break;
  }
}


void cactus() {
  if (cactusON == 1 && cactusButton == 1) {
    Serial.println("cactus function is on");
    maxHumidity = 100;
    minHumidity = 50;
    maxMoisture = 500;
    minMoisture = 200;
    moistureValueFunction();
    humidityValueFunction();
  }
}


void plant3() {
  if (plant3ON == 1 && plant3Button == 1) {
    Serial.println("Arboricola function is on");
    maxHumidity = 40;
    minHumidity = 10;
    maxMoisture = 1023;
    minMoisture = 200;
    moistureValueFunction();
    humidityValueFunction();
  }
}


void orchid() {
  if (orchidON == 1 && orchidButton == 1) {
    Serial.println("orchid function is on");
    maxHumidity = 70;
    minHumidity = 20;
    maxMoisture = 3000;
    minMoisture = 500;
    moistureValueFunction();
    humidityValueFunction();
  }
}


//reading the moisture sensor value, mapping it to 10 lights
int moistureValueFunction() {
  moistureSensor = analogRead(A2);
  int moistureConstrained = constrain(moistureSensor, minMoisture, maxMoisture);
  moistureIndicator = map(moistureConstrained, minMoisture, maxMoisture, 1, 12);
  moistureLEDFunction();
}


//Mapping and constraining the humidty value
void humidityValueFunction() {
  humiditySensor = dht.readHumidity();
  if (isnan(humiditySensor)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  int humidtyConstrained = constrain(humiditySensor, minHumidity, maxHumidity);
  humidityIndicator = map(humidtyConstrained, minHumidity, maxHumidity, 1, 12);
  humidtyLEDFunction();
}


//creating an activate circuit feature for the ir sensor
void irSensorFunction() {
  int irSensor = analogRead(A1);
  if (irSensor < 650 && irSensor > 300) {
    activateCircuit = 1;
  }
  else {
    activateCircuit = 0;
  }
  Serial.println(activateCircuit);
}



//using the mapped moistureIndicator to light the same number of LED lights
void moistureLEDFunction() {
  if (activateCircuit != 0) {
    Serial.print("moisture sensor reading: ");
    Serial.println(moistureSensor);
    if (moistureSensor > maxMoisture) { //overwatered case
      //light all neopixels red
      Serial.print("OVERWATERED!!! moisture: ");
      Serial.print(moistureIndicator);
      Serial.println(" turn on ten lights");
      //lcd.print("overwatered!");
      // lcd.clear();
      pixels.setPixelColor(0, pixels.Color(255, 0, 0));
      pixels.setPixelColor(1, pixels.Color(255, 0, 0));
      pixels.setPixelColor(2, pixels.Color(255, 0, 0));
      pixels.setPixelColor(3, pixels.Color(255, 0, 0));
      pixels.setPixelColor(4, pixels.Color(255, 0, 0));
      pixels.setPixelColor(5, pixels.Color(255, 0, 0));
      pixels.setPixelColor(6, pixels.Color(255, 0, 0));
      pixels.setPixelColor(7, pixels.Color(255, 0, 0));
      pixels.setPixelColor(8, pixels.Color(255, 0, 0));
      pixels.setPixelColor(9, pixels.Color(255, 0, 0));
      pixels.show();
      //LCD say 'overwatered'
    }
    else if (moistureIndicator <= 4) { //underwatered
      //lcd.print("underwatered!");
      //lcd.clear();
      switch (moistureIndicator) { //light up the number of pixels in red
        case 1:
          Serial.print("UNDERWATERED! moisture: ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on one light");
          //lcd.print("underwatered!");
          // lcd.clear();
          pixels.setPixelColor(0, pixels.Color(255, 0, 0));
          pixels.setPixelColor(1, pixels.Color(0, 0, 0));
          pixels.setPixelColor(2, pixels.Color(0, 0, 0));
          pixels.setPixelColor(3, pixels.Color(0, 0, 0));
          pixels.setPixelColor(4, pixels.Color(0, 0, 0));
          pixels.setPixelColor(5, pixels.Color(0, 0, 0));
          pixels.setPixelColor(6, pixels.Color(0, 0, 0));
          pixels.setPixelColor(7, pixels.Color(0, 0, 0));
          pixels.setPixelColor(8, pixels.Color(0, 0, 0));
          pixels.setPixelColor(9, pixels.Color(0, 0, 0));
          pixels.show();
          break;
        case 2:
          Serial.print("UNDERWATERED! moisture: ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on two lights");
          pixels.setPixelColor(1, pixels.Color(255, 0, 0));
          pixels.setPixelColor(2, pixels.Color(255, 0, 0));
          pixels.setPixelColor(3, pixels.Color(0, 0, 0));
          pixels.setPixelColor(4, pixels.Color(0, 0, 0));
          pixels.setPixelColor(5, pixels.Color(0, 0, 0));
          pixels.setPixelColor(6, pixels.Color(0, 0, 0));
          pixels.setPixelColor(7, pixels.Color(0, 0, 0));
          pixels.setPixelColor(8, pixels.Color(0, 0, 0));
          pixels.setPixelColor(9, pixels.Color(0, 0, 0));
          pixels.setPixelColor(10, pixels.Color(0, 0, 0));
          pixels.show();
          break;
        case 3:
          Serial.print("UNDERWATERED! moisture: ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on three lights");
          pixels.setPixelColor(0, pixels.Color(255, 0, 0));
          pixels.setPixelColor(1, pixels.Color(255, 0, 0));
          pixels.setPixelColor(2, pixels.Color(255, 0, 0));
          pixels.setPixelColor(3, pixels.Color(0, 0, 0));
          pixels.setPixelColor(4, pixels.Color(0, 0, 0));
          pixels.setPixelColor(5, pixels.Color(0, 0, 0));
          pixels.setPixelColor(6, pixels.Color(0, 0, 0));
          pixels.setPixelColor(7, pixels.Color(0, 0, 0));
          pixels.setPixelColor(8, pixels.Color(0, 0, 0));
          pixels.setPixelColor(9, pixels.Color(0, 0, 0));
          pixels.show();
          break;
        case 4:
          Serial.print("UNDERWATERED! moisture: ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on four lights");
          pixels.setPixelColor(0, pixels.Color(255, 0, 0));
          pixels.setPixelColor(1, pixels.Color(255, 0, 0));
          pixels.setPixelColor(2, pixels.Color(255, 0, 0));
          pixels.setPixelColor(3, pixels.Color(255, 0, 0));
          pixels.setPixelColor(4, pixels.Color(0, 0, 0));
          pixels.setPixelColor(5, pixels.Color(0, 0, 0));
          pixels.setPixelColor(6, pixels.Color(0, 0, 0));
          pixels.setPixelColor(7, pixels.Color(0, 0, 0));
          pixels.setPixelColor(8, pixels.Color(0, 0, 0));
          pixels.setPixelColor(9, pixels.Color(0, 0, 0));
          pixels.show();
          break;
      }
      //LCD says underwatered
    }
    else if (moistureIndicator > 4 && moistureIndicator <= 12) { //moisture is fine

      //lcd.clear();
      switch (moistureIndicator) { //light up the number of lights in blue
        case 5:
          Serial.print("LEVEL IS FINE moisture: ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on five lights");
          pixels.setPixelColor(0, pixels.Color(0, 0, 255));
          pixels.setPixelColor(1, pixels.Color(0, 0, 255));
          pixels.setPixelColor(2, pixels.Color(0, 0, 255));
          pixels.setPixelColor(3, pixels.Color(0, 0, 255));
          pixels.setPixelColor(4, pixels.Color(0, 0, 255));
          pixels.setPixelColor(5, pixels.Color(0, 0, 0));
          pixels.setPixelColor(6, pixels.Color(0, 0, 0));
          pixels.setPixelColor(7, pixels.Color(0, 0, 0));
          pixels.setPixelColor(8, pixels.Color(0, 0, 0));
          pixels.setPixelColor(9, pixels.Color(0, 0, 0));
          pixels.show();
          break;
        case 6:
          Serial.print("LEVEL IS FINE :) moisture: ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on six lights");
          pixels.setPixelColor(0, pixels.Color(0, 0, 255));
          pixels.setPixelColor(1, pixels.Color(0, 0, 255));
          pixels.setPixelColor(2, pixels.Color(0, 0, 255));
          pixels.setPixelColor(3, pixels.Color(0, 0, 255));
          pixels.setPixelColor(4, pixels.Color(0, 0, 255));
          pixels.setPixelColor(5, pixels.Color(0, 0, 255));
          pixels.setPixelColor(6, pixels.Color(0, 0, 0));
          pixels.setPixelColor(7, pixels.Color(0, 0, 0));
          pixels.setPixelColor(8, pixels.Color(0, 0, 0));
          pixels.setPixelColor(9, pixels.Color(0, 0, 0));
          pixels.show();
          break;
        case 7:
          Serial.print("LEVEL IS FINE :) moisture: ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on seven lights");
          pixels.setPixelColor(0, pixels.Color(0, 0, 255));
          pixels.setPixelColor(1, pixels.Color(0, 0, 255));
          pixels.setPixelColor(2, pixels.Color(0, 0, 255));
          pixels.setPixelColor(3, pixels.Color(0, 0, 255));
          pixels.setPixelColor(4, pixels.Color(0, 0, 255));
          pixels.setPixelColor(5, pixels.Color(0, 0, 255));
          pixels.setPixelColor(6, pixels.Color(0, 0, 255));
          pixels.setPixelColor(7, pixels.Color(0, 0, 0));
          pixels.setPixelColor(8, pixels.Color(0, 0, 0));
          pixels.setPixelColor(9, pixels.Color(0, 0, 0));
          pixels.show();
          break;
        case 8:
          Serial.print("moisture: LEVEL IS FINE :) ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on eight lights");
          pixels.setPixelColor(0, pixels.Color(0, 0, 255));
          pixels.setPixelColor(1, pixels.Color(0, 0, 255));
          pixels.setPixelColor(2, pixels.Color(0, 0, 255));
          pixels.setPixelColor(3, pixels.Color(0, 0, 255));
          pixels.setPixelColor(4, pixels.Color(0, 0, 255));
          pixels.setPixelColor(5, pixels.Color(0, 0, 255));
          pixels.setPixelColor(6, pixels.Color(0, 0, 255));
          pixels.setPixelColor(7, pixels.Color(0, 0, 255));
          pixels.setPixelColor(8, pixels.Color(0, 0, 0));
          pixels.setPixelColor(9, pixels.Color(0, 0, 0));
          pixels.show();
          break;
        case 9:
          Serial.print("LEVEL IS FINE :) moisture: ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on nine lights");
          pixels.setPixelColor(0, pixels.Color(0, 0, 255));
          pixels.setPixelColor(1, pixels.Color(0, 0, 255));
          pixels.setPixelColor(2, pixels.Color(0, 0, 255));
          pixels.setPixelColor(3, pixels.Color(0, 0, 255));
          pixels.setPixelColor(4, pixels.Color(0, 0, 255));
          pixels.setPixelColor(5, pixels.Color(0, 0, 255));
          pixels.setPixelColor(6, pixels.Color(0, 0, 255));
          pixels.setPixelColor(7, pixels.Color(0, 0, 255));
          pixels.setPixelColor(8, pixels.Color(0, 0, 255));
          pixels.setPixelColor(9, pixels.Color(0, 0, 0));
          pixels.show();
          break;
        case 10:
          Serial.print("LEVEL IS FINE :) moisture: ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on ten lights");
          pixels.setPixelColor(0, pixels.Color(0, 0, 255));
          pixels.setPixelColor(1, pixels.Color(0, 0, 255));
          pixels.setPixelColor(2, pixels.Color(0, 0, 255));
          pixels.setPixelColor(3, pixels.Color(0, 0, 255));
          pixels.setPixelColor(4, pixels.Color(0, 0, 255));
          pixels.setPixelColor(5, pixels.Color(0, 0, 255));
          pixels.setPixelColor(6, pixels.Color(0, 0, 255));
          pixels.setPixelColor(7, pixels.Color(0, 0, 255));
          pixels.setPixelColor(8, pixels.Color(0, 0, 255));
          pixels.setPixelColor(9, pixels.Color(0, 0, 255));
          pixels.show();
          break;
        case 11:
          Serial.print("LEVEL IS FINE :) moisture: ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on ten lights");
          pixels.setPixelColor(0, pixels.Color(0, 0, 255));
          pixels.setPixelColor(1, pixels.Color(0, 0, 255));
          pixels.setPixelColor(2, pixels.Color(0, 0, 255));
          pixels.setPixelColor(3, pixels.Color(0, 0, 255));
          pixels.setPixelColor(4, pixels.Color(0, 0, 255));
          pixels.setPixelColor(5, pixels.Color(0, 0, 255));
          pixels.setPixelColor(6, pixels.Color(0, 0, 255));
          pixels.setPixelColor(7, pixels.Color(0, 0, 255));
          pixels.setPixelColor(8, pixels.Color(0, 0, 255));
          pixels.setPixelColor(9, pixels.Color(0, 0, 255));
          pixels.show();
          break;
        case 12:
          Serial.print("LEVEL IS FINE :) moisture: ");
          Serial.print(moistureIndicator);
          Serial.println(" turn on ten lights");
          pixels.setPixelColor(0, pixels.Color(0, 0, 255));
          pixels.setPixelColor(1, pixels.Color(0, 0, 255));
          pixels.setPixelColor(2, pixels.Color(0, 0, 255));
          pixels.setPixelColor(3, pixels.Color(0, 0, 255));
          pixels.setPixelColor(4, pixels.Color(0, 0, 255));
          pixels.setPixelColor(5, pixels.Color(0, 0, 255));
          pixels.setPixelColor(6, pixels.Color(0, 0, 255));
          pixels.setPixelColor(7, pixels.Color(0, 0, 255));
          pixels.setPixelColor(8, pixels.Color(0, 0, 255));
          pixels.setPixelColor(9, pixels.Color(0, 0, 255));
          pixels.show();
          break;
      }
      //LCD says everything is fine
    }
  }
  else {//otherwise keep everything off
    Serial.println("turn off all moisture lights");
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.setPixelColor(4, pixels.Color(0, 0, 0));
    pixels.setPixelColor(5, pixels.Color(0, 0, 0));
    pixels.setPixelColor(6, pixels.Color(0, 0, 0));
    pixels.setPixelColor(7, pixels.Color(0, 0, 0));
    pixels.setPixelColor(8, pixels.Color(0, 0, 0));
    pixels.setPixelColor(9, pixels.Color(0, 0, 0));
    pixels.show();
  }
}
//HUMIDITY--------------------------------------------------------------------------------------------------------------------
void humidtyLEDFunction() {
  if (activateCircuit != 0) {
    //Serial.print("humidity sensor reading: ");
    //Serial.println(humiditySensor);
    if (humiditySensor > maxHumidity) { //too humid case
      //light all neopixels red
      Serial.print("TOO HUMID!!! humidity: ");
      Serial.print(humidityIndicator);
      Serial.println(" turn on ten lights");
      // lcd.print("too humid!");
      //  lcd.clear();
      pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(255, 0, 0));
      pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(255, 0, 0));
      pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(255, 0, 0));
      pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(255, 0, 0));
      pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(255, 0, 0));
      pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(255, 0, 0));
      pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(255, 0, 0));
      pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(255, 0, 0));
      pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(255, 0, 0));
      pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(255, 0, 0));
      pixelsHumidty.show();
      //LCD say 'too much humidity'
    }
    else if (humidityIndicator <= 4) { //low humidity case
      //lcd.print("humidity low!");
      // lcd.clear();
      switch (humidityIndicator) { //light up the number of pixels in red
        case 1:
          Serial.print("HUMIDITY TOO LOW! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on one light");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(255, 0, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));

          pixelsHumidty.show();
          break;
        case 2:
          Serial.print("HUMIDITY TOO LOW! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on two lights");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(255, 0, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(255, 0, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(10, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.show();
          break;
        case 3:
          Serial.print("HUMIDITY TOO LOW! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on three lights");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(255, 0, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(255, 0, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(255, 0, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.show();
          break;
        case 4:
          Serial.print("HUMIDITY TOO LOW! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on four lights");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(255, 0, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(255, 0, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(255, 0, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(255, 0, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.show();
          break;
      }
      //LCD says low humidity

    }
    else if (humidityIndicator > 4 && humidityIndicator <= 12) { //moisture is fine

      //  lcd.clear();
      switch (humidityIndicator) { //light up the number of lights in yellow
        case 5:
          Serial.print("HUMIDITY IS FINE! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on five lights");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.show();
          break;
        case 6:
          Serial.print("HUMIDITY IS FINE! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on six lights");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.show();
          break;
        case 7:
          Serial.print("HUMIDITY IS FINE! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on seven lights");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.show();
          break;
        case 8:
          Serial.print("HUMIDITY IS FINE! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on eight lights");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.show();
          break;
        case 9:
          Serial.print("HUMIDITY IS FINE! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on nine lights");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
          pixelsHumidty.show();
          break;
        case 10:
          Serial.print("HUMIDITY IS FINE! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on ten lights");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.show();
          break;
        case 11:
          Serial.print("HUMIDITY IS FINE! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on ten lights");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.show();
          break;
        case 12:
          Serial.print("HUMIDITY IS FINE! humidity: ");
          Serial.print(humidityIndicator);
          Serial.println(" turn on ten lights");
          pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(240, 92, 0));
          pixelsHumidty.show();
          break;
      }
      //LCD says everything is fine
    }
  }
  else {//otherwise keep everything off
    Serial.println("turn off all HUMIDITY lights");
    pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(0, 0, 0));
    pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(0, 0, 0));
    pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(0, 0, 0));
    pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(0, 0, 0));
    pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(0, 0, 0));
    pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(0, 0, 0));
    pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(0, 0, 0));
    pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
    pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
    pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
    pixelsHumidty.show();
  }
}



