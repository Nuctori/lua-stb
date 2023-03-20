#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "lua_stb.h"
#include "stb/stb_image_resize.h"


static int lstbi_new_image_resize(lua_State* L) {
  lua_newuserdata(L, 1);
  luaL_setmetatable(L, "__ImageResize__");
  return 1;
}

static int lstbir_resize(lua_State* L) {
  luaL_checkudata(L, 1, "__ImageResize__");
  ImageData* imageData = (ImageData*)luaL_checkudata(L, 2, "__ImageData__");
  int iw = imageData->w;
  int ih = imageData->h;
  int in = imageData->n;
  const unsigned char * idata = imageData->data;

  lua_Integer ow = luaL_checkinteger(L, 3);
  lua_Integer oh = luaL_checkinteger(L, 4);
  lua_Integer n = luaL_optinteger(L, 5, in);
  if(n == 0) {
    n == in;
  }

  unsigned char *odata = (unsigned char *) allocate(ow * oh * n);
  stbir_resize(idata, iw, ih, 0, odata, ow, oh, 0, STBIR_TYPE_UINT8, n, STBIR_ALPHA_CHANNEL_NONE, 0,
                STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
                STBIR_FILTER_BOX, STBIR_FILTER_BOX,
                STBIR_COLORSPACE_SRGB, NULL
  );


  ImageData* outImageData = (ImageData*)lua_newuserdata(L, sizeof(ImageData));
  outImageData->h = oh;
  outImageData->w = ow;
  outImageData->n = n;
  outImageData->data = odata;
  luaL_setmetatable(L, "__ImageData__");
  return 1;
}
