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
  
  lua_pushliteral(L, "__mode");
  lua_pushliteral(L, "kv");
  lua_rawset(L, -3);

  luaL_setfuncs(L, libs, 0);

  lua_pop(L, 1);
}

LUAMOD_API int luaopen_lua_stb(lua_State *L) {
  luaL_checkversion(L);
  // 创建Aoi对象元表
  lua_stb_register(L, "__Image__", (luaL_Reg []) {
    {"load", lstbi_load},
    {"load_from_memory", lstbi_load_from_memory},
    {NULL, NULL},
  });
  // 创建Unit对象元表
  lua_stb_register(L, "__ImageWrite__", (luaL_Reg []) {
    {"write_png", lstbi_write_png},
    {"write_bmp", lstbi_write_bmp},
    {"write_tga", lstbi_write_tga},
    {"write_jpg", lstbi_write_jpg},
    {NULL, NULL},
  });

  lua_stb_register(L, "__ImageResize__", (luaL_Reg []) {
    {"resize", lstbir_resize},
    {NULL, NULL},
  });

  // 创建Unit对象元表
  lua_stb_register(L, "__ImageData__", (luaL_Reg []) {
    {"__index", image_data_index},
    {"__newindex",  image_data_newindex},
    {"__gc",  image_data_gc},
    {NULL, NULL},
  });

    
  lua_createtable(L, 0, 2);
  lstbi_new_image(L);
  lua_setfield(L, -2, "image");

  lstbi_new_image_write(L);
  lua_setfield(L, -2, "image_write");

  lstbi_new_image_resize(L);
  lua_setfield(L, -2, "image_resize");


  return 1;
}