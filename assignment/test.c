#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLENGTH 30

int main(){    
    FILE *fp;
    int status;
    char path1[MAXLENGTH], path2[MAXLENGTH], path3[MAXLENGTH], path4[MAXLENGTH];
    int results[4];

    //storing expected files
    char w[] = "warehouse.txt";
    char m[] = "manufacturing.txt";
    char s[] = "sales.txt";
    char d[] = "distribution.txt";

    //initializing counts
    int wcount = 0, mcount = 0, scount = 0, dcount = 0;

    //storing results in local variables
    fp = popen("ls report | grep distribution", "r");
    fgets(path1, MAXLENGTH, fp);

    fp = popen("ls report | grep manufacturing", "r");
    fgets(path2, MAXLENGTH, fp);

    fp = popen("ls report | grep sales", "r");
    fgets(path3, MAXLENGTH, fp);

    fp = popen("ls report | grep warehouse", "r");
    fgets(path4, MAXLENGTH, fp);

    //loops to compare expected results vs actual results
    for(int x=0; x<sizeof(d); x++){
        if(path1[x]== d[x]){
            dcount++;
        }
    };
    if(dcount == sizeof(d) - 1){
        results[0] = 1;
    } else {
        results[0] = 0;
    }

    for(int x=0; x<sizeof(m); x++){
        if(path2[x]== m[x]){
            mcount++;
        } 
    };
    if(mcount == sizeof(m) - 1){
        results[1] = 1;
    } else {
        results[1] = 0;
    }

    for(int x=0; x<sizeof(s); x++){
        if(path3[x]== s[x]){
            scount++;
        }
    };
    if(scount == sizeof(s) - 1){
        results[2] = 1;
    } else {
        results[2] = 0;
    }


    for(int x=0; x<sizeof(w); x++){
        if(path4[x]== w[x]){
            wcount++;
        }
    };
    if(wcount == sizeof(w) - 1){
        results[3] = 1;
    } else {
        results[3] = 0;
    }

    printf("%d, %d, %d, %d \n", results[0], results[1], results[2], results[3]);
    //returns array of results
    return 0;
}