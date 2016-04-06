/*
  Boiler termostat
 */
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>
#include <EEPROM.h>

#define OLED_RESET 4
#define ONE_WIRE_BUS 9

Adafruit_SSD1306 oled(OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

#define ERR_TEMP 10
#define MIN_TEMP 40
#define MAX_TEMP 70

#define BTN_SET  10
#define BTN_PLUS  11
#define BTN_MINUS  12

#define DELAY_TEMP 5000
#define DELAY_DISPLAY 250

Button btnSet = Button(BTN_SET, BUTTON_PULLUP_INTERNAL);
Button btnPlus = Button(BTN_PLUS, BUTTON_PULLUP_INTERNAL);
Button btnMinus = Button(BTN_MINUS, BUTTON_PULLUP_INTERNAL);

enum { DEFAULT_STATE, MAX_TEMP_STATE, MIN_TEMP_STATE };

byte state = DEFAULT_STATE;
byte max_temp_val = MAX_TEMP;
byte min_temp_val = MIN_TEMP;

int current_time = 0;
int temp_time = 5000;
int display_time = 250;
int tempC;

void setup() {

  max_temp_val = EEPROM.read(0);
  min_temp_val = EEPROM.read(1);

  if ((max_temp_val < MIN_TEMP) || (max_temp_val > MAX_TEMP))
  {
    max_temp_val = MAX_TEMP;
  }

  if ((min_temp_val < MIN_TEMP) || (min_temp_val > MAX_TEMP))
  {
    min_temp_val = MIN_TEMP;
  }

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
}

void loop() {

  current_time = millis();

  switch(state)
  {
    case DEFAULT_STATE:
      btnSet.scan();
      if(btnSet.stateChanged()) {
        if(btnSet.isPressed()) {
          state = MAX_TEMP_STATE;
        }
      }
      break;
    case MAX_TEMP_STATE:
      btnPlus.scan();
      if(btnPlus.stateChanged()) {
        if(btnPlus.isPressed()) {
          if(max_temp_val < MAX_TEMP) max_temp_val++;
        }
      }

      btnMinus.scan();
      if(btnMinus.stateChanged()) {
        if(btnMinus.isPressed()) {
          if(max_temp_val > MIN_TEMP) max_temp_val--;
        }
      }

      btnSet.scan();
      if(btnSet.stateChanged()) {
        if(btnSet.isPressed()) {
          EEPROM.write(0, max_temp_val);
          state = MIN_TEMP_STATE;
        }
      }
      break;
    case MIN_TEMP_STATE:

      btnPlus.scan();
      if(btnPlus.stateChanged()) {
        if(btnPlus.isPressed()) {
          if(min_temp_val < MAX_TEMP) min_temp_val++;
        }
      }

      btnMinus.scan();
      if(btnMinus.stateChanged()) {
        if(btnMinus.isPressed()) {
          if(min_temp_val > MIN_TEMP) min_temp_val--;
        }
      }

      btnSet.scan();
      if(btnSet.stateChanged()) {
        if(btnSet.isPressed()) {
          EEPROM.write(1, min_temp_val);
          state = DEFAULT_STATE;
        }
      }
      break;
  }

  if(current_time - temp_time >= DELAY_TEMP)
  {
    temp_time = current_time;
    sensors.requestTemperatures(); // Send the command to get temperatures
    tempC = sensors.getTempCByIndex(0);
  }

  if(current_time - display_time >= DELAY_DISPLAY)
  {
    display_time = current_time;

    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(1);
    oled.setCursor(0,0);
    oled.println(max_temp_val);
    oled.setCursor(25,5);
    oled.setTextSize(8);
    oled.println(tempC);
    oled.setCursor(0,55);
    oled.setTextSize(1);
    oled.println(min_temp_val);

    oled.display();
  }
}
