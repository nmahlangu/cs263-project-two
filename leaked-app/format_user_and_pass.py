import sys

try:
	f = open("output.txt","r")
except:
	sys.exit("output.txt not found")

for line in f:
	line = line.replace("\n","")
	# found
	if line[0] == "P":
		words = line.split(" ")
		user = words[3]
		_pass = words[5]
		print user+"\t"+_pass

f.close()