#define STB_IMAGE_IMPLEMENTATION

#include "lua_stb.h"
#include "stb/stb_image.h"


static int lstbi_new_image(lua_State* L) {
  lua_newuserdata(L, 1);
  luaL_setmetatable(L, "__Image__");
  return 1;
}

static int lstbi_load(lua_State* L) {
  luaL_checkudata(L, 1, "__Image__");
  const char * filename = luaL_checkstring(L, 2);
  lua_Integer req_comp = luaL_optinteger(L, 3, 0);

  int  w, h, n;
  unsigned char *data = stbi_load(filename,&w, &h, &n, req_comp);

  luaL_setimagedata(L, data, w, h, n);
  stbi_image_free(data);
  lua_pushinteger(L, w);
  lua_pushinteger(L, h);
  lua_pushinteger(L, n);
  return 4;
}

static int lstbi_load_from_memory(lua_State* L) {
  luaL_checkudata(L, 1, "__Image__");
  size_t len;
  const char * imageData = luaL_checklstring (L, 2, &len);
  lua_Integer req_comp = luaL_optinteger(L, 3, 0);

  int  w, h, n;
  unsigned char *data = stbi_load_from_memory(imageData, len,&w, &h, &n, req_comp);

  luaL_setimagedata(L, data, w, h, n);
  stbi_image_free(data);
  lua_pushinteger(L, w);
  lua_pushinteger(L, h);
  lua_pushinteger(L, n);
  return 4;
}