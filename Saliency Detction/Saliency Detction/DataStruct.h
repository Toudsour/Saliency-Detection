struct Sparam
{
	int L;
	double constraint;   //lambda1
	double lambda2;
	constraint_type mode;
	bool pos;
	bool ols;
	int numThreads;
	Sparam()
	{
		L=MAXSTEP;
		constraint=0.15;
		lambda2=0;
		mode=PENALTY;
		pos=false;
		ols=false;
		numThreads=4;
	}
};