#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int calculate_pw_hash(char* s, int len)
{
	int result = 0;
  	for (int i = 0; i < len; i++){
    	result = (result ^ ((unsigned int)s[i] + ((unsigned int)s[i] << 8))) << 1;
  	}
  	if (len > 0)
    	result = result | s[0];
  	return result ^ 0xfeedface;
}

int main(int argc, char** argv)
{	
	if (argc != 2) {
		printf("Usage: ./hash arg1\n");
		return 0;
	}
	printf("%#010x\n", calculate_pw_hash(argv[1], strlen(argv[1])));
}



