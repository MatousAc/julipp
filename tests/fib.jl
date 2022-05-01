# var a = 0;
# var temp;

# for (var b = 1; a < 10000; b = temp + b) {
#   print a;
#   temp = a;
#   a = b;
# }



local temp;
local o = 1;
local n = 0;
local i = 1
while (i < 10)
  print o
  temp = o
  o = n
  n = temp + o
  i = i + 1
end

# comment