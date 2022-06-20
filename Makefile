# CXX		  := i686-w64-mingw32-g++
# CXX         := x86_64-w64-mingw32-g++
CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib 

# DEFINE  := WIN32=WIN32
DEFINE  := LINUX=LINUX

LIBRARIES	:=  -lstdc++fs 

# EXECUTABLE	:= sftp_pat.exe
EXECUTABLE	:= sftp_pat

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I $(INCLUDE) -L $(LIB) $^ -o $@ $(LIBRARIES) -D $(DEFINE)

clean:
	-rm $(BIN)/*
