

1. If you set and then again set to that location, it simply "inserts" the new
value at that particular position. Does not OVERWRITE

2. If you are reading an array, send a pointer and a size variable. The read
function will allocate appropriate amount of memory and return the size in the
size variable.

3. Whenever setting an array, send the array and the number of elements in the
array.


4. To compile: type 'make segment.out'

