/**
 * @author evolver
 * this contract implements a poll contract as a supplement to the gamevote contract
 * features:
 * - store poll information for actor/target contracts (like sub methods and parameters)
 * still in developement
 */
 
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
#define REGISTER 5

// sub methods
// #define ACT 0
// #define BUILD 1
// #define DOCK 2
// #define EQUIP 3
// #define EXPLODE 4
// #define MINING 5
// #define REPAIR 6
// #define SCAN 7
// #define STORE 8
// #define TREAT 9

// map flags
#define POLLSTER 11
#define ACTOR 12
#define TARGET 13
#define SUBMETHOD 14
#define PARAMETER 15
#define TIMESTAMP 16

// contract attributes
// basic contract 
long authIDs[10];
long sendBuffer[8];
long currentFee = ONESIGNA;

struct TXINFO {
    long txId,
        timestamp,
        sender,
        amount,
        message[8];
} currentTX;

constructor();

void constructor(void) {
    // this function will be called only once on first activation.
	authIDs[0] = 0; //TODO: set initial ID
}

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
		
		// if sender is not authenticated then break
		if(IsAuthenticated(currentTX.sender) == 0) {
			break;
		}
		
        switch (currentTX.message[0]) {
			case STOREPOLL:
				StorePollInMap();
				break;
			case CHECKPOLL:
				CheckPoll();
				break;
			case REGISTER:
				Register();
				break;
			default:
				// gasfee
				break;
        }
    } while (true);
}

// main Methods

void Register(void) {
	
	// ### incoming ###
	// currentTX.sender = voteContractID
	// currentTX.message[0] = contract method (REGISTER)
	// currentTX.message[1] = parameter (0-9)
	// currentTX.message[2] = authID (123)
	// currentTX.message[3] = targetContractID (123 (eg. ZeptorLight))
	// currentTX.message[4] = timestamp (1 for act immediately otherwise timestamp in the future)
	// currentTX.message[5] = free
	// currentTX.message[6] = free
	// currentTX.message[7] = free
	
	// register immediately
	if(currentTX.message[3] == 1) {
		
		// ### outgoing to ACTOR ###
		// recipient = targetContractID
		// message[0] = method (REGISTER)
		// message[1] = parameter (0-9)
		// message[2] = authID (123)
		// message[3] = free
		// message[4] = free
		// message[5] = free
		// message[6] = free
		// message[7] = free
		
		SetSendBufferForTargetContract(REGISTER, currentTX.message[1], currentTX.message[2], 0, 0, 0, 0, 0);
		SendBufferWithFee(currentTX.message[3]);
		
	} else {
		// store registration
		long hash = GetB4FromHash256(currentTX.message[1], currentTX.message[2], currentTX.message[3], 0);
	
		setMapValue(hash, REGISTER, 1); // save pollsterID into map
		setMapValue(hash, ACTOR, currentTX.message[2]); // save actorID into map
		setMapValue(hash, TARGET, currentTX.message[3]); // save targetID into map
		//setMapValue(hash, SUBMETHOD, 0); // save sub method into map
		setMapValue(hash, PARAMETER, currentTX.message[1]); // save parameter into map
		setMapValue(hash, TIMESTAMP, currentTX.message[4]); // save timestamp into map
		
	}
	
}

void StorePollInMap(void) {
	
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
		
	// act immediately
	if(currentTX.message[6] == 1) {
		// ### outgoing to ACTOR ###
		// recipient = actorContractID
		// message[0] = sub method
		// message[1] = PARAMETER
		// message[2] = ACTOR (wich part on this poll)
		// message[3] = POLLSTER (for reward)
		// message[4] = free
		// message[5] = free
		// message[6] = free
		// message[7] = free
		
		SetSendBufferForTargetContract(currentTX.message[1], currentTX.message[2], ACTOR, currentTX.message[5], 0, 0, 0, 0);
		SendBufferWithFee(currentTX.message[3]);
		
		// ### outgoing to TARGET ###
		// recipient = targetContractID
		// message[0] = sub method
		// message[1] = PARAMETER
		// message[2] = TARGET (wich part on this poll)
		// message[3] = POLLSTER (for reward)
		// message[4] = free
		// message[5] = free
		// message[6] = free
		// message[7] = free
		
		SetSendBufferForTargetContract(currentTX.message[1], currentTX.message[2], TARGET, currentTX.message[5], 0, 0, 0, 0);
		SendBufferWithFee(currentTX.message[4]);
		
	} else {
		
		// store act
		long hash = GetB4FromHash256(currentTX.message[5], currentTX.message[3], currentTX.message[4], 0);
	
		setMapValue(hash, POLLSTER, currentTX.message[5]); // save pollsterID into map
		setMapValue(hash, ACTOR, currentTX.message[3]); // save actorID into map
		setMapValue(hash, TARGET, currentTX.message[4]); // save targetID into map
		setMapValue(hash, SUBMETHOD, currentTX.message[1]); // save sub method into map
		setMapValue(hash, PARAMETER, currentTX.message[2]); // save parameter into map
		setMapValue(hash, TIMESTAMP, currentTX.message[6]); // save timestamp into map
	}
	
}

