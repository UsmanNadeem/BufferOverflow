#! /bin/bash
# a=1
# for i in `seq 1 90`; do
#   new=$(printf "vtable%d.cpp" "$a")
#   cp vtable.cpp "$new"
#   let a=a+1
# done

# rename from vtable to RIPEalpha
a=1
for i in `seq 1 90`; do
  old=$(printf "vtable%d.cpp" "$a")
  new=$(printf "RIPEalpha%d.cpp" "$a")
  mv "$old" "$new"
  let a=a+1
done


# to compile
# a=1
# for i in `seq 1 90`; do
#   new=$(printf "vtable%d.cpp" "$a")
#   compiledName=$(printf "vtable%d.out" "$a")
#   clang++ "$new" -m32 -o "$compiledName"
#   let a=a+1
# done

# run
# a=1
# for i in `seq 1 90`; do
#   compiledName=$(printf "vtable%d.out" "$a")
#   ./"$compiledName" d
#   let a=a+1
# done
