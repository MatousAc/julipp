T = true
F = false

if T || F
println("T or F is true")
else
println("T or F is false")
end
println("after end")

println(F && F)
println(T || T)
println(T && F)
println(F || T)

mut = "original"
println(mut)
T || (mut = "new")
println(mut)
F && (mut = "newer")
println(mut)
F && (mut = "newer")
println(mut)
T && (mut = "newest")
println(mut)

