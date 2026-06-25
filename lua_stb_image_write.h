#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "lua_stb.h"
#include "stb/stb_image_write.h"
#include <stdio.h>

typedef struct lua_stb_write_context {
  lua_State *L;
  int callback_ref;
  int had_error;
  char error_message[256];
} lua_stb_write_context;

static int lstbi_new_image_write(lua_State* L) {
  lua_newuserdata(L, 1);
  luaL_setmetatable(L, "__ImageWrite__");
  return 1;
}

static void lua_stb_check_write_dims(lua_State *L, lua_Integer x, lua_Integer y, lua_Integer comp) {
  lua_stb_check_positive_dims(L, x, y, comp);
  if (x > INT_MAX || y > INT_MAX || comp > INT_MAX) {
    luaL_error(L, "image dimensions are too large");
  }
}

static int lstbi_write_png(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");
  const char * filename = luaL_checkstring(L, 2);
  lua_Integer x = luaL_checkinteger(L, 3);
  lua_Integer y = luaL_checkinteger(L, 4);
  lua_Integer comp = luaL_checkinteger(L, 5);
  ImageData* imageData = lua_stb_check_image_data(L, 6);
  const unsigned char *data = imageData->data;
  lua_Integer stride_bytes = luaL_checkinteger(L, 7);
  lua_stb_check_write_dims(L, x, y, comp);
  size_t min_stride = lua_stb_checked_row_stride(L, x, comp);
  if (comp > 4 || stride_bytes <= 0 || stride_bytes > INT_MAX || (size_t)stride_bytes < min_stride) {
    return luaL_error(L, "invalid image write arguments");
  }
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
  ImageData* imageData = lua_stb_check_image_data(L, 6);
  const unsigned char *data = imageData->data;
  lua_stb_check_write_dims(L, x, y, comp);
  if (comp > 4) {
    return luaL_error(L, "invalid image write arguments");
  }
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
  ImageData* imageData = lua_stb_check_image_data(L, 6);
  const unsigned char *data = imageData->data;
  lua_stb_check_write_dims(L, x, y, comp);
  if (comp > 4) {
    return luaL_error(L, "invalid image write arguments");
  }
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
  ImageData* imageData = lua_stb_check_image_data(L, 6);
  const unsigned char *data = imageData->data;
  lua_Integer quality = luaL_checkinteger(L, 7);
  lua_stb_check_write_dims(L, x, y, comp);
  if (comp > 4 || quality < 1 || quality > 100) {
    return luaL_error(L, "invalid image write arguments");
  }
  
  int ret = stbi_write_jpg(filename, x, y, comp, data, quality);
  lua_pushinteger(L, ret);
  return 1;
}

static void write_to_char(void *context, void *data, int size) {
  lua_stb_write_context *ctx = (lua_stb_write_context *)context;
  if (ctx->had_error) {
    return;
  }

  lua_State *L = ctx->L;
  lua_rawgeti(L, LUA_REGISTRYINDEX, ctx->callback_ref);
  lua_pushlstring(L, (const char *)data, (size_t)size);
  if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
    const char *message = lua_tostring(L, -1);
    if (message != NULL) {
      snprintf(ctx->error_message, sizeof(ctx->error_message), "%s", message);
    } else {
      snprintf(ctx->error_message, sizeof(ctx->error_message), "%s", "lua callback failed");
    }
    lua_pop(L, 1);
    ctx->had_error = 1;
  }
}

static int lstbi_write_png_to_func(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");
  lua_Integer x = luaL_checkinteger(L, 2);
  lua_Integer y = luaL_checkinteger(L, 3);
  lua_Integer comp = luaL_checkinteger(L, 4);
  ImageData* imageData = lua_stb_check_image_data(L, 5);
  lua_Integer stride_bytes = luaL_checkinteger(L, 6);
  lua_stb_check_write_dims(L, x, y, comp);
  size_t min_stride = lua_stb_checked_row_stride(L, x, comp);
  if (comp > 4 || stride_bytes <= 0 || stride_bytes > INT_MAX || (size_t)stride_bytes < min_stride) {
    return luaL_error(L, "invalid image write arguments");
  }

  luaL_checktype(L, 7, LUA_TFUNCTION);
  lua_stb_write_context ctx = {0};
  ctx.L = L;
  lua_pushvalue(L, 7);
  ctx.callback_ref = luaL_ref(L, LUA_REGISTRYINDEX);

  int ret = stbi_write_png_to_func(write_to_char, &ctx, x, y, comp, imageData->data, stride_bytes);
  luaL_unref(L, LUA_REGISTRYINDEX, ctx.callback_ref);
  if (ctx.had_error) {
    return luaL_error(L, "%s", ctx.error_message);
  }
  if (ret == 0) {
    return luaL_error(L, "png write callback failed");
  }
  lua_pushinteger(L, ret);
  return 1;
}

