local temp;
local o = 1;
local n = 0;
local i = 1
while (i < 10)
  println(o)
  temp = o
  o = n
  n = temp + o
  i = i + 1
end

# comment