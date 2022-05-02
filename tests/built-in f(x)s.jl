# strlen
println("strlen")
println(strlen("hey"))
global str = """
We've got
ourselves a 
string!!!"""
println(strlen(str))
another = "string" + 223
println(strlen(str * another + 3))
# println strlen(78) # error

# sqrt
println("sqrt")
println(sqrt(36))
println(sqrt(12.1))
# println(sqrt("str")) # antoher error

# abs
println("abs")
println(abs(2.2))
println(abs(-18.13))
# println(abs("a string")) # err

# floor
println("floor")
println(floor(sqrt(31)))
println(floor(-34.3))
# println(floor()) # err

# ceil
println("ceil")
println(ceil(sqrt(31)))
println(ceil(-34.3))
println(ceil(2.2))

# round
println("round")
println(round(sqrt(31)))
println(round(-34.5))
println(round(2.6))
println(round(8.4))


