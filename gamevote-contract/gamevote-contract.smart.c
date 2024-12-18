
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
#define ONE_WHOLE 100000000
#define ONEHOUR 60
#define TWENTYFOURHOURS 1440

// contract methods
#define DEPOSITING 1_000_000
#define CONTRACT_SPECIFIC 1_000_001
#define GAME_SPECIFIC 1_000_002
#define WITHDRAWALING 1_000_003

// vote contract specific
#define VOTE_FOR_POLL 1_000_004

// sub CONTRACT_SPECIFIC methods
//#define GAMEVOTE_CONTRACT 1_001_000
//#define ITEMBASE_CONTRACT 1_001_001
//#define MARKETPLACE_CONTRACT 1_001_002
//#define LOCATION_CONTRACT 1_001_003
//#define TEMPAUTH_CONTRACT 1_001_004
//#define SET_ITEMBASE 1_001_005
//#define SET_MARKETPLACE 1_001_006
//#define SET_OWNER 1_001_007
//#define SET_NFT_CONTRACT 1_001_008
//#define AUTHENTICATE 1_001_009
//#define CREATE_STATION 1_001_010

// sub GAME_SPECIFIC methods 
//#define ACT 1_002_001
//#define BUILD 1_002_002
//#define DELIVER 1_002_003
//#define DESCRIBE 1_002_004
//#define DOCK 1_002_005
//#define EQUIP 1_002_006
//#define EXPLODE 1_002_007
//#define INSURE 1_002_008
//#define INVENT 1_002_009
//#define MINING 1_002_010
//#define REFINE 1_002_011
//#define REPAIR 1_002_012
//#define SCAN 1_002_013
//#define STORE 1_002_014
//#define TRADE_CREATEORDER 1_002_015
//#define TRADE_ACCEPTORDER 1_002_016

// (ext)map flags
// standard
#define HASH 1_003_000
#define PROVIDER_ID 1_003_001
#define ACTOR_ID 1_003_002
#define TARGET_ID 1_003_003
#define MAINMETHOD 1_003_004
#define SUBMETHOD 1_003_005
#define PARAMETER 1_003_006
#define PARAMETER2 1_003_007
#define PARAMETER3 1_003_008
#define PARAMETER4 1_003_009

// vote contract specific
#define DEPOSITMENT 1_004_001
#define ENTITLEMENT 1_004_002
#define ELECTIONS 1_004_003
#define TIMEOUT 1_004_004
#define AGREEERS 1_004_005
#define REJECTERS 1_004_006
#define VOTEPOINTS 1_004_007

// item base specific
// item properties
//#define CATEGORY 1_005_001
//#define TYPE 1_005_002
//#define SIZE 1_005_003
//#define PROPERTIES 1_005_004
//#define DEFAULT_SPEED 1_005_005
//#define DEFAULT_STRUCTURE 1_005_006
//#define DEFAULT_ARMOR 1_005_007
//#define DEFAULT_SHIELD 1_005_008
//#define DEFAULT_CARGO 1_005_009
//#define COCKPITS 1_005_010
//#define SLOT 1_005_011
//#define SLOTS 1_005_012
//#define HANGAR 1_005_013
//#define HANGARS 1_005_014

// types
//#define GUN 1_006_001
//#define TURRET 1_006_002
//#define PULPIT 1_006_003
//#define LAUNCHER 1_006_004
//#define ENGINE 1_006_005
//#define SHIELD 1_006_006
//#define CARGO 1_006_007
//#define SCANNER 1_006_008
//#define DRONE 1_006_009

//#define FACILITY 1_006_010
//#define HANGAR_MODULE 1_006_011
//#define REFINERY_FACILITY 1_006_012
//#define ASSEMBLE_FACILITY 1_006_013
//#define ADMINISTRATIVE_FACILITY 1_006_014
//#define OBSERVATORY_FACILITY 1_006_015
//#define TRADE_HUB 1_006_016

//#define SHIP 1_006_017
//#define STATION 1_006_018
#define ASTEROID 1_006_019
//#define MOON 1_006_020
//#define PLANET 1_006_021
#define STAR 1_006_022

