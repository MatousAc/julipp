# presents complext control flow and scope
br = false 							# global def
while (true)
	global JulyaxAge = 2	# global def
	JulippsNum = 7				# local def
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
println(JulippsNum)			# Variable not Defined
