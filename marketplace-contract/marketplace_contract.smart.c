 /**
 * @author evolver
 * this smart contract implements the marketplace for items
 * features:
 * - interacts with other artificial contracts
 * - it can create buy- and sellorders
 * TODOs:
 * - the marketplace have to check the TRADE_HUB of the artificial object contract
 * - the contract have to check the storage of the artificial object contract
 * - the contract have to send a store out to the artificial contract wich has a trade-hub
 * - maybe check the authentification on artificial contract side (or implement the marketplace contract id there)
 */
 
#include APIFunctions

#program name ItemBaseContract
#program description This contract serves as a database for item properties and inventions
#program activationAmount .5

#pragma maxAuxVars 4
#pragma maxConstVars 3
#pragma optimizationLevel 3
#pragma version 2.2.1

// enumeration substitute
const long ONE_WHOLE = 100000000;

// contract methods
//#define DEPOSITING 1_000_000
#define CONTRACT_SPECIFIC 1_000_001
#define GAME_SPECIFIC 1_000_002
//#define WITHDRAWALING 1_000_003

// vote contract specific
//#define VOTE_FOR_POLL 1_000_004

// sub CONTRACT_SPECIFIC methods
#define GAMEVOTE_CONTRACT 1_001_000
//#define ITEMBASE_CONTRACT 1_001_001
//#define MARKETPLACE_CONTRACT 1_001_002
#define LOCATION_CONTRACT 1_001_003
#define TEMPAUTH_CONTRACT 1_001_004
//#define SET_ITEMBASE 1_001_005
//#define SET_MARKETPLACE 1_001_006
//#define SET_OWNER 1_001_007
//#define SET_NFT_CONTRACT 1_001_008
#define AUTHENTICATE 1_001_009
//#define CREATE_STATION 1_001_010

// sub GAME_SPECIFIC methods 
//#define ACT 1_002_001
//#define BUILD 1_002_002
//#define DELIVER 1_002_003
#define DESCRIBE 1_002_004
//#define DOCK 1_002_005
//#define EQUIP 1_002_006
//#define EXPLODE 1_002_007
//#define INSURE 1_002_008
#define INVENT 1_002_009
//#define MINING 1_002_010
//#define REFINE 1_002_011
//#define REPAIR 1_002_012
//#define SCAN 1_002_013
#define STORE 1_002_014
#define TRADE 1_002_015

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
// #define ENTITLEMENT 1_004_002
// #define ELECTIONS 1_004_003
// #define TIMEOUT 1_004_004
// #define AGREEERS 1_004_005
// #define REJECTERS 1_004_006
// #define VOTEPOINTS 1_004_007

// item base specific
// item properties
//#define CATEGORY 1_005_001
#define TYPE 1_005_002
//#define SIZE 1_005_003
//#define PROPERTIES 1_005_004
//#define DEFAULT_SPEED 1_005_005
//#define DEFAULT_STRUCTURE 1_005_006
//#define DEFAULT_ARMOR 1_005_007
//#define DEFAULT_SHIELD 1_005_008
//#define DEFAULT_CARGO 1_005_009
//#define COCKPITS 1_005_010
#define SLOT 1_005_011
#define SLOTS 1_005_012
//#define HANGAR 1_005_013
#define HANGARS 1_005_014

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
#define TRADE_HUB 1_006_016

//#define SHIP 1_006_017
#define STATION 1_006_018
//#define ASTEROID 1_006_019
//#define MOON 1_006_020
//#define PLANET 1_006_021
//#define STAR 1_006_022

// slot and Hangar types
//#define INTERNAL 1_007_001
//#define EXTERNAL 1_007_002

// slot types
//#define AMMO_SLOT 1_007_003
//#define GUN_SLOT 1_007_004
//#define TURRET_SLOT 1_007_005
//#define PULPIT_SLOT 1_007_006
//#define LAUNCHER_SLOT 1_007_007
//#define ENGINE_SLOT 1_007_008
//#define SHIELD_SLOT 1_007_009
//#define CARGO_SLOT 1_007_010
//#define SCANNER_SLOT 1_007_011
//#define FACILITY_SLOT 1_007_012

// hangar types
//#define DRONE_HANGAR 1_007_013
//#define SHIP_HANGAR 1_007_014
//#define STATION_HANGAR 1_007_015

// sizes and weights
//#define SMALL 1_008_017
//#define MEDIUM 1_008_018
//#define LARGE 1_008_019
//#define XLARGE 1_008_020
//#define CAPITAL 1_008_021

//#define LIGHT 1_008_022
//#define HEAVY 1_008_023

// item tree
//#define ELEMENT 1_009_000
//#define REFINED 1_009_001
//#define COMMON 1_009_002
//#define ADVANCED 1_009_003
//#define COMPONENT 1_009_004
//#define SYSTEM 1_009_005
//#define ARTICLE 1_009_006

