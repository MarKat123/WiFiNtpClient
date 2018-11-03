#include "printTestResults.h"
#include "arrLabels.h"
#include <arduino.h>

int computeThisValue(int outArr[], const int indices[], int numEls);

using namespace std;

const int minInd[]  = {1,2,3,5,6,7,8};
const int hourInd[] = {12,13,15,16,17,18};
const int dayInd[]  = {22,23,25,26,27,28,30,31,32,33};
const int yearInd[] = {45,46,47,48,50,51,52,53};
const int dstInd[]  = {57,58};

void printTestResults(int outArr[]) {
    Serial.println();
    Serial.print("Minutes: "); 
    Serial.print(computeThisValue(outArr, minInd, sizeof(minInd)));
    Serial.print(" ");
    Serial.println(outArr[Min]);
    Serial.print("Hours: ");   
    Serial.println(computeThisValue(outArr, hourInd, sizeof(hourInd)));
    Serial.print(" ");
    Serial.println(outArr[Hour]);
    Serial.print("Days: ");   
    Serial.println(computeThisValue(outArr, dayInd, sizeof(dayInd)));
    Serial.print(" ");
    Serial.println(outArr[Day]);
    Serial.print("Years: ");  
    Serial.println(computeThisValue(outArr, yearInd, sizeof(yearInd)));
    Serial.print(" ");
    Serial.println(outArr[Year]);
    Serial.print("DST: ");     
    Serial.println(computeThisValue(outArr, dstInd, sizeof(dstInd)));    
}

int computeThisValue(int outArr[], const int indices[], int sizeArr) {
    int offsetWeights[]={1,2,4,8,10,20,40,80,100,200};
    int weightedReturn=0;
    int ii, numEls, arrInx;
    numEls = sizeArr/sizeof(indices[0]);
    for (ii=1; ii<=numEls; ii++){
        arrInx = indices[numEls-ii];
        weightedReturn = weightedReturn + offsetWeights[ii-1] * outArr[arrInx];
    }
    return(weightedReturn);
}