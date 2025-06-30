
/**
 * @author evolver
 * this smart contract implements a item contract for ingame item objects (like scanner, facilities, ect.)
 * features:
 * - only responds to authenticated IDs (like gameVote-contract as initial contract)
 * - represents the data structure of an item in-game object
 * - interact with different game logic methods (like build, equip, dock, ect.)
 * - interacts with other game contracts (like natural object contracts)
 */
 
#include APIFunctions

#program name miningStationContract
#program description this is a mining station
#program activationAmount .5

#pragma maxAuxVars 4
#pragma maxConstVars 3
#pragma optimizationLevel 3
#pragma version 2.2.1

#define ONE_WHOLE 100000000

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
#define SET_OWNER 1_001_007
//#define SET_NFT_CONTRACT 1_001_008
#define AUTHENTICATE 1_001_009
#define BIGBANG_CREATE 1_001_010

// sub GAME_SPECIFIC methods 
//#define ACT 1_002_001
#define BUILD 1_002_002
//#define DELIVER 1_002_003
//#define DESCRIBE 1_002_004
#define DOCK 1_002_005
#define EQUIP 1_002_006
#define EXPLODE 1_002_007
#define INSURE 1_002_008
#define INVENT 1_002_009
#define MINE 1_002_010
#define REFINE 1_002_011
//#define REPAIR 1_002_012
//#define SALVAGE 1_002_013
#define SCAN 1_002_014
#define STORE 1_002_015
//#define TRADE_CREATEORDER 1_002_016
//#define TRADE_ACCEPTORDER 1_002_017

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
#define SIZE 1_005_003
//#define PROPERTIES 1_005_004
//#define DEFAULT_SPEED 1_005_006
//#define DEFAULT_STRUCTURE 1_005_007
//#define DEFAULT_ARMOR 1_005_008
//#define DEFAULT_SHIELD 1_005_009
//#define DEFAULT_CARGO 1_005_010
//#define COCKPITS 1_005_011
#define SLOT 1_005_012
//#define SLOTS 1_005_013
//#define HANGAR 1_005_014
//#define HANGARS 1_005_015
//#define SERVICE 1_005_016
#define SERVICES 1_005_017
#define HANDLED_BY 1_005_018
#define MANUFACTURED_BY 1_005_019

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

#define FACILITY 1_006_010
//#define HANGAR_MODULE 1_006_011
//#define REFINERY_FACILITY 1_006_012
//#define ASSEMBLE_FACILITY 1_006_013
//#define ADMINISTRATIVE_FACILITY 1_006_014
//#define OBSERVATORY_FACILITY 1_006_015
//#define TRADE_HUB 1_006_016

//#define SHIP 1_006_018
//#define STATION 1_006_019
// #define ASTEROID 1_006_019
// #define MOON 1_006_020
// #define PLANET 1_006_021
// #define STAR 1_006_022

// slot and hangar types
//#define INTERNAL 1_007_001
//#define EXTERNAL 1_007_002

// slot types
// #define AMMO_SLOT 1_007_003
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
#define ELEMENT 1_009_000
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
#define OWNER 1_011_001
#define STATUS 1_011_002
#define AMOUNT 1_011_003

// extContract flags
#define ACTOR 1_012_001
#define TARGET 1_012_002

// contract attributes
// basic contract 
//long authIDs[10];
long sendBuffer[8];
long currentFee = ONE_WHOLE;

// advanced
long contractID = 0;
long status = 0;
long gameVoteContract = 0;
long itemBaseContract = 0;
long marketplaceContract = 0;
long locationContract = 0;
long objectSymbol = 0; // Article = FelDrill
long objectType = 0; // smallShip; mediumStation
//long originalNFTID = 0;
long insurence = 0;


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

constructor();

void constructor(void) {
    // this function will be called only once on first activation.
	gameVoteContract = 1111; //TODO: set initial ID
	status = 0;
	currentPOLL.hash = 0;
	//currentINFO.freeHangars = 0;
	contractID = GetcontractID();
	gameVoteContract = 1111;
	status = 0;
	setMapValue(GAMEVOTE_CONTRACT, 1, 1111);
	setMapValue(STATUS, 1, 0);
	setMapValue(OWNER, 1, getCreator());
}

