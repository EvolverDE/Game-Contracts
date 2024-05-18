
/**
 * @author evolver
 * this is the smart contract for the flashwork second layer solution
 * features:
 * - depositing and lock a amount for the second layer
 * - get profit with votepoints
 * - request a withdrawal that needs to be voted on
 * - vote for withdrawal requests
 * - prevent doublevotes
 * - prevent fraud
 * still in developement
 */

#include APIFunctions
#program name AXTPoolContractRef
#program activationAmount .5
#pragma optimizationLevel 3
#pragma maxAuxVars 3
#pragma version 2.2.1

// @evolver Ich wuerde einfache Zahlen wie 1,2,3,4 fuer die Methoden Ids verwenden. Damit ich erspare ich mir das nervige umrechnen in LE

#define DEPOSITING 1
#define ACT 2
#define WITHDRAWALING 3
#define VOTE_FOR_POLL 4

// gutes conversion tool: https://www.simonv.fr/TypesConvert/?integers
// #define DEPOSITING 0xc915c24f95f28d9d
// #define ACT 0x4b8e064b14d74d64
// #define WITHDRAWALING 0x64ff1046423d38df
// #define VOTE_FOR_POLL 0xddca044a939d3bb7

#define REJECT 0x0000000000000000
#define DEPOSIT 0x0000000000000001
#define ACTION 0x0000000000000002
#define VOTEPOINTS 0x0000000000000003
#define WITHDRAWAL 0x0000000000000004

long contractProvider = 0;
long maxGlobalVotePoints = 0; // 100%

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
			case DEPOSITING:
				Depositing();
				break;
			case ACT:
				Act();
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
	// check minimal depositAmount

	// @evolver GetIndexOfProviderInMap ist eine teure Funktion...wenn notwendig wuerde ich die nur einmal aufrufen. Hier ist definitiv noch optimierungsspielraum
	if((contractProvider == 0 || currentTX.amount <= 100_0000_0000)
	//&& IsIDOK(currentTX.sender) == 1
// 	&& GetIndexOfProviderInMap(currentTX.sender) >= GetLastIndexOfProvidersInMap()
	) {
		
		// get the last map entry index.
		long index = GetIndexOfProviderInMap(currentTX.sender);
		long lastIndex = GetLastIndexOfProvidersInMap();
		
		// check if not already exist
		if(index >= lastIndex) {
			// set new entry as last entry
			// @evolver index, index ???? Das riecht nach Verbesserung
			setMapValue(index, index, currentTX.sender);
		}
		setMapValue(currentTX.sender, DEPOSIT, currentTX.amount);
		contractProvider++;
	} else {
		SendBack();
	}
	
}

// this serves to initiate an action
void Act(void) {
	
	if(IsIDOK(currentTX.sender) == 0) {
		SendBack();
		return;
	}
	
	// TODO: save command(parameter) somewhere(maybe prevTX?) and determine key1 and key2
	
	// long targetContractID = currentTX.message[1]; // target contract
	// long command = currentTX.message[2]; // maybe ingame: mining; fitting; other contract activities
	// long parameter = currentTX.message[3]; // maybe mining(123); fitting(contractID/objectID)
	
	// long targetFactor = getExtMapValue(key1, key2, targetContractID);
	
	// sendAmountAndMessage(long amount, long * buffer, targetContractID);
	// long msg[4];
	// msg[0] = 
	// sendMessage({long1, long2}, accountID);
	
	// sendAmount(Get_Current_Balance() - GAS_FEE, Initiator);
	
	if(contractProvider <= 1) {
		// no voting necessary
		
		setMapValue(currentTX.sender, ACTION, 0);
		setMapValue(currentTX.sender, WITHDRAWAL, 0);
		// send ACT as method, currentTX.message[2] as command, currentTX.message[3] as parameter and 1 as executeflag to currentTX.message[1] as targetContractID
		sendMessageSC(currentTX.message[1], ACT, currentTX.message[2], currentTX.message[3], 1);
	} else {
		if(getMapValue(currentTX.sender, WITHDRAWAL) <= 0) {
			// set action poll with currentTX.message[1] as targetContractID
			setMapValue(currentTX.sender, ACTION, currentTX.message[1]);
			// send ACT as method, currentTX.message[2] as command, currentTX.message[3] as parameter and 0 as executeflag to currentTX.message[1] as targetContractID
			sendMessageSC(currentTX.message[1], ACT, currentTX.message[2], currentTX.message[3], 0);
		} else {
			SendBack();
		}
	}
}

