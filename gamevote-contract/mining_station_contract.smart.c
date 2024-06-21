/**
 * @author hansmeuller
 * i´m doing my part
 */

#include APIFunctions

#program name mining_station_contract
#program description this is a mining station
#program activationAmount .5

#pragma maxAuxVars 3
#pragma maxConstVars 3
#pragma optimizationLevel 1
#pragma version 2.2.1

#define ONESIGNA 100000000

#define DEPOSITING 1
#define ACT 2
#define WITHDRAWALING 3

#define ACTION 5

#define MINING_DRILL 1
#define REFINERY 2
#define STORAGE 3
#define POWER_GENERATOR 4
#define SHIELD_GENERATOR 5

long stationComponents[5];
long gamevoteContract = 0;
long sendBuffer[8];
long currentFee = ONESIGNA;
long insurance = 0;
long owner = 0;
long status = 0;
long location = 0;

struct TXINFO {
    long txId,
        timestamp,
        sender,
        amount,
        message[8];
} currentTX;

