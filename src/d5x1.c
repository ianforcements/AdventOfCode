/******************************************************************************
*	Advent of Code
*	Day 5, Exercise 1
*	By Ian Hutchinson
*	ian.hutchinson@gmail.com
*
*	This code will:
*	For a command line argument, repeatedly compute the MD5 digest of that
*	argument with an increasing digest index until LONG_MAX is reached
*	every time the first five characters of the digest equal 00000, the
*	sixth character will be added to the output string.
*	This gives the password needed for the day's first exercise.
*	
*	Compile note:
*	be sure to pass -lcrypto and -lssl to the compiler
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <openssl/evp.h> // provides SSL functions
#include <limits.h>	// I do some big loops here and use limits to prevent infinite looping
#include <assert.h>	// asserts are used for validation

int main(int argc, char *argv[])
{
	assert(argc = 1);
	int argLength = snprintf(NULL, 0, "%s", argv[1]);
	assert(argLength != 0);	
	char doorID[argLength];
	strncpy(doorID, argv[1], argLength);
	doorID[argLength] = '\0';
	int doorIDLength = strlen((char*)doorID);
	unsigned long hashIndex = 0; // combined with door ID
	const long maxLongStringLength = snprintf(NULL, 0, "%ld", LONG_MAX)+1; // how many decimal characters are needed to represent LONG_MAX
	char hashIndexString[maxLongStringLength];	// we will need a string representation of the index
	char password[9] = "";			  	// we are given the password length
	// init MD5 hashing apparatus
	//char md5Hash[EVP_MAX_MD_SIZE] = "";		 // readable output string
	EVP_MD_CTX *mdctx;				 // message digest context, passed to the hashing functions
	const EVP_MD *md;				 // the message digest type, fed to EVP_get_digestbyname
	unsigned char md_value[EVP_MAX_MD_SIZE];	 // the actual output of the digest functions
	unsigned int md_len;				 // length of md_value
	
	OpenSSL_add_all_digests(); // adds all the possible digest types
	md = EVP_get_digestbyname("md5"); // the exercise specifies that we use MD5 today
	printf("Cracking password. Please wait...\n");
	
	// the cracking loop
	for (hashIndex = 0; hashIndex < LONG_MAX; hashIndex++) {
		// find the str representation of hashIndex
		// first check that the output would be greater than 0 characters, and if it is then proceed
		// this level of validation isn't really necessary for this exercise but it is good practice
		int cnt = snprintf(NULL, 0, "%lu", hashIndex);
		assert(cnt > 0);
		snprintf(hashIndexString, cnt+1, "%lu", hashIndex);
		
		mdctx = EVP_MD_CTX_create(); // create the digest context

		EVP_DigestInit_ex(mdctx, md, NULL); // initialise digest
		EVP_DigestUpdate(mdctx, doorID, doorIDLength); // pass data, this can be called multiple times
		EVP_DigestUpdate(mdctx, hashIndexString, cnt); // we concatenate the hash index
		EVP_DigestFinal_ex(mdctx, md_value, &md_len); // finalise, output variables are passed now
		EVP_MD_CTX_destroy(mdctx); // clean up
		
		// now we have computed the hash, perform a test on the hash result
		// if the first five characters are 0, then the next character
		// is the next character of the password

		if((md_value[0] == 0) && (md_value[1] == 0) && (md_value[2] < 16)) {
			password[strlen(password)+1] = '\0';
			sprintf(&password[strlen(password)], "%01x", md_value[2]);
			printf("Password:\t%s\n", password);
			if (strlen(password) == 8) {
				printf("Password length reached!\n");
				break;
			}
		}
	
	}
	/* call this once before exit. */
	EVP_cleanup();


	printf("Input:\t%s\n", doorID);
	printf("Output:\t%s\n", password);
	

	return 0;
}
