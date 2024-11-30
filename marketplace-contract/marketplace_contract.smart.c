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
#define ITEMBASE_CONTRACT 1_001_001
//#define MARKETPLACE_CONTRACT 1_001_002
#define LOCATION_CONTRACT 1_001_003
#define TEMPAUTH_CONTRACT 1_001_004
#define SET_ITEMBASE 1_001_005
//#define SET_MARKETPLACE 1_001_006
//#define SET_OWNER 1_001_007
//#define SET_NFT_CONTRACT 1_001_008
#define AUTHENTICATE 1_001_009
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
#define STORE 1_002_014
#define TRADE_CREATEORDER 1_002_015
#define TRADE_ACCEPTORDER 1_002_016

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
//#define ENTITLEMENT 1_004_002
//#define ELECTIONS 1_004_003
//#define TIMEOUT 1_004_004
//#define AGREEERS 1_004_005
//#define REJECTERS 1_004_006
//#define VOTEPOINTS 1_004_007

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
#define TRADE_HUB 1_006_016

//#define SHIP 1_006_017
#define STATION 1_006_018
//#define ASTEROID 1_006_019
//#define MOON 1_006_020
//#define PLANET 1_006_021
//#define STAR 1_006_022

// slot and hangar types
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
#define SELLORDER 1_010_001
#define SYMBOL 1_010_002
#define PRICE 1_010_003
#define HOLDER 1_010_004

// artificialObject contract specific
#define OWNER 1_011_001
#define STATUS 1_011_002
#define AMOUNT 1_011_003

// extContract flags
//#define ACTOR 1_012_001
//#define TARGET 1_012_002

// contract attributes
// basic contract
long contractID = 0;
long gameVoteContract = 0;
long itemBaseContract = 0;
long orderNumber = 0;
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
	amount,
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
	contractID = GetContractID();
	gameVoteContract = 1111; //TODO: set initial ID
	setMapValue(GAMEVOTE_CONTRACT, 1, gameVoteContract);
	currentPOLL.hash = 0;
	currentPOLL.actorID = 0;
	currentPOLL.amount = 0;
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
		currentPOLL.amount = getExtMapValue(AMOUNT, hashValue, pollSaveID);
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

void getOrderDetails(long hashValue)
{
	currentORDER.hash = hashValue;
	currentORDER.isSellOrder = getMapValue(SELLORDER, hashValue);
	currentORDER.symbol = getMapValue(SYMBOL, hashValue);
	currentORDER.price = getMapValue(PRICE, hashValue);
	currentORDER.location = getMapValue(LOCATION_CONTRACT, hashValue);
	currentORDER.owner = getMapValue(OWNER, hashValue);
	currentORDER.holder = getMapValue(HOLDER, hashValue);
	currentORDER.amount = getMapValue(AMOUNT, hashValue);
	currentORDER.status = getMapValue(STATUS, hashValue);

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
    if (itemBaseContract == 0 && currentPOLL.subMethod != SET_ITEMBASE)
	{
		return;
	}
	switch (currentPOLL.subMethod)
	{
		case SET_ITEMBASE:
			itemBaseContract = currentPOLL.parameter;
			setMapValue(ITEMBASE_CONTRACT, 1, currentPOLL.parameter);
			break;
		case AUTHENTICATE:
			setMapValue(TEMPAUTH_CONTRACT, currentPOLL.parameter, SetTimeOut(16));
			break;
	}
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
		case TRADE_CREATEORDER:
			CreateOrder();
			break;
		case TRADE_ACCEPTORDER:
			AcceptOrder();
			break;
		default:
			break;
	}
}