// marketplace contract specific
#define SYMBOL 1_010_001
#define PRICE 1_010_002

// artificial object contract specific
#define OWNER 1_011_001
#define STATUS 1_011_002
#define AMOUNT 1_011_003

// extContract flags
//#define ACTOR 1_012_001
//#define TARGET 1_012_002

// contract attributes
// basic contract
long contractId = 0;
long gameVoteContract = 0;
long sendBuffer[8];
long currentFee = ONE_WHOLE;

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
	partOfPoll,
	mainMethod,
	subMethod,
	parameter,
	parameter2,
	parameter3,
	parameter4;
} currentPOLL;

struct ORDERINFO {
    long hash,
	isSellOrder,
	symbol,
	amount,
	available,
	price,
	total,
	location,
	owner,
	holder,
	status;
} currentORDER;

constructor();

void constructor(void) {
    // this function will be called only once on first activation.
	contractId = GetContractID();
	gameVoteContract = 1111; //TODO: set initial ID
	setMapValue(GAMEVOTE_CONTRACT, 1, gameVoteContract);
	currentPOLL.hash = 0;
	currentPOLL.actorID = 0;
}

long GetContractID() {
	A_To_Tx_After_Timestamp(Get_Creation_Timestamp());
	return Get_A1();
}

void getTxDetails(void)
{
    currentTX.txId = Get_A1();
    currentTX.amount = getAmount(currentTX.txId);
    currentTX.timestamp = Get_Timestamp_For_Tx_In_A();
    currentTX.sender = getSender(currentTX.txId);
    readMessage(currentTX.txId, 0, currentTX.message);
    readMessage(currentTX.txId, 1, currentTX.message + 4);
}
void getPollDetails(long hashValue, long pollSaveID)
{

    if (currentPOLL.hash != hashValue)
    {
        currentPOLL.hash = hashValue;
        currentPOLL.providerID = getExtMapValue(PROVIDER_ID, hashValue, pollSaveID);
        currentPOLL.pollAmount = getExtMapValue(DEPOSITMENT, currentPOLL.providerID, pollSaveID);
        currentPOLL.actorID = getExtMapValue(ACTOR_ID, hashValue, pollSaveID);
        currentPOLL.targetID = getExtMapValue(TARGET_ID, hashValue, pollSaveID);
        currentPOLL.mainMethod = getExtMapValue(MAINMETHOD, hashValue, pollSaveID);
        currentPOLL.subMethod = getExtMapValue(SUBMETHOD, hashValue, pollSaveID);
        currentPOLL.parameter = getExtMapValue(PARAMETER, hashValue, pollSaveID);
        currentPOLL.parameter2 = getExtMapValue(PARAMETER2, hashValue, pollSaveID);
        currentPOLL.parameter3 = getExtMapValue(PARAMETER3, hashValue, pollSaveID);
        currentPOLL.parameter4 = getExtMapValue(PARAMETER4, hashValue, pollSaveID);
    }

    currentPOLL.partOfPoll = currentTX.message[1];

}

void getOrderDetails(long hashValue, long holderID)
{
	currentORDER.hash = hashValue;
	currentORDER.symbol = getMapValue(SYMBOL, hashValue);
	currentORDER.price = getMapValue(PRICE, hashValue);
	currentORDER.location = getMapValue(LOCATION_CONTRACT, hashValue);
	currentORDER.owner = getMapValue(OWNER, hashValue);

	currentORDER.amount = getMapValue(AMOUNT, hashValue);
	currentORDER.status = 1; // getMapValue(STATUS, hashValue);

	if (currentORDER.amount < 0)
	{
		currentORDER.amount = currentORDER.amount * -1;
		currentORDER.isSellOrder = 1;
		currentORDER.holder = contractId;
	}
	else if (currentORDER.amount > 0)
	{
		currentORDER.isSellOrder = 0;
		currentORDER.holder = holderID;
	}
	else
	{
		currentORDER.isSellOrder = -1;
		currentORDER.holder = 0;
		currentORDER.status = 0;
	}

	if (currentORDER.status == 1)
	{
		currentORDER.available = getExtMapValue(hashValue, getExtMapValue(STORE, currentORDER.holder, currentORDER.location), currentORDER.location);
		currentORDER.total = currentORDER.amount * currentORDER.price;
	}
	else
	{
		currentORDER.available = 0;
		currentORDER.total = 0;
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

        // if sender is not authenticated then break
        if (IsAuthenticated(currentTX.sender) == 0)
        {
            break;
        }
        else
        {

            // ### income ###
            // currentTX.sender = voteContract
            // currentTX.message[0] = currentHash
            // currentTX.message[1] = ACTOR (wich part on this poll)
            // currentTX.message[2] = free
            // currentTX.message[3] = free
            // currentTX.message[4] = free
            // currentTX.message[5] = free
            // currentTX.message[6] = free
            // currentTX.message[7] = free

            if (getCodeHashOf(currentTX.sender) != 0)
			{
				if(currentTX.message[0] == HASH)
				{
					getPollDetails(currentTX.message[1], gameVoteContract);
				}
			}
			else
			{
				break;
			}
        }

        switch (currentPOLL.mainMethod)
        {
            case CONTRACT_SPECIFIC:
                ContractSpecific();
                break;
            case GAME_SPECIFIC:
                GameSpecific();
                break;
            default:
                break;
        }
    } while (true);
}

