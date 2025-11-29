# os_tutorial

# WSL2

originally from https://github.com/cfenollosa/os-tutorial

## launch qemu under Windows while running WSL2

qemu-system-x86_64.exe -s -S \\wsl.localhost\Debian\home\user\devel\os\build\os-image.bin -boot c

* -s,-S  awaits a debugger 


##  Connect gdb debugger to qemu(on Windows)

ip route show | grep -i default | awk '{ print $3}'

gdb	-ex "target remote 172.26.112.1:1234" -ex "symbol-file build/kernel.elf"

##  Compile with debug symbols ex:(hello.c)

```
void dummy_test_entrypoint() {
}

void main() {
    char* video_memory = (char*) 0xb8000;
    *video_memory = 'X';
}

```


gcc -m32 -g -ffreestanding -fno-pie -c hello.c -o hello.o

objdump -d hello.o

```
Disassembly of section .text:

00000000 <dummy_test_entrypoint>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   90                      nop
   4:   5d                      pop    %ebp
   5:   c3                      ret

00000006 <main>:
   6:   55                      push   %ebp
   7:   89 e5                   mov    %esp,%ebp
   9:   83 ec 10                sub    $0x10,%esp
   c:   c7 45 fc 00 80 0b 00    movl   $0xb8000,-0x4(%ebp)
  13:   8b 45 fc                mov    -0x4(%ebp),%eax
  16:   c6 00 58                movb   $0x58,(%eax)
  19:   90                      nop
  1a:   c9                      leave
  1b:   c3                      ret
```

## gdb commands


Useful GDB commands
| Command | Description |
| --- | --- |
|run (r)		|Run the program until it finishes, crashes, or hits a breakpoint|
|start			|Run and pause at the first line of main()|
|next (n)		|Execute the next line (step over function calls)|
|step (s)		|Execute the next line (step into function calls)|
|stepi 			|Executes the next instcution|
|continue (c)	|Resume execution until the next breakpoint or crash|
|break file.hh:N or b file.hh:N	|Set a breakpoint at line N in file.hh|
|break func		|Set a breakpoint at function func()|
|delete			|Remove all breakpoints|
|print var or p var	|Print the value of variable val|
|list (l)		|Show source code around current line|
|info locals	|Show all local variables|
|quit (q)		|Exit GDB|

---　\
b main\
continue\
step

---\
stepi\
disassemble main\

set {char}0xb8000 = 0x42\
set {char}0xb8000 = 'G'

whatis *video_memory\
output: type = char