// sub methods
void CreateOrder(void)
{

    /* incoming message (input)
    * currentPOLL.mainMethod = GAME_SPECIFIC
    * currentPOLL.subMethod = TRADE_CREATEORDER
    * currentPOLL.parameter = IRON (SYMBOL, ARTICLE, ...)
    * currentPOLL.parameter2 = -123_0000_0000 (sell amount)
    * currentPOLL.parameter3 = 456_0000_0000 (price)
    * currentPOLL.parameter4 = 6666 (station contract(location))
    * currentPOLL.actorID = 10000 (origin actor(owner))
    * currentPOLL.targetID = 3333 (this marketplace)
    * currentPOLL.partOfPoll = TARGET
    */

    if (getExtMapValue(STATUS, 1, currentPOLL.parameter4) == 1) 
    {
        long slot = FindTradeHubSlot(currentPOLL.parameter4);
        if (slot != 0)
        {

            currentORDER.hash = 0;
            currentORDER.owner = currentPOLL.actorID;
            currentORDER.symbol = currentPOLL.parameter;
            currentORDER.location = currentPOLL.parameter4;
            currentORDER.status = 0;
            if (currentPOLL.parameter2 < 0)
            {
                // sellorder
                currentORDER.isSellOrder = 1;
                currentORDER.amount = currentPOLL.parameter2 * -1;
                currentORDER.price = currentPOLL.parameter3;
                currentORDER.total = currentORDER.price * currentORDER.amount;
                currentORDER.holder = contractID;
                
                currentORDER.available = getExtMapValue(currentORDER.symbol, getExtMapValue(STORE, currentORDER.owner, currentORDER.location), currentORDER.location);
                
                if (currentORDER.amount > currentORDER.available)
                {
                    currentORDER.amount = currentORDER.available;
                }

                if (currentORDER.amount >= ONE_WHOLE)
                {
                    currentORDER.status = 1;
                }
            }
            else
            {
                // buyorder

                currentORDER.isSellOrder = 0;
                currentORDER.amount = currentPOLL.parameter2;
                currentORDER.price = currentTX.amount - (currentTX.amount - currentPOLL.amount);
                currentORDER.total = currentORDER.price * currentORDER.amount;
                currentORDER.holder = 0;
                //currentORDER.location = 0;
                currentORDER.available = 0;

                if (currentORDER.price > ONE_WHOLE)
                {
                    currentORDER.status = 1;
                }
               
            }

            if(currentORDER.status == 1)
            {
                CreateNewOrder();
            }

        }
    }

}

void AcceptOrder(void)
{
    // accept order
    /* incoming message (input)
     * currentPOLL.mainMethod = GAME_SPECIFIC
     * currentPOLL.subMethod = TRADE_ACCEPTORDER
     * currentPOLL.parameter = HASH (of order)
     * currentPOLL.parameter2 = 123_0000_0000 (amount)
     * currentPOLL.parameter3 = 0
     * currentPOLL.parameter4 = 0
     * currentPOLL.actorID = 10000 (origin actor(buyer))
     * currentPOLL.targetID = 3333 (marketplace)
     */

    // change or cancel order
    /* incoming message (input)
    * currentPOLL.mainMethod = GAME_SPECIFIC
    * currentPOLL.subMethod = TRADE_CHANGEORDER
    * currentPOLL.parameter = HASH (of order)
    * currentPOLL.parameter2 = 123_0000_0000 (amount)
    * currentPOLL.parameter3 = 456_0000_0000 (price)
    * currentPOLL.parameter4 = 0
    * currentPOLL.actorID = 10000 (owner)
    * currentPOLL.targetID = 3333 (marketplace)
    */

    getOrderDetails(currentPOLL.parameter);

    // check for station exists
    if (getExtMapValue(STATUS, 1, currentORDER.location) == 1)
    {
        // check for trade hub exists on station
        long slot = FindTradeHubSlot(currentORDER.location);
        if (slot != 0)
        {
            if (currentORDER.owner == currentPOLL.actorID)
            {
                // change or cancel order
                ChangeOrderConditions();
            }
            else
            {
                // check if the order is a sellorder
                if (currentORDER.isSellOrder == 1)
                {
                    // check if item amount is available
                    // check funds for buying
                    if (currentORDER.amount >= currentPOLL.parameter2 && currentPOLL.amount >= currentORDER.price * (currentPOLL.parameter2 / ONE_WHOLE))
                    {
                        ExecuteAcceptedOrder(currentPOLL.parameter2);
                    }
                }
                else
                {
                    // buyorder
                    long available = getExtMapValue(currentORDER.symbol, getExtMapValue(STORE, currentPOLL.actorID, currentORDER.location), currentORDER.location);
                    // check if item amount is available
                    if (available >= currentPOLL.parameter2 && currentPOLL.parameter2 <= currentORDER.amount)
                    {
                        ExecuteAcceptedOrder(currentPOLL.parameter2);
                    }
                }
            }

        }
    }
}

