local function assert_eq(actual, expected, msg)
  if actual ~= expected then
    error((msg or "assert_eq failed") .. string.format(": expected %s, got %s", tostring(expected), tostring(actual)), 2)
  end
end

local function assert_error(pattern, fn)
  local ok, err = pcall(fn)
  if ok then
    error("expected error matching " .. pattern, 2)
  end
  if not tostring(err):match(pattern) then
    error(string.format("expected error matching %q, got %q", pattern, tostring(err)), 2)
  end
end

local stb = require("lua_stb")
assert_eq(type(stb), "table", "module must load")
assert_eq(type(stb.image), "userdata", "image handle")
assert_eq(type(stb.image_write), "userdata", "image_write handle")
assert_eq(type(stb.image_resize), "userdata", "image_resize handle")

local image = stb.image
local image_write = stb.image_write
local image_resize = stb.image_resize

local data, w, h, n = image:load("tests/fixture.pgm")
assert_eq(w, 2, "fixture width")
assert_eq(h, 2, "fixture height")
assert_eq(n, 1, "fixture channels")
assert_eq(#data, 4, "fixture len")
assert_eq(data.w, 2, "data.w")
assert_eq(data.h, 2, "data.h")
assert_eq(data.n, 1, "data.n")
assert_eq(data.len, 4, "data.len")
assert_eq(data[1], 0, "first pixel")
assert_eq(data[4], 255, "last pixel")

local forced, fw, fh, fn = image:load("tests/fixture.pgm", 4)
assert_eq(fw, 2, "forced width")
assert_eq(fh, 2, "forced height")
assert_eq(fn, 4, "forced channels")
assert_eq(#forced, 16, "forced len")

assert_error("image index out of range", function()
  return data[0]
end)

assert_error("image index out of range", function()
  data[5] = 1
end)

assert_error("value is out of range", function()
  data[1] = 256
end)

local resized = image_resize:resize(data, 4, 4, 1)
assert_eq(resized.w, 4, "resized.w")
assert_eq(resized.h, 4, "resized.h")
assert_eq(resized.n, 1, "resized.n")
assert_eq(#resized, 16, "resized len")

assert_error("invalid image write arguments", function()
  image_write:write_png("tests_tmp_out.png", 4, 4, 1, resized, 0)
end)

assert_eq(image_write:write_png("tests_tmp_out.png", 4, 4, 1, resized, 4), 1, "write_png")

local chunks = {}
local ok = image_write:write_png_to_func(4, 4, 1, resized, 4, function(chunk)
  chunks[#chunks + 1] = #chunk
end)
assert_eq(ok, 1, "write_png_to_func return")
assert(#chunks > 0, "callback should receive chunks")

local bmp_chunks = {}
local bmp_ok = image_write:write_bmp_to_func(4, 4, 1, resized, function(chunk)
  bmp_chunks[#bmp_chunks + 1] = #chunk
end)
assert_eq(bmp_ok, 1, "write_bmp_to_func return")
assert(#bmp_chunks > 0, "bmp callback should receive chunks")

assert_error("boom", function()
  image_write:write_png_to_func(4, 4, 1, resized, 4, function()
    error("boom")
  end)
end)

local mem = assert(io.open("tests/fixture.pgm", "rb")):read("*a")
local data2, w2, h2, n2 = image:load_from_memory(mem, 4)
assert_eq(w2, 2, "memory width")
assert_eq(h2, 2, "memory height")
assert_eq(n2, 4, "memory channels")
assert_eq(#data2, 16, "memory len")

print("lua-stb tests passed")
