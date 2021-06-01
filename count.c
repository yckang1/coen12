/* Yicheng Kang
 * COEN12 T 2:15pm
 * Lab 1
 */

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#define MAX_WORD_LENGTH 30

void numberofWords(char *file);

int main(int argc, char *argv[]) {
	if(argc==1) {
		printf("Missing file!\n");
		return 1;
	}
	numberofWords(argv[1]);
}

void numberofWords(char *file) {
	FILE *fp;
	int counter=0;
	char filename[MAX_WORD_LENGTH];

	fp = fopen(file, "r");

	//Checks if the file was inserted
	if(fp==NULL) {
		printf("There is no file.\n");
		return;
	}
	//Reads the number of words and increments the counter
	while(fscanf(fp,"%s",filename)==1) {
		counter++;
	}

	//Closes the file and prints out the counter
	fclose(fp);
	printf("There are %d total words.\n", counter);
	return;
}
