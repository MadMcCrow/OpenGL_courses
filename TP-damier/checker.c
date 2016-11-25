// Module de création du checker
// David Schreck$

#ifndef GL_included
#include "glutils.h"
#include "glmath.h"
#endif

#include "stdio.h"
#include "stdlib.h"

typedef int face[3];

int main()
{
    printf("Largeur checker tx ?");
	int tx = 0;
    scanf("%d", &tx); 
	printf("Largeur checker ty ?");
	int ty =0;
	scanf("%d", &ty); 
	vec3* v = malloc ((tx+1)*(ty+1)*sizeof(vec3));
	int pos = 0;
	for (int i =0; i<tx+1; i++)
	{
		for (int j=0;j<ty+1;j++)
		{
        v[pos].row[0]= (float)i;
        v[pos].row[1]= (float)j;
        v[pos++].row[2]=  0.0;
        for (int t = 0; t<3; t++)
            printf("%.1f  ",v[pos-1].row[t]);
        printf("\n");
		}
		
	}
	
    face *f = malloc ((tx)*(ty)*sizeof(vec3));
	pos = 0;
#if 0
	for (int i=0; i<(tx)*(ty);i++)
	{
        // triangle ABC A=A B=A+1 C=A+Tx
        f[pos][0] = i;
        f[pos][1] = i+1;
        f[pos][2] = i+tx+1;
        pos++;
        int count=0;
        for (int t = 0; t<3; t++)
            printf("%d  ",f[pos-1][t]);
        printf("\n");
        
        // triangle BCD B=A+1 C=A+Tx D= A+Tx+1
        f[pos][0] = i+1;
        f[pos][1] = i+tx+1;
        f[pos][2] = i+tx+2;
        pos++;
        count++;
        if (count==tx)
        {
            count=0;
            i++;
        }
        
        for (int t = 0; t<3; t++)
            printf("%d  ",f[pos-1][t]);
        printf("\n\n");

	}
#endif
int i = 0;
while (i<(tx*ty)+1)
{
    //
     if ((i+1)%(tx+1) == 0){
            printf("jumped: i = %d, pos=%d \n",i, pos);
            i++;}
    // triangle ABC A=A B=A+1 C=A+Tx
        f[pos][0] = i;
        f[pos][1] = i+1;
        f[pos][2] = i+tx+1;
        pos++;
        for (int t = 0; t<3; t++)
            printf("%d ",f[pos-1][t]);
        printf("\n");
        
        // triangle BCD B=A+1 C=A+Tx D= A+Tx+1
        f[pos][0] = i+1;
        f[pos][1] = i+tx+1;
        f[pos][2] = i+tx+2;
        pos++;
          for (int t = 0; t<3; t++)
            printf("%d ",f[pos-1][t]);
        printf("\n\n");
       
        i++;
}


	//charger dans le VBO
	
	free(v);
}
