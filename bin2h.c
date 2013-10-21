// converts binary file to c code, such as a header file
// copyright Chris Lomont 1994
#include <stdio.h>
#include <string.h>

void main(int argc, char * argv[])
	{
	FILE * infile, * outfile;
	int count=0;
	char fname[300],*p,data_name[100];
	unsigned char ch;

	if (argc < 2)
		{
		printf("Usage: bin2h infile [outfile.inc] [data name]\n");
		printf("Converts binary data to hex c code\n");
		return;
		}
	infile = fopen(argv[1],"rb");
	if (infile == NULL)
		{
		printf("Cannot open %s\n",argv[1]);
		return;
		}

	if (argc > 2)
		strcpy(fname,argv[2]);
	else
		{
		strcpy(fname,argv[1]);
		p = strchr(fname,'.');
		if (p) *p = 0;
		strcat(fname,".inc");
		}
	if (argc == 4)
		strcpy(data_name,argv[3]);
	else
		{
		strcpy(data_name,fname);
		p = strchr(data_name,'.');
		if (p) *p = 0;
		}
	outfile = fopen(fname,"wt");

	fprintf(outfile,"// data file from binary %s\n\n",argv[1]);
	fprintf(outfile,"unsigned char %s[] = {\n\t",data_name);
		
	while (!feof(infile))
		{
		ch = fgetc(infile);
		fprintf(outfile,"0x%02x,",ch);
		count++;
		if ((count & 15) == 0)
			fprintf(outfile,"\n\t");
		}

	fprintf(outfile,"\n\t};\n\n// end of %s\n",fname);

	fclose(infile);
	fclose(outfile);
	} // main


// end - bin2h.c

