/*
 	Samuel Woodruff
	CS4760 Project_1
	1/27/19
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include<ctype.h>

int nextNumber(FILE *fp, FILE *fp2);
void appendPids(FILE *ofp, int forks);
void reverseNumbers(char *input, char *output, char *projectFile);
int main(int argc, char *argv[]){
	
	//process command line arguments
	int c;
	char *input = NULL;
	char *output = NULL;
	
	while( (c=getopt(argc, argv, "o:hi:")) != EOF)
	{
		//getopt arguments
		switch(c){
			case 'h':
				printf("\n\nProject 1 Options");
				printf("\n---------------------------------------------------------------\n");
				printf("	-i inputfilename \n");
				printf("	-o outputfilename \n");
				printf("	If no inputfilename is specified, then input.dat\n\t"
				"will be the default. If no outputfilename is specified,\n\t"
				"then output.dat will be the default.\n\n");
				break;
			case 'i':
				input = optarg;	
				break;
			case 'o':
				output = optarg;
				break;
		}
	}
	//set default files if none specified
	if(input == NULL)
	{
		input = "input.dat";
	}
	if(output == NULL)
	{
		output = "output.dat";
	}

	char *projectFile = argv[0];
	//Reverse numbers
	reverseNumbers(input,output, projectFile);	
 
	printf("Row reversal successful\n");
	return 0;
}

void reverseNumbers(char *input, char *output, char* projectFile){
	//Open inputfile
	FILE *fp;
	FILE *fp2;
	fp = fopen(input, "r");
	fp2 = fopen(input, "r");	
	if(fp == NULL || fp2 == NULL)
	{
		fprintf(stderr, "%s: Error in %s: ", projectFile, input);
		perror("");
		exit(0);
	}
	
	//Open outputfile
	FILE *ofp;
	ofp = fopen(output, "w");
	if(ofp == NULL){
		fprintf(stderr, "%s: Error in %s: ", projectFile, output);
		perror("");
		exit(0);
	}

	//open temp file to write to
	FILE *temp = fopen("temp","w");
	if(temp == NULL){
		fprintf(stderr, "%s: Error in TEMP file: ", output);
		perror("");
		exit(0);
	}	
	//number of forks to perform
	int forks = nextNumber(fp, fp2);
	
	//starts processes
	int count;
	int k;
	int n;

	for(k = 0; k < forks; k++){
		if(fork() == 0){
		
			//Get number of elements and store in array	
			count = nextNumber(fp,fp2);
			int numbers[count];
			for(n = 0; n < count; n++){
				numbers[n] = nextNumber(fp,fp2);
			} 
				
			//write pid to files
			fprintf(ofp, "%d: ", getpid());
			fprintf(temp,"%d ", getpid());
			
	
			//write numbers to file
			for(k = count - 1 ; k >= 0; k--){
				fprintf(ofp, " %d", numbers[k]);
			}
			fprintf(ofp, "\n");
			exit(0);
		}
		
		//Move file pointers ahead
		count = nextNumber(fp,fp2);
		int m;
		for(m = 0; m < count; m++){
			int move = nextNumber(fp,fp2);
		}
	}

	fclose(temp);
	fclose(fp);
	fclose(fp2);
	
	/*
 *	Without large for loop the forked processes
 *	wouldnt finish fast enough for me. If you remove this the list of child pids
 *	and parent id will display in the middle of the reversed rows.
  	*/
	int t;
	for(t = 0; t < 1000000; t++){

	}	
	//insert pids at end of file
	appendPids(ofp, forks);	

	fclose(ofp);

	//remove temporary file
	remove("temp");
}

void appendPids(FILE *ofp, int forks){
	FILE *pids = fopen("temp","r");
	fprintf(ofp, "%s", "All childern were: ");
	char ch = fgetc(pids);
	while(ch != EOF)
	{
		fputc(ch, ofp);
		ch = fgetc(pids);
	} 
	
	
	fprintf(ofp, "%s", "\nThe parent process was: ");
	fprintf(ofp, "%d" , getpid());
	fprintf(ofp, "%s", "\n");

	fclose(pids);
}

int nextNumber(FILE *fp, FILE *fp2){
	int digCount = 0;
	char ch;
	ch = fgetc(fp);

	//Get number of digits
	while(ch != '\n' && ch != ' '){
		digCount++;
		ch = fgetc(fp);
	} 
	
	if(digCount == 0){
		fprintf(stderr,"\nFile is not formatted correctly\n");
		remove("temp");
		exit(0);
		printf("AFTER EXIT");
	}
	else{
		char temp[digCount];
	
		//put into array using second file pointer
		ch = fgetc(fp2);
	
		int k;
		for(k = 0; k < digCount; k++)
		{
			temp[k] = ch;
			ch = fgetc(fp2);
		}
	
		int num = atoi(temp);
		return num;
	}
	
}
