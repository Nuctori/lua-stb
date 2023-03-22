# lua_stb

lua_stb is a Lua library that provides bindings to some of the [stb](https://github.com/nothings/stb) single-file public domain libraries for C/C++. These libraries are designed to be simple, portable and easy to integrate into your projects.

## Features

lua_stb currently supports the following stb libraries:

- [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h): image loading/decoding from file/memory
- [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h): image writing to disk as PNG/BMP/TGA/JPEG
- [stb_image_resize.h](https://github.com/nothings/stb/blob/master/stb_image_resize.h): image resizing with high-quality filters

## Installation

To install lua_stb, you need a C compiler and Lua >= 5.3. You can use `make` to compile and install the library:

```bash
$ git clone https://github.com/Nuctori/lua_stb
$ cd lua_stb
$ make build
```

This will install the library in Lua's default path. You can change the installation directory by editing the `config` file.

## Usage

To use lua_stb, you need to require the `lua_stb` module and then use its submodules: `image`, `image_write` and `image_resize`. For example:

```lua
local stb = require "lua_stb"
local image = stb.image
local imageW = stb.image_write
local imageR = stb.image_resize


-- data: unsigned char array for image [r,g,b,a, r,g,b,a, ...]
-- w: image weight
-- h: image height
-- n: image channels
local data, w, h, n = image:load("in.png")

-- You can easily modify images without worrying about string memory copu
-- useage: draw a red line at a height of ten pixels in the picture 
for dx=1, w do
    data[n * w * 10 + dx * n + 1] = 255; -- Red
    data[n * w * 10 + dx * n + 2] = 0;   -- Greed
    data[n * w * 10 + dx * n + 3] = 0;   -- Blue
    data[n * w * 10 + dx * n + 4] = 255; -- Alpha
end

-- resize the image
local newW = w / 2
local newH = h / 2
local odata = imageR:resize(data, newW, newH, n);

imageW:write_png("out.png", newW, newH, n, odata, newW * n)
-- lua-stb alse supports exporting to other file formats
-- imageW:write_bmp("out.bmp", newW, newH, n, odata)
-- imageW:write_tga("out.tga", newW, newH, n, odata)
-- imageW:write_jpg("out.jpg", newW, newH, n, odata, quality)

--  write to func 
imageW:write_png_to_func(w, h, n, data, w * n, function(image)
    local f = io.open ("write_by_func.png", "w+")
    f:write(image)
    f:close()
end)
-- imageW:write_bmp_to_func(w, h, n, data, func)
-- imageW:write_tga_to_func(w, h, n, data, func)
-- imageW:write_jpg_to_func(w, h, n, data, func, quality)


-- lua-stb support load image from menory 
local f = io.open ("in.png", "r")
local r = f:read('*a')
local data2,x,y,n=image :load_from_memory(r)
local odata=imageR :resize(data2,x*2,y*2,n);
imageW :write_png("out2.png",x*2,y*2,n ,odata,(x*2)*n)

```

For more details on each submodule's functions and parameters,
please refer to their respective header files in the stb repository.

## License

lua_stp is licensed under the MIT license (the same as Lua itself).