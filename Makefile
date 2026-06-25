CC ?= cc
LUA_VERSION ?= 5.4
LUA_PKG ?= lua$(LUA_VERSION)
LUA_BIN ?= lua$(LUA_VERSION)
PKG_CONFIG ?= pkg-config

CFLAGS ?= -O2 -std=c99 -Wall -Wextra
CPPFLAGS += -I. -Istb $(shell $(PKG_CONFIG) --cflags $(LUA_PKG) 2>/dev/null)
LDFLAGS += $(shell $(PKG_CONFIG) --libs $(LUA_PKG) 2>/dev/null)
LIBS ?= -lm

UNAME_S := $(shell uname -s 2>/dev/null)
ifeq ($(OS),Windows_NT)
TARGET_EXT ?= dll
SHARED_FLAGS ?= -shared
else ifneq (,$(findstring Darwin,$(UNAME_S)))
TARGET_EXT ?= so
SHARED_FLAGS ?= -bundle -undefined dynamic_lookup
else
TARGET_EXT ?= so
SHARED_FLAGS ?= -shared -fPIC
endif

TARGET := lua_stb.$(TARGET_EXT)
SOURCES := lua_stb.c

.PHONY: all build test clean

all: build

build: $(TARGET)

$(TARGET): $(SOURCES) lua_stb.h lua_stb_image.h lua_stb_image_write.h lua_stb_image_resize.h stb/stb_image.h stb/stb_image_write.h stb/stb_image_resize.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SHARED_FLAGS) -o $@ $(SOURCES) $(LDFLAGS) $(LIBS)

test: build
	$(LUA_BIN) tests/test.lua

clean:
	$(RM) $(TARGET)