// slot and hangar types
//#define INTERNAL 1_007_001
//#define EXTERNAL 1_007_002

// slot types
// #define AMMO_SLOT 1_007_003
// #define GUN_SLOT 1_007_004
// #define TURRET_SLOT 1_007_005
// #define PULPIT_SLOT 1_007_006
// #define LAUNCHER_SLOT 1_007_007
// #define ENGINE_SLOT 1_007_008
// #define SHIELD_SLOT 1_007_009
// #define CARGO_SLOT 1_007_010
// #define SCANNER_SLOT 1_007_011
// #define FACILITY_SLOT 1_007_012

// hangar types
// #define DRONE_HANGAR 1_007_013
// #define SHIP_HANGAR 1_007_014
// #define STATION_HANGAR 1_007_015

// sizes and weights
//#define SMALL 1_008_001
//#define MEDIUM 1_008_002
//#define LARGE 1_008_003
//#define XLARGE 1_008_004
//#define CAPITAL 1_008_005

//#define LIGHT 1_008_006
//#define HEAVY 1_008_007

// item tree
//#define ELEMENT 1_009_000
//#define REFINED 1_009_001
//#define COMMON 1_009_002
//#define ADVANCED 1_009_003
//#define COMPONENT 1_009_004
//#define SYSTEM 1_009_005
//#define ARTICLE 1_009_006

// marketplace contract specific
//#define SELLORDER 1_010_001
//#define SYMBOL 1_010_002
//#define PRICE 1_010_003
//#define HOLDER 1_010_004

// artificialObject contract specific
//#define OWNER 1_011_001
//#define STATUS 1_011_002
#define AMOUNT 1_011_003

// extContract flags
#define ACTOR 1_012_001
#define TARGET 1_012_002

// contract attributes
// basic contract 
// long authIDs[10];
long sendBuffer[8];
long currentFee = ONE_WHOLE;

long contractProvider = 0;
long contractElections = 0;
long maxGlobalVotePoints = 1;

struct TXINFO
{
    long txId,
	timestamp,
	sender,
	amount;
	long message[8];
} currentTX;

struct POLLINFO
{
    long hash,
	pollAmount,
	amount,
	providerID,
	actorID,
	targetID,
	mainMethod,
	subMethod,
	parameter,
	parameter2,
	parameter3,
	parameter4,
	status,
	timeStamp,
	timeOut,
	voteResult;
} currentPOLL;

constructor();

void constructor(void) {
	
    // this function will be called only once on first activation.
    currentPOLL.hash = 0;
	setMapValue(18657918865985095, 1, 0); // 18657918865985095 = BIGBANG
}

void getTxDetails()
{
    currentTX.txId = Get_A1();
    currentTX.amount = getAmount(currentTX.txId);
    currentTX.timestamp = Get_Timestamp_For_Tx_In_A();
    currentTX.sender = getSender(currentTX.txId);
    readMessage(currentTX.txId, 0, currentTX.message);
    readMessage(currentTX.txId, 1, currentTX.message + 4);
}
void getPollDetails(long hashValue, long currentAmount)
{
	
    if (currentPOLL.hash != hashValue)
    {
        currentPOLL.hash = hashValue;
        currentPOLL.providerID = getMapValue(PROVIDER_ID, hashValue);
        currentPOLL.pollAmount = getMapValue(DEPOSITMENT, currentPOLL.providerID);
		currentPOLL.amount = getMapValue(AMOUNT, hashValue);
        currentPOLL.actorID = getMapValue(ACTOR_ID, hashValue);
        currentPOLL.targetID = getMapValue(TARGET_ID, hashValue);
        currentPOLL.mainMethod = getMapValue(MAINMETHOD, hashValue);
        currentPOLL.subMethod = getMapValue(SUBMETHOD, hashValue);
        currentPOLL.parameter = getMapValue(PARAMETER, hashValue);
        currentPOLL.parameter2 = getMapValue(PARAMETER2, hashValue);
        currentPOLL.parameter3 = getMapValue(PARAMETER3, hashValue);
        currentPOLL.parameter4 = getMapValue(PARAMETER4, hashValue);
        currentPOLL.timeStamp = getMapValue(TIMEOUT, hashValue);
	}
	
    //currentPOLL.status = CheckThePoll(hashValue);
    currentPOLL.timeOut = GetTimeIsUp(currentPOLL.timeStamp);
	
    if (currentAmount != 0)
    {
        currentPOLL.voteResult = Voting(currentAmount);
	}
    else
    {
        currentPOLL.voteResult = 0;
	}
	
}

