CXX      := clang++
CXXFLAGS := -std=c++17
CXXFLAGS += -DDEBUG -g -fsanitize=address
LDFLAGS  := -L/usr/lib -lstdc++ -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objs
APP_DIR  := $(BUILD)/apps
PROG_DIR := $(BUILD)/progs
TARGET   := xv_fuzzer
INCLUDE  := -Iinc/
SRC      := src
SRCFS    := $(wildcard src/*.cpp)
MOD		 := $(wildcard src/mod/*.cpp)
PROGS    := xv-clang xy-fuzz

# Here pattern may contain a ‘%’ which acts as a wildcard, matching any number of any characters within a word.
# The substitution reference $(text:pattern=replacement) is a shorthand for this.

OBJECTS  := $(MOD:%.cpp=$(OBJ_DIR)/%.o) # replace all cpp file in $(MOD) with obj file in $(OBJ_DIR)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: build xv-fuzz xv-clang

$(OBJ_DIR)/%.o: %.cpp
	@echo compiling $@
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info xv-fuzz xy-clang

# $(OBJ_DIR)/xv-fuzz.o: $(SRCD)/xv-fuzz
# 	@echo "do xv-fuzz.o compile"
# 	$(CXX) $(CFLAGS) $(INCLUDE) $(OBJ_DIR)/$@.cpp -o $@

xv-fuzz: build $(SRC)/xv-fuzz.cpp $(OBJECTS)
	@echo linking $@
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(SRC)/xv-fuzz.cpp -o $(PROG_DIR)/xv-fuzz.o
	@$(CXX) $(CXXFLAGS) $(PROG_DIR)/xv-fuzz.o $(OBJECTS) -o $(APP_DIR)/$@ $(LDFLAGS)

xv-clang: build $(SRC)/xv-clang.cpp $(OBJECTS)
	@echo linking $@
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(SRC)/xv-clang.cpp -o $(PROG_DIR)/xv-clang.o
	@$(CXX) $(CXXFLAGS) $(PROG_DIR)/xv-clang.o $(OBJECTS) -o $(APP_DIR)/$@ $(LDFLAGS)

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(PROG_DIR)

debug: CXXFLAGS += -DDEBUG -g -fsanitize=address
# debug: all

release: CXXFLAGS += -O2
# release: all

clean:
	-@rm -rf $(OBJ_DIR)/*
	-@rm -rf $(PROG_DIR)/*
	-@rm -rf $(APP_DIR)/*

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Programs dir: 	${PROG_DIR}    "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"