long GetcontractID() {
	A_To_Tx_After_Timestamp(Get_Creation_Timestamp());
	return Get_A1();
}

void getTxDetails(void) {
    currentTX.txId = Get_A1();
    currentTX.amount = getAmount(currentTX.txId);
    currentTX.timestamp = Get_Timestamp_For_Tx_In_A();
    currentTX.sender = getSender(currentTX.txId);
    readMessage(currentTX.txId, 0, currentTX.message);
    readMessage(currentTX.txId, 1, currentTX.message + 4);
}
void getPollDetails(long hashValue, long pollSaveID) {
	
    if (currentPOLL.hash != hashValue) {
        currentPOLL.hash = hashValue;
        currentPOLL.providerID = getExtMapValue(PROVIDER_ID, hashValue, pollSaveID);
        currentPOLL.pollAmount = getExtMapValue(DEPOSITMENT, currentPOLL.providerID, pollSaveID);
		currentPOLL.amount = getExtMapValue(AMOUNT, hashValue, pollSaveID);
        currentPOLL.mainMethod = getExtMapValue(MAINMETHOD, hashValue, pollSaveID);
        currentPOLL.subMethod = getExtMapValue(SUBMETHOD, hashValue, pollSaveID);
        currentPOLL.parameter = getExtMapValue(PARAMETER, hashValue, pollSaveID);
        currentPOLL.parameter2 = getExtMapValue(PARAMETER2, hashValue, pollSaveID);
        currentPOLL.parameter3 = getExtMapValue(PARAMETER3, hashValue, pollSaveID);
        currentPOLL.parameter4 = getExtMapValue(PARAMETER4, hashValue, pollSaveID);
        currentPOLL.actorID = getExtMapValue(ACTOR_ID, hashValue, pollSaveID);
        currentPOLL.targetID = getExtMapValue(TARGET_ID, hashValue, pollSaveID);
	}
	
    currentPOLL.partOfPoll = currentTX.message[7];
	
	if (currentPOLL.partOfPoll == 0)
	{
		if (currentPOLL.targetID == contractID)
		{
			currentPOLL.partOfPoll = TARGET;
		}
		else
		{
			currentPOLL.partOfPoll = ACTOR;
		}
	}
}

