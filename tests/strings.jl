println("
multi
line?
")

println("hi" * " bye")
println("lo" * 6 * "l")

println("hi" != "h")
println("hi" == "hi")
println("" + 4 + 3 == "43")

local ml1 = "a
string here"

local ml2 = "a
string here"

println(ml1 == ml2)

println("hey" < "hi"); println("hey" > "hi")


local tripleQuoted = """
The reason I would use "triple quotes"
instead of the regular two double quotes \n-> ""
is because is can easily print " or "" using
triple quotes;
"""
println(tripleQuoted)
println("\\.\t.\n.\b.\n\'\v.\"")

local testStr = "string\r" * "just erased" * "\nright?"
println(testStr * "\b.\ngreat.\n\"it\" worked!")
