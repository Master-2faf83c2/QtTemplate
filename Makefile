PROJECT_NAME = app
ROOT_DIR = $(CURDIR)
BUILD_DIR = $(ROOT_DIR)/build
INSTALL_DIR = $(BUILD_DIR)/install
SCRIPTS_PYTHON = $(ROOT_DIR)/scripts/python
PYTHON = python
VERSION = "v0.0.1"

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    OS_NAME := LinuxMac
else ifeq ($(UNAME_S),Darwin)
    OS_NAME := LinuxMac
else ifeq ($(OS),Windows_NT)
    OS_NAME := Windows
	SHELL := cmd.exe
	PYTHON = python
else
    OS_NAME := LinuxMac
endif

ifeq ($(OS_NAME), Windows)
RUN_CMD = cd $(BUILD_DIR) && chcp 65001 && $(PROJECT_NAME).exe
else
RUN_CMD = cd $(BUILD_DIR) && ./$(PROJECT_NAME)
endif

all: debug

app:
	$(PYTHON) $(SCRIPTS_PYTHON)/$(OS_NAME).py $(ROOT_DIR) $(PROJECT_NAME) $(BUILD_DIR) Release NO_INSTALL $(VERSION)

debug:
	$(PYTHON) $(SCRIPTS_PYTHON)/$(OS_NAME).py $(ROOT_DIR) $(PROJECT_NAME) $(BUILD_DIR) Debug NO_INSTALL $(VERSION)

app_install:
	$(PYTHON) $(SCRIPTS_PYTHON)/$(OS_NAME).py $(ROOT_DIR) $(PROJECT_NAME) $(BUILD_DIR) Release INSTALL $(VERSION)

debug_install:
	$(PYTHON) $(SCRIPTS_PYTHON)/$(OS_NAME).py $(ROOT_DIR) $(PROJECT_NAME) $(BUILD_DIR) Debug INSTALL $(VERSION)

run:
	$(RUN_CMD)
