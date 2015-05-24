BOARD_TAG = uno
MONITOR_PORT  = /dev/ttyACM0
ARDUINO_LIBS = Wire Adafruit_NeoPixel Time DCF77
ARDUINO_DIR = /home/fhuizing/Workspace/Arduino/arduino-1.6.4
ARCHITECTURE = avr
BOARD_SUB = atmega328p
CXXFLAGS_STD = -std=c++11 -DFIXEDPT_WBITS=16

include /home/fhuizing/Workspace/Arduino/Arduino-Makefile/Arduino.mk
