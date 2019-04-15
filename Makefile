# for ubuntu
ifneq ("$(wildcard $($HOME/arduino))","")
ARDUINO_DIR="$(wildcard $($HOME/arduino))"
else ifneq ("$(wildcard $(/Applications/arduino))","")
ARDUINO_DIR="$(wildcard $(/Applications/arduino))"
endif


ARDMK_DIR=${HOME}/elektronik/Arduino-Makefile
USER_LIB_PATH := $(realpath ../../libraries)

BOARD_TAG    = mega
BOARD_SUB    = atmega2560
ARDUINO_LIBS = FastLED

USER_DEFINES += -DDEBUG_UART_ON

all:
	@echo "${USER_INCLUDES}"

include ${ARDMK_DIR}/Arduino.mk


