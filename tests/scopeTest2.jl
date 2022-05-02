# global && local mix
global a = "global a"
local b = "global b" # still bascically global
global c = "global c"
global d = "first d"
begin
  local a = "outer a"
  local b = "outer b"
  print d
  local d = "outer d"
  begin
    print d
    global d;
    print d
    global a = "new global a"
    local b = 34;;
    print a
    print b
    print c
    print c = "new global c"
    local c = "and a local c too!"
    d = "d has been changed"
    print c
    print d
    local d = "a local d"
    print d
  end
  print a
  print b
  print c
  print d
end
print a
print b
print c
print d