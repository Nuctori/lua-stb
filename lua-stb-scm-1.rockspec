package = "lua-stb"
version = "scm-1"
source = {
  url = "git+https://github.com/Nuctori/lua-stb",
}
description = {
  summary = "Lua bindings for stb image libraries",
  detailed = "Lua C extension that exposes stb_image, stb_image_write, and stb_image_resize.",
  license = "MIT",
  homepage = "https://github.com/Nuctori/lua-stb",
}
dependencies = {
  "lua >= 5.3",
}
build = {
  type = "builtin",
  modules = {
    ["lua_stb"] = {
      sources = {
        "lua_stb.c",
      },
      incdirs = {
        ".",
      },
      libdirs = {
        ".",
      },
      defines = {
        "LUA_STB_USE_BUNDLED_STB",
      },
    },
  },
}
