#############################################################################
##                                                                         ##
##                    Copyright (C) 2019-2019 Julian Uy                    ##
##                  https://sites.google.com/site/awertyb                  ##
##                                                                         ##
##   This program is free software; you can redistribute it and/or modify  ##
##   it under the terms of the GNU General Public License as published by  ##
##    the Free Software Foundation; either version 2 of the License, or    ##
##                   (at your option) any later version.                   ##
##                                                                         ##
##     This program is distributed in the hope that it will be useful,     ##
##     but WITHOUT ANY WARRANTY; without even the implied warranty of      ##
##      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      ##
##               GNU General Public License for more details.              ##
##                                                                         ##
## You should have received a copy of the GNU General Public License along ##
## with this program; if not, write to the Free Software Foundation, Inc., ##
##       51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.       ##
##                                                                         ##
#############################################################################

CC = i686-w64-mingw32-gcc
CXX = i686-w64-mingw32-g++
WINDRES := i686-w64-mingw32-windres
GIT_TAG := $(shell git describe --abbrev=0 --tags)
ALLSRCFLAGS += -DGIT_TAG=\"$(GIT_TAG)\" -DWINVER=0x0501 -D_WIN32_WINNT=0x0501
CFLAGS += -O2 -flto
CFLAGS += $(ALLSRCFLAGS) -Wall -Wno-unused-value -Wno-format -I. -I.. -I../ncbind -DNDEBUG -DWIN32 -D_WIN32 -D_WINDOWS 
CFLAGS += -D_USRDLL -DMINGW_HAS_SECURE_API -DUNICODE -D_UNICODE -DNO_STRICT -fpermissive
WINDRESFLAGS += $(ALLSRCFLAGS) --codepage=65001
LDFLAGS += -static -static-libstdc++ -static-libgcc -shared -Wl,--kill-at
LDLIBS += -lshlwapi -luuid -lstrmiids -lole32

%.o: %.c
	@printf '\t%s %s\n' CC $<
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	@printf '\t%s %s\n' CXX $<
	$(CXX) -c $(CFLAGS) -o $@ $<

%.o: %.rc
	@printf '\t%s %s\n' WINDRES $<
	$(WINDRES) $(WINDRESFLAGS) $< $@

SOURCES := ../tp_stub.cpp ../ncbind/ncbind.cpp main.cpp krdslavf.rc
OBJECTS := $(SOURCES:.c=.o)
OBJECTS := $(OBJECTS:.cpp=.o)
OBJECTS := $(OBJECTS:.rc=.o)

BINARY ?= krdslavf.dll
ARCHIVE ?= krdslavf.$(GIT_TAG).7z

all: $(BINARY)

archive: $(ARCHIVE)

clean:
	rm -f $(OBJECTS) $(BINARY) $(ARCHIVE)

$(ARCHIVE): $(BINARY) 
	rm -f $(ARCHIVE)
	7z a $@ $^

$(BINARY): $(OBJECTS) 
	@printf '\t%s %s\n' LNK $@
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
