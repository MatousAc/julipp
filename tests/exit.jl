# does exit() work?
val = 0
println(val + ": nest 0")
begin
  println(val + ": nest 1")
  val = 1
  println(val + ": nest 1")
end

println(val + ": nest 0")

begin
  println(val + ": nest 1")
  val = 1
  println(val + ": nest 1")
  begin
    println(val + ": nest 2")
    val = 2
		exit() 
		# we should not do anything else
    println(val + ": nest 2")
    begin
      println(val + ": nest 3")
      val = 3
      println(val + ": nest 3")
    end
    println(val + ": nest 2")
  end
  println(val + ": nest 1")
end

println(val + ": nest 0")
