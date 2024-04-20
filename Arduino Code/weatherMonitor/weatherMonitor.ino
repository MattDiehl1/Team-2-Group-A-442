#include <LowPower.h>



// Basic demo for reading Humidity and Temperature
#include <Wire.h>
#include <Adafruit_HTS221.h>
#include <Adafruit_Sensor.h>

// For SPI mode, we need a CS pin
#define HTS_CS 10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define HTS_SCK 13
#define HTS_MISO 12
#define HTS_MOSI 11
#define ledPin 6
#define sensorPin A0
float farenheit_temp;

Adafruit_HTS221 hts;
void setup(void) {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("CLEARSHEET");
  Serial.println("LABEL,Time,Time Elapsed, Humidity (rH),Temperature (F), Raining?");
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit HTS221 test!");

  // Try to initialize!
  //if (!hts.begin_I2C()) {
  if (!hts.begin_SPI(HTS_CS)) {
//if (!hts.begin_SPI(HTS_CS, HTS_SCK, HTS_MISO, HTS_MOSI)) {
    Serial.println("Failed to find HTS221 chip");
    while (1) { delay(10); }
  }
  Serial.println("HTS221 Found!");

//  hts.setDataRate(HTS221_RATE_1_HZ);
  Serial.print("Data rate set to: ");
  switch (hts.getDataRate()) {
   case HTS221_RATE_ONE_SHOT: Serial.println("One Shot"); break;
   case HTS221_RATE_1_HZ: Serial.println("1 Hz"); break;
   case HTS221_RATE_7_HZ: Serial.println("7 Hz"); break;
   case HTS221_RATE_12_5_HZ: Serial.println("12.5 Hz"); break;
  }

}

int readSensor() {

  int sensorValue = analogRead(sensorPin);  // Read the analog value from sensor

  int outputValue = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data

  analogWrite(ledPin, outputValue); // generate PWM signal

  return outputValue;             // Return analog rain value

} 

void loop() {

  sensors_event_t temp;
  sensors_event_t humidity;
  hts.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  farenheit_temp = 32+((float)temp.temperature * (float)9/5);
  Serial.print("DATA,TIME,");
  Serial.print("TIMER,");
  Serial.print(humidity.relative_humidity);
  Serial.print(",");
  Serial.print(farenheit_temp);
  Serial.print(",");
  if(readSensor() > 50){
  Serial.println("Drizzle");
  }
  else if(readSensor() > 500){
    Serial.println("Storm");
  }
  else if(readSensor() < 5){ 
    Serial.println("No rain");
  }
  delay(2000);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
