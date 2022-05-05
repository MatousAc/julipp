# presents complext control flow and scope
br = false 							# global def
while (true)
	global JulyaxAge = 2	# global def
	if (!br)							# global access
		println("1st pass")	# 1st pass
		br = true						# global assign
	elseif (br)
		println("2nd pass")	# 2nd pass
		br = true						# global assign
		break
	else
		JulyaxAge +=1				# global update
		break
	end
end

println(JulyaxAge)			# 2

begin
	local JulyaxAge = 8
	JulippsNum = 7				# local def
	while (JulyaxAge != 0)
		JulyaxAge -= 1
		print(".")					# ........
	end
	println("\n", JulyaxAge) # 0 
end
println(JulyaxAge)			# 2
println(JulippsNum)			# 7