void CheckPoll(void) {
	
	// ### incoming ###
	// currentTX.sender = gamevoteContract
	// currentTX.message[0] = contract method (CHECKPOLL)
	// currentTX.message[1] = pollHash (mapIndex)
	// currentTX.message[2] = free
	// currentTX.message[3] = free
	// currentTX.message[4] = free
	// currentTX.message[5] = free
	// currentTX.message[6] = free
	// currentTX.message[7] = free
	
	long hash = currentTX.message[1];
	long isTimeUp = GetTimeIsUp(getMapValue(hash, TIMESTAMP));
	
	if(isTimeUp == 0) {
		if(getMapValue(hash, REGISTER) == 1) {
			
			// ### outgoing to TARGET ###
			// recipient = targetContractID
			// message[0] = method (REGISTER)
			// message[1] = parameter (0-9)
			// message[2] = authID (123)
			// message[3] = free
			// message[4] = free
			// message[5] = free
			// message[6] = free
			// message[7] = free
			
			SetSendBufferForTargetContract(REGISTER, getMapValue(hash, PARAMETER), getMapValue(hash, ACTOR), 0, 0, 0, 0, 0);
			SendBufferWithFee(getMapValue(hash, TARGET));
			
		} else {
			ExecutePollInMap(hash);
		}
		
	} else if(isTimeUp == 1) {
		ClearMap(hash);
	}
	
}


// sub methods
void ExecutePollInMap(long hash) {
	
	// ### outgoing to ACTOR ###
	// recipient = actorContractID
	// message[0] = sub method
	// message[1] = PARAMETER
	// message[2] = ACTOR (wich part on this poll)
	// message[3] = POLLSTER (for reward)
	// message[4] = free
	// message[5] = free
	// message[6] = free
	// message[7] = free
	
	SetSendBufferForTargetContract(getMapValue(hash, SUBMETHOD), getMapValue(hash, PARAMETER), ACTOR, getMapValue(hash, POLLSTER), 0, 0, 0, 0);
	SendBufferWithFee(getMapValue(hash, ACTOR));
	
	
	// ### outgoing to TARGET ###
	// recipient = targetContractID
	// message[0] = sub method
	// message[1] = PARAMETER
	// message[2] = TARGET (wich part on this poll)
	// message[3] = POLLSTER (for reward)
	// message[4] = free
	// message[5] = free
	// message[6] = free
	// message[7] = free
	
	SetSendBufferForTargetContract(getMapValue(hash, SUBMETHOD), getMapValue(hash, PARAMETER), TARGET, getMapValue(hash, POLLSTER), 0, 0, 0, 0);
	SendBufferWithFee(getMapValue(hash, TARGET));
	
	ClearMap(hash);
}

void ClearMap(long hash) {
	// setMapValue(hash, POLLSTER, 0);
	// setMapValue(hash, ACTOR, 0);
	// setMapValue(hash, TARGET, 0);
	setMapValue(hash, SUBMETHOD, 0);
	setMapValue(hash, PARAMETER, 0);
	setMapValue(hash, TIMESTAMP, 0);
}

// support methods
long GetB4FromHash256(long a1, long a2, long a3, long a4) {
	Set_A1_A2(a1, a2);
    Set_A3_A4(a3, a4);
	SHA256_A_To_B();
	return Get_B4();
}

long GetTimeIsUp(long timeOut) {
	
	if(timeOut == 0) {
		return 2;
	}
	
	if(Get_Block_Timestamp() <= timeOut) {
		return 0;
	} else {
		return 1;
	}
	
}

long IsAuthenticated(long sender) {
	
	for(long i = 0; i < authIDs.length; i++) {
		if(authIDs[i] == sender) {
			return 1;
		}
	}
	
	return 0;
}

// contract methods
void SetSendBufferForTargetContract(long buffer1, long buffer2, long buffer3, long buffer4, long buffer5, long buffer6, long buffer7, long buffer8) {
	sendBuffer[0] = buffer1;
	sendBuffer[1] = buffer2;
	sendBuffer[2] = buffer3;
	sendBuffer[3] = buffer4;
	sendBuffer[4] = buffer5;
	sendBuffer[5] = buffer6;
	sendBuffer[6] = buffer7;
	sendBuffer[7] = buffer8;
}

void SendBufferWithFee(long recipient) {
	sendAmountAndMessage(currentFee, sendBuffer, recipient);
	sendMessage(sendBuffer + 4, recipient);
}