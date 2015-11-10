# creates a pretty printed password table
from Crypto.Cipher import AES
import struct
import binascii

key = "3B99C216F1AE2DD69B70F5E800FC9AEC".decode("hex")
mode = AES.MODE_ECB

decryptor = AES.new(key, mode)

filename = "password.db"
with open(filename) as f: 
  content = binascii.hexlify(f.read())

target_file = "plaintext-password-table.txt"
target = open(target_file, "w")
for i in range(0, len(content), 32) :
  line = content[i: (i + 32)]
  decrypted_line = decryptor.decrypt(line.decode("hex"))
  hex_line = [hex(ord(n)).lstrip("0x") for n in decrypted_line]
  hash_pass = '0x' + "".join(hex_line[-4:])

  username = ""
  for i, char in enumerate(decrypted_line) :
    if hex_line[i] == '' :
      break
    username += char

  target_line = username + ", " + hash_pass + "\n"
  target.write(target_line)
