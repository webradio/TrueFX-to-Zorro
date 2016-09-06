# TrueFX-to-Zorro
Convert TrueFX.com tick data to Zorro-Trader.com .t6 bar format

## In and Out

######Download URL example (requires login after free registration)
http://truefx.com/dev/data/2016/MAY-2016/EURUSD-2016-05.zip

######Content
EUR/USD,20160502 00:00:00.959,1.14601,1.14605   
EUR/USD,20160502 00:00:00.991,1.14599,1.14605   
EUR/USD,20160502 00:00:01.018,1.14599,1.14604  
EUR/USD,20160502 00:00:01.078,1.14599,1.14605

######Zorro .t6 format
`typedef struct T6
{
  DATE  time;
  float fHigh, fLow;	
  float fOpen, fClose;	
  float fVal, fVol; // additional data, like ask-bid spread, volume etc.
} T6;`

## Aggregation
Each bar contains predefined number of ticks, like 1000. Time increment is irregular then. Cut also for weekends, holidays.

## Example conversion
ZHistoryEdit.exe (linked from Zorro manual and not found by google) can create .t6 files 