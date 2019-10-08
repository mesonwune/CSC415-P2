/***********************
 
 File: mtFileStats.c
 Compile: gcc mtFileStats.c -o mtFileStats -lpthread
 Run: ./mtFileStats [file1] [file2] ...
 
 Description: Similar to Unix utility wc
 [file1], [file2] etc are text files. mtFileStats counts the
 number of lines, words and characters in each file, reports
 the counts for each file, and total counts over all the files.
 
 ***********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define MAXCHARS 80

struct FileInfo {
    char *name;    /* name of file */
    int numLines;  /* number of lines in file */
    int numWords;  /* number of words in file */
    int numChars;  /* number of characters in file */
} fileInfo;

int countWords(char *);
struct FileInfo *info; /* array of counts for each file */
void *runThread(void *); /* run thread */
pthread_mutex_t lock; /* create a mutex lock for accumulating the numbers */
char inString[MAXCHARS];
char *rs;

int main(int argc, char **argv) {
    int numLines = 0, numWords = 0, numChars = 0; /* total counts */
    
    /* allocate array of structs containing counts for each file */
    info = (struct FileInfo *) malloc((argc-1) * sizeof(struct FileInfo));
    
    int numThread;
    pthread_t *thread;
    int  iret, i;
    
    /* set the numThread to the number of files */
    numThread = argc - 1;
    thread = (pthread_t *) malloc(numThread * sizeof(pthread_t));
    
    /* pass the IDs in to the structs */

    for (i=0; i<numThread; i++) {
        struct FileInfo* fileInformation = malloc(sizeof *fileInformation);
        fileInformation->name = (char *) malloc(20 * sizeof(char));
        strncpy(fileInformation->name, argv[i+1], 20);
        
        fileInformation->numLines = 0;
        fileInformation->numChars = 0;
        fileInformation->numWords = 0;
        
        if ((iret = pthread_create(&thread[i], NULL, runThread, fileInformation))) {
            printf("Error creating thread %d\n", i);
            exit(0);
        }
    }
    
    
    for (i=0; i<numThread; i++) {
        if (((iret = pthread_join( thread[i], NULL)))) {
            printf("Error terminating thread %d\n", i);
            exit(0);
        }
    }
    
    /* try to accumulate the counts here, not in the thread */
    for (int i = 0; i < numThread; i++)
    {
    	numLines += info[i].numLines;
    	numChars += info[i].numChars;
    	numWords += info[i].numWords;
    }

    printf("Total: %d lines, %d words, %d characters\n", numLines, numWords, numChars);

    exit(0);
}

/*running thread*/

void *runThread( void *file )
{
	struct FileInfo *fi = (struct FileInfo*)file;


	int id = fi->numLines;
	fi->numLines = 0;

    FILE *fp;
    fp = fopen(fi->name,"r");

    if (fp == NULL)
    {
    	printf("Error: cannot open file%s, terminating %d \n", fi->name, (int) pthread_self());
    	pthread_exit(0);
    }

    rs = fgets(inString, MAXCHARS, fp);
    
    while (rs != NULL)
    {
        fi->numLines++;
        fi->numChars += strlen(inString);
        fi->numWords += countWords(inString);
        rs = fgets(inString, MAXCHARS, fp);
    }

    printf("Thread %d %s: %d lines, %d words, %d characters\n", (int) pthread_self, fi->name,
           fi->numLines, fi->numWords, fi->numChars);

    info[id].numLines = fi->numLines;
    info[id].numChars = fi->numChars;
    info[id].numWords = fi->numWords; 
    
    pthread_exit(0);
}


/***********************
 
 int countWords(char *inS)
 
 inS: input null-terminated string
 
 returns number of words in string, delimited by spaces
 
 ***********************/

int countWords(char *inS) {
    char *token;
    int numTokens = 0;
    int i=0;
    
    for (i=1; i<strlen(inS); i++) {
        if ((isalnum(inS[i-1]) || ispunct(inS[i-1])) && (inS[i] == ' ')) {
            numTokens++;
        }
    }
    
    if (isalnum(inS[strlen(inS)-2]) || ispunct(inS[strlen(inS)-2])) {
        numTokens++;
    }
    return numTokens;
}
