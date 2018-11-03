#include <arduino.h>
#include "computeDate.h"
#include "arrLabels.h"

using namespace std;

void computeDate(int outArr[], unsigned long timeSecs)
{
    bool leapYear = false;

    int year, hours, mins;
    int yearDays;
    int quot100, rem100, quot10, rem10;

    const long fourLeapSecs = 126230400UL; // 3 normal years + 1 leap
    const long leapYearSecs = 31622400UL;  // Leap year
    const long yearSecs     = 31536000UL;
    const long daySecs      = 86400UL;
    const int  hourSecs     = 3600;
    const int  minSecs      = 60;

    long secsLeft;
    long leapYearBase;

    //const int timeZoneAdj = -4 * hourSecs;
    // const int timeZoneAdj = 0;

    // timeSecs = timeSecs+timeZoneAdj;         // Adjust UTC to local
    //                                          // To Do:  figure out how to adjust for DST
    //                                          // before moving on to the next calculations
    //                                          // or maybe assume DST adjustment now and readjust 
    //                                          // if not DST?   To Do.                                             
                                             
                                             
    //                                     // if not DST?   To Do.                                            

    leapYearBase = timeSecs/fourLeapSecs;
    secsLeft     = timeSecs % fourLeapSecs;

    if (secsLeft < leapYearSecs) {  
        leapYear = true;
        year = 0;
    }
    else {
        year     = secsLeft/yearSecs;
        secsLeft = secsLeft % yearSecs;
    }

    year = year + 4 * leapYearBase;
    yearDays = (secsLeft/daySecs) + 1;           // current partial day counts as a day
    secsLeft = secsLeft % daySecs;

    if (leapYear==true) {
        switch(yearDays) {
            case 0 ... 57: {
                outArr[DST_0] = 0;
                outArr[DST_1] = 0;
                break;
            }
            case 58: {
                outArr[DST_0] = 0;
                outArr[DST_1] = 1;
                break;
            }
            case 59 ... 365: {
                outArr[DST_0] = 1;
                outArr[DST_1] = 1;
                break;
            }
            case 366: {
                outArr[DST_0] = 1;
                outArr[DST_1] = 0;
                break;
            }
            default: {
                outArr[DST_0] = 0;
                outArr[DST_1] = 0;
            }
        }
    }

    hours    = secsLeft/hourSecs;
    secsLeft = secsLeft % hourSecs;

    mins = secsLeft/minSecs;
    secsLeft = secsLeft % minSecs;

    quot10 = year/10;
    rem10 = year % 10;

    outArr[year_1] = rem10 & 1;
    outArr[year_2] = (rem10 >> 1) & 1;
    outArr[year_4] = (rem10 >> 2) & 1;
    outArr[year_8] = (rem10 >> 3) & 1;

    outArr[year_10] = quot10 & 1;
    outArr[year_20] = (quot10 >> 1) & 1;
    outArr[year_40] = (quot10 >> 2) & 1;
    outArr[year_80] = (quot10 >> 3) & 1;

    quot10 = mins/10;
    rem10 = mins % 10;

    outArr[min_1] = rem10 & 1;
    outArr[min_2] = (rem10 >> 1) & 1;
    outArr[min_4] = (rem10 >> 2) & 1;
    outArr[min_8] = (rem10 >> 3) & 1;

    outArr[min_10] = quot10 & 1;
    outArr[min_20] = (quot10 >> 1) & 1;
    outArr[min_40] = (quot10 >> 2) & 1;

    quot100 = yearDays/100;
    rem100 = yearDays % 100;
    quot10 = rem100/10;
    rem10 = rem100 % 10;

    outArr[day_1] = rem10 & 1;
    outArr[day_2] = (rem10 >> 1) & 1;
    outArr[day_4] = (rem10 >> 2) & 1;
    outArr[day_8] = (rem10 >> 3) & 1;

    outArr[day_10] = quot10 & 1;
    outArr[day_20] = (quot10 >> 1) & 1;
    outArr[day_40] = (quot10 >> 2) & 1;
    outArr[day_80] = (quot10 >> 3) & 1;

    outArr[day_100] = quot100 & 1;
    outArr[day_200] = (quot100 >> 1) & 1;

    quot10 = hours/10;
    rem10 = hours % 10;

    outArr[hour_1] = rem10 & 1;
    outArr[hour_2] = (rem10 >> 1) & 1;
    outArr[hour_4] = (rem10 >> 2) & 1;
    outArr[hour_8] = (rem10 >> 3) & 1;

    outArr[hour_10] = quot10 & 1;
    outArr[hour_20] = (quot10 >> 1) & 1;

    outArr[Year] = year;
    outArr[Day]  = yearDays;
    outArr[Hour] = hours;
    outArr[Min]  = mins;
    outArr[Sec]  = secsLeft;

    return;

}
