print "
multi
line?
"

print "hi" * " bye"
print "lo" * 6 * "l"

print("hi" != "h")
print("hi" == "hi")
print("" + 4 + 3 == "43")

local ml1 = "a
string here"

local ml2 = "a
string here"

print(ml1 == ml2)

print("hey" < "hi"); print("hey" > "hi")


local tripleQuoted = """
The reason I would use "triple quotes"
instead of the regular two double quotes \n-> ""
is because is can easily print " or "" using
triple quotes;
"""
print(tripleQuoted)
print("\\.\t.\n.\b.\n\'\v.\"")

local testStr = "string\r" * "just erased" * "\nright?"
print(testStr * "\b.\ngreat.\n\"it\" worked!")
