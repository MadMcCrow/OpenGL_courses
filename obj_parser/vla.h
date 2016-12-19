//--------------------------------------------------------------------
// This is a library of function to handle vla-like data on the heap
// Very nice indeed.
// /!\ this code is meant for C99 or C11 only (add -std=99 to GNU-GCC)
//--------------------------------------------------------------------
#ifndef VLA_H
#define VLA_H

// allocation failure procedure. ignore.
void mem_issue(void);


// floating point variable length array:
typedef struct { float* array;  int fill; int length; }fVLA;

// generator.
fVLA fvla_create(int length);

// resize -up.
fVLA fvla_increase(fVLA tab);

// resize -down - not used.
fVLA fvla_decrease(fVLA tab);

// re-optimize :
// level is between 0 an 1. it define the margin level you want.
fVLA fvla_refresh(fVLA tab, float level);

// add val to the end of the VLA.
fVLA fvla_fill(fVLA tab, float val);

//export to a simple float array.
float* fvla_export(fVLA tab, float* export);

//destructor.
int fvla_delete(fVLA tab);


// integer variable length array:
typedef struct { int* array;  int fill; int length; }iVLA;

// generator.
iVLA ivla_create(int length);

// resize -up.
iVLA ivla_increase(iVLA tab);

// resize -down - not used.
iVLA ivla_decrease(fVLA tab);

// re-optimize :
iVLA ivla_refresh(iVLA tab, int level);

// add val to the end of the VLA.
iVLA ivla_fill(iVLA tab, int val);

//export to a simple int array.
int* ivla_export(iVLA tab, int* export);

// destructor.
int ivla_delete(iVLA tab);

#endif