static int lstbi_write_bmp_to_func(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");

  lua_Integer x = luaL_checkinteger(L, 2);
  lua_Integer y = luaL_checkinteger(L, 3);
  lua_Integer comp = luaL_checkinteger(L, 4);
  ImageData* imageData = lua_stb_check_image_data(L, 5);
  lua_stb_check_write_dims(L, x, y, comp);
  if (comp > 4) {
    return luaL_error(L, "invalid image write arguments");
  }

  luaL_checktype(L, 6, LUA_TFUNCTION);
  lua_stb_write_context ctx = {0};
  ctx.L = L;
  lua_pushvalue(L, 6);
  ctx.callback_ref = luaL_ref(L, LUA_REGISTRYINDEX);

  int ret = stbi_write_bmp_to_func(write_to_char, &ctx, x, y, comp, imageData->data);
  luaL_unref(L, LUA_REGISTRYINDEX, ctx.callback_ref);
  if (ctx.had_error) {
    return luaL_error(L, "%s", ctx.error_message);
  }
  if (ret == 0) {
    return luaL_error(L, "bmp write callback failed");
  }
  lua_pushinteger(L, ret);
  return 1;
}

static int lstbi_write_tga_to_func(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");

  lua_Integer x = luaL_checkinteger(L, 2);
  lua_Integer y = luaL_checkinteger(L, 3);
  lua_Integer comp = luaL_checkinteger(L, 4);
  ImageData* imageData = lua_stb_check_image_data(L, 5);
  lua_stb_check_write_dims(L, x, y, comp);
  if (comp > 4) {
    return luaL_error(L, "invalid image write arguments");
  }

  luaL_checktype(L, 6, LUA_TFUNCTION);
  lua_stb_write_context ctx = {0};
  ctx.L = L;
  lua_pushvalue(L, 6);
  ctx.callback_ref = luaL_ref(L, LUA_REGISTRYINDEX);

  int ret = stbi_write_tga_to_func(write_to_char, &ctx, x, y, comp, imageData->data);
  luaL_unref(L, LUA_REGISTRYINDEX, ctx.callback_ref);
  if (ctx.had_error) {
    return luaL_error(L, "%s", ctx.error_message);
  }
  if (ret == 0) {
    return luaL_error(L, "tga write callback failed");
  }
  lua_pushinteger(L, ret);
  return 1;
}

static int lstbi_write_jpg_to_func(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageWrite__");

  lua_Integer x = luaL_checkinteger(L, 2);
  lua_Integer y = luaL_checkinteger(L, 3);
  lua_Integer comp = luaL_checkinteger(L, 4);
  ImageData* imageData = lua_stb_check_image_data(L, 5);
  lua_Integer quality = luaL_checkinteger(L, 6);
  lua_stb_check_write_dims(L, x, y, comp);
  if (comp > 4 || quality < 1 || quality > 100) {
    return luaL_error(L, "invalid image write arguments");
  }

  luaL_checktype(L, 7, LUA_TFUNCTION);
  lua_stb_write_context ctx = {0};
  ctx.L = L;
  lua_pushvalue(L, 7);
  ctx.callback_ref = luaL_ref(L, LUA_REGISTRYINDEX);

  int ret = stbi_write_jpg_to_func(write_to_char, &ctx, x, y, comp, imageData->data, quality);
  luaL_unref(L, LUA_REGISTRYINDEX, ctx.callback_ref);
  if (ctx.had_error) {
    return luaL_error(L, "%s", ctx.error_message);
  }
  if (ret == 0) {
    return luaL_error(L, "jpg write callback failed");
  }
  lua_pushinteger(L, ret);
  return 1;
}
