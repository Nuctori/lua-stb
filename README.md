# lua-stb

`lua-stb` is a Lua C extension that exposes a small, practical subset of the
[stb](https://github.com/nothings/stb) image helpers:

- `stb_image.h` for loading images from files and memory
- `stb_image_write.h` for writing PNG, BMP, TGA, and JPG
- `stb_image_resize.h` for resizing image buffers

The repository vendors the stb headers it needs, so a clone of this repo is
enough to build the module.

## Requirements

- A C99 compiler
- Lua 5.3 or newer
- `pkg-config`

## Build

```bash
make build
```

You can choose the Lua version at build time:

```bash
make LUA_VERSION=5.3 build
make LUA_VERSION=5.4 build
```

## Test

```bash
make test
```

The tests use a tiny 2x2 PGM fixture and cover:

- module loading
- file and memory loading
- info-only header reads
- resize
- bounds checking on image data
- PNG writing and callback writing
- string-returning writer helpers

## Usage

```lua
local stb = require("lua_stb")
local image = stb.image
local writer = stb.image_write
local resizer = stb.image_resize

local data, w, h, n = image:load("input.pgm")

print(data.w, data.h, data.n, #data)
print(data[1])
data[1] = 255

local bigger = resizer:resize(data, w * 2, h * 2, n)
writer:write_png("output.png", bigger.w, bigger.h, bigger.n, bigger, bigger.w * bigger.n)

writer:write_png_to_func(bigger.w, bigger.h, bigger.n, bigger, bigger.w * bigger.n, function(chunk)
  io.write(chunk)
end)

local width, height, channels = image:info("input.pgm")
local png = writer:write_png_to_string(bigger.w, bigger.h, bigger.n, bigger, bigger.w * bigger.n)
```

`ImageData` values support:

- 1-based numeric indexing for pixel bytes
- `w`, `h`, `n`, `len`, and `length` fields
- `#image_data` for the full byte length

Additional convenience APIs:

- `image:info(path)` and `image:info_from_memory(bytes)` for header-only reads
- `image_write:write_*_to_string(...)` for returning encoded image data directly

## Installation

This project currently ships as a source build. The simplest path is:

```bash
make build
```

If you use LuaRocks, install directly from the included rockspec:

```bash
luarocks make lua-stb-scm-1.rockspec
```

## License

MIT. See [LICENSE](LICENSE).