void main(void) {
	do {
		A_To_Tx_After_Timestamp(currentTX.timestamp);
		if (Get_A1() == 0) {
			break;
		}
		getTxDetails();
		
		// if sender is not authenticated then break
		if (IsAuthenticated(currentTX.sender) == 0) {
			break;
		} else {
			
			 if (getCodeHashOf(currentTX.sender) != 0)
			 {
				 // currentTX.sender is a contract

				 if (currentTX.message[0] == HASH)
				 {
					 getPollDetails(currentTX.message[1], gameVoteContract);
				 } 
				 else if(status == 0 && currentTX.message[0] == 18657918865985095 && currentTX.message[1] == BIGBANG_CREATE) // 18657918865985095 = BIGBANG
				 {
					 // if status = 0 and sender = locationContract then just create this object
					 currentPOLL.mainMethod = CONTRACT_SPECIFIC;
					 currentPOLL.subMethod = BIGBANG_CREATE;
				 }
				 else
				 {

					 // currentTX.message[0] = mainMethod
					 // currentTX.message[1] = subMethod
					 // currentTX.message[2] = parameter
					 // currentTX.message[3] = parameter2
					 // currentTX.message[4] = parameter3
					 // currentTX.message[5] = parameter4
					 // currentTX.message[6] = actorID (to process sub method(parameter))
					 // currentTX.message[7] = targetcontractID (to process sub method(parameter))

					 if (currentTX.message[1] == HASH)
					 {
						 getPollDetails(currentTX.message[2], gameVoteContract);
						 currentPOLL.mainMethod = currentTX.message[0];
					 }
					 else if (currentTX.message[2] == HASH)
					 {
						 getPollDetails(currentTX.message[3], gameVoteContract);
						 currentPOLL.mainMethod = currentTX.message[0];
						 currentPOLL.subMethod = currentTX.message[1];
					 }
					 else if (currentTX.message[3] == HASH)
					 {
						 getPollDetails(currentTX.message[4], gameVoteContract);
						 currentPOLL.mainMethod = currentTX.message[0];
						 currentPOLL.subMethod = currentTX.message[1];
						 currentPOLL.parameter = currentTX.message[2];
					 }
					 else if (currentTX.message[4] == HASH)
					 {
						 getPollDetails(currentTX.message[5], gameVoteContract);
						 currentPOLL.mainMethod = currentTX.message[0];
						 currentPOLL.subMethod = currentTX.message[1];
						 currentPOLL.parameter = currentTX.message[2];
						 currentPOLL.parameter2 = currentTX.message[3];
					 }
					 else if (currentTX.message[5] == HASH)
					 {
						 getPollDetails(currentTX.message[6], gameVoteContract);
						 currentPOLL.mainMethod = currentTX.message[0];
						 currentPOLL.subMethod = currentTX.message[1];
						 currentPOLL.parameter = currentTX.message[2];
						 currentPOLL.parameter2 = currentTX.message[3];
						 currentPOLL.parameter3 = currentTX.message[4];
					 }
					 else if (currentTX.message[6] == HASH)
					 {
						 getPollDetails(currentTX.message[7], gameVoteContract);
						 currentPOLL.mainMethod = currentTX.message[0];
						 currentPOLL.subMethod = currentTX.message[1];
						 currentPOLL.parameter = currentTX.message[2];
						 currentPOLL.parameter2 = currentTX.message[3];
						 currentPOLL.parameter3 = currentTX.message[4];
						 currentPOLL.parameter4 = currentTX.message[5];
					 }
					 else
					 {
						 // no gameVote reference
						 currentPOLL.mainMethod = currentTX.message[0];
						 currentPOLL.subMethod = currentTX.message[1];
						 currentPOLL.parameter = currentTX.message[2];
						 currentPOLL.parameter2 = currentTX.message[3];
						 currentPOLL.parameter3 = currentTX.message[4];
						 currentPOLL.parameter4 = currentTX.message[5];
						 currentPOLL.actorID = currentTX.message[6];
						 currentPOLL.targetID = currentTX.message[7];
						 
						 currentPOLL.partOfPoll = TARGET;
					 }

				 }
				
			 }
			 else
			 {
				 // currentTX.sender is not a contract
				 // TODO: maybe owner-things
				 break;
			 }
		}
		
		switch (currentPOLL.mainMethod) {
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
void ContractSpecific()
{
    /*
     * currentPOLL.actorID = owner / authID
     * currentPOLL.mainMethod = CONTRACT_SPECIFIC
     * currentPOLL.subMethod = SET_ITEMBASE
     * currentPOLL.parameter = itemBaseID
     * currentPOLL.parameter2 =
     * currentPOLL.parameter3 =
     * currentPOLL.parameter4 =
     */

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
    /*case SET_MARKETPLACE:
        marketplaceContract = currentPOLL.parameter;
        setMapValue(MARKETPLACE_CONTRACT, 1, currentPOLL.parameter);
        break;*/
    case TYPE:
        objectSymbol = currentPOLL.parameter;
        objectType = getExtMapValue(objectSymbol, TYPE, itemBaseContract);
        setMapValue(TYPE, 1, objectSymbol);
        break;
    case SET_OWNER:
        setMapValue(OWNER, 1, currentPOLL.actorID);
        break;
    // case SET_NFT_CONTRACT:
        // setMapValue(SET_NFT_CONTRACT, 1, currentPOLL.parameter);
        // break;
    case AUTHENTICATE:
        setMapValue(TEMPAUTH_CONTRACT, currentPOLL.parameter, SetTimeOut(16));
        break;
    case LOCATION_CONTRACT:
        locationContract = currentPOLL.parameter;
        setMapValue(LOCATION_CONTRACT, 1, currentPOLL.parameter);
        break;
    case BIGBANG_CREATE:
        if (getCodeHashOf(currentTX.sender) != 0 && IsAuthenticated(currentTX.sender) == 1 && objectType == FACILITY)
        {
            setMapValue(STATUS, 1, 1);
            status = 1;
        }
        break;
    }
}
void GameSpecific(void) {
	switch (currentPOLL.subMethod) {
		
		//case ACT:
			//    Act();
			//    break;
		case BUILD:
			Build();
			break;
			//case DELIVER:
			//    Deliver();
			//    break;
			//case DESCRIBE:
			//    Describe();
			//    break;
		case DOCK:
			Dock();
			break;
		case EQUIP:
			Equip();
			break;
		case EXPLODE:
			Explode();
			break;
		case INSURE:
			Insure();
			break;
			//case INVENT:
			//    Invent();
			//    break;
		case MINE:
			Mine();
			break;
		case REFINE:
			Refining();
			break;
		/*case REPAIR:
			Repair();
			break;*/
		case SCAN:
			Scan();
			break;
		case STORE:
			Store();
			break;
			//case TRADE:
			//    Trade();
			//    break;
			//case TREAT:
			//    Treat();
			//    break;
		default:
			break;
	}
}

void Build()
{
    if (status == 0 && CheckSubItems(0, 0) == 1)
    {
        status = 1;
        setMapValue(STATUS, 1, 1);
    }
}

void Dock()
{
    // TODO: check if objectType == FACILITY (get from itembase -> property)
    // TODO: check if objectSymbol == MedHgMod (get from itembase -> property)
    // TODO: how much dockplaces are currently available
    
    long vehicleSymbol = getExtMapValue(TYPE, 1, currentPOLL.actorID);

    if (status == 1 && IsServiceGiven(DOCK, vehicleSymbol) == 1)
    {
        // 
        SetSendBufferForTargetContract(HASH, currentPOLL.hash, 0, 0, 0, 0, 0, ACTOR);
        SendBufferWithAmount(currentFee, currentPOLL.actorID);
    }

}

void Equip()
{
    /* equip the object
    * equip.mainMethod = GAME_SPECIFIC
    * equip.subMethod = EQUIP
    * equip.parameter = toolID
    * equip.parameter2 = articleID
    * equip.parameter3 = slotNumber
    * equip.parameter4 = actorID (owner)
    * equip.actorID = sourceID (FelDrill)
    * equip.targetID = targetID (ZeptLigh)
    */

    if (status == 1) 
    {
        if (currentTX.sender == locationContract)
        {
            long itemSymbol = getExtMapValue(TYPE, 1, currentPOLL.parameter2);
        
            if(IsServiceGiven(EQUIP, itemSymbol) == 1)
            {
                // check if item fits in slot of vehicle
                long slotSymbol = getExtMapValue(getExtMapValue(TYPE, 1, currentPOLL.targetID), currentPOLL.parameter3, itemBaseContract);
                if (getExtMapValue(itemSymbol, SLOT, itemBaseContract) == getExtMapValue(slotSymbol, TYPE, itemBaseContract) &&
                    getExtMapValue(itemSymbol, SIZE, itemBaseContract) == getExtMapValue(slotSymbol, SIZE, itemBaseContract))
                {
                    long slotItem = getExtMapValue(SLOT, currentPOLL.parameter3, currentPOLL.targetID);
                    if (slotItem != 0)// && slotItem != currentPOLL.parameter2)
                    {
                        // some item is fitted

                        /* message 
                        * currentPOLL.mainMethod = GAME_SPECIFIC
                        * currentPOLL.subMethod = STORE
                        * currentPOLL.parameter = ITEM/ID (ELEMENT,ARTICLE,...)
                        * currentPOLL.parameter2 = amount (123)
                        * currentPOLL.parameter3 = 0 (FROM)
                        * currentPOLL.parameter4 = 0 (TO)
                        * currentPOLL.actorID = owner (account)
                        * currentPOLL.targetID = 7777 (vehicle contract)
                        * currentPOLL.partOfPoll = (ACTOR/TARGET)
                        */

                        // store this item into vehicle of this tool
                        SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, slotItem, 1, 0, 0, currentPOLL.parameter4, currentPOLL.actorID);
                        SendBufferWithAmount(currentFee, currentPOLL.actorID);
                    }

                    // slot is free (fit item) OR item is fittet (unfit item)
                    SetSendBufferForTargetContract(HASH, currentPOLL.hash, 0, 0, 0, 0, 0, TARGET);
                    SendBufferWithAmount(currentFee, currentPOLL.targetID);
                }
                else
                {
                    // store this item back into vehicle of this tool
                    SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentPOLL.parameter2, 1, 0, 0, currentPOLL.parameter4, currentPOLL.actorID);
                    SendBufferWithAmount(currentFee, currentPOLL.actorID);
                }
            }

        }

    }
}

