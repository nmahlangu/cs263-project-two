Overview
========
password_cracker.py takes in 2 files: usernames.txt and john.txt. The former
is a list of usernames to crack passwords for, the latter is a dictionary of
passwords (in this case from John The Ripper). Basically this python script
reads in each username from usernames.txt and sends a GET request (in the same
format the website login of ChatMax sends one) w/ every single password until
one works or there are no more passwords for each username. As it runs it
reports the cracked passwords of usernames to STDOUT, as well as logging it
to a file called password_cracker_output.txt.
Note: This code attempts to crack the password for EVERY SINGLE USERNAME in 
password.db and successfully crackes 431/489 in 52.29 minutes. 

Instructions
============
Place password_cracker.py, usernames.txt, and john.txt in the same directory.
Then execute python password_cracker.py.