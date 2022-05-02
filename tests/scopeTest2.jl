# global && local mix
global a = "global a"
local b = "global b" # still bascically global
global c = "global c"
global d = "first d"
begin
  local a = "outer a"
  local b = "outer b"
  println(d)
  local d = "outer d"
  begin
    println(d)
    global d;
    println(d)
    global a = "new global a"
    local b = 34;;
    println(a)
    println(b)
    println(c)
    println(c = "new global c")
    local c = "and a local c too!"
    d = "d has been changed"
    println(c)
    println(d)
    local d = "a local d"
    println(d)
  end
  println(a)
  println(b)
  println(c)
  println(d)
end
println(a)
println(b)
println(c)
println(d)