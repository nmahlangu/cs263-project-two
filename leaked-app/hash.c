#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
   Reverse engineering
   ===================
   Below is the assembly code for the function calculate_pw_hash.
   To get this assembly code, do:
   	- run proton (./proton 8080 .)
   	- use ps to get the process id of the server (ps -Af | grep proton)
   	- use gdb on that process id (sudo gdb -p X, where X is the process id)
   	- set gdb to attach to child process (set follow-fork-mode child)
   	- set a breakpoint at calculate_pw_hash (b calculate_pw_hash)
   	- Continue
   	- On the web application, enter a valid username and any password
   	- view the assembly of the function (layout asm)

   Final step was to convert this assembly code to C code. Note: my code
   reverses the order of the bytes.
   My comments for doing so are below.

   Assembly
   ========
   0x8048c1e <calculate_pw_hash+1>         mov    %esp,%ebp                                  
   0x8048c20 <calculate_pw_hash+3>         sub    $0x10,%esp

   // make space for local variables on the stack 
   0x8048c23 <calculate_pw_hash+6>         movl   $0x0,-0xc(%ebp)                            
   0x8048c2a <calculate_pw_hash+13>        movl   $0x0,-0x8(%ebp)                            
   0x8048c31 <calculate_pw_hash+20>        jmp    0x8048c5b <calculate_pw_hash+62>   

   // for loop used to iterate over the length of the string
   // get char value at index i of string and store it at -0x4(%ebp) 
   0x8048c33 <calculate_pw_hash+22>        mov    -0x8(%ebp),%edx                            
   0x8048c36 <calculate_pw_hash+25>        mov    0x8(%ebp),%eax
   0x8048c39 <calculate_pw_hash+28>        add    %edx,%eax
   0x8048c3b <calculate_pw_hash+30>        movzbl (%eax),%eax                                
   0x8048c3e <calculate_pw_hash+33>        movsbl %al,%eax           # cast char to unsigned int
   0x8048c41 <calculate_pw_hash+36>        mov    %eax,-0x4(%ebp)    # store char in -0x4(%ebp) 
   0x8048c44 <calculate_pw_hash+39>        mov    -0x4(%ebp),%eax
   // add char value to (char value << 8)
   0x8048c47 <calculate_pw_hash+42>        shl    $0x8,%eax          # shift eax left by 8 bits
   0x8048c4a <calculate_pw_hash+45>        mov    %eax,%edx          # put eax into edx
   0x8048c4c <calculate_pw_hash+47>        mov    -0x4(%ebp),%eax    # move local3 into eax
   0x8048c4f <calculate_pw_hash+50>        add    %edx,%eax          # add
   // increment counter and double result 
   0x8048c51 <calculate_pw_hash+52>        xor    %eax,-0xc(%ebp)    # xor (add result) w/ local1
   0x8048c54 <calculate_pw_hash+55>        shll   -0xc(%ebp)         # double local1
   0x8048c57 <calculate_pw_hash+58>        addl   $0x1,-0x8(%ebp)    # add 1 to local2

   // function is something like f(char* s, int len)
   // do a for loop that will execute `len` times
   0x8048c5b <calculate_pw_hash+62>        mov    -0x8(%ebp),%eax                            
   0x8048c5e <calculate_pw_hash+65>        cmp    0xc(%ebp),%eax                             
   0x8048c61 <calculate_pw_hash+68>        jl     0x8048c33 <calculate_pw_hash+22>

   // if `len` > 0, xor the result with the first character of the string        
   0x8048c63 <calculate_pw_hash+70>        cmpl   $0x0,0xc(%ebp)                             
   0x8048c67 <calculate_pw_hash+74>        jle    0x8048c75 <calculate_pw_hash+88>       
   0x8048c69 <calculate_pw_hash+76>        mov    0x8(%ebp),%eax                             
   0x8048c6c <calculate_pw_hash+79>        movzbl (%eax),%eax                                
   0x8048c6f <calculate_pw_hash+82>        movsbl %al,%eax                                   
   0x8048c72 <calculate_pw_hash+85>        or     %eax,-0xc(%ebp)   

   // return (result xor 0xfeedface)                         
   0x8048c75 <calculate_pw_hash+88>        mov    -0xc(%ebp),%eax                            
   0x8048c78 <calculate_pw_hash+91>        xor    $0xfeedface,%eax                           
   0x8048c7d <calculate_pw_hash+96>        mov    %eax,-0xc(%ebp)                            
   0x8048c80 <calculate_pw_hash+99>        mov    -0xc(%ebp),%eax                            
   0x8048c83 <calculate_pw_hash+102>       leave                                             
   0x8048c84 <calculate_pw_hash+103>       ret                                               
   0x8048c85 <logger>                      push   %ebp                                       
   0x8048c86 <logger+1>                    mov    %esp,%ebp                                  
   0x8048c88 <logger+3>                    push   %ebx
 */

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