long FindTradeHubSlot(long targetID)
{
	long slots = getExtMapValue(getExtMapValue(SLOTS, getExtMapValue(TYPE, STATION, targetID), itemBaseContract), 0, itemBaseContract);

	for (long i = 1; i <= slots; i++)
	{
		//long tmp1 = getExtMapValue(SLOT, i, targetID);
		//long tmp2 = getExtMapValue(TYPE, getExtMapValue(SLOT, i, targetID), itemBaseContract);

		if (getExtMapValue(TYPE, getExtMapValue(SLOT, i, targetID), itemBaseContract) == TRADE_HUB)
		{
			return i;
		}

	}
	return 0;
}

void CreateNewOrder(void)
{
	long hash = GetB3FromHash256(currentORDER.owner, currentORDER.location, currentORDER.symbol, currentTX.timestamp);
	setMapValue(HASH, orderNumber++, hash);
	setMapValue(SELLORDER, hash, currentORDER.isSellOrder);
	setMapValue(SYMBOL, hash, currentORDER.symbol);
	setMapValue(AMOUNT, hash, currentORDER.amount);
	setMapValue(PRICE, hash, currentORDER.price);
	setMapValue(LOCATION_CONTRACT, hash, currentORDER.location);
	setMapValue(OWNER, hash, currentORDER.owner);
	setMapValue(HOLDER, hash, currentORDER.holder);
	setMapValue(STATUS, hash, currentORDER.status);

	if (currentORDER.isSellOrder == 1)
	{

		// sellorder

		/* outgoing message
		* currentPOLL.mainMethod = GAME_SPECIFIC
		* currentPOLL.subMethod = STORE
		* currentPOLL.parameter = SYMBOL
		* currentPOLL.parameter2 = amount
		* currentPOLL.parameter3 = from owner
		* currentPOLL.parameter4 = to holder
		* currentPOLL.actorID = 7777 ( this marketplace contract)
		* currentPOLL.targetID = 3333 (station contract)
		* currentPOLL.partOfPoll = TARGET
		*/

		SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentORDER.symbol, currentORDER.amount, currentORDER.owner, currentORDER.holder, contractID, currentORDER.location);
		SendBufferWithAmount(currentFee, currentORDER.location);
	}
}

