#Для Windowd 32bit
# CXX		  := i686-w64-mingw32-g++
#Для Windowd 64bit
CXX         := x86_64-w64-mingw32-g++
# Для Linux
# CXX		  := g++

CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib 

ifeq ($(CXX),x86_64-w64-mingw32-g++)
   DEFINE       := WIN32=WIN32
   EXECUTABLE	:= sftp_pat.exe
else ifeq ($(CXX),i686-w64-mingw32-g++)
   DEFINE       := WIN32=WIN32
   EXECUTABLE	:= sftp_pat.exe
else ifeq ($(CXX),g++)
   DEFINE       := LINUX=LINUX
   EXECUTABLE	:= sftp_patch
endif

LIBRARIES	:=  -static-libstdc++ -static-libgcc -lstdc++fs 

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I $(INCLUDE) -L $(LIB) $^ -o $@ $(LIBRARIES) -D $(DEFINE)

clean:
	-rm $(BIN)/*