.PHONY : build clean 

default :
	@echo "======================================="
	@echo "Please use 'make dependence' command to clone the dependence "stb" by git.."
	@echo "Please use 'make build' command to build it.."
	@echo "Please use 'make clean' command to clean it.."
	@echo "======================================="

CC = cc

LIB += -lm

MICRO += -std=c99 -Wall -I. -I/usr/local/include -I/usr/include/lua5.4 -L. -L/usr/local/lib  -L/usr/include/lua5.4

WARNING += -Wall -Wno-unused-variable -Wno-unused-function

clone:
	git clone https://github.com/nothings/stb.git

build:
	$(CC) -o lua_stb.so lua_stb.c lua_stb.h lua_stb_image.h stb/stb_image.h stb/stb_image_write.h stb/stb_image_resize.h -O3 -fPIC --shared $(MICRO) $(LIB)

clean_local:
	rm -rf lua_stb.so