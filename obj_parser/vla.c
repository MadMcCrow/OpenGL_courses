//--------------------------------------------------------------------
// This code allow easy use of array with cool complexity.
// It is meant for array too big to sit on the stack,
// while not reallocating every single time
// Very nice indeed.
// /!\ this code is meant for C99 or C11 only (add -std=99 to GNU-GCC)
//--------------------------------------------------------------------
#ifndef STD_LIB
#define STD_LIB
//#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#endif

// allocation failure:
void mem_issue(void)
{
	printf("go fuck yourself, you and your stupid machine");
	exit(EXIT_FAILURE);
}

//
// floating point variable length array:
//
typedef struct { float* array;  int fill; int length; }fVLA;

fVLA fvla_create(int length)
{
	if (length <= 0) length = 1;
	fVLA tab;
	tab.array = malloc(sizeof(float) * length);
	if (tab.array == NULL)
		 mem_issue();
	tab.length = length;
	tab.fill = 0;
	return tab;
}

fVLA fvla_increase(fVLA tab)
{
	tab.array = realloc(tab.array, 2 * sizeof(float) * tab.length);
	if (tab.array == NULL){
	mem_issue();
	}else
	{
	tab.length = 2 * tab.length;
	}
	return tab;
}

fVLA fvla_decrease(fVLA tab)
{
	int q = 1 + (((sizeof(float) * tab.length) - 1) / 2); // worst case : a bit in excess.
	tab.array = realloc(tab.array, q );
	tab.length = 1 + ((tab.length - 1) / 2);
	if (tab.array == NULL) 
		mem_issue();
	return tab;
}

fVLA fvla_refresh(fVLA tab, float level)
{
	fVLA copy = fvla_create(tab.length);
	
	for ( int i = 0; i<= tab.fill; i++)
	{
	copy.array[i] = tab.array[i];
	}
	
	free(tab.array);
	if (tab.fill >= level * tab.length )
	{
	 tab.array = malloc( sizeof(float) * tab.length * (2-level) +1);
	}else{
	 tab.array = malloc( sizeof(float) * tab.length * (2-level) +1);
	}
	if (tab.array == NULL )
	{
	free (tab.array);
	tab.array = copy.array;
	fputs("optimizing failed; fallback to original data",stderr);
	}
	return tab;
}

fVLA fvla_fill(fVLA tab, float val)  // we add val to the end of the VLA
{	
	if (tab.fill > tab.length - 1) {
		tab = fvla_increase(tab);
	}
	
	tab.array[tab.fill] = val;
	tab.fill++;
	return tab;
}

float* fvla_export(fVLA tab, float* export)
{
	for (int i = 0; i < tab.fill; i++) 
	{
		export[i] = tab.array[i];
	}
	return export;
}

int fvla_delete(fVLA tab)
{
	free(tab.array);
	tab.array = NULL;
	return 0;
}


//
// integer variable length array:
//
typedef struct { int* array;  int fill; int length; }iVLA;

iVLA ivla_create(int length)
{
	if (length <= 0) length = 1;
	iVLA tab;
	tab.array = malloc(sizeof(int) * length);
	if (tab.array == NULL)
		 mem_issue();
	tab.length = length;
	tab.fill = 0;
	return tab;
}

iVLA ivla_increase(iVLA tab)
{
	tab.array = realloc(tab.array, 2 * sizeof(int) * tab.length);
	if (tab.array == NULL) {
	mem_issue();
	}else
	{
	tab.length = 2 * tab.length;
	}
	return tab;
}

iVLA ivla_decrease(iVLA tab)
{
	int q = 1 + (((sizeof(int) * tab.length) - 1) / 2); 
	tab.array = realloc(tab.array, q );
	tab.length = 1 + ((tab.length - 1) / 2);
	if (tab.array == NULL) 
		mem_issue();
	return tab;
}

iVLA ivla_refresh(iVLA tab, float level)
{
	iVLA copy = ivla_create(tab.length);
	
	for ( int i = 0; i<= tab.fill; i++)
	{
	copy.array[i] = tab.array[i];
	}
	
	free(tab.array);
	if (tab.fill >= level * tab.length )
	{
	 tab.array = malloc( sizeof(int) * tab.length * (2-level) +1);
	}else{
	 tab.array = malloc( sizeof(int) * tab.length * (2-level) +1);
	}
	if (tab.array == NULL )
	{
	free (tab.array);
	tab.array = copy.array;
	fputs("optimizing failed; fallback to original data",stderr);
	}
	return tab;
}

iVLA ivla_fill(iVLA tab, int val)  // we add val to the end of the VLA
{	
	if (tab.fill > tab.length - 1) {
		tab = ivla_increase(tab);
	}	
	tab.array[tab.fill] = val;
	tab.fill++;
	return tab;
}


int* ivla_export(iVLA tab, int* export)
{
	for (int i = 0; i < tab.fill; i++) export[i] = tab.array[i];
	return export;
}

int ivla_delete(iVLA tab)
{
	free(tab.array);
	tab.array = NULL;
	return 0;
}




