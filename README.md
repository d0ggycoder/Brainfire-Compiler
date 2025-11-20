# Brainfire Compiler

A simple C-based compiler for the Brainfire Esolang made by an idiot that has no idea what they're doing  
**NOTE: MUST HAVE GCC INSTALLED FOR IT TO FUNCTION PROPERLY**

## Brainfire Documentation:  

Brainfire is a simple programming esolang with highly limited functionality. It functions very similarly to a raw turing machine as it only implements the most basic and simple instructions.  

It works by holding an internal pointer to a "feedtape" of 128 bytes, which can each store a char value. The pointer can be shifted left and right, the values can be incremented and deincremented, and the contents can be read.  

Brainfire implements very simple code execution, with code being read sequentially. However, it does also implement simple loops which are marked by a start and end point; when the program reaches the end point, if the value currently being read on the feedtape is zero, it continues execution normally. Otherwise, if the value is non-zero, execution jumps back to the most recent loop start.

## Syntax:
 - (Shift right) `>` : shifts the value pointer right
 - (Shift left) `<` : shifts the value pointer left
 - (Increment) `+` : increments the value pointed to by the value pointer
 - (Deincrement) `-` : deincrements the value pointed to by the value pointer
 - (Read) `.` : prints the current value as an ascii character (i.e. if the value is 65, it will print 'a')
 - (Begin loop) `(` : Begins a loop (sets a point to jump back to)
 - (End Loop) `)` : Ends a loop (jumps back to the most recent begin loop if the value is non-zero) 
 - (Comment) `/` : Initiates or ends a comment
 - Whitespace (` `, `\n`,`\t`) is ignored by the compiler 

## Usage:
### Linux:
**WARNING LINUX IS CURRENLTY UNSUPPORTED WITH THE CURRENT VERSION AND WILL LIKELY NOT RUN**
If the brainfire executable does not already exist, then build it using  
`gcc internals/brainfire-linux.c -o brainfire-linux`  

1. To use, create a file with the file extension `.bf`, then type the following command:  
`./brainfire-linux [pathname].bf`  
2. This generates an intermediary assembly file and a compiled executable with the same path name as the source. For example, if the source file is `example.bf`, then after running `./brainfire-linux example.bf`, there will now be the files `example.s` and `example.exe` or some equivalents.
3. Then, you can execute the generated executable using `./example`

### Windows:
1. To use, create a file with the file extension `.bf`, then use the following command:  
`./compile.bat [file-name] -o [destination] {options}` 
2. Input the file name as the path of your `.bf` file and the destination as any name. See the -help option for any additional information on the command and other options
3. Execute the generated executable by doing `./[destination]`