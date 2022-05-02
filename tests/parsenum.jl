# getting input from the user
println("Enter multiplier")
multiplier = readline()
multiplier = parsenum(multiplier)
println("multiply input by ", multiplier)

print(">> ")
num = readline()
while num != "q" && num != "quit"
println(parsenum(num) * multiplier)
print(">> ")
num = readline()
end
