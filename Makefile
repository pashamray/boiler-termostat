PROJECT_DIR   = .
ARDMK_DIR = $(PROJECT_DIR)/Arduino-Makefile
ARDUINO_DIR = c:/Arduino
USER_LIB_PATH := $(PROJECT_DIR)/lib
ARDUINO_LIBS = Adafruit-GFX-Library Adafruit_SSD1306 Wire OneWire SPI Arduino-Temperature-Control-Library Button EEPROM

BOARD_TAG = uno

include $(ARDMK_DIR)/Arduino.mk
