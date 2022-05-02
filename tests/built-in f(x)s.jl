# strlen
print("strlen")
print strlen("hey")
global str = """
We've got
ourselves a 
string!!!"""
print strlen(str)
another = "string" + 223
print strlen(str * another + 3)
# print strlen(78) # error

# sqrt
print("sqrt")
print(sqrt(36))
print(sqrt(12.1))
# print(sqrt("str")) # antoher error

# abs
print("abs")
print(abs(2.2))
print(abs(-18.13))
# print(abs("a string")) # err

# floor
print("floor")
print(floor(sqrt(31)))
print(floor(-34.3))
# print(floor()) # err

# ceil
print("ceil")
print(ceil(sqrt(31)))
print(ceil(-34.3))
print(ceil(2.2))

# round
print("round")
print(round(sqrt(31)))
print(round(-34.5))
print(round(2.6))
print(round(8.4))