void main(void)
{
    do
    {
        A_To_Tx_After_Timestamp(currentTX.timestamp);
        if (Get_A1() == 0)
        {
            break;
		}
		
        getTxDetails();
		
        switch (currentTX.message[0])
        {
            case CONTRACT_SPECIFIC:
				MainMethod(CONTRACT_SPECIFIC);
				break;
            case DEPOSITING:
				Depositing();
				break;
            case GAME_SPECIFIC:
				MainMethod(GAME_SPECIFIC);
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
void Depositing(void)
{
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
	if (getCodeHashOf(currentTX.sender) == 0 && (contractProvider == 0 || currentTX.amount <= 100_0000_0000) && IsIDOK(currentTX.sender, 0) == 1 && getMapValue(DEPOSITMENT, currentTX.sender) == 0)
	{
		// Deposits amount
		setMapValue(DEPOSITMENT, currentTX.sender, currentTX.amount);

		if (contractProvider <= 1)
		{
			setMapValue(ENTITLEMENT, currentTX.sender, 1);
			contractProvider++;
		}
		else
		{
			long timeout = SetTimeOut(ONEHOUR);
			long hashValue = GetB3FromHash256(currentTX.sender, currentTX.amount, 0, timeout);

			setMapValue(ELECTIONS, contractElections++, hashValue);

			setMapValue(ENTITLEMENT, currentTX.sender, 0);
			setMapValue(PROVIDER_ID, hashValue, currentTX.sender);
			setMapValue(MAINMETHOD, hashValue, DEPOSITING);
			setMapValue(TIMEOUT, hashValue, timeout);
		}
	}
	else
	{
		SendBack();
	}
}

void MainMethod(long type)
{
	// ### incoming ###
	// currentTX.sender = providerID (for reward)
	// currentTX.message[0] = main method (CONTRACT_SPECIFIC, GAME_SPECIFIC, ...)
	// currentTX.message[1] = sub method (SETOWNER, MINING, ...)
	// currentTX.message[2] = parameter
	// currentTX.message[3] = parameter2
	// currentTX.message[4] = parameter3
	// currentTX.message[5] = parameter4
	// currentTX.message[6] = actorID (to process sub method(parameter))
	// currentTX.message[7] = targetContractID (to process sub method(parameter))

	if (IsIDOK(currentTX.sender, 0) != 0 && IsIDOK(currentTX.message[6], 1) != 0 && IsIDOK(currentTX.message[7], 1) != 0 && getMapValue(ENTITLEMENT, currentTX.sender) == 1)
	{

		long timeout = SetTimeOut(ONEHOUR);
		long hashValue = GetB3FromHash256(currentTX.sender, currentTX.message[6], currentTX.message[7], timeout);

		setMapValue(ELECTIONS, contractElections++, hashValue);

		setMapValue(PROVIDER_ID, hashValue, currentTX.sender);
		setMapValue(AMOUNT, hashValue, currentTX.amount - ONE_WHOLE);
		setMapValue(ACTOR_ID, hashValue, currentTX.message[6]);
		setMapValue(TARGET_ID, hashValue, currentTX.message[7]);

		setMapValue(MAINMETHOD, hashValue, type);
		setMapValue(SUBMETHOD, hashValue, currentTX.message[1]);
		setMapValue(PARAMETER, hashValue, currentTX.message[2]);
		setMapValue(PARAMETER2, hashValue, currentTX.message[3]);
		setMapValue(PARAMETER3, hashValue, currentTX.message[4]);
		setMapValue(PARAMETER4, hashValue, currentTX.message[5]);
		setMapValue(TIMEOUT, hashValue, timeout);

		currentPOLL.hash = hashValue;
		currentPOLL.providerID = currentTX.sender;
		currentPOLL.pollAmount = getMapValue(DEPOSITMENT, currentTX.sender);
		currentPOLL.amount = currentTX.amount - ONE_WHOLE;
		currentPOLL.actorID = currentTX.message[6];
		currentPOLL.targetID = currentTX.message[7];
		currentPOLL.mainMethod = type;
		currentPOLL.subMethod = currentTX.message[1];
		currentPOLL.parameter = currentTX.message[2];
		currentPOLL.parameter2 = currentTX.message[3];
		currentPOLL.parameter3 = currentTX.message[4];
		currentPOLL.parameter4 = currentTX.message[5];
		currentPOLL.timeStamp = timeout;

		if (contractProvider <= 1)
		{
			// no voting necessary
			setMapValue(AGREEERS, currentPOLL.hash, 1);
			Execute(1);
		}
	}
	else if (IsIDOK(currentTX.sender, 0) != 0 && getCodeHashOf(currentTX.message[1]) != 0 && getCodeHashOf(currentTX.message[2]) != 0 && getCodeHashOf(currentTX.message[3]) != 0 && getMapValue(18657918865985095, 1) == 0)
	{
		setMapValue(18657918865985095, 1, currentTX.message[3]); // 18657918865985095 = BIGBANG
		SetSendBufferForTargetContract(18657918865985095, STAR, 0, 0, 0, 0, 0, 0);
		SendBufferWithAmount(currentFee, currentTX.message[1]);
		SetSendBufferForTargetContract(18657918865985095, ASTEROID, currentTX.message[1], 0, 0, 0, 0, 0);
		SendBufferWithAmount(currentFee, currentTX.message[2]);
	}
	else
	{
		SendBack();
	}
}

// this is for a complete deregistration from the contract (no vote entitlement anymore)
void Withdrawaling(void)
{
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

	if (IsIDOK(currentTX.sender, 0) == 1 && getMapValue(ENTITLEMENT, currentTX.sender) == 1)
	{

		long deposit = getMapValue(DEPOSITMENT, currentTX.sender);

		// set DEPOSIT, ENTITLEMENT and VOTEPOINTS to 0
		setMapValue(DEPOSITMENT, currentTX.sender, 0);
		setMapValue(ENTITLEMENT, currentTX.sender, 0);
		setMapValue(VOTEPOINTS, currentTX.sender, 0);

		contractProvider--;

		if (deposit >= ONE_WHOLE)
		{

			if (contractProvider == 0)
			{
				sendAmount(Get_Current_Balance(), currentTX.sender);
			}
			else
			{
				sendAmount(deposit, currentTX.sender);
			}
		}

	}
	else
	{
		SendBack();
	}
}

// this is about voting for a poll
void VoteForPoll(void)
{
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

	if (currentPOLL.hash != 0 && contractProvider > 1 && currentPOLL.providerID != currentTX.sender && IsIDOK(currentTX.sender, 0) != 0 && getMapValue(ENTITLEMENT, currentTX.sender) == 1 && getMapValue(currentPOLL.hash, currentTX.sender) == 0 && currentPOLL.timeOut == 0 && currentPOLL.voteResult != 0)
	{

		if (currentPOLL.voteResult == 1)
		{
			// rejected
			setMapValue(DEPOSITMENT, currentTX.sender, getMapValue(DEPOSITMENT, currentTX.sender) + currentTX.amount - currentFee);
			setMapValue(REJECTERS, currentPOLL.hash, getMapValue(REJECTERS, currentPOLL.hash) + 1);
		}
		else
		{
			// accepted
			setMapValue(DEPOSITMENT, currentPOLL.hash, getMapValue(DEPOSITMENT, currentPOLL.hash) + currentTX.amount - currentFee);
			setMapValue(AGREEERS, currentPOLL.hash, getMapValue(AGREEERS, currentPOLL.hash) + 1);
		}

		// refresh status
		currentPOLL.status = CheckThePoll(currentPOLL.hash);

		// set to already voted
		setMapValue(currentPOLL.hash, currentTX.sender, 1);

		// get the votepoints from the current voter
		long maxVotePoints = getMapValue(VOTEPOINTS, currentTX.sender) + 1;
		// check if the max vote points of voter is bigger then the global votepoints
		if (maxVotePoints > maxGlobalVotePoints)
		{
			// set global vote points to the max vote points of voter
			maxGlobalVotePoints = maxVotePoints;
		}

		// increment votepoints of current voter
		setMapValue(VOTEPOINTS, currentTX.sender, maxVotePoints);

		// if status == 2 then wait for more votes else execute the poll
		if (currentPOLL.status != 2)
		{
			Execute(currentPOLL.status);
		}

	}
	else
	{

		if (currentPOLL.timeOut == 1)
		{
			// if status == 2 then reject the poll
			if (currentPOLL.status == 2)
			{
				Execute(0);
			}
			else
			{
				Execute(currentPOLL.status);
			}
		}
		else
		{
			SendBack();
		}

	}
}

// ### INTERNAL METHODS AND FUNCTIONS ###

// a vote of >50% is required for a successful poll
long CheckThePoll(long hashValue)
{
	long rejecters = getMapValue(REJECTERS, hashValue);
	long accepters = getMapValue(AGREEERS, hashValue);

	if (rejecters > accepters && contractProvider == 2)
	{
		// poll rejected
		return 0;
	}
	else if (rejecters < accepters && contractProvider == 2)
	{
		// poll accepted
		return 1;
	}

	if (rejecters > ((contractProvider -1)/ 2))
	{
		// poll rejected
		return 0;
	}
	else if (accepters > ((contractProvider -1)/ 2))
	{
		// poll accepted
		return 1;
	}

	// not enough votes
	return 2;
}
void Execute(long pollStatus)
{
	setMapValue(TIMEOUT, currentPOLL.hash, 0);
	setMapValue(DEPOSITMENT, currentPOLL.hash, 0);
	long pieceAmount = (currentPOLL.pollAmount * getMapValue(VOTEPOINTS, currentPOLL.providerID) / maxGlobalVotePoints) - currentFee;
	if (pieceAmount < 0)
	{
		pieceAmount = 0;
	}

	if (pollStatus == 1)
	{
		switch (currentPOLL.mainMethod)
		{
			case DEPOSITING:
				setMapValue(ENTITLEMENT, currentPOLL.providerID, 1);
				contractProvider++;
				break;
			case CONTRACT_SPECIFIC:

				// send reward to pollsterID
				if (pieceAmount >= ONE_WHOLE)
				{
					sendAmount(pieceAmount, currentPOLL.providerID);
				}

				// ### outgoing ###
				// recipient = targetContractID (123 (eg. ZeptorLight))
				// message[0] = currentHash
				// message[1] = voteContractID (optional)
				// message[2] = free
				// message[3] = free
				// message[4] = free
				// message[5] = free
				// message[6] = free
				// message[7] = free

				SetSendBufferForTargetContract(HASH, currentPOLL.hash, 0, 0, 0, 0, 0, 0);
				SendBufferWithAmount(currentPOLL.amount + (currentPOLL.pollAmount  - pieceAmount), currentPOLL.targetID);

				break;
			case GAME_SPECIFIC:

				// send reward to pollsterID
				if (pieceAmount >= ONE_WHOLE)
				{
					sendAmount(pieceAmount, currentPOLL.providerID);
				}
				// ### outgoing to ACTOR ###
				// recipient = actorContractID
				// message[0] = currentHash
				// message[1] = ACTOR (wich part on this poll)
				// message[2] = free
				// message[3] = free
				// message[4] = free
				// message[5] = free
				// message[6] = free
				// message[7] = free
				
				if(getCodeHashOf(currentPOLL.actorID) != 0)
				{
					SetSendBufferForTargetContract(HASH, currentPOLL.hash, 0, 0, 0, 0, 0, ACTOR);
					SendBufferWithAmount((currentPOLL.pollAmount - (pieceAmount / 2)) / 2, currentPOLL.actorID);
				}

				// ### outgoing to TARGET ###
				// recipient = targetContractID
				// message[0] = currentHash
				// message[1] = TARGET (wich part on this poll)
				// message[2] = free
				// message[3] = free
				// message[4] = free
				// message[5] = free
				// message[6] = free
				// message[7] = free

				if(getCodeHashOf(currentPOLL.targetID) != 0)
				{
					SetSendBufferForTargetContract(HASH, currentPOLL.hash, 0, 0, 0, 0, 0, TARGET);
					SendBufferWithAmount(currentPOLL.amount + ((currentPOLL.pollAmount - (pieceAmount / 2)) / 2), currentPOLL.targetID);
				}

				break;
		}
	}
	else
	{
		sendAmount(currentPOLL.pollAmount + (currentPOLL.amount - currentFee), currentPOLL.providerID);
	}
}

void SendBack() { sendAmount(currentTX.amount - currentFee, currentTX.sender); }

// ### SUPPORT FUNCTIONS ###

long IsIDOK(long id, long contractOK)
{
	// ID can't be one of the map flags
	
	switch (id)
	{
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
		if (contractOK == 1 || getCodeHashOf(id) == 0)
		{
			return 1;
		}
		break;
	}
	
	return 0;
}

void SetSendBufferForTargetContract(long buffer1, long buffer2, long buffer3, long buffer4, long buffer5, long buffer6, long buffer7, long buffer8)
{
	sendBuffer[0] = buffer1;
	sendBuffer[1] = buffer2;
	sendBuffer[2] = buffer3;
	sendBuffer[3] = buffer4;
	sendBuffer[4] = buffer5;
	sendBuffer[5] = buffer6;
	sendBuffer[6] = buffer7;
	sendBuffer[7] = buffer8;
}

void SendBufferWithAmount(long amount, long recipient)
{
	sendAmountAndMessage(amount - currentFee, sendBuffer, recipient);
	sendMessage(sendBuffer + 4, recipient);
}

// 0 = voting error
// 1 = voting rejected
// 2 = voting accepted
long Voting(long vote)
{
	
	if (vote == 0)
	{
		return 0;
	}
	
	long voteCost = GetVoteCost();
	if (IsVoteValid(vote, voteCost, 10) == 1)
	{
		if (vote < voteCost)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return 0;
	}
}

// calculate max deviation percent
long IsVoteValid(long vote, long target, long maxDeviationPercent)
{
	
	// example: vote:21 signa; target:20 signa; maxDeviationPercent:10 percent
	
	// percentageResult = 20_0000_0000 * 10 / 100 = 2_0000_0000
	long percentageResult = target * maxDeviationPercent / 100;
	
	// vote:21_0000_0000 < target:20_0000_0000 - percentageResult:2_0000_0000 = 18_0000_0000 || vote:21_0000_0000 > target:20_0000_0000 + percentageResult:2_0000_0000 = 22_0000_0000
	// 21 signa < 18 signa || 21 signa > 22 signa = vote not ok
	if (vote < target - percentageResult || vote > target + percentageResult)
	{
		// vote not ok
		return 0;
	}
	else
	{
		// vote ok
		return 1;
	}
	
}

long GetVoteCost(void)
{
	return currentPOLL.pollAmount / (contractProvider / 2);
}

long SetTimeOut(long time) { return Get_Block_Timestamp() + ((time / 4) << 32); } //+(360 << 32); 15 * ~4min/block = 60min = 1 hour locktime

// 0 = time is not up
// 1 = time is up
// 2 = error
long GetTimeIsUp(long timeOut)
{
	
	if (timeOut == 0)
	{
		return 2;
	}
	
	if (Get_Block_Timestamp() < timeOut)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	
}

long GetB3FromHash256(long a1, long a2, long a3, long a4)
{
	Set_A1_A2(a1, a2);
	Set_A3_A4(a3, a4);
	SHA256_A_To_B();
	return Get_B3();
}