void Explode()
{

}

void Insure()
{

}

void Mine()
{
    // TODO: check if objectType == FACILITY (get from itembase -> property)
    // TODO: check if objectSymbol == QuaryFac (get from itembase -> property)
    // TODO: how much miningplaces are currently available

    /*
     * mainMethod                         =GameSpecific
     * subMethod                          =MINING
     * Parameter            :targetID     =FelDrill
     * Parameter2           :element      =IRON (26 to 118)
     * Parameter3           :amountNQT    =100_0000_0000
     * Parameter4           :actorID      =gamer/provider
     * Parameter5/actorID   :sourceID     =Asteroid
     * Parameter6/targetID  :toolID       =QuaryFac
     */

    if (status == 1)
    {
        if (currentTX.sender == gameVoteContract &&
            currentPOLL.partOfPoll == TARGET &&
            getCodeHashOf(currentPOLL.actorID) != 0 &&
            IsAuthenticated(currentPOLL.actorID) == 0)
        {
            // target part (from gamevote)
            // temporarily authenticate the asteroid
            setMapValue(TEMPAUTH_CONTRACT, currentPOLL.actorID, SetTimeOut(16));
        }
        else if (currentTX.sender == currentPOLL.actorID)
        {
            // target part (from asteroid)
            // is element available
            if (getExtMapValue(ELEMENT, currentPOLL.parameter2, currentPOLL.actorID) >= currentPOLL.parameter3)
            {
                // check tool has service
                if (getExtMapValue(getExtMapValue(objectSymbol, SERVICES, itemBaseContract), MINE, itemBaseContract) == 1)
                {
                    // send POLL to targerID
                    // store the element into the location of this tool
                    SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentPOLL.parameter2, currentPOLL.parameter3, 0, 0, currentPOLL.parameter4, TARGET);
                    SendBufferWithAmount(currentFee, currentPOLL.parameter);

                }
                
            }
        }

    }

}

