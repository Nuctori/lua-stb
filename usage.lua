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
local newW = w // 2
local newH = h // 2
local odata = imageR:resize(data, newW // 1, newH // 1, n);


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
local data2, x, y, n = image:load_from_memory(r)
local odata = imageR:resize(data2, x*2, y*2, n);
imageW:write_png("out2.png", x*2,  y*2, n, odata, (x* 2) * n)