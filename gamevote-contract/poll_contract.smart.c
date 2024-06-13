
#include APIFunctions

#program name PollContract
#program description This contract serves as a distributor contract for polls
#program activationAmount .5

#pragma maxAuxVars 3
#pragma maxConstVars 3
#pragma optimizationLevel 3
#pragma version 2.2.1

// enumeration substitute
#define ONESIGNA 100000000
#define ONEHOUR 60
#define TWENTYFOURHOURS 1440

// contract methods
#define STOREPOLL 3
#define CHECKPOLL 4

// sub methods
#define ACT 0
#define BUILD 1
#define DOCK 2
#define EQUIP 3
#define EXPLODE 4
#define MINING 5
#define REPAIR 6
#define SCAN 7
#define STORE 8
#define TREAT 9

// map flags
#define TIMESTAMP 0
#define HASH256 1
#define ACTOR 2
#define TARGET 3
#define SUBMETHOD 4
#define PARAMETER 5


struct TXINFO {
    long txId,
        timestamp,
        sender,
        amount,
        message[8];
} currentTX;

void getTxDetails(void) {
    currentTX.txId = Get_A1();
	currentTX.amount = getAmount(currentTX.txId);
	currentTX.timestamp = Get_Timestamp_For_Tx_In_A();
	currentTX.sender = getSender(currentTX.txId);
	readMessage(currentTX.txId, 0, currentTX.message);
    readMessage(currentTX.txId, 1, currentTX.message + 4);
}

void main(void) {
    do{
        A_To_Tx_After_Timestamp(currentTX.timestamp);
        if (Get_A1() == 0) 
		{
            break;
        }
        getTxDetails();
		
        switch (currentTX.message[0]) {
			case STOREPOLL:
				StorePollInMap();
				break;
			case CHECKPOLL:
				CheckPoll();
				break;
			default:
				break;
        }
    } while (true);
}

// main Methods
void StorePollInMap() {
	
	// ### incoming ###
	// currentTX.sender = gamevoteContract 
	// currentTX.message[0] = contract method (STOREPOLL)
	// currentTX.message[1] = sub method (mining)
	// currentTX.message[2] = parameter (123)
	// currentTX.message[3] = actorContractID (to process sub method(parameter))
	// currentTX.message[4] = targetContractID (to process sub method(parameter))
	// currentTX.message[5] = providerID/pollsterID (for reward)
	// currentTX.message[6] = timestamp (vote timeout in the future)
	// currentTX.message[7] = free
	
	setMapValue(currentTX.message[5], currentTX.message[3], currentTX.message[6]); // save timestamp into map
	
	long hashResult = GetB4FromHash256(currentTX.message[5], currentTX.message[3], currentTX.message[4], currentTX.message[6]);
	setMapValue(currentTX.message[5], 0, hashResult) // save hash256 into map
	setMapValue(hashResult, TIMESTAMP, currentTX.message[6]) // save timestamp into map
	setMapValue(hashResult, ACTOR, currentTX.message[3]); // save actorID into map
	setMapValue(hashResult, TARGET, currentTX.message[4]); // save targetID into map
	setMapValue(hashResult, SUBMETHOD, currentTX.message[1]); // save sub method into map
	setMapValue(hashResult, currentTX.message[1], currentTX.message[2]); // save parameter into map
	
}

void CheckPoll() {
	// ### incoming ###
	// currentTX.sender = gamevoteContract
	// currentTX.message[0] = contract method (CHECKPOLL)
	// currentTX.message[1] = pollsterID (poll initiator)
	// currentTX.message[2] = actorContractID (action initiator)
	// currentTX.message[3] = free
	// currentTX.message[4] = free
	// currentTX.message[5] = free
	// currentTX.message[6] = free
	// currentTX.message[7] = free
	
	
}


// sub methods
void ExecutePollInMap () {
	
}

void RejectPollInMap() {
	
}

// support methods
long GetB4FromHash256(long a1, long a2, long a3, long a4) {
	Set_A1_A2(a1, a2);
    Set_A3_A4(a3, a4);
	SHA256_A_To_B();
	return Get_B4();
}
