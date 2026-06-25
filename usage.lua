local stb = require("lua_stb")
local image = stb.image
local writer = stb.image_write
local resizer = stb.image_resize

local data, w, h, n = image:load("input.pgm")

print("loaded:", w, h, n, #data)

for x = 1, w do
  local i = (x - 1) * n + 1
  if i <= #data then
    data[i] = 255
  end
end

local resized = resizer:resize(data, w * 2, h * 2, n)
writer:write_png("output.png", resized.w, resized.h, resized.n, resized, resized.w * resized.n)

local file = assert(io.open("output_callback.png", "wb"))
writer:write_png_to_func(resized.w, resized.h, resized.n, resized, resized.w * resized.n, function(chunk)
  file:write(chunk)
end)
file:close()

local f = assert(io.open("input.pgm", "rb"))
local memory = f:read("*a")
f:close()

local data2, w2, h2, n2 = image:load_from_memory(memory)
print("memory:", w2, h2, n2, #data2)
