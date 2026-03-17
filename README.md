# nsc - No-Semicolon C

Write C without semicolons. `nsc` adds them automatically and compiles your code via gcc.

This is a proof of concept, not a production tool. Built to explore how preprocessors work at a low level.

```c
#include <stdio.h>

int main() {
    int x = 10
    int y = 20
    printf("%d", x + y)
    return 0
}
```

```bash
$ ./nsc testfile.nsc
Compiled. Run with: ./testfile
$ ./testfile
30
```

## Build

```bash
gcc nsc.c -o nsc
```

## Usage

```bash
./nsc <filename>.nsc
```

The output binary gets the same name as your input file - `hello.nsc` compiles to `./hello`.

## How it works

1. Reads your `.nsc` file line by line
2. Decides if each line needs a semicolon
3. Writes the corrected code to a hidden temp file `/tmp/nsc_temp.c`
4. Compiles it with gcc
5. Deletes the temp file - only your binary remains

### Semicolon rules

A line gets `;` unless it:
- is blank
- ends with `{ } , : ;`
- starts with `#`, `//` or `/*`
- starts with a keyword like `if`, `for`, `while`, `struct` etc.

---

## Dilemmas

**`MAX_LINE 4096`** - size of one memory page on Linux, standard convention for line buffers in C.

**`sizeof(wiersz)` vs `MAX_LINE` in fgets** - `sizeof` automatically adjusts if the array changes. `MAX_LINE` hardcoded would need manual updates everywhere.

**Is `kopia` necessary?** - yes, because `const char *wiersz` promises no modification. Trimming whitespace requires a local copy.

**`int` vs `size_t` for keyword length** - `strlen` returns `size_t` but `int` is fine here since no keyword overflows it. Chose `int` for simplicity.

**`sizeof/sizeof` vs `NULL` sentinel** - avoids a dummy `NULL` in the array, loop condition is explicitly numeric.

**`temp` file architecture** - modifying the original risks data loss. Asking for `-o` adds friction. Hidden temp file + auto-naming from input for the cleanest UX.

**`snprintf` with `sizeof(bash)`** - builds the gcc command string safely. `sizeof(bash)` prevents buffer overflow if the filename is long. Used `MAX_LINE * 2` because the command prefix alone takes 23 chars.

---
## Notes to my future self

- **Block comments** - `/* */` across multiple lines breaks detection. Need `in_comment` state in `main`.
- **Colored output** - ANSI codes for red errors, green success.
- **`--disp-only`** - print generated `.c` without compiling.
- **Pass flags to gcc** - `./nsc file.nsc -Wall -O2`

---

## License


This project is licensed under the [MIT](https://opensource.org/licenses/MIT) License.
