#define NTICKS 1000

#include <stdio.h>
#include <stdlib.h>
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

typedef struct T6
{
  double time;
  float fHigh, fLow;	
  float fOpen, fClose;	
  float fVal, fVol; // additional data, like ask-bid spread, volume etc.
} T6;

/* borrowed from Reactos https://doxygen.reactos.org/df/d85/variant_8c_source.html */
/* pointed by http://stackoverflow.com/questions/22476192/how-is-variant-time-date-double-8-byte-handled */
double SystemTimeToVariantTimeMs(
    const unsigned short year, 
    const unsigned short month, 
    const unsigned short day, 
    const unsigned short hour, 
    const unsigned short min, 
    const unsigned short sec, 
    const unsigned int   msec)
{
    int m12 = (month - 14) / 12;
    double dateVal = 
        /* Convert Day/Month/Year to a Julian date - from PostgreSQL */
        (1461 * (year + 4800 + m12)) / 4 + (367 * (month - 2 - 12 * m12)) / 12 -
        (3 * ((year + 4900 + m12) / 100)) / 4 + day - 32075
        -1757585 /* Convert to + days from 1 Jan 100 AD */
        -657434; /* Convert to +/- days from 1 Jan 1899 AD */
    double dateSign = (dateVal < 0.0) ? -1.0 : 1.0;
    dateVal += dateSign * (msec + sec*1000 + min*60000 + hour*3600000) / 86400000.0;
    return dateVal;
}

/* Parse ticks in the CSV file. If wr==false, count created bars and write  */
/* to pNumbars. If wr==true, parse ticks and this time write bars to the    */
/* output file in reverse order, knowing total number from pNumbars         */
void parsecsv(bool wr, int *pNumbars, FILE *fr, FILE *fw)
{
	
}

int main(int argc, char **argv)
{
    FILE *fr;
    FILE *fw;
    char L[64];
    double T;
    double prevT = 0.0;
    int N = 0;
    T6 Bar;
    
    #define FLUSHBAR() {               \
      Bar.fVal /= N;                   \
      fwrite(&Bar, sizeof(T6), 1, fw); \
      N = 0;                           \
    }
      //printf("Time=%f O=%f H=%f L=%f C=%f Val=%f Vol=%f\n", 
      //  Bar.time, Bar.fOpen, Bar.fHigh, Bar.fLow, Bar.fClose, Bar.fVal, Bar.fVol); 
    
    if(argc < 3) {
        fprintf(stderr, "Usage : %s TRUEFX-CSV-INPUT ZORRO-T6-OUTPUT\n\
        E.g. %s EURUSD-2016-05.csv EURUSD-2016-t1000.t6\n", argv[0], argv[0]);
        return(-1);
    }
    fr = fopen(argv[1], "r"); //read as text
    if(NULL == fr) {
        fprintf(stderr, "Error opening input file %s\n", argv[1]);
        return(-2);
    }
    fw = fopen(argv[2], "wb"); //write as binary
    if(NULL == fr) {
        fprintf(stderr, "Error opening output file %s\n", argv[2]);
        return(-3);
    }

    while( fgets(L, sizeof(L)-1, fr)!=NULL )
    {
        int i;
        
        /* read and decode one csv line */
        // EUR/USD,20160506 20:49:59.937,1.14023,1.14033
        //         8   1214 17 20 23 26  30
        for (i = 8; i <= 28; i++) {
            L[i] -= '0'; /* prepare to convert to integers */
        }
        for (i = 30; (L[i] != '\0') && (L[i] != ','); i++) { 
            /* scroll to comma between bid and ask */ 
        }
        L[i] = '\0'; /* make bid to an own string by putting null at the end */
        double Bid = atof(&L[30]);
        double Ask = atof(&L[i+1]);
        T = SystemTimeToVariantTimeMs(
            /* year  */ 1000*L[8]  + 100*L[9] +  10*L[10] + L[11],
            /* month */                          10*L[12] + L[13],
            /* day   */                          10*L[14] + L[15],
            /* hour  */                          10*L[17] + L[18], 
            /* min   */                          10*L[20] + L[21], 
            /* sec   */                          10*L[23] + L[24], 
            /* msec  */              100*L[26] + 10*L[27] + L[28]);
        //printf("Datetime=%f Bid=%f Ask=%f\n", T, Bid, Ask); // 42496.868055 
        
        /* start new bar after any big time gap - weekend or even an hour */
        if ((T-prevT>1.0/24.0) && (N>0) && (prevT>0)) {
            FLUSHBAR();
        }
        prevT = T;
        
        /* aggregation */
        if (++N == 1) {
            Bar.time = T;
            Bar.fVol = 1;
            Bar.fVal = (Ask - Bid);
            Bar.fOpen = Bar.fHigh = Bar.fLow = Bar.fClose = Bid; 
        } else {
            Bar.fVol += 1;
            Bar.fVal += (Ask - Bid);
            Bar.fClose = Bid;
            Bar.fHigh = max(Bar.fHigh, Bid);
            Bar.fLow  = min(Bar.fLow,  Bid);
        }
        
        /* reached number of ticks, write the bar to output file*/
        if (N>=NTICKS) {
            FLUSHBAR();
        }
    }
    if (N>0) {
        FLUSHBAR();
    }

    fclose(fw);
    fclose(fr);   
    return(0);
}
