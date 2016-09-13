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
.gz format is the one supported by zlib `stdio`-like functions `gzgets()` and others. TrueFX files are .zip. Here is a (Windows) command line to convert .zip to .gz  

    funzip.exe EURUSD-2016-06.zip | zpipe.exe > EURUSD-2016-06.csv.gz

Note that 7zip can also create .gz files calling it "gzip format", but it appears to be not the same that zlib examples create and read. 

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