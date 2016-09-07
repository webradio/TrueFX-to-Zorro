#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct T6
{
  double time;
  float fHigh, fLow;	
  float fOpen, fClose;	
  float fVal, fVol; // additional data, like ask-bid spread, volume etc.
} T6;

#define ATOI1(c) (c-'0')

int main(int argc, char **argv)
{
    FILE *fr, *fw;
    char L[255];
    tm Time;
    int MSec;
    double T;
    
    if(argc < 3) {
        fprintf(stderr, "Usage : %s TRUEFX-CSV-INPUT ZORRO-T6-OUTPUT\n\
        E.g. %s EURUSD-2016-05.csv EURUSD-2016-t1000.t6\n", argv[0], argv[0]);
        return(-1);
    }
    fr = fopen(argv[1], "r");
    if(NULL == fr) {
        fprintf(stderr, "Error opening input file %s\n", argv[1]);
        return(-2);
    }
    fw = fopen(argv[2], "w");
    if(NULL == fr) {
        fprintf(stderr, "Error opening output file %s\n", argv[2]);
        return(-3);
    }

    printf("hello world from %s %s\n", argv[0], argv[1]);
    while( fgets(L, sizeof(L)-1, fr)!=NULL )
    {
        // EUR/USD,20160601 00:00:00.133,1.11341,1.11343
        //         8   1214 17 20 23 26
        //char s[12];
        memset(&Time, 0, sizeof(Time));
        Time.tm_year = 1000*ATOI1(L[8])  + 100*ATOI1(L[9]) + 10*ATOI1(L[10]) + 1*ATOI1(L[11]); 
        Time.tm_mon  =   10*ATOI1(L[12]) +   1*ATOI1(L[13]) - 1; /* Months *since* january: 0-11 */
        Time.tm_mday =   10*ATOI1(L[14]) +   1*ATOI1(L[15]); 
        Time.tm_hour =   10*ATOI1(L[17]) +   1*ATOI1(L[18]); 
        Time.tm_min  =   10*ATOI1(L[20]) +   1*ATOI1(L[21]); 
        Time.tm_sec  =   10*ATOI1(L[23]) +   1*ATOI1(L[24]); 
             MSec    =  100*ATOI1(L[26]) +  10*ATOI1(L[27]) + 1*ATOI1(L[28]); 
        T = mktime(&Time);// + (MSec/1000)/24/3600;     
        printf("Year=%d Month=%d Day=%d Hour=%d Min=%d Sek=%d\n T=%f", 
            Time.tm_year, Time.tm_mon, Time.tm_mday, Time.tm_hour, Time.tm_min, Time.tm_sec, T);
        break;
    }

    fclose(fw);
    fclose(fr);   
    return(0);
}
