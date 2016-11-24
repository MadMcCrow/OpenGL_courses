// Module de création du checker
// David Schreck

void checker()
{
	int tx = magic();
	int ty = magic();
	v= malloc (tx*ty*6*3*sizeof*v)
	int pos = 0;
	for (int i; i<tx; ++i)
	{
		for (int j;j<ty;++j)
		{
		v[pos++]=i;
		v[pos++]=j;
		v[pos++]=0.0;

		v[pos++]=i;
		v[pos++]=j+1;
		v[pos++]=0.0;
		
		v[pos++]=i+1;
		v[pos++]=j;
		v[pos++]=0.0;

		v[pos++]=i;
		v[pos++]=j+1;
		v[pos++]=0.0;

		v[pos++]=i+1;
		v[pos++]=j;
		v[pos++]=0.0;

		v[pos++]=i+1;
		v[pos++]=j+1;
		v[pos++]=0.0;		

		}
	}
	//charger dans le VBO
	
	free(v);
}