// main methods
void ContractSpecific(void)
{
    setMapValue(TEMPAUTH_CONTRACT, currentPOLL.parameter, 1);
}
void GameSpecific(void)
{
    // ### income ###
    // currentTX.sender = gamevoteContract
    // currentTX.message[0] = currentHash
    // currentTX.message[1] = TARGET (wich part on this poll)
    // currentTX.message[2] = voteContractID (optional)
    // currentTX.message[3] = free
    // currentTX.message[4] = free
    // currentTX.message[5] = free
    // currentTX.message[6] = free
    // currentTX.message[7] = free

    switch (currentPOLL.subMethod)
    {
		case TRADE:
			Trade();
			break;
		default:
			break;
    }

}

// sub methods

void Trade()
{

	/* incoming message (input)
	* currentPOLL.mainMethod = GAME_SPECIFIC
	* currentPOLL.subMethod = TRADE
	* currentPOLL.parameter = SYMBOL
	* currentPOLL.parameter2 = IRON (SYMBOL, [HASH (of TradeEntry)])
	* currentPOLL.parameter3 = -123 (sell amount)
	* currentPOLL.parameter4 = 456 (price)
	* currentPOLL.actorID = 10000 (origin actor(owner))
	* currentPOLL.targetID = 3333 (station contract(location))
	* currentPOLL.partOfPoll = TARGET
	*/

	/* TODOs: checking and processing (putput)
	* - status
	* - TRADE_HUB of target contract
	* - Amount of item strored at target contract
	* - check if amount of item is owned by actorID at target storage 
	*/

	if (getExtMapValue(STATUS, 1, currentPOLL.targetID) == 1) 
	{
		long slot = FindTradeHubSlot(currentPOLL.targetID);
		if (slot != 0)
		{
			long amount = currentPOLL.parameter3;

			if (amount < 0)
			{
				amount = amount * -1;
			}

			if (currentPOLL.parameter == SYMBOL)
			{
				// currentPOLL.parameter is SYMBOL
				// create new order
				// check item and enough amount on target contract map for selling
				// OR positive amount for buying
				if (getExtMapValue(currentPOLL.parameter2, getExtMapValue(STORE, currentPOLL.actorID, currentPOLL.targetID), currentPOLL.targetID) >= amount || currentPOLL.parameter3 >= 0)
				{
					CreateNewOrder(slot, amount);
				}
				
			}
			else
			{
				// currentPOLL.parameter is not SYMBOL
				// accept open order

				/* check status of target contract
				 * check item and enough amount on target contract map for buying (owner = marketplace contract)
				 * check item and enough amount on target contract map for selling (owner = actorID; target is direct, not marketplace)
				 * check if orderHash is ok (open)
				 * 
				 * 
				 * close order when finished
				 */

				

				long storeId = contractId;

				if (currentPOLL.parameter3 < 0)
				{
					// selling to a buyorder
					storeId = currentPOLL.actorID;
				}

				long storedAmount = getExtMapValue(currentPOLL.parameter2, getExtMapValue(STORE, storeId, currentPOLL.targetID), currentPOLL.targetID);

				if (storedAmount >= amount)
				{

					getOrderDetails(currentPOLL.parameter2, currentPOLL.actorID);

					if (currentORDER.location == currentPOLL.targetID && (currentORDER.owner == storeId || contractId == storeId))
					{
						if (currentPOLL.parameter3 < 0)
						{
							// selling to a buyorder

							/* outgoing message
							* currentPOLL.mainMethod = GAME_SPECIFIC
							* currentPOLL.subMethod = TRADE
							* currentPOLL.parameter = slotNumber (1...n their must be the TRADE_HUB)
							* currentPOLL.parameter2 = HASH (of order)
							* currentPOLL.parameter3 = 123
							* currentPOLL.parameter4 = 10000 (origin actor) FROM
							* currentPOLL.actorID = 7777 (marketplace contract) TO
							* currentPOLL.targetID = 3333 (this contract)
							* currentPOLL.partOfPoll = TARGET
							*/

							SetSendBufferForTargetContract(GAME_SPECIFIC, TRADE, slot, currentORDER.symbol, amount, storeId, currentPOLL.targetID, currentPOLL.targetID);
							SendBufferWithAmount(ONE_WHOLE, currentPOLL.targetID);
						}
						else
						{
							// buying from a sellorder

							/* outgoing message
							* currentPOLL.mainMethod = GAME_SPECIFIC
							* currentPOLL.subMethod = TRADE
							* currentPOLL.parameter = slotNumber (1...n their must be the TRADE_HUB)
							* currentPOLL.parameter2 = HASH (of order)
							* currentPOLL.parameter3 = 123
							* currentPOLL.parameter4 = 10000 (origin actor) FROM
							* currentPOLL.actorID = 7777 (marketplace contract) TO
							* currentPOLL.targetID = 3333 (this contract)
							* currentPOLL.partOfPoll = TARGET
							*/

							SetSendBufferForTargetContract(GAME_SPECIFIC, TRADE, slot, getMapValue(SYMBOL, currentPOLL.parameter2), amount, storeId, currentPOLL.targetID, currentPOLL.targetID);
							SendBufferWithAmount(ONE_WHOLE, currentPOLL.targetID);
						}

						if(storedAmount == amount)
						{
							setMapValue(AMOUNT, currentPOLL.parameter, 0);
							setMapValue(STATUS, currentPOLL.parameter, 0);
						}
					}
				}
			}
		}
	}
}

