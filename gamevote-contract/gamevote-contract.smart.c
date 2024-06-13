
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

#program name GameVoteContract
#program description This contract serves as a consensus regulator (from a few to many voters) for target contracts
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
#define DEPOSITING 1
#define ACT 2
#define STOREPOLL 3
#define CHECKPOLL 4
#define WITHDRAWALING 5
#define VOTE_FOR_POLL 6

// gutes conversion tool: https://www.simonv.fr/TypesConvert/?integers
// #define DEPOSITING 0xc915c24f95f28d9d
// #define ACT 0x4b8e064b14d74d64
// #define WITHDRAWALING 0x64ff1046423d38df
// #define VOTE_FOR_POLL 0xddca044a939d3bb7


// map flags
#define REJECT 5
#define DEPOSIT 6
#define ACTION 7
#define VOTEPOINTS 8

long contractProvider = 0;
long pollContractID = 0; // set static before deploy contract
long maxGlobalVotePoints = 0; // 100%

long sendBuffer[8];

long providerIDs[100];

long currentFee = ONESIGNA;

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
	// get the last map entry index.
	long providerIndex = GetIndexOfProviderInMap(currentTX.sender);
	long lastIndex = GetLastIndexOfProvidersInMap();
	if((contractProvider == 0 || currentTX.amount <= 100_0000_0000) && IsIDOK(currentTX.sender) == 1 &&	providerIndex >= lastIndex) {
		
		// check if not already exist
		if(providerIndex >= lastIndex) {
			// set new entry as last entry
			// TODO: optimize 
			setMapValue(providerIndex, providerIndex, currentTX.sender);
		
		}
		setMapValue(currentTX.sender, DEPOSIT, currentTX.amount);
		providerIDs[contractProvider] = currentTX.sender;
		contractProvider++;
	} else {
		SendBack();
	}
	
}

// this serves to initiate an action
void Act(void) {
	
	// currentTX.sender = providerID (for reward)
	// currentTX.message[0] = method (ACT)
	// currentTX.message[1] = sub method (mining)
	// currentTX.message[2] = parameter (123)
	// currentTX.message[3] = actorContractID (to process sub method(parameter))
	// currentTX.message[4] = targetContractID (to process sub method(parameter))
	// currentTX.message[5] = free
	// currentTX.message[6] = free
	// currentTX.message[7] = free
	
	if(IsIDOK(currentTX.sender) == 0) {
		SendBack();
		return;
	}
	
	if(contractProvider <= 1) {
		// no voting necessary
		
		setMapValue(currentTX.sender, ACTION, 0);
		// send STOREPOLL as contract method, currentTX.message[1] as sub method, currentTX.message[2] as parameter, currentTX.message[3] as actorContractID, currentTX.message[4] as targetContractID, currentTX.sender as rewardRecipient and 1 as timeout for executing immediately
		
		SetSendBufferForTargetContract(STOREPOLL, currentTX.message[1], currentTX.message[2], currentTX.message[3], currentTX.message[4], currentTX.sender, 1, 0);
		SendBufferWithFee(pollContractID);
		
	} else {
		// check if there is no other poll active...
		if(getMapValue(currentTX.sender, ACTION) <= 0) {
			
			// recipientID = pollContractID (to process the pollresult)
			// message[0] = contract method (STOREPOLL)
			// message[1] = sub method (mining)
			// message[2] = parameter (123)
			// message[3] = actorContractID (to process sub method(parameter))
			// message[4] = targetContractID (to process sub method(parameter))
			// message[5] = providerID/pollsterID (for reward)
			// message[6] = timestamp (vote timeout in the future)
			// message[7] = free
			
			// save actorContractID in map for polling (his intend is voted on)
			setMapValue(currentTX.sender, ACTION, currentTX.message[3]);
			// send STOREPOLL as contract method, currentTX.message[1] as sub method, currentTX.message[2] as parameter, currentTX.message[3] as actorContractID, currentTX.message[4] as targetContractID, currentTX.sender as rewardRecipient and timeout time 
			SetSendBufferForTargetContract(STOREPOLL, currentTX.message[1], currentTX.message[2], currentTX.message[3], currentTX.message[4], currentTX.sender, SetTimeOut(ONEHOUR), 0);
			SendBufferWithFee(pollContractID);
			
		} else {
			SendBack();
		}
	}
}

