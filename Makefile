COM ?= COM4

ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

.PHONY: .upload .compile reset_bots servo_set

.upload:
	arduino-cli.exe upload $(ROOT_DIR)/$(TARGET)/ -p $(COM)

.compile:
	arduino-cli compile $(TARGET)/

reset_bots: TARGET = reset_bots
reset_bots: | .compile .upload

servo_reset_test: TARGET=servo_reset_test
servo_reset_test: | .compile .upload

servo_set: TARGET=servo_set
servo_set: | .compile .upload