void Refining()
{
    // TODO: check if objectType == FACILITY (get from itembase -> property)
    // TODO: check if objectSymbol == MedRfFac (get from itembase -> property)
    // TODO: how much melting pots are currently available
    if (status == 1 && IsServiceGiven(REFINE, currentPOLL.parameter) == 1)
    {

    }
}

void Scan()
{
    /*
     * mainMethod           : -             = GAME_SPECIFIC, 
     * subMethod            : -             = SCAN,
     * Parameter            : toolID        = SmalScan/Observ,
     * Parameter2           : -             = 0,
     * Parameter3           : -             = 0,
     * Parameter4           : actorID       = gamer/provider,
     * Parameter5/actorID   : toScanID      = Natural Contract,
     * Parameter6/targetID  : vehicleID     = ZeptLigh/FelDrill
     * PartOfPoll           : -             = ACTOR / TARGET
     */

    if (status == 1)
    {
        if (currentTX.sender == currentPOLL.targetID)
        {
            // check tool has service
            if (getExtMapValue(getExtMapValue(objectSymbol, SERVICES, itemBaseContract), SCAN, itemBaseContract) == 1)
            {
                // send POLL to targerID
                // store the element into the location of this tool
                SetSendBufferForTargetContract(HASH, currentPOLL.hash, 0, 0, 0, 0, 0, TARGET);
                SendBufferWithAmount(currentFee, currentPOLL.actorID);

            }

        }

    }

}

