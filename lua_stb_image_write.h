#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "lua_stb.h"
#include "stb/stb_image_write.h"

static int lstbi_new_image_write(lua_State* L) {
  lua_newuserdata(L, 1);
  luaL_setmetatable(L, "__ImageWrite__");
  return 1;
}

static int lstbi_write_png(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");
  const char * filename = luaL_checkstring(L, 2);
  lua_Integer x = luaL_checkinteger(L, 3);
  lua_Integer y = luaL_checkinteger(L, 4);
  lua_Integer comp = luaL_checkinteger(L, 5);
  ImageData* imageData = luaL_checkudata(L, 6, "__ImageData__");
  const char* data = imageData->data;
  lua_Integer stride_bytes = luaL_checkinteger(L, 7);
  int ret = stbi_write_png(filename, x, y, comp, data, stride_bytes);
  lua_pushinteger(L, ret);
  return 1;
}

static int lstbi_write_bmp(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");
  const char * filename = luaL_checkstring (L, 2);
  lua_Integer x = luaL_checkinteger(L, 3);
  lua_Integer y = luaL_checkinteger(L, 4);
  lua_Integer comp = luaL_checkinteger(L, 5);
  ImageData* imageData = luaL_checkudata(L, 6, "__ImageData__");
  const char* data = imageData->data;
  int ret = stbi_write_bmp(filename, x, y, comp, data);
  lua_pushinteger(L, ret);
  return 1;
}

static int lstbi_write_tga(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");
  const char * filename = luaL_checkstring(L, 2);
  lua_Integer x = luaL_checkinteger(L, 3);
  lua_Integer y = luaL_checkinteger(L, 4);
  lua_Integer comp = luaL_checkinteger(L, 5);
  ImageData* imageData = luaL_checkudata(L, 6, "__ImageData__");
  const char* data = imageData->data;
  int ret = stbi_write_tga(filename, x, y, comp, data);
  lua_pushinteger(L, ret);
  return 1;
}

static int lstbi_write_jpg(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");
  const char * filename = luaL_checkstring(L, 2);
  lua_Integer x = luaL_checkinteger(L, 3);
  lua_Integer y = luaL_checkinteger(L, 4);
  lua_Integer comp = luaL_checkinteger(L, 5);
  ImageData* imageData = luaL_checkudata(L, 6, "__ImageData__");
  const char* data = imageData->data;
  lua_Integer quality = luaL_checkinteger(L, 7);
  
  int ret = stbi_write_jpg(filename, x, y, comp, data, quality);
  lua_pushinteger(L, ret);
  return 1;
}

void *write_to_char(void *context, void *data, int size) {
  lua_pushlstring((lua_State*)context, data, size);
}

static int lstbi_write_png_to_func(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");
  lua_Integer x = luaL_checkinteger(L, 2);
  lua_Integer y = luaL_checkinteger(L, 3);
  lua_Integer comp = luaL_checkinteger(L, 4);
  ImageData* imageData = luaL_checkudata(L, 5, "__ImageData__");
  lua_Integer stride_bytes = luaL_checkinteger(L, 6);

  if (lua_isfunction(L, -1)) {
      stbi_write_png_to_func(write_to_char, L, x, y, comp, imageData->data, stride_bytes);
      lua_pcall(L, 1, 0, 0);
  }
  return 0;
}

static int lstbi_write_bmp_to_func(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");

  lua_Integer x = luaL_checkinteger(L, 2);
  lua_Integer y = luaL_checkinteger(L, 3);
  lua_Integer comp = luaL_checkinteger(L, 4);
  ImageData* imageData = luaL_checkudata(L, 5, "__ImageData__");

  if (lua_isfunction(L, -1)) {
      stbi_write_bmp_to_func(write_to_char, L, x, y, comp, imageData->data);
      lua_pcall(L, 1, 0, 0);
  }
  return 0;
}

static int lstbi_write_tga_to_func(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");

  lua_Integer x = luaL_checkinteger(L, 2);
  lua_Integer y = luaL_checkinteger(L, 3);
  lua_Integer comp = luaL_checkinteger(L, 4);
  ImageData* imageData = luaL_checkudata(L, 5, "__ImageData__");

  if (lua_isfunction(L, -1)) {
      stbi_write_tga_to_func(write_to_char, L, x, y, comp, imageData->data);
      lua_pcall(L, 1, 0, 0);
  }
  return 0;
}

static int lstbi_write_jpg_to_func(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");

  lua_Integer x = luaL_checkinteger(L, 2);
  lua_Integer y = luaL_checkinteger(L, 3);
  lua_Integer comp = luaL_checkinteger(L, 4);
  ImageData* imageData = luaL_checkudata(L, 5, "__ImageData__");
  lua_Integer quality = luaL_checkinteger(L, 6);


  if (lua_isfunction(L, -1)) {
      stbi_write_jpg_to_func(write_to_char, L, x, y, comp, imageData->data, quality);
      lua_pcall(L, 1, 0, 0);
  }
  return 0;
}