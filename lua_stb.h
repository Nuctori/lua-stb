
#include <stdlib.h>
#include <string.h>

#ifndef release
    #define release(ptr) free(ptr)
#endif

#ifndef allocate
    #define allocate(size) malloc(size)
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifndef ImageDataDef
    typedef struct ImageData
    {
        int w;
        int h;
        int n;
        unsigned char * data;
    } ImageData;


    static int luaL_setimagedata(lua_State* L, const unsigned char * data, int w, int h, int n) {
        ImageData* imageData = lua_newuserdata(L, sizeof(ImageData));
        imageData->w = w;
        imageData->h = h;
        imageData->n = n;
        const unsigned char * imageDataChar = allocate(w * h * n); 
        imageData->data = memcpy(imageDataChar, data, w * h * n);
        luaL_setmetatable(L, "__ImageData__");
        return 1;
    }

    static int image_data_index (lua_State* L) { 
        ImageData* imageData = luaL_checkudata(L, 1, "__ImageData__");
        lua_Integer index = luaL_checkinteger(L, 2);
        if(index - 1 < imageData->w * imageData->h * imageData->n ){
            lua_pushinteger(L, imageData->data[index - 1]);
        }
        return 1; 
    }


    static int image_data_newindex (lua_State* L) { 
        ImageData* imageData = luaL_checkudata(L, 1, "__ImageData__");
        lua_Integer index = luaL_checkinteger(L, 2);
        lua_Integer value = luaL_checkinteger(L, 3);
        if (value < 0 || value > 255) {
            luaL_error(L, "value is out of range[0, 255]");
        }
        imageData->data[index - 1] = value;
        return 0; 
    }

    static int image_data_gc (lua_State* L) { 
        ImageData* imageData = luaL_checkudata(L, 1, "__ImageData__");
        release(imageData->data);
        return 0; 
    }

    #define ImageDataDef 1
#endif
