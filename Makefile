UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    OS_NAME = Linux
endif
ifeq ($(UNAME_S),Darwin)
    OS_NAME = Mac
endif
ifeq ($(OS),Windows_NT)
    OS_NAME = Windows
endif

all:
	@echo "Current OS is $(OS_NAME)"