void ExecuteAcceptedOrder(long amount)
{
    long amountFactor = amount / ONE_WHOLE;

    if (currentORDER.isSellOrder == 1)
    {
        // sellorder

        /* outgoing message
        * currentPOLL.mainMethod = GAME_SPECIFIC
        * currentPOLL.subMethod = STORE
        * currentPOLL.parameter = SYMBOL
        * currentPOLL.parameter2 = amount
        * currentPOLL.parameter3 = from holder
        * currentPOLL.parameter4 = to buyer
        * currentPOLL.actorID = 7777 (this marketplace contract)
        * currentPOLL.targetID = 3333 (station contract)
        */

        // send the item amount to the buyers hangar
        SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentORDER.symbol, amount, currentORDER.holder, currentPOLL.actorID, contractID, currentORDER.location);
        SendBufferWithAmount(currentFee, currentORDER.location);
        
        // send the total to the seller
        sendAmount(amountFactor * currentORDER.price, currentORDER.owner);
        
        // set the item amount change
        currentORDER.amount = currentORDER.amount - amount;
        if (currentORDER.amount <= 0)
        {
            // close the order
            setMapValue(STATUS, currentORDER.hash, 0);
        }

    }
    else
    {
        // buyorder

        /* outgoing message
        * currentPOLL.mainMethod = GAME_SPECIFIC
        * currentPOLL.subMethod = STORE
        * currentPOLL.parameter = SYMBOL
        * currentPOLL.parameter2 = amount
        * currentPOLL.parameter3 = from seller
        * currentPOLL.parameter4 = to buyer
        * currentPOLL.actorID = 7777 (this marketplace contract)
        * currentPOLL.targetID = 3333 (station contract)
        */

        // send the item amount to the buyers hangar
        SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentORDER.symbol, amount, currentPOLL.actorID, currentORDER.owner, contractID, currentORDER.location);
        SendBufferWithAmount(currentFee, currentORDER.location);

        // send the total to the seller
        sendAmount(amountFactor * currentORDER.price, currentPOLL.actorID);

        // set the item amount change
        currentORDER.amount = currentORDER.amount - amount;
        if (currentORDER.amount <= 0)
        {
            // close the order
            setMapValue(STATUS, currentORDER.hash, 0);
        }
    }
}

void ChangeOrderConditions(void)
{
	
    if (currentORDER.price != currentPOLL.parameter3)
    {
        setMapValue(PRICE, currentORDER.hash, currentPOLL.parameter3);
    }

    if (currentORDER.amount != currentPOLL.parameter2)
    {
        if(currentORDER.isSellOrder == 1)
        {
            if (currentORDER.amount > currentPOLL.parameter2)
            {
                // change order

                setMapValue(AMOUNT, currentORDER.hash, currentPOLL.parameter2);

                /* outgoing message
                * currentPOLL.mainMethod = GAME_SPECIFIC
                * currentPOLL.subMethod = STORE
                * currentPOLL.parameter = SYMBOL
                * currentPOLL.parameter2 = amount
                * currentPOLL.parameter3 = from holder
                * currentPOLL.parameter4 = to owner
                * currentPOLL.actorID = 7777 (this marketplace contract)
                * currentPOLL.targetID = 3333 (station contract)
                */

                SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentORDER.symbol, currentORDER.amount - currentPOLL.parameter2, currentORDER.holder, currentORDER.owner, contractID, currentORDER.location);
                SendBufferWithAmount(currentFee, currentORDER.location);
            }
            else
            {
                // cancel order

                setMapValue(STATUS, currentORDER.hash, -1);

                /* outgoing message
                * currentPOLL.mainMethod = GAME_SPECIFIC
                * currentPOLL.subMethod = STORE
                * currentPOLL.parameter = SYMBOL
                * currentPOLL.parameter2 = amount
                * currentPOLL.parameter3 = from holder
                * currentPOLL.parameter4 = to owner
                * currentPOLL.actorID = 7777 (this marketplace contract)
                * currentPOLL.targetID = 3333 (station contract)
                */

                SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentORDER.symbol, currentORDER.amount, currentORDER.holder, currentORDER.owner, contractID, currentORDER.location);
                SendBufferWithAmount(currentFee, currentORDER.location);

            }
        }
        else
        {
            // buyorder

            setMapValue(AMOUNT, currentORDER.hash, currentPOLL.parameter2);

        }
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
long GetB3FromHash256(long a1, long a2, long a3, long a4)
{
    Set_A1_A2(a1, a2);
    Set_A3_A4(a3, a4);
    SHA256_A_To_B();
    return Get_B3();
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

long IsAuthenticated(long sender)
{

    if(sender == gameVoteContract || getMapValue(TEMPAUTH_CONTRACT, sender) == 1)
	{
		return 1;
	}

	return 0; 
}
