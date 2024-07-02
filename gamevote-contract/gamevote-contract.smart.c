
/**
 * @author evolver
 * this contract implements a gamevote contract based on the flashwork second layer solution
 * features:
 * - depositing and lock a amount for the second layer
 * - get profit with votepoints
 * - request a action that needs to be voted on
 * - vote for action requests
 * - prevent doublevotes
 * - prevent fraud
 * still in developement
 * good conversion tool for hex/numerics: https://www.simonv.fr/TypesConvert/?integers
 */

#include APIFunctions

#program name GameVoteContract
#program description This contract serves as a consensus regulator (from a few to many voters) for target contracts
#program activationAmount 2.6

#pragma maxAuxVars 10
#pragma maxConstVars 10
#pragma optimizationLevel 3
#pragma version 2.2.1

// enumeration substitute
#define ONESIGNA 100000000
#define ONEHOUR 60
#define TWENTYFOURHOURS 1440

// contract methods
#define DEPOSITING 1
#define REGISTER 2
#define ACTING 3
#define VOTE_FOR_POLL 4
#define WITHDRAWALING 5

// map flags
#define DEPOSITMENT 1
#define ENTITLEMENT 2
#define ELECTIONS 3
#define PROVIDER_ID 4
#define ACTOR_ID 5
#define TARGET_ID 6
#define MAINMETHOD 7
#define SUBMETHOD 8
#define PARAMETER 9
#define TIMEOUT 10
#define AGREEERS 11
#define REJECTERS 12
#define VOTEPOINTS 13

// extContract flags
#define ACTOR 14
#define TARGET 15

// contract attributes
// basic contract 
// long authIDs[10];
long sendBuffer[8];
long currentFee = ONESIGNA;

long contractProvider = 0;
long contractElections = 0;
long maxGlobalVotePoints = 1;

struct TXINFO {
    long txId,
        timestamp,
        sender,
        amount,
        message[8];
} currentTX;

struct POLLINFO {
	long hash,
		pollAmount,
		providerID,
		actorID,
		targetID,
		mainMethod,
		subMethod,
		parameter,
		status,
		timeStamp,
		timeOut,
		voteResult;
} currentPOLL;

constructor();

void constructor(void) {
    // this function will be called only once on first activation.
	currentPOLL.hash = 0;
}

void getTxDetails(void) {
    currentTX.txId = Get_A1();
	currentTX.amount = getAmount(currentTX.txId);
	currentTX.timestamp = Get_Timestamp_For_Tx_In_A();
	currentTX.sender = getSender(currentTX.txId);
	readMessage(currentTX.txId, 0, currentTX.message);
    readMessage(currentTX.txId, 1, currentTX.message + 4);
}

void getPollDetails(long hashValue, long currentAmount) {
	
	if (currentPOLL.hash != hashValue) {
		currentPOLL.hash = hashValue;
		currentPOLL.providerID = getMapValue(PROVIDER_ID, hashValue);
		currentPOLL.pollAmount = getMapValue(DEPOSITMENT, currentPOLL.providerID);
		currentPOLL.actorID = getMapValue(ACTOR_ID, hashValue);
		currentPOLL.targetID = getMapValue(TARGET_ID, hashValue);
		currentPOLL.mainMethod = getMapValue(MAINMETHOD, hashValue);
		currentPOLL.subMethod = getMapValue(SUBMETHOD, hashValue);
		currentPOLL.parameter = getMapValue(PARAMETER, hashValue);
		currentPOLL.timeStamp = getMapValue(TIMEOUT, hashValue);
	}
	
	//currentPOLL.status = CheckThePoll(hashValue);
	currentPOLL.timeOut = GetTimeIsUp(currentPOLL.timeStamp);
	
	if (currentAmount != 0) {
		currentPOLL.voteResult = Voting(currentAmount);
	} else {
		currentPOLL.voteResult = 0;
	}
	
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
			case REGISTER:
				Register();
				break;
			case DEPOSITING:
				Depositing();
				break;
			case ACTING:
				Acting();
				break;
			case WITHDRAWALING:
				Withdrawaling();
				break;
			case VOTE_FOR_POLL:
				VoteForPoll();
				break;
			default:
				break;
        }
    } while (true);
}

// ### EXTERNAL METHODS ###

// this is for a registration into the contract (get vote entitlement)
void Depositing(void) {
	
	// ### incoming ###
	// currentTX.sender = some of the depositers ID
	// currentTX.amount <= 100 signa TODO: change for mainnet
	// currentTX.message[0] = free
	// currentTX.message[1] = free
	// currentTX.message[2] = free
	// currentTX.message[3] = free
	// currentTX.message[4] = free
	// currentTX.message[5] = free
	// currentTX.message[6] = free
	// currentTX.message[7] = free
	
	/**
	 * - ContractIDs cannot deposit
	 * - DEV: if there are no contract providers, its the first deposit
	 * - DEV: current TX amount is 100 signa for tests
	 * - the senderID can not be a map flag
	 * - the senderID has no deposit right now
	 * - TODO: take the CreatorID into account when contract providers count = 0
	 */
	if(getCodeHashOf(currentTX.sender) == 0 && (contractProvider == 0 || currentTX.amount <= 100_0000_0000) && IsIDOK(currentTX.sender) == 1 && getMapValue(DEPOSITMENT, currentTX.sender) == 0) {
		
		// Deposits amount
		setMapValue(DEPOSITMENT, currentTX.sender, currentTX.amount);
		
		if(contractProvider <= 1) {
			setMapValue(ENTITLEMENT, currentTX.sender, 1);
			contractProvider++;
		} else {
			
			long timeout = SetTimeOut(ONEHOUR);
			long hashValue = GetB4FromHash256(currentTX.sender, currentTX.amount, 0, timeout);
			
			setMapValue(ELECTIONS, contractElections++, hashValue);
			
			setMapValue(ENTITLEMENT, currentTX.sender, 0);
			setMapValue(PROVIDER_ID, hashValue, currentTX.sender);
			setMapValue(MAINMETHOD, hashValue, DEPOSITMENT);
			setMapValue(TIMEOUT, hashValue, timeout);
			
		}
		
	} else {
		SendBack();
	}
	
}

void Register(void) {
	
	// ### incoming ###
	// currentTX.sender = providerID (for reward)
	// currentTX.message[0] = method (REGISTER)
	// currentTX.message[1] = parameter (authID index 0-9)
	// currentTX.message[2] = actorID (authID 123)
	// currentTX.message[3] = targetContractID (123 (eg. ZeptorLight))
	// currentTX.message[4] = free
	// currentTX.message[5] = free
	// currentTX.message[6] = free
	// currentTX.message[7] = free
	
	/**
	 * - the senderID can not be a map flag
	 * - the authenticatedID for the target contract can not be a map flag
	 * - the senderID has to be entitled
	 * - if there are no contract providers, a survey cannot be started
	 * - 
	 */
	if(IsIDOK(currentTX.sender) == 1 && IsIDOK(currentTX.message[2]) == 1 && getMapValue(currentTX.sender, ENTITLEMENT) == 1 && contractProvider > 0) {
		
		if(contractProvider <= 1) {
			// no voting necessary
			
			// ### outgoing ###
			// recipient = targetContractID (123 (eg. ZeptorLight))
			// message[0] = method (REGISTER)
			// message[1] = parameter (authID index 0-9)
			// message[2] = actorID (authID 123)
			// message[3] = free
			// message[4] = free
			// message[5] = free
			// message[6] = free
			// message[7] = free
			
			SetSendBufferForTargetContract(REGISTER, currentTX.message[1], currentTX.message[2], 0, 0, 0, 0, 0);
			SendBufferWithFee(currentTX.message[3]);
			
		} else {
			
			long timeout = SetTimeOut(ONEHOUR);
			long hashValue = GetB4FromHash256(currentTX.sender, currentTX.amount, 0, timeout);
			
			setMapValue(ELECTIONS, contractElections++, hashValue);
			
			setMapValue(PROVIDER_ID, hashValue, currentTX.sender);
			setMapValue(ACTOR_ID, hashValue, currentTX.message[2]);
			setMapValue(TARGET_ID, hashValue, currentTX.message[3]);
			
			setMapValue(MAINMETHOD, hashValue, REGISTER);
			setMapValue(PARAMETER, hashValue, currentTX.message[1]);
			setMapValue(TIMEOUT, hashValue, timeout);
			
		}
		
	} else {
		SendBack();
	}
	
}

// this serves to initiate an action
void Acting(void) {
	
	// ### incoming ###
	// currentTX.sender = providerID (for reward)
	// currentTX.message[0] = main method (ACTING)
	// currentTX.message[1] = sub method (mining)
	// currentTX.message[2] = parameter (123)
	// currentTX.message[3] = actorContractID (to process sub method(parameter))
	// currentTX.message[4] = targetContractID (to process sub method(parameter))
	// currentTX.message[5] = free
	// currentTX.message[6] = free
	// currentTX.message[7] = free
	
	if(IsIDOK(currentTX.sender) != 0 && IsIDOK(currentTX.message[3]) != 0 && IsIDOK(currentTX.message[4]) != 0 && getMapValue(ENTITLEMENT, currentTX.sender) == 1) {
		
		if(contractProvider <= 1) {
			// no voting necessary
			
			// ### outgoing to ACTOR ###
			// recipient = actorContractID
			// message[0] = main method
			// message[1] = sub method
			// message[2] = PARAMETER
			// message[3] = ACTOR (wich part on this poll)
			// message[4] = free
			// message[5] = free
			// message[6] = free
			// message[7] = free
			
			SetSendBufferForTargetContract(ACTING, currentTX.message[1], currentTX.message[2], ACTOR, 0, 0, 0, 0);
			SendBufferWithFee(currentTX.message[3]);
			
			
			// ### outgoing to TARGET ###
			// recipient = targetContractID
			// message[0] = main method
			// message[1] = sub method
			// message[2] = PARAMETER
			// message[3] = TARGET (wich part on this poll)
			// message[4] = free
			// message[5] = free
			// message[6] = free
			// message[7] = free
			
			SetSendBufferForTargetContract(ACTING, currentTX.message[1], currentTX.message[2], TARGET, 0, 0, 0, 0);
			SendBufferWithFee(currentTX.message[4]);
			
		} else {
			
			long timeout = SetTimeOut(ONEHOUR);
			long hashValue = GetB4FromHash256(currentTX.sender, currentTX.message[3], currentTX.message[4], timeout);
			
			setMapValue(ELECTIONS, contractElections++, hashValue);
			
			setMapValue(PROVIDER_ID, hashValue, currentTX.sender);
			setMapValue(ACTOR_ID, hashValue, currentTX.message[3]);
			setMapValue(TARGET_ID, hashValue, currentTX.message[4]);
			
			setMapValue(MAINMETHOD, hashValue, ACTING);
			setMapValue(SUBMETHOD, hashValue, currentTX.message[1]);
			setMapValue(PARAMETER, hashValue, currentTX.message[2]);
			setMapValue(TIMEOUT, hashValue, timeout);
			
		}
		
	} else {
		SendBack();
	}
	
}

// this is for a complete deregistration from the contract (no vote entitlement anymore)
void Withdrawaling(void) {
	
	// ### incoming ###
	// currentTX.sender = some of the depositers ID
	// currentTX.message[0] = free
	// currentTX.message[1] = free
	// currentTX.message[2] = free
	// currentTX.message[3] = free
	// currentTX.message[4] = free
	// currentTX.message[5] = free
	// currentTX.message[6] = free
	// currentTX.message[7] = free
	
	/**
	 * - the senderID can not be a map flag
	 * - the senderID has to be entitled
	 * - 
	 * - 
	 */
	
	if(IsIDOK(currentTX.sender) == 1 && getMapValue(ENTITLEMENT, currentTX.sender) == 1) {
		
		long deposit = getMapValue(DEPOSITMENT, currentTX.sender);
		
		// set DEPOSIT, ENTITLEMENT and VOTEPOINTS to 0
		setMapValue(DEPOSITMENT, currentTX.sender, 0);
		setMapValue(ENTITLEMENT, currentTX.sender, 0);
		setMapValue(VOTEPOINTS, currentTX.sender, 0);
		
		contractProvider--;
		
		if(deposit >= ONESIGNA) {
			
			if(contractProvider == 0) {
				sendAmount(Get_Current_Balance(), currentTX.sender);
			} else {
				sendAmount(deposit, currentTX.sender);
			}
		}
		
	} else {
		SendBack();
	}
}

// this is about voting for a poll
void VoteForPoll(void) {
	
	// ### incoming ###
	// currentTX.sender = some other contractProviderID as voter
	// currentTX.message[0] = contract method (VOTE_FOR_POLL)
	// currentTX.message[1] = hashValue
	// currentTX.message[2] = free
	// currentTX.message[3] = free
	// currentTX.message[4] = free
	// currentTX.message[5] = free
	// currentTX.message[6] = free
	// currentTX.message[7] = free
	
	getPollDetails(currentTX.message[1], currentTX.amount);
	
	/**
	 * - the hash of the poll must not be 0
	 * - the contractProvider count has to be bigger then 1
	 * - the current voters ID must not be the poll initiator
	 * - the voters ID can not be a map flag
	 * - the voters ID must be entitled
	 * - the voters ID must not have already voted for this poll
	 * - the poll must not have already timed out
	 * - the vote result of this voter must be ok
	 */
	
	if (currentPOLL.hash != 0 && contractProvider > 1 && currentPOLL.providerID != currentTX.sender && IsIDOK(currentTX.sender) != 0 && getMapValue(currentTX.sender, ENTITLEMENT) == 1 && getMapValue(currentPOLL.hash, currentTX.sender) == 0 && currentPOLL.timeOut == 0 && currentPOLL.voteResult != 0) {
		
		if (currentPOLL.voteResult == 1) {
			// rejected
			setMapValue(DEPOSITMENT, currentTX.sender, getMapValue(DEPOSITMENT, currentTX.sender) + currentTX.amount - currentFee);
			setMapValue(REJECTERS, currentPOLL.hash, getMapValue(REJECTERS, currentPOLL.hash) + 1);
		} else {
			// accepted
			setMapValue(DEPOSITMENT, currentPOLL.hash, getMapValue(DEPOSITMENT, currentPOLL.hash) + currentTX.amount - currentFee);
			setMapValue(AGREEERS, currentPOLL.hash, getMapValue(AGREEERS, currentPOLL.hash) + 1);
		}
		
		// refresh status
		currentPOLL.status = CheckThePoll(currentPOLL.hash);
		
		// set to already voted
		setMapValue(currentPOLL.hash, currentTX.sender, 1);
		
		// get the votepoints from the current voter
		long maxVotePoints = getMapValue(VOTEPOINTS, currentTX.sender) +1;
		// check if the max vote points of voter is bigger then the global votepoints
		if(maxVotePoints > maxGlobalVotePoints) {
			// set global vote points to the max vote points of voter
			maxGlobalVotePoints = maxVotePoints;
		}
		
		// increment votepoints of current voter
		setMapValue(VOTEPOINTS, currentTX.sender, maxVotePoints);
		
		// if status == 2 then wait for more votes else execute the poll
		if (currentPOLL.status != 2) {
			Execute(currentPOLL.status);
		}
		
	} else {
		
		if (currentPOLL.timeOut == 1) {
			// if status == 2 then reject the poll
			if (currentPOLL.status == 2) {
				Execute(0);
			} else {
				Execute(currentPOLL.status);
			}
		} else {
			SendBack();
		}
		
	}
	
}

// ### INTERNAL METHODS AND FUNCTIONS ###

// a vote of >50% is required for a successful poll
long CheckThePoll(long hashValue) {
	
	long rejecters = getMapValue(REJECTERS, hashValue);
	long accepters = getMapValue(AGREEERS, hashValue);
	
	if(rejecters > accepters && contractProvider == 2) {
		// poll rejected
		return 0;
	} else if(rejecters < accepters && contractProvider == 2) {
		// poll accepted
		return 1;
	}
	
	if(rejecters > (contractProvider / 2)) {
		// poll rejected
		return 0;
	} else if(accepters > (contractProvider / 2)) {
		// poll accepted
		return 1;
	}
	
	// not enough votes
	return 2;
}

void Execute(long pollStatus) {
	
	setMapValue(TIMEOUT, currentPOLL.hash, 0);
	
	if (pollStatus == 1) {
		
		// send reward to pollsterID
		sendAmount(currentPOLL.pollAmount * getMapValue(VOTEPOINTS, currentPOLL.providerID) / maxGlobalVotePoints, currentPOLL.providerID);
		
		switch (currentPOLL.mainMethod) {
			case DEPOSITING:
				setMapValue(ENTITLEMENT, currentPOLL.providerID, 1);
				contractProvider++;
				break;
			case REGISTER:
				
				// ### outgoing ###
				// recipient = targetContractID (123 (eg. ZeptorLight))
				// message[0] = method (REGISTER)
				// message[1] = parameter (authID index 0-9)
				// message[2] = actorID (authID 123)
				// message[3] = free
				// message[4] = free
				// message[5] = free
				// message[6] = free
				// message[7] = free
				
				SetSendBufferForTargetContract(REGISTER, currentPOLL.parameter, currentPOLL.actorID, 0, 0, 0, 0, 0);
				SendBufferWithFee(currentPOLL.targetID);
				
				break;
			case ACTING:
				
				// ### outgoing to ACTOR ###
				// recipient = actorContractID
				// message[0] = main method (ACTING)
				// message[1] = sub method
				// message[2] = PARAMETER
				// message[3] = ACTOR (wich part on this poll)
				// message[4] = free
				// message[5] = free
				// message[6] = free
				// message[7] = free
				
				SetSendBufferForTargetContract(ACTING, currentPOLL.subMethod, currentPOLL.parameter, ACTOR, 0, 0, 0, 0);
				SendBufferWithFee(currentPOLL.actorID);
				
				// ### outgoing to TARGET ###
				// recipient = targetContractID
				// message[0] = main method (ACTING)
				// message[1] = sub method
				// message[2] = PARAMETER
				// message[3] = TARGET (wich part on this poll)
				// message[4] = free
				// message[5] = free
				// message[6] = free
				// message[7] = free
				
				SetSendBufferForTargetContract(ACTING, currentPOLL.subMethod, currentPOLL.parameter, TARGET, 0, 0, 0, 0);
				SendBufferWithFee(currentPOLL.targetID);
				
				break;
		}
	}
	
}

void SendBack(void) {sendAmount(currentTX.amount - currentFee, currentTX.sender);}

// ### SUPPORT FUNCTIONS ###

long IsIDOK(long id) {
	// ID can't be one of the map flags
	
	switch(id) {
		case 0:
		case DEPOSITMENT:
		case ENTITLEMENT:
		case ELECTIONS:
		case PROVIDER_ID:
		case ACTOR_ID:
		case TARGET_ID:
		case MAINMETHOD:
		case SUBMETHOD:
		case PARAMETER:
		case TIMEOUT:
		case AGREEERS:
		case REJECTERS:
		case VOTEPOINTS:
			break;
		default:
			// contractIDs are excluded
			if(getCodeHashOf(id) == 0) {
				return 1;
			}
			break;
	}
	
	return 0;
}

// long GetPollType(long pollsterID) {
	
	// long tempValue = getMapValue(pollsterID, REGISTER);
	
	// if(tempValue > 0) {
		// return REGISTER;
	// }
	
	// tempValue = getMapValue(pollsterID, ACTION);
	// if(tempValue > 0) {
		// return ACTION;
	// }
	
	// return REJECT;
	
// }

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

// 0 = voting error
// 1 = voting rejected
// 2 = voting accepted
long Voting(long vote) {
	
	if(vote == 0) {
		return 0;
	}
	
	long voteCost = GetVoteCost();
	if(IsVoteValid(vote, voteCost, 10) == 1) {
		if(vote < voteCost) {
			return 1;
		} else {
			return 2;
		}
	} else {
		return 0;
	}
}

// calculate max deviation percent
long IsVoteValid(long vote, long target, long maxDeviationPercent) {
	
	// example: vote:21 signa; target:20 signa; maxDeviationPercent:10 percent
	
	// percentageResult = 20_0000_0000 * 10 / 100 = 2_0000_0000
	long percentageResult = target * maxDeviationPercent / 100;
	
	// vote:21_0000_0000 < target:20_0000_0000 - percentageResult:2_0000_0000 = 18_0000_0000 || vote:21_0000_0000 > target:20_0000_0000 + percentageResult:2_0000_0000 = 22_0000_0000
	// 21 signa < 18 signa || 21 signa > 22 signa = vote not ok
	if(vote < target - percentageResult || vote > target + percentageResult) {
		// vote not ok
		return 0;
	} else {
		// vote ok
		return 1;
	}

}

long GetVoteCost() {
	return currentPOLL.pollAmount / (contractProvider / 2);
}

long SetTimeOut(long time){return Get_Block_Timestamp() + ((time / 4) << 32);} //+(360 << 32); 15 * ~4min/block = 60min = 1 hour locktime

// 0 = time is not up
// 1 = time is up
// 2 = error
long GetTimeIsUp(long timeOut) {
	
	if(timeOut == 0) {
		return 2;
	}
	
	if(Get_Block_Timestamp() < timeOut) {
		return 0;
	} else {
		return 1;
	}
	
}

long GetB4FromHash256(long a1, long a2, long a3, long a4) {
	Set_A1_A2(a1, a2);
    Set_A3_A4(a3, a4);
	SHA256_A_To_B();
	return Get_B4();
}
