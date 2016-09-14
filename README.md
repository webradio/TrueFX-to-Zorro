# TrueFX-to-Zorro
Convert TrueFX.com tick data to Zorro-Trader.com .t6 bar format

## In and Out

######Download URL example (requires login after free registration)
[http://truefx.com/dev/data/2016/MAY-2016/EURUSD-2016-05.zip](http://truefx.com/dev/data/2016/MAY-2016/EURUSD-2016-05.zip)

######Content
    EUR/USD,20160502 00:00:00.959,1.14601,1.14605
    EUR/USD,20160502 00:00:00.991,1.14599,1.14605
    EUR/USD,20160502 00:00:01.018,1.14599,1.14604
    EUR/USD,20160502 00:00:01.078,1.14599,1.14605

######Zorro .t6 format
    typedef struct T6
    {
      DATE  time;           //double, 8bytes, base 1900-01-01
      float fHigh, fLow;    //single
      float fOpen, fClose;	
      float fVal, fVol;     // additional data, like ask-bid spread, volume etc.
    } T6;
    
## Aggregation
Each bar contains predefined number of ticks, like 1000. Time increment is then irregular by definition. Begin a fresh bar also after weekends, holidays.

###### Spread Increase Before Weekend
USD/JPY,20160506 20:59:56.266,107.089996,107.166  
USD/JPY,20160506 20:59:56.291,107.089996,107.209999  
USD/JPY,20160506 20:59:56.414,**106.986,107.265999**  
USD/JPY,20160506 20:59:58.136,**106.986,107.265999**  
USD/JPY,20160506 20:59:59.085,**106.978996,107.273003**  
USD/JPY,20160506 20:59:59.203,**106.978996,107.273003**  
USD/JPY,20160506 20:59:59.437,**106.917999,107.488998**  
USD/JPY,20160509 00:00:00.026,107.359001,107.366997  
USD/JPY,20160509 00:00:00.140,107.359001,107.366997  
![Zorro Chart Viewer with N-tick chart](./doc/img/PlotCurve_USDJPY.png)

## Example conversion
[ZHistoryEdit.exe](http://www.zorro-trader.com/download.php) (linked from [Zorro download page](http://www.zorro-trader.com/download.php)) can create .t6 files 

## Converting .zip to .gz
.gz format is the one supported by zlib `stdio`-like functions `gzgets()` and others. TrueFX files are .zip. 7zip is able to create .gz files calling it "gzip format". Here's an example code using zlib using stdio-like access, reading the first line from compressed file:

    #include "zlib/zlib.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    int main(int argc, char **argv)
    {
        const char *fname = "short.txt.gz";
        gzFile fr;
        Byte *uncompr;
        uLong uncomprLen = 10000*sizeof(int);
        uncompr  = (Byte*)calloc((uInt)uncomprLen, 1);
        strcpy((char*)uncompr, "garbage");

        fr = gzopen(fname, "rb");
        if (fr == NULL) {
            printf("gzopen error\n");
            exit(1);
        }
        gzgets(fr, (char*)uncompr, (int)uncomprLen);
        printf("uncompr==%s\n", uncompr);
        gzclose(fr);

        free(uncompr);
        return(0);
    } 
    

## Links (which contributed to the code)
[http://stackoverflow.com/questions/7775027/how-to-create-file-of-x-size](http://stackoverflow.com/questions/7775027/how-to-create-file-of-x-size)  
[Converting from year-month-day-... to microsoft VariantTime](https://doxygen.reactos.org/df/d85/variant_8c_source.html) - Reactos sources, they in turn borrowed from PostgreSQL  
[how-is-variant-time-date-double-8-byte-handled on Stackoverflow]() - pointing to Reactos

## Links (just for info)
[Recursively list directories in C](http://www.lemoda.net/c/recursive-directory/)  
[Doubly-Linked List in C](http://www.lemoda.net/c/doubly-linked-list/index.html)
[How are zlib, gzip and zip related](http://stackoverflow.com/questions/20762094/how-are-zlib-gzip-and-zip-related-what-do-they-have-in-common-and-how-are-they)  
[zlib cannot handle zip](http://www.zlib.net/zlib_faq.html#faq11), see contrib/minizip for wrapper or see [Mark Adler's sunzip](https://github.com/madler/sunzip)   
[fUnZip at Info-ZIP.org](http://www.info-zip.org/) - for extracting into pipe
[zlib, example of stdio-like gzopen() and **gzgets()**](https://github.com/madler/zlib/blob/master/test/example.c)  
[standalone conversion for epoch](http://codereview.stackexchange.com/questions/38275/convert-between-date-time-and-time-stamp-without-using-standard-library-routines)
see also CRAN lubridate source; not used in this code

## Thrown away
Here is a (Windows) command line to convert .zip to gzip **stream (not a valid .gz file)** and decompress again to check if it worked. Keep in mind that the magic two-bytes header {31, 139} is not present in this case, `gzgets()` simply returns bytes from the file in this case.

    funzip.exe EURUSD-2016-06.zip | zpipe.exe > EURUSD-2016-06.csv.gz
    cat EURUSD-2016-06.csv.gz | zpipe.exe -d | more
