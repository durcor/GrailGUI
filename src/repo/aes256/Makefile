LIBS 	= -lssl -lcrypto
CFLAGS	= -g -o	

all: clean aes_enc_dec

# 	TODO:
#  !! Publish for code review
#  Implement Triple-AES (K1(K2(K1(p))))
#  Add salting
#  Implement more secure key base requirement
#  Convert into a class  
#  Change error handling
#  Change digest from MD5 to SHA256

#	MAYBE:
#  base64 encoding

# encrypts/decrypts a file with AES256
# usage: 
# ./aes_enc_dec <input filename> <key> <mode (e/d)> <output filename> 
aes_enc_dec: 
	g++ $(CFLAGS) aes_enc_dec aes_enc_dec.cc $(LIBS)    


# Small example of encrypting a string using AES256
aes_enc_dec_str:
	g++ $(CFLAGS) aes_enc_dec_str aes_enc_dec_str.cc $(LIBS)

clean:
	rm -f openfile aes_enc_dec 

