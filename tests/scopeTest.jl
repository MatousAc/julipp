# trying if local variables shadow
global a = "global a"
global b = "global b"
global c = "global c"
begin
  local a = "outer a"
  local b = "outer b"
  begin
    local a = "inner a"
    print a
    print b
    print c
    print c = "new global c"
  end
  print a
  print b
  print c
end
print a
print b
print c

