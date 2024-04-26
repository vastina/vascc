#!/usr/bin/bash

filename="$1"
outputname="$2"

as "$filename" -o "$outputname.o"

ld -plugin /usr/libexec/gcc/x86_64-linux-gnu/13/liblto_plugin.so    \
-plugin-opt=/usr/libexec/gcc/x86_64-linux-gnu/13/lto-wrapper        \
-plugin-opt=-pass-through=-lgcc_s                                   \
-plugin-opt=-pass-through=-lgcc                                     \
-plugin-opt=-pass-through=-lc                                       \
-plugin-opt=-pass-through=-lgcc_s                                   \
-plugin-opt=-pass-through=-lgcc                                     \
--eh-frame-hdr                                                      \
-m elf_x86_64                                                       \
-dynamic-linker                                                     \
/lib64/ld-linux-x86-64.so.2                                         \
/usr/lib/x86_64-linux-gnu/crt1.o                                    \
/usr/lib/x86_64-linux-gnu/crti.o                                    \
/usr/lib/gcc/x86_64-linux-gnu/13/crtbegin.o                         \
-L/usr/lib/gcc/x86_64-linux-gnu/13                                  \
-L/usr/lib/gcc/x86_64-linux-gnu/13/../../../../lib64                \
-L/lib/x86_64-linux-gnu                                             \
-L/lib/../lib64                                                     \
-L/usr/lib/x86_64-linux-gnu                                         \
-L/usr/lib/gcc/x86_64-linux-gnu/13/../../..                         \
-lm -lgcc_s -lgcc -lc -lgcc_s -lgcc                                 \
/usr/lib/gcc/x86_64-linux-gnu/13/crtend.o                           \
/usr/lib/x86_64-linux-gnu/crtn.o                                    \
"$outputname.o"                                                     \
-o $outputname

rm -f "$outputname.o"
