a = 0; global temp

b = 1
while true
  print a
  temp = a
  a = b
	b = temp + b
  if a > 2000
	break
  end
end

# should count down from 9
count = 20
while count != 0
	count = count - 1
	if count > 9
		continue
	end
	print count
end
print "Blastoff" / ""
