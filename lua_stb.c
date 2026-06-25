#define LUA_LIB
#include "lua_stb.h"

#include "lua_stb_image.h"
#include "lua_stb_image_write.h"
#include "lua_stb_image_resize.h"


static inline void lua_stb_register(lua_State *L, const char* meta, luaL_Reg libs[]) {
  if (LUA_MINSTACK - lua_gettop(L) < 3)
    luaL_checkstack(L, 3, "check lua stack error.");

  luaL_newmetatable(L, meta);

  lua_pushstring (L, "__index");
  lua_pushvalue(L, -2);
  lua_rawset(L, -3);

  luaL_setfuncs(L, libs, 0);

  lua_pop(L, 1);
}

static int lua_stb_image_data_index(lua_State *L) {
  ImageData *imageData = lua_stb_check_image_data(L, 1);
  if (lua_type(L, 2) == LUA_TSTRING) {
    const char *key = lua_tostring(L, 2);
    if (strcmp(key, "w") == 0) {
      lua_pushinteger(L, imageData->w);
      return 1;
    }
    if (strcmp(key, "h") == 0) {
      lua_pushinteger(L, imageData->h);
      return 1;
    }
    if (strcmp(key, "n") == 0) {
      lua_pushinteger(L, imageData->n);
      return 1;
    }
    if (strcmp(key, "len") == 0 || strcmp(key, "length") == 0) {
      lua_pushinteger(L, lua_stb_image_len(imageData));
      return 1;
    }
  }

  lua_Integer index = luaL_checkinteger(L, 2);
  lua_Integer len = lua_stb_image_len(imageData);
  if (index < 1 || index > len) {
    return luaL_error(L, "image index out of range");
  }
  lua_pushinteger(L, imageData->data[index - 1]);
  return 1;
}

static int lua_stb_image_data_len(lua_State *L) {
  ImageData *imageData = lua_stb_check_image_data(L, 1);
  lua_pushinteger(L, lua_stb_image_len(imageData));
  return 1;
}

static int lua_stb_image_data_newindex(lua_State *L) {
  ImageData *imageData = lua_stb_check_image_data(L, 1);
  lua_Integer index = luaL_checkinteger(L, 2);
  lua_Integer value = luaL_checkinteger(L, 3);
  lua_Integer len = lua_stb_image_len(imageData);
  if (index < 1 || index > len) {
    return luaL_error(L, "image index out of range");
  }
  if (value < 0 || value > 255) {
    return luaL_error(L, "value is out of range[0, 255]");
  }
  imageData->data[index - 1] = (unsigned char)value;
  return 0;
}

static int lua_stb_image_data_gc(lua_State *L) {
  ImageData *imageData = lua_stb_check_image_data(L, 1);
  if (imageData->data != NULL) {
    release(imageData->data);
  }
  imageData->data = NULL;
  return 0;
}

LUAMOD_API int luaopen_lua_stb(lua_State *L) {
  luaL_checkversion(L);

  lua_stb_register(L, "__Image__", (luaL_Reg []) {
    {"load", lstbi_load},
    {"load_from_memory", lstbi_load_from_memory},
    {"info", lstbi_info},
    {"info_from_memory", lstbi_info_from_memory},
    {NULL, NULL},
  });

  lua_stb_register(L, "__ImageWrite__", (luaL_Reg []) {
    {"write_png", lstbi_write_png},
    {"write_bmp", lstbi_write_bmp},
    {"write_tga", lstbi_write_tga},
    {"write_jpg", lstbi_write_jpg},
    {"write_png_to_func", lstbi_write_png_to_func},
    {"write_bmp_to_func", lstbi_write_bmp_to_func},
    {"write_tga_to_func", lstbi_write_tga_to_func},
    {"write_jpg_to_func", lstbi_write_jpg_to_func},
    {"write_png_to_string", lstbi_write_png_to_string},
    {"write_bmp_to_string", lstbi_write_bmp_to_string},
    {"write_tga_to_string", lstbi_write_tga_to_string},
    {"write_jpg_to_string", lstbi_write_jpg_to_string},
    {NULL, NULL},
  });

  lua_stb_register(L, "__ImageResize__", (luaL_Reg []) {
    {"resize", lstbir_resize},
    {NULL, NULL},
  });

  luaL_newmetatable(L, "__ImageData__");
  lua_pushcfunction(L, lua_stb_image_data_index);
  lua_setfield(L, -2, "__index");
  lua_pushcfunction(L, lua_stb_image_data_newindex);
  lua_setfield(L, -2, "__newindex");
  lua_pushcfunction(L, lua_stb_image_data_gc);
  lua_setfield(L, -2, "__gc");
  lua_pushcfunction(L, lua_stb_image_data_len);
  lua_setfield(L, -2, "__len");
  lua_pop(L, 1);

    
  lua_createtable(L, 0, 2);
  lstbi_new_image(L);
  lua_setfield(L, -2, "image");

  lstbi_new_image_write(L);
  lua_setfield(L, -2, "image_write");

  lstbi_new_image_resize(L);
  lua_setfield(L, -2, "image_resize");


  return 1;
}
