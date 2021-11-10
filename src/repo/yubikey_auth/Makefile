LIBS = -lcurl
CFLAGS = -g -o

all: clean validateOtp

# 	Skeleton code for validating a Yubikey OTP
# --- Flow:
# 	- User plugs in their Yubikey  
#	- User types "./validateOtp " 
#	- User presses Yubikey button
#	- The OTP is sent to the OTP validation server and validated
#	- The user is notified whether their key is valid or not

# 	Ultimate goal with AES encryption is to use the Yubikey OTP alongside
# 	a user-provided AES key to ensure the user is accessing the repository

#	Questions to resolve:
#	- How to tie a specific Yubikey to a person? Theoretically anyone who knows
#	the password and has a valid Yubikey can enter (Yubikey does not have to be owner's)
#
#	- Figure out the deprecation hell with Yubico API
#		- Do I need to use KSM or is the single HTTP get enough?
#		- Do I need to decrypt the OTP through another endpoint, or does the new
#			one HTTP format handle decryption
validateOtp: 
	g++ $(CFLAGS) validateOtp validateOtp.cc $(LIBS)

clean: 
	rm -f validateOtp