/*
 ** Do a zoom out in a Tipsy movie.
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

void main(int argc, char **argv) {
	double x0,y0,z0,delta1,delta2;
	double t,dt;
	int iFrame,i;
//	char macro[256];
//	char tipsyfile[256];

	FILE *fp = NULL;
	
	/* Check command line parameters */
	if (argc != 6)
	{
			printf("Usage: zoom <tipsymacro.macro> <tipsyfiles> <delta_start> <delta_end> <iFrame>\n");
			exit(1);
	}

//	macro = argv[1];
//	tipsyfile = argv[2];
	delta1 = atof(argv[3]);
	delta2 = atof(argv[4]);
	iFrame = atoi(argv[5]);

	//sprintf(achFile,"%s.log",msrOutName(msr));
	fp = fopen(argv[1],"w");
	assert(fp != NULL);

	x0 = 0.0;
	y0 = 0.0;
	z0 = 0.0;
	dt = 0.001;
	t = iFrame*dt;

	/* Print header */
	fprintf(fp,"movie\n");
	fprintf(fp,"psize gas 0\n");
	
	fprintf(fp,"openb %s.%.5i\n",argv[2],iFrame);
	fprintf(fp,"loads 0\n");
	fprintf(fp,"redshift 0 physical 0 1 0 vacuum 1\n");
	fprintf(fp,"xall\n");
	fprintf(fp,"readarray %s.%.05i.den\n",argv[2],iFrame);

	delta1 += 1.0;
	i = 1;
	while (delta1 <= delta2)
	{
		fprintf(fp,"setbox 1 %g %g %g %g %g %g\n", x0, y0, z0, delta1, delta1, delta1);
		fprintf(fp,"abox 1\n");
		fprintf(fp,"zarray gas revrain 0 22\n");
//		fprintf(fp,"label 790 10 7 t:%g\n",t);
		fprintf(fp,"hard movie out.xwd\n");
		fprintf(fp,"shell xwdtopnm <out.xwd | pnmtopng > %s.%.05i.%.03i.png\n", argv[2], iFrame, i);
		
		delta1 += 1.0;
		i++;
	}
	
	fprintf(fp,"closeb\n");
	fprintf(fp,"end\n");
	
	fclose(fp);
}
