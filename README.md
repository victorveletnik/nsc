# nsc — No-Semicolon C

Write C without semicolons. `nsc` adds them for you and compiles with gcc.

## Build
gcc nsc.c -o nsc

## Usage
./nsc <file.nsc> [-o output]

## Example
int main() {
    printf("hello \n")
    return 0
}

$ ./nsc hello.nsc -o hello
