# presents cool features and native functions
println("Enter number to do calculations with (q to quit)")
in = ""; print(">> ")
while in
	in = readline()
	if in == "q" do break; end			# i use do differently here
	num = parsenum(in)
	println("length: ", length(in)) # built in julia functions
	println("round:  ", round(num))
	println("ceil:   ", ceil(num))
	println("floor:  ", floor(num))
	println("abs:    ", abs(num))
	println("sqrt:   ", sqrt(num))
	print(">> ")
end
# fun additions
println("string" \ ("\n" * in))		# q
num = 22													# string
println(num %= 3)									# 1
println(num -= -1)								# 2
println(num ^= 2)									# 4
num \= 2
println(num)											# 0.5