// this is for a complete deregistration from the contract (no vote entitlement anymore)
void Withdrawaling(void) {
	
	if(IsIDOK(currentTX.sender) == 1 && GetIndexOfProviderInMap(currentTX.sender) < GetLastIndexOfProvidersInMap()) {
		
		long deposit = getMapValue(currentTX.sender, DEPOSIT);
		
		if(deposit >= ONESIGNA) {
			
			// set DEPOSIT and VOTEPOINTS to 0
			setMapValue(currentTX.sender, DEPOSIT, 0);
			setMapValue(currentTX.sender, VOTEPOINTS, 0);
			
			SetMaxVotePoints();
			contractProvider--;
			
			if(contractProvider == 0) {
				sendAmount(Get_Current_Balance(), currentTX.sender);
			} else {
				sendAmount(deposit, currentTX.sender);
			}
			
		} else {
			SendBack();
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
	
	long pollingActorContract = getMapValue(pollsterID, pollType);
	
	// if there is no will for ACTION then break
	if(pollingActorContract == 0) {
		SendBack();
		return;
	}
	
	// get pollsterID's DEPOSIT
	long pollingAmount = getMapValue(pollsterID, DEPOSIT);
	long vote = currentTX.amount;
	long votingEntitled = getMapValue(currentTX.sender, DEPOSIT);
	
	// check if entitled to vote
	if(pollingAmount > 0 && votingEntitled > 0) {
		
		// get TimeOut as Value from pollContractID as ContractID with PollsterID as Key1, pollingActorContract as Key2
		long isTimeUp = GetTimeIsUp(getExtMapValue(pollsterID, pollingActorContract, pollContractID));
		if(isTimeUp == 2) {
			SendBack();
			return;
		} else {
			if(isTimeUp == 1) {
				CheckThePoll(pollsterID, pollType, pollingAmount, 1);
				return;
			}
		}
		
		long votingResult = Voting(vote, pollingAmount);

		if(votingResult == 0) {
			// vote is out of acceptable range
			SendBack();
			return;
		}

		if(votingResult == 1) {
			// vote "reject" so increase the deposit
			setMapValue(currentTX.sender, DEPOSIT, votingEntitled + vote);
		}

		votingEntitled = getMapValue(currentTX.sender, pollsterID);
		// check if already voted
		if(votingEntitled == 0) {
			// check voteCost with currentTX.amount as vote
			if(votingResult == 2) {
				
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
				CheckThePoll(pollsterID, pollType, pollingAmount, 0);
			} else {
				SendBack();
			}
			
		} else {
			SendBack();
		}
	} else {
		if(pollingAmount == 0) {
			// if there is no DEPOSIT of PollsterID then clean up maps
			ExecuteAndCleanUpMap(pollsterID, REJECT, pollingAmount);
		}
		SendBack();
	}
}

// ### INTERNAL METHODS AND FUNCTIONS ###

// counting the votes of the deregistration
// a vote of >50% is required for a successful deregistration (and withdrawaling)
void CheckThePoll(long pollsterID, long pollType, long pollingAmount, long timeOut) {
	
	if(contractProvider > 2) {
		
		long okVotes = 0;
		long notOKVotes = 0;
		for (long i = 0; i < contractProvider; i++) {
			long vote = getMapValue(i, pollsterID);

			// just count until value reaches zero...
			if(vote == 0) {
				break;
			}

			long historyVoteResult = Voting(vote, pollingAmount);
			if(historyVoteResult == 2) { 
				// vote accepted
				okVotes++;
			} else { 
				// vote rejected
				notOKVotes++;
			}
		}
		
		if(okVotes > contractProvider / 2) {
			// poll OK
			ExecuteAndCleanUpMap(pollsterID, pollType, pollingAmount);
		} else {
			if(notOKVotes > contractProvider / 2) {
				// poll is NOT OK and voting has ended because they cant reach a OK end anymore
				ExecuteAndCleanUpMap(pollsterID, REJECT, pollingAmount);
			} else {
				
				if(timeOut == 1) {
					ExecuteAndCleanUpMap(pollsterID, REJECT, pollingAmount);
				}
				
				// poll is still NOT OK, wait for more votes...
			}
		}
		
	} else if(contractProvider == 2) {
		long vote2 = currentTX.message[2];
		
		if(vote2 == pollingAmount){
			// poll OK
			ExecuteAndCleanUpMap(pollsterID, pollType, pollingAmount);
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
			
			// get actorContractID from action poll
			long actorContractID = getMapValue(pollsterID, ACTION);
			
			// ### outgoing ###
			// recipient = pollContractID
			// message[0] = contract method (CHECKPOLL)
			// message[1] = pollsterID (poll initiator)
			// message[2] = actorContractID (action initiator)
			// message[3] = free
			// message[4] = free
			// message[5] = free
			// message[6] = free
			// message[7] = free
			
			// send CHECKPOLL as method, pollsterID as rewardRecipient, actorContractID as initiator to pollContractID
			SetSendBufferForTargetContract(CHECKPOLL, pollsterID, actorContractID, 0, 0, 0, 0, 0);
			SendBufferWithFee(pollContractID);
			
			break;
		
	}
	
	// set pollType to 0
	setMapValue(pollsterID, pollType, 0);
}

void SendBack(void) {sendAmount(currentTX.amount, currentTX.sender);}

// ### SUPPORT FUNCTIONS ###

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

long GetNextZeroValue(long pollsterID) {
	
	// get the last vote index for the pollsterID.
	for (long i = 0; i < contractProvider; i++) {
		long result = getMapValue(i, pollsterID);
		if(result == 0) {
			return i;
		}
	}
}

long IsIDOK(long id) {
	if(id != DEPOSIT && id != ACTION && id != VOTEPOINTS) {
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
	
	return REJECT;
	
}

void SetSendBufferForTargetContract(long pollType, long subMethod, long parameter, long sender, long executeTime, long reserve1, long reserve2, long reserve3) {
	sendBuffer[0] = pollType;
	sendBuffer[1] = subMethod;
	sendBuffer[2] = parameter;
	sendBuffer[3] = sender;
	sendBuffer[4] = executeTime;
	sendBuffer[5] = reserve1;
	sendBuffer[6] = reserve2;
	sendBuffer[7] = reserve3;
}

void SendBufferWithFee(long recipient) {
	sendAmountAndMessage(currentFee, sendBuffer, recipient);
	sendMessage(sendBuffer + 4, recipient);
}

// 0 = voting error
// 1 = voting rejected
// 2 = voting accepted
long Voting(long vote, long pollingAmount) {
	long voteCost = pollingAmount / (contractProvider / 2);
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
	
	if(vote < target - (target * maxDeviationPercent / 100) || vote > target + (target * maxDeviationPercent / 100)  ) {
		// vote not ok
		return 0;
	} else {
		// vote ok
		return 1;
	}

}

void SetMaxVotePoints() {
	maxGlobalVotePoints = 0;
	long i = 0;
	long mapID = 0;
	do {
		mapID = getMapValue(i, i);
		long tempVotePoints = getMapValue(mapID, VOTEPOINTS);
		if(tempVotePoints > maxGlobalVotePoints) {
			maxGlobalVotePoints = tempVotePoints;
		}
		i++;
	} while(mapID != 0);
}

long SetTimeOut(long time){return Get_Block_Timestamp() + ((time / 4) << 32);} //+(360 << 32); 15 * ~4min/block = 60min = 1 hour locktime
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

