BOARD_TAG = uno
MONITOR_PORT  = /dev/ttyACM0
ARDUINO_LIBS = Wire Adafruit_NeoPixel dcf77
ARDUINO_DIR = /home/fhuizing/Workspace/Arduino/arduino-1.7.3-linux64
ARCHITECTURE = avr
BOARD_SUB = atmega328p
CXXFLAGS_STD = -std=c++11 -Wall -Wextra -DFIXEDPT_WBITS=10

include /home/fhuizing/Workspace/Arduino/Arduino-Makefile/Arduino.mk