void Store()
{
    if (status == 0)
    {
        CheckSubItems(currentPOLL.parameter, currentPOLL.parameter2);
    }
}
// support methods
long CheckSubItems(long symbol, long amount)
{
    long matAmount = getExtMapValue(INVENT, objectSymbol, itemBaseContract);
    long matLeft = matAmount;
    long hash = GetB3FromHash256(INVENT, objectSymbol, 0, 0);

    for (long i = 1; i <= matAmount; i++)
    {
        long voteHash = getExtMapValue(hash, i, itemBaseContract);
        long subSymbol = getExtMapValue(PARAMETER2, voteHash, gameVoteContract);
        long subNeedAmount = getExtMapValue(PARAMETER3, voteHash, gameVoteContract);
        long subIsAmount = getMapValue(BUILD, subSymbol);
        long leftAmount = subNeedAmount - subIsAmount;

        if (leftAmount <= 0)
        {
            matLeft--;
        }

        // is Symbol == the searched Symbol
        if (symbol != 0 && subSymbol == symbol)
        {
            setMapValue(BUILD, symbol, subIsAmount + amount);
            if (subIsAmount + amount >= leftAmount)
            {
                matLeft--;
            }
        }

    }

    if (matLeft <= 0)
    {
        return 1;
    }

    return 0;
}
long IsServiceGiven(long service, long productSymbol)
{
    // check service is given by this facility (on itembase)
    // check if product can be handled by service
    // check if product can made with the service of this facility
    // check if product size is less or equal to the facility size
    // check getcodehash of actorID is 0 (no contract)

    long test0 = getExtMapValue(objectSymbol, SERVICES, itemBaseContract);
    long test1 = getExtMapValue(test0, service, itemBaseContract);
    long test2 = getExtMapValue(productSymbol, HANDLED_BY, itemBaseContract);
    long test3 = getExtMapValue(test2, service, itemBaseContract);
    long test4 = getExtMapValue(productSymbol, SIZE, itemBaseContract);
    long test5 = getExtMapValue(objectSymbol, SIZE, itemBaseContract);


    if (getExtMapValue(getExtMapValue(objectSymbol, SERVICES, itemBaseContract), service, itemBaseContract) != 0 &&
        getExtMapValue(getExtMapValue(productSymbol, HANDLED_BY, itemBaseContract), service, itemBaseContract) != 0 &&
        getExtMapValue(productSymbol, SIZE, itemBaseContract) <= getExtMapValue(objectSymbol, SIZE, itemBaseContract))
    {
        switch (service)
        {
        case BUILD:
        case REFINE:
            if (getExtMapValue(getExtMapValue(productSymbol, MANUFACTURED_BY, itemBaseContract), objectSymbol, itemBaseContract) != 0)
            {
                return 1;
            }
            break;
        //case EQUIP:
        //case DOCK:
           

            //default:
            //break;
        }
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

long IsAuthenticated(long sender)
{
    if (sender == gameVoteContract || sender == itemBaseContract || sender == marketplaceContract || sender == locationContract || GetTimeIsUp(getMapValue(TEMPAUTH_CONTRACT, sender)) == 0)// || getExtMapValue(LOCATION_CONTRACT, 1, sender) == contractId)
    {
        return 1;
    }

    return 0;
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
void SendBufferWithAmount(long amount, long recipient) {
	sendAmountAndMessage(amount - currentFee, sendBuffer, recipient);
	sendMessage(sendBuffer + 4, recipient);
}
