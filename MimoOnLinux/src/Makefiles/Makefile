# Architecture detection
ARCH := $(shell cat ../configs/arch_config)

ifeq ($(ARCH),ARM)
	CC     = arm-linux-gnueabi-gcc
	CXX    = arm-linux-gnueabi-g++
	CFLAGS = -march=armv7-a
	GOARCH = arm
endif

ifeq ($(ARCH),x86)
	CC     = gcc
	CXX    = g++
	CFLAGS = -m32
	GOARCH = 386
endif

ifeq ($(ARCH),PowerPC)
	CC     = powerpc-linux-gnu-gcc
	CXX    = powerpc-linux-gnu-g++
	CFLAGS = -mcpu=powerpc
	GOARCH = ppc64
endif

# Paths
C_SRC        = ../C/Main.c
CPP_SRC      = ../C/libroot/src/C++/main.cpp
HEADER_DIR   = ../C/libroot/src/C++/headers
SERVICES_DIR = ../C/Services
GO_SRC       = ../C/Apps/MimoGet.go
GO_BIN       = ../C/Apps/MimoGet
OUT          = ../C/MimoImage

# Objects
C_OBJ   = Main.o
CPP_OBJ = libroot.o

# Services
SERVICE_CSRC    = $(wildcard $(SERVICES_DIR)/*.c)
SERVICE_CPPSRC  = $(wildcard $(SERVICES_DIR)/*.cpp)
SERVICE_CBIN    = $(patsubst $(SERVICES_DIR)/%.c,$(SERVICES_DIR)/%,$(SERVICE_CSRC))
SERVICE_CPPBIN  = $(patsubst $(SERVICES_DIR)/%.cpp,$(SERVICES_DIR)/%,$(SERVICE_CPPSRC))

# Targets
all: $(OUT) services mimoget

$(OUT): $(C_OBJ) $(CPP_OBJ)
	$(CXX) $(CFLAGS) $(C_OBJ) $(CPP_OBJ) -o $(OUT)

$(C_OBJ): $(C_SRC)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $(C_SRC) -o $(C_OBJ)

$(CPP_OBJ): $(CPP_SRC)
	$(CXX) $(CFLAGS) -I$(HEADER_DIR) -c $(CPP_SRC) -o $(CPP_OBJ)

services: $(SERVICE_CBIN) $(SERVICE_CPPBIN)

$(SERVICES_DIR)/%: $(SERVICES_DIR)/%.c
	$(CC) $(CFLAGS) -I$(HEADER_DIR) $< -o $@
	chmod +x $@

$(SERVICES_DIR)/%: $(SERVICES_DIR)/%.cpp $(CPP_OBJ)
	$(CXX) $(CFLAGS) -I$(HEADER_DIR) $< $(CPP_OBJ) -o $@
	chmod +x $@

mimoget: $(GO_SRC)
	GOOS=linux GOARCH=$(GOARCH) go build -o $(GO_BIN) $(GO_SRC)
	chmod +x $(GO_BIN)

clean:
	rm -f $(C_OBJ) $(CPP_OBJ) $(OUT)
	rm -f $(SERVICE_CBIN) $(SERVICE_CPPBIN)
	rm -f $(GO_BIN)
