# presents implicit declaration, strings,
# functions, and control flow
str = """
Julipp's got triple-quoted strings
so that using regular "quotes" is
somewhat easier."""
single = "single quoted"
if (length(str) > 10)
	println(str) # prints ^
else; println(single)
end

while (length(single) < 16)
	single *= "\"" # string concatenation
end
print(single) # single quoted"""
