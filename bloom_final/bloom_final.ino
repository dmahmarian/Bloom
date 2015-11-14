//need to fix the light circuit - can it go through the arduino safely? need two strips to work. 
//figure out voltage divider 

//humidity sensor on pin 2(power on 5v on the side); ir sensor on A1(power on 5v on the side); 
//moistureLED data on A0(power through vin and the voltage regulator, make sure there is a ground connected to the double pin on the LED);
//humidity LED input still needs to be defined(same power input as moisture LED); 


#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PINMOISTURE       11
#define PINHUMIDITY       10
#define NUMPIXELS 10
int moistureIndicator, humidityIndicator, humidtySensor;
boolean activateCircuit = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PINMOISTURE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsHumidty = Adafruit_NeoPixel(NUMPIXELS, PINHUMIDITY, NEO_GRB + NEO_KHZ800);

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
  moistureValueFunction();
  humidityValueFunction();
  irSensorFunction();
  moistureLEDFunction();
  humidtyLEDFunction();
}

//reading the moisture sensor value, mapping it to 10 lights
int moistureValueFunction() {
  int moistureSensor = analogRead(A0);
  //max moisture (dry)= 1000; lowest (fully watered) = 130;
  int moistureConstrained = constrain(moistureSensor, 130, 600);
  moistureIndicator = map(moistureConstrained, 130, 600, 10, 1);
}

//Mapping and constraining the humidty value 
void humidityValueFunction() {
  //delay(2000);
  float humiditySensor = dht.readHumidity();
  if (isnan(humiditySensor)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  int humidtyConstrained = constrain(humiditySensor, 25, 100);
  humidityIndicator = map(humidtyConstrained, 25, 100, 1, 10);
}

//creating an activate circuit feature for the ir sensor
void irSensorFunction() {
  int irSensor = analogRead(A1);
  if (irSensor < 650 && irSensor > 300) {
    activateCircuit = 1;
   //delay(500);
  }
  else {
    activateCircuit = 0;
  }
  Serial.println(activateCircuit);
}


//using the mapped moistureIndicator to light the same number of LED lights
void moistureLEDFunction() {
  if (activateCircuit !=0) {
    switch (moistureIndicator) {
      case 1:
        Serial.print("moisture: ");
        Serial.print(moistureIndicator);
        Serial.println(" turn on one light");
        pixels.setPixelColor(0, pixels.Color(0, 0, 255));
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
        Serial.print("moisture: ");
        Serial.print(moistureIndicator);
        Serial.println(" turn on two lights");
        pixels.setPixelColor(1, pixels.Color(0, 0, 255));
        pixels.setPixelColor(2, pixels.Color(0, 0, 255));
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
        Serial.print("moisture: ");
        Serial.print(moistureIndicator);
          Serial.println(" turn on three lights");
        pixels.setPixelColor(0, pixels.Color(0, 0, 255));
        pixels.setPixelColor(1, pixels.Color(0, 0, 255));
        pixels.setPixelColor(2, pixels.Color(0, 0, 255));
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
        Serial.print("moisture: ");
        Serial.print(moistureIndicator);
        Serial.println(" turn on four lights");
        pixels.setPixelColor(0, pixels.Color(0, 0, 255));
        pixels.setPixelColor(1, pixels.Color(0, 0, 255));
        pixels.setPixelColor(2, pixels.Color(0, 0, 255));
        pixels.setPixelColor(3, pixels.Color(0, 0, 255));
        pixels.setPixelColor(4, pixels.Color(0, 0, 0));
        pixels.setPixelColor(5, pixels.Color(0, 0, 0));
        pixels.setPixelColor(6, pixels.Color(0, 0, 0));
        pixels.setPixelColor(7, pixels.Color(0, 0, 0));
        pixels.setPixelColor(8, pixels.Color(0, 0, 0));
        pixels.setPixelColor(9, pixels.Color(0, 0, 0));
        pixels.show();
        break;
      case 5:
        Serial.print("moisture: ");
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
        Serial.print("moisture: ");
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
        Serial.print("moisture: ");
        Serial.print(moistureIndicator);
        Serial.println(" turn on sevel lights");
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
        Serial.print("moisture: ");
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
        Serial.print("moisture: ");
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
        Serial.print("moisture: ");
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
  }
    else {
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



void humidtyLEDFunction() {
  if (activateCircuit !=0) {
    Serial.println(humidityIndicator);
    switch (humidityIndicator) {
      case 1:
        Serial.print("humidity: ");
        Serial.print(humidityIndicator);
        Serial.println(" turn on one light");
        pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240,92,0));
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
         Serial.print("humidity: ");
        Serial.print(humidityIndicator);
        Serial.println(" turn on two lights");
        pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240,92,0));
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
      case 3:
         Serial.print("humidity: ");
        Serial.print(humidityIndicator);
        Serial.println(" turn on three lights");
        pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240,92,0));
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
         Serial.print("humidity: ");
        Serial.print(humidityIndicator);
        Serial.println(" turn on four lights");
        pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.show();
        break;
      case 5:
         Serial.print("humidity: ");
        Serial.print(humidityIndicator);
        Serial.println(" turn on five lights");
        pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.show();
        break;
      case 6:
         Serial.print("humidity: ");
        Serial.print(humidityIndicator);
        Serial.println(" turn on six lights");
        pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.show();
        break;
      case 7:
         Serial.print("humidity: ");
        Serial.print(humidityIndicator);
        Serial.println(" turn on seven lights");
        pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.show();
        break;
      case 8:
         Serial.print("humidity: ");
        Serial.print(humidityIndicator);
        Serial.println(" turn on eight lights");
        pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.show();
        break;
      case 9:
         Serial.print("humidity: ");
        Serial.print(humidityIndicator);
        Serial.println(" turn on nine lights");
        pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(0, 0, 0));
        pixelsHumidty.show();
        break;
      case 10:
         Serial.print("humidity: ");
        Serial.print(humidityIndicator);
        Serial.println(" turn on ten lights");
        pixelsHumidty.setPixelColor(0, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(1, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(2, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(3, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(4, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(5, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(6, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(7, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(8, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.setPixelColor(9, pixelsHumidty.Color(240,92,0));
        pixelsHumidty.show();
        break;
    }
  }
    else {
      Serial.println("turn off all humidity lights");
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






