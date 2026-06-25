#ifndef LUA_STB_H
#define LUA_STB_H

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#ifndef release
#define release(ptr) free(ptr)
#endif

#ifndef allocate
#define allocate(size) malloc(size)
#endif

typedef struct ImageData {
  int w;
  int h;
  int n;
  unsigned char *data;
} ImageData;

static void lua_stb_check_positive_dims(lua_State *L, lua_Integer w, lua_Integer h, lua_Integer n) {
  if (w <= 0 || h <= 0 || n <= 0) {
    luaL_error(L, "image dimensions and channels must be positive");
  }
}

static size_t lua_stb_checked_size(lua_State *L, lua_Integer w, lua_Integer h, lua_Integer n) {
  lua_stb_check_positive_dims(L, w, h, n);
  if (w > INT_MAX || h > INT_MAX || n > INT_MAX) {
    luaL_error(L, "image dimensions are too large");
  }
  if ((size_t)w > SIZE_MAX / (size_t)h) {
    luaL_error(L, "image buffer size overflows");
  }
  size_t pixels = (size_t)w * (size_t)h;
  if ((size_t)n > SIZE_MAX / pixels) {
    luaL_error(L, "image buffer size overflows");
  }
  return pixels * (size_t)n;
}

static size_t lua_stb_checked_row_stride(lua_State *L, lua_Integer w, lua_Integer n) {
  lua_stb_check_positive_dims(L, w, 1, n);
  if ((size_t)w > SIZE_MAX / (size_t)n) {
    luaL_error(L, "row stride overflows");
  }
  return (size_t)w * (size_t)n;
}

static ImageData *lua_stb_check_image_data(lua_State *L, int idx) {
  return (ImageData *)luaL_checkudata(L, idx, "__ImageData__");
}

static int lua_stb_push_image_data(lua_State *L, const unsigned char *data, lua_Integer w, lua_Integer h, lua_Integer n) {
  size_t size = lua_stb_checked_size(L, w, h, n);
  ImageData *imageData = (ImageData *)lua_newuserdata(L, sizeof(ImageData));
  imageData->w = (int)w;
  imageData->h = (int)h;
  imageData->n = (int)n;
  imageData->data = (unsigned char *)allocate(size);
  if (imageData->data == NULL) {
    release((void *)data);
    return luaL_error(L, "out of memory");
  }
  memcpy(imageData->data, data, size);
  luaL_getmetatable(L, "__ImageData__");
  lua_setmetatable(L, -2);
  return 1;
}

static lua_Integer lua_stb_image_len(const ImageData *imageData) {
  size_t size = (size_t)imageData->w * (size_t)imageData->h * (size_t)imageData->n;
  if (size > (size_t)LUA_MAXINTEGER) {
    return LUA_MAXINTEGER;
  }
  return (lua_Integer)size;
}

#endif
