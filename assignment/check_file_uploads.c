/*  This program is responsible for checking that all expected files have been submitted,
    and logs those that have not been submitted in missingUploads.txt
*/

#include <unistd.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define MAXLENGTH 30

void writeToMissingUploads(char department[MAXLENGTH], char* time, FILE * fp);

int check_file_uploads() {

    FILE *fp;
    time_t current = time(NULL);

    //converts current time into form that can be written to file
    char * time_str = ctime(&current);
    time_str[strlen(time_str)-1] = '\0';

    //path char arrays to store search results
    char path1[MAXLENGTH], path2[MAXLENGTH], path3[MAXLENGTH], path4[MAXLENGTH];
    
    //storing expected file names
    char w[] = "warehouse.xml";
    char m[] = "manufacturing.xml";
    char s[] = "sales.xml";
    char d[] = "distribution.xml";

    //initializing counts
    int wcount = 0, mcount = 0, scount = 0, dcount = 0;

    //storing results in local variables
    fp = popen("ls upload | grep distribution", "r");
    fgets(path1, MAXLENGTH, fp);

    fp = popen("ls upload | grep manufacturing", "r");
    fgets(path2, MAXLENGTH, fp);

    fp = popen("ls upload | grep sales", "r");
    fgets(path3, MAXLENGTH, fp);

    fp = popen("ls upload | grep warehouse", "r");
    fgets(path4, MAXLENGTH, fp);

    //loops to compare expected results vs actual results
    chdir("/");
    fp = fopen("/workspaces/system-software/assignment/logs/missingUploads.txt", "a+");
    for(int x=0; x<sizeof(d); x++){
        if(path1[x]== d[x]){
            dcount++;
        }
    };

    //if counts dont match the size of the expected name then an entry is written to the missing files log
    if(dcount != sizeof(d) - 1){
        writeToMissingUploads(d, time_str, fp);
    }

    for(int x=0; x<sizeof(m); x++){
        if(path2[x]== m[x]){
            mcount++;
        } 
    };

    if(mcount != sizeof(m) - 1){
        writeToMissingUploads(m, time_str, fp);
    }

    for(int x=0; x<sizeof(s); x++){
        if(path3[x]== s[x]){
            scount++;
        }
    };
    if(scount != sizeof(s) - 1){
        writeToMissingUploads(s, time_str, fp);
    }
    for(int x=0; x<sizeof(w); x++){
        if(path4[x]== w[x]){
            wcount++;
        }
    };
    if(wcount != sizeof(w) - 1){
        writeToMissingUploads(w, time_str, fp);
    }
    
    //close file
    fclose(fp);

    return 0;
}

void writeToMissingUploads(char department[MAXLENGTH], char* time, FILE * fp){
    char message[] = " department did not submit their report on ";
    char ptr [50];

    //size of each element for writing
    int size = 1;

    fwrite(department, strlen(department), size, fp);
    fwrite(message, strlen(message), size, fp);
    fwrite(time, strlen(time), size, fp);        
    fwrite("\n", strlen("\n"), 1, fp);
}
      