// this is for a complete deregistration from the contract (no vote entitlement anymore)
void Withdrawaling(void) {
	
	if(IsIDOK(currentTX.sender) == 1 && GetIndexOfProviderInMap(currentTX.sender) < GetLastIndexOfProvidersInMap()) {
		
		// set the withdrawalAmount to vote for
		if(contractProvider <= 1) {
			// no voting necessary
			if(getMapValue(currentTX.sender, DEPOSIT) >= 1) {
				setMapValue(currentTX.sender, DEPOSIT, 0);
				contractProvider--;
				sendAmount(Get_Current_Balance(), currentTX.sender);
			} else {
				SendBack();
			}
			
		} else {
			if(getMapValue(currentTX.sender, ACTION) <= 0) {
				setMapValue(currentTX.sender, WITHDRAWAL, 1);
			} else {
				SendBack();
			}
		}
		
	} else {
		SendBack();
	}
}

// this is about voting for a poll
// vote == 0 -> abstained from voting
// vote != pollingAmount -> vote rejected
// vote == pollingAmount -> vote accepted
void VoteForPoll(void) {
	
	// no vote needed
	if(contractProvider <= 1) {
		return;
	}
	
	long pollsterID = currentTX.message[1];
	
	// the pollsterID cannot vote for himself
	if(pollsterID == currentTX.sender || (IsIDOK(pollsterID) == 0 && IsIDOK(currentTX.sender) == 0)) {
		return;
	}
	
	long pollType = GetPollType(pollsterID);
	
	if(pollType == REJECT) {
		return;
	}
	
	long pollingAmount = getMapValue(pollsterID, pollType);
	long vote = currentTX.message[1];
	long votingEntitled = getMapValue(currentTX.sender, DEPOSIT);
	
	if(vote != pollingAmount / (contractProvider / 2)) {
		// vote "reject" so increase the deposit
		setMapValue(currentTX.sender, DEPOSIT, votingEntitled + currentTX.amount);
	}
	
	// check if withdrawal and entitled
	if(pollingAmount > 0 && votingEntitled > 0) {
		pollingAmount = getMapValue(pollsterID, DEPOSIT);
		votingEntitled = getMapValue(pollsterID, currentTX.sender);
		// check if already voted
		if(votingEntitled == 0) {
			// check voteCost with currentTX.amount as vote
			if(currentTX.amount >= pollingAmount / (contractProvider / 2)) {
				
				// set the vote: iteration for pollsterID, accepted(vote == pollingAmount) or rejected(vote != pollingAmount)
				setMapValue(GetNextZeroValue(pollsterID), pollsterID, vote);
				// prevent doublevoting by set key1 to the votersID, key2 to pollsterID and value to accepted(vote == pollingAmount) or rejected(vote != pollingAmount)
				setMapValue(currentTX.sender, pollsterID, vote);
				// get max vote points of voter
				long maxVotePoints = getMapValue(currentTX.sender, VOTEPOINTS);
				// check the max vote points of voter
				if(maxVotePoints > maxGlobalVotePoints) {
					// set global vote points to the max vote points of voter
					maxGlobalVotePoints = maxVotePoints;
				}
				// set vote points +1 for the voter
				setMapValue(currentTX.sender, VOTEPOINTS, maxVotePoints +1);
				// checking withdrawal
				CheckThePoll(pollsterID, pollType, pollingAmount);
			} else {
				SendBack();
			}
			
		} else {
			SendBack();
		}
	} else {
		if(pollingAmount == 0) {
			ExecuteAndCleanUpMap(pollsterID, REJECT, pollingAmount);
		}
		SendBack();
	}
}

// STILL IN DEVELOPEMENT
void FullPayoutContractBalance(void) {
	long payoutVotes = 0;
	long maxPayoutSum = 0;
	long memberID = 0;
	
	for (long j = 0; j < contractProvider; j++) {
		long i = 0;
		
		do {
			memberID = getMapValue(i, i);
		    maxPayoutSum += getMapValue(memberID, 0);
			if(maxPayoutSum > 0) {
				payoutVotes++;
			}
			i++;
		} while(memberID != 0);
	}
	
	memberID = 0;
	
	if(payoutVotes == contractProvider) {
		long maxPayoutOne = 0;
		long k = 0;
		if(maxPayoutSum == Get_Current_Balance()) {
			//TODO: withdrawal
			do {
				memberID = getMapValue(k, k);
				maxPayoutOne = getMapValue(memberID, 0);
				
				// TODO: withdrawaling(memberID, maxPayoutOne)
				
				// set withdrawal to zero
				setMapValue(memberID, 0, 0);
				k++;
			} while(memberID != 0);
		} else {
			do {
				memberID = getMapValue(k, k);
				// set withdrawal to zero
				setMapValue(memberID, 0, 0);
				k++;
			} while(memberID != 0);
		}
	}
	// wait for more...
}

// ### INTERNAL METHODS AND FUNCTIONS ###

// counting the votes of the deregistration
// a vote of >50% is required for a successful deregistration (and withdrawaling)
void CheckThePoll(long pollsterID, long pollType, long pollingAmount) {
	
	if(contractProvider > 2) {
		
		long okVotes = 0;
		long notOKVotes = 0;
		for (long i = 0; i < contractProvider; i++) {
			long vote = getMapValue(i, pollsterID);
			// just count until value reaches zero...
			if(vote == 0) {
				break;
			}
			
			if(vote == pollingAmount / (contractProvider / 2)) { 
				// vote accepted
				okVotes++;
			} else { 
				// vote rejected
				notOKVotes++;
			}
		}
		
		if(okVotes > contractProvider / 2) {
			// poll OK
			ExecuteAndCleanUpMap(pollsterID, pollType, pollingAmount - 132200000);
		} else {
			if(notOKVotes > contractProvider / 2) {
				// poll is NOT OK and voting has ended because they cant reach a OK end anymore
				ExecuteAndCleanUpMap(pollsterID, REJECT, pollingAmount);
			} else {
				// poll is still NOT OK, wait for more votes...
			}
		}
		
	} else if(contractProvider == 2) {
		long vote2 = currentTX.message[2];
		
		if(vote2 == pollingAmount){
			// poll OK
			ExecuteAndCleanUpMap(pollsterID, pollType, pollingAmount - 132200000);
		} else {
			// poll is NOT OK and voting has ended because they cant reach a OK end anymore
			ExecuteAndCleanUpMap(pollsterID, REJECT, pollingAmount);
		}
		
	} else {
		// poll OK
		ExecuteAndCleanUpMap(pollsterID, pollType, pollingAmount);
	}
}

void ExecuteAndCleanUpMap(long pollsterID, long pollType, long pollingAmount) {
	// set all votes to 0 (abstained from voting)
	long lastZero = GetNextZeroValue(pollsterID);
	for(long i = 0 ; i < lastZero; i++)	{
		setMapValue(i, pollsterID, 0);
	}
	
	// set voters back to zero (its for preventing doublevoting).
	long providerID = 0;
	long j = 0;
	do {
		providerID = getMapValue(j, j);
		// providerID for pollsterID accepted(his withdrawal) or rejected(not his withdrawal) or abstained(0)
		setMapValue(providerID, pollsterID, 0);
		j++;
	} while(providerID != 0);
	
	switch (pollType) {
		case REJECT:
			// send pollingAmount to target contract
			if(pollingAmount > 0) {
				// TODO
			}
			break;
		case ACTION:
			// send providing reward
			sendAmount(pollingAmount, pollsterID);
			
			// send the execution action to target contract
			
			// get targetContractID from action poll
			long targetContractID = getMapValue(pollsterID, ACTION);
			// get command from targetContractID
			long command = getExtMapValue(0, ACTION, targetContractID);
			// get command parameter from targetContractID
			long parameter = getExtMapValue(0, command, targetContractID);
			// send ACT as method, command, parameter and 1 as executeflag to targetContractID
			sendMessageSC(targetContractID, ACT, command, parameter, 1);
			
			break;
		case WITHDRAWAL:
			// set pollster's vote points back to 0
			setMapValue(pollsterID, VOTEPOINTS, 0);
			// send back pollster's deposit
			sendAmount(pollingAmount, pollsterID);
			break;
	}
	
	// set pollType to 0
	setMapValue(pollsterID, pollType, 0);
}

void SendBack(void) {sendAmount(currentTX.amount, currentTX.sender);}

//### SUPPORT FUNCTIONS ###

long GetLastIndexOfProvidersInMap() {
	return GetIndexOfProviderInMap(0);
}

long GetIndexOfProviderInMap(long providerID) {
	long i = 0;
	long mapID = 0;
	do {
		mapID = getMapValue(i, i);
		i++;
	} while(mapID != 0 && mapID != providerID);
	return i -1;
}

long GetNextZeroValue(long withdrawerID) {
	
	// get the last vote index for the withdrawerID.
	for (long i = 0; i < contractProvider; i++) {
		long result = getMapValue(i, withdrawerID);
		if(result == 0) {
			return i;
		}
	}
}

long IsIDOK(long id) {
	if(id != DEPOSIT && id != ACTION && id != VOTEPOINTS && id != WITHDRAWAL) {
		return 1;
	} else {
		return 0;
	}
}

long GetPollType(long pollsterID) {
	
	long tempValue = getMapValue(pollsterID, ACTION);
	if(tempValue > 0) {
		return ACTION;
	}
	
	tempValue = getMapValue(pollsterID, WITHDRAWAL);
	
	if(tempValue > 0) {
		return WITHDRAWAL;
	} else {
		return REJECT;
	}
	
}

void sendMessageSC(long recipient, long messageToSend1, long messageToSend2, long messageToSend3, long messageToSend4) {
    Set_B1(recipient);
    Set_A1_A2(messageToSend1, messageToSend2);
    Set_A3_A4(messageToSend3, messageToSend4);
    Send_A_To_Address_In_B();
}