long FindTradeHubSlot(long targetID)
{
	//long name = getExtMapValue(TYPE, STATION, targetID);
	//long slotsHash = getExtMapValue(SLOTS, getExtMapValue(TYPE, STATION, targetID), targetID);
	long slots = getExtMapValue(getExtMapValue(SLOTS, getExtMapValue(TYPE, STATION, targetID), targetID), 0, targetID);

	for (long i = 1; i <= slots; i++)
	{
		if (getExtMapValue(SLOT, i, targetID) == TRADE_HUB)
		{
			return i;
		}
	}
	return 0;
}

void CreateNewOrder(long tradeHubSlot, long amount)
{
	if (currentPOLL.parameter3 >= 0)
	{
		// buyorder
		currentPOLL.parameter4 = (currentTX.amount / currentPOLL.parameter3) - currentFee;
	}
	
	long hash = GetB3FromHash256(currentPOLL.actorID, currentPOLL.targetID, currentPOLL.parameter2, currentTX.timestamp);
	setMapValue(HASH, 1, hash);
	setMapValue(SYMBOL, hash, currentPOLL.parameter2);
	setMapValue(AMOUNT, hash, currentPOLL.parameter3);
	setMapValue(PRICE, hash, currentPOLL.parameter4);
	setMapValue(LOCATION_CONTRACT, hash, currentPOLL.targetID);
	setMapValue(OWNER, hash, currentPOLL.actorID);
	setMapValue(STATUS, hash, 1);
	
	if (currentPOLL.parameter3 < 0)
	{
		// sellorder

		/* outgoing message
		* currentPOLL.mainMethod = GAME_SPECIFIC
		* currentPOLL.subMethod = TRADE
		* currentPOLL.parameter = slotNumber (1...n their must be the TRADE_HUB)
		* currentPOLL.parameter2 = IRON
		* currentPOLL.parameter3 = 123
		* currentPOLL.parameter4 = 10000 (origin actor)
		* currentPOLL.actorID = 7777 (marketplace contract)
		* currentPOLL.targetID = 3333 (this contract)
		* currentPOLL.partOfPoll = TARGET
		*/

		SetSendBufferForTargetContract(GAME_SPECIFIC, TRADE, tradeHubSlot, currentPOLL.parameter2, amount, currentPOLL.parameter4, currentPOLL.targetID, currentPOLL.targetID);
		SendBufferWithAmount(ONE_WHOLE, currentPOLL.targetID);
	}
}

// contract methods
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


// support methods
long IsAuthenticated(long sender)
{

    if(sender == gameVoteContract || getMapValue(TEMPAUTH_CONTRACT, sender) == 1)
	{
		return 1;
	}

	return 0; 
}

long GetB3FromHash256(long a1, long a2, long a3, long a4)
{
    Set_A1_A2(a1, a2);
    Set_A3_A4(a3, a4);
    SHA256_A_To_B();
    return Get_B3();
}

long CleanUpMap(long key1, long itemIndex)
{
    long finished = 1;
    do
    {
        finished = 1;
        long nextItem = getMapValue(key1, itemIndex + 1);
        if (nextItem != 0)
        {
            setMapValue(key1, itemIndex, nextItem);
            itemIndex++;
            finished = 0;
        }
        else
        {
            setMapValue(key1, itemIndex, 0);
        }

    } while (finished == 0);

    return itemIndex;
}
