BOARD_TAG = uno
MONITOR_PORT  = /dev/ttyACM0
ARDUINO_LIBS = Wire Adafruit_NeoPixel RTClib SPI Time Timezone
ARDUINO_DIR = /home/fhuizing/Workspace/Arduino/arduino-1.5.8
ARCHITECTURE = avr
BOARD_SUB = atmega328p
CXXFLAGS_STD = -std=c++11 -D__UTC__=$(shell date +%s)

include /home/fhuizing/Workspace/Arduino/Arduino-Makefile/Arduino.mk
