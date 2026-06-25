CC ?= cc
LUA_VERSION ?= 5.4
LUA_PKG ?= lua$(LUA_VERSION)
LUA_BIN ?= lua$(LUA_VERSION)

CFLAGS ?= -O2 -std=c99 -Wall -Wextra
CPPFLAGS += -I. -Istb $(shell pkg-config --cflags $(LUA_PKG) 2>/dev/null)
LDFLAGS += $(shell pkg-config --libs $(LUA_PKG) 2>/dev/null)
LIBS ?= -lm

TARGET := lua_stb.so
SOURCES := lua_stb.c

.PHONY: all build test clean

all: build

build: $(TARGET)

$(TARGET): $(SOURCES) lua_stb.h lua_stb_image.h lua_stb_image_write.h lua_stb_image_resize.h stb/stb_image.h stb/stb_image_write.h stb/stb_image_resize.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -shared -o $@ $(SOURCES) $(LDFLAGS) $(LIBS)

test: build
	$(LUA_BIN) tests/test.lua

clean:
	$(RM) $(TARGET)
