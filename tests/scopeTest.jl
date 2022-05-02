# trying if local variables shadow
global a = "global a"
global b = "global b"
global c = "global c"
begin
  local a = "outer a"
  local b = "outer b"
  begin
    local a = "inner a"
    println(a)
    println(b)
    println(c)
    println(c = "new global c")
  end
  println(a)
  println(b)
  println(c)
end
println(a)
println(b)
println(c)

