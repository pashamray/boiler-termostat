/*
  Boiler termostat
 */
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
#define ONE_WIRE_BUS 9

Adafruit_SSD1306 oled(OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
DeviceAddress boilerThermometer;

#define ERR_TEMP 10
#define MIN_TEMP 60
#define MAX_TEMP 70

// function that will be called when an alarm condition exists during DallasTemperatures::processAlarms();
void newAlarmHandler(const uint8_t* deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  oled.print("A: ");
  oled.print(tempC);
  oled.display(); 
}


void setup() {

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  oled.display();
  delay(2000);

  // Clear the buffer.
  oled.clearDisplay();

  // Start up the library
  sensors.begin();
    
  // search for devices on the bus and assign based on an index
  if (!sensors.getAddress(boilerThermometer, 0)) {
    oled.println("Unable to find address for Device 0"); 
    oled.display();
  }

  sensors.setHighAlarmTemp(boilerThermometer, MIN_TEMP);
  sensors.setLowAlarmTemp(boilerThermometer, MAX_TEMP);

  // attach alarm handler
  sensors.setAlarmHandler(&newAlarmHandler);
}

void loop() {
  
  sensors.requestTemperatures(); // Send the command to get temperatures
 
  float tempC = sensors.getTempCByIndex(0);
  
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.setCursor(10,0);
  oled.clearDisplay();
  oled.println(tempC);
  oled.display();
 
  // call alarm handler function defined by sensors.setAlarmHandler
  // for each device reporting an alarm
  sensors.processAlarms();

  delay(1000);
}

