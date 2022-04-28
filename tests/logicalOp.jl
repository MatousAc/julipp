var T = true
var F = false

if T || F
print "T or F is true"
else
print "T or F is false"
end
print "after end"

print F && F
print T || T
print T && F
print F || T

var mut = "original"
print mut
T || (mut = "new")
print mut
F && (mut = "newer")
print mut
F && (mut = "newer")
print mut
T && (mut = "newest")
print mut

