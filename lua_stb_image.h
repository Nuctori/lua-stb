#define STB_IMAGE_IMPLEMENTATION

#include "lua_stb.h"
#include "stb/stb_image.h"


static int lstbi_new_image(lua_State* L) {
  lua_newuserdata(L, 1);
  luaL_setmetatable(L, "__Image__");
  return 1;
}

static int lua_stb_image_load_error(lua_State *L) {
  const char *reason = stbi_failure_reason();
  if (reason != NULL) {
    return luaL_error(L, "%s", reason);
  }
  return luaL_error(L, "image data error");
}

static int lua_stb_image_push_info(lua_State *L, int w, int h, int n) {
  lua_pushinteger(L, w);
  lua_pushinteger(L, h);
  lua_pushinteger(L, n);
  return 3;
}

static int lstbi_load(lua_State* L) {
  luaL_checkudata(L, 1, "__Image__");
  const char * filename = luaL_checkstring(L, 2);
  lua_Integer req_comp = luaL_optinteger(L, 3, 0);
  if (req_comp < 0 || req_comp > 4) {
    return luaL_error(L, "req_comp must be between 0 and 4");
  }

  int w, h, n;
  unsigned char *data = stbi_load(filename,&w, &h, &n, req_comp);
  if (data == NULL) {
    return lua_stb_image_load_error(L);
  }

  int out_n = req_comp == 0 ? n : (int)req_comp;
  lua_stb_push_image_data(L, data, w, h, out_n);
  stbi_image_free(data);
  lua_pushinteger(L, w);
  lua_pushinteger(L, h);
  lua_pushinteger(L, out_n);
  return 4;
}

static int lstbi_load_from_memory(lua_State* L) {
  luaL_checkudata(L, 1, "__Image__");
  size_t len;
  const unsigned char *imageData = (const unsigned char *)luaL_checklstring(L, 2, &len);
  if (len > (size_t)INT_MAX) {
    return luaL_error(L, "image data is too large");
  }
  lua_Integer req_comp = luaL_optinteger(L, 3, 0);
  if (req_comp < 0 || req_comp > 4) {
    return luaL_error(L, "req_comp must be between 0 and 4");
  }

  int w, h, n;
  unsigned char *data = stbi_load_from_memory(imageData, (int)len, &w, &h, &n, req_comp);
  if (data == NULL) {
    return lua_stb_image_load_error(L);
  }

  int out_n = req_comp == 0 ? n : (int)req_comp;
  lua_stb_push_image_data(L, data, w, h, out_n);
  stbi_image_free(data);
  lua_pushinteger(L, w);
  lua_pushinteger(L, h);
  lua_pushinteger(L, out_n);
  return 4;
}

static int lstbi_info(lua_State *L) {
  luaL_checkudata(L, 1, "__Image__");
  const char *filename = luaL_checkstring(L, 2);
  int w, h, n;
  if (!stbi_info(filename, &w, &h, &n)) {
    return lua_stb_image_load_error(L);
  }
  return lua_stb_image_push_info(L, w, h, n);
}

static int lstbi_info_from_memory(lua_State *L) {
  luaL_checkudata(L, 1, "__Image__");
  size_t len;
  const unsigned char *imageData = (const unsigned char *)luaL_checklstring(L, 2, &len);
  if (len > (size_t)INT_MAX) {
    return luaL_error(L, "image data is too large");
  }
  int w, h, n;
  if (!stbi_info_from_memory(imageData, (int)len, &w, &h, &n)) {
    return lua_stb_image_load_error(L);
  }
  return lua_stb_image_push_info(L, w, h, n);
}
