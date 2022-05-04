# presents binary and logical operators
println(.7 + 2 \ 0.5) 	# 0.95
# precedence
# +-%
# */\
# 3x
# ^
println(5 % 4 * 12 )		# 12
println(2 * 5 % 2)			# 0
println(2 * 3 ^ 2)			# 18
println((2 * 3) ^ 2)		# 36

# modulus w/ doubles
println(3.2 % 2) 				# 1.2

# logical ops
global x = 3
false && println(x)			#
println(x>2 || exit())	# true

# implied multiplication
global m = 12; global b = 4
println(x = 3x + b)			# 13
println(.5(m)x + b)			# 82.0
