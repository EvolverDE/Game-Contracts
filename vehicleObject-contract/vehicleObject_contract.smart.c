
/**
 * @author evolver
 * this smart contract implements a artificial contract for ingame article objects (like spaceships, stations, ect.)
 * features:
 * - only responds to authenticated IDs (like gameVote-contract as initial contract)
 * - represents the data structure of an artificial in-game object
 * - interact with different game logic methods (like build, equip, dock, scan, ect.)
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
#define MARKETPLACE_CONTRACT 1_001_002
#define LOCATION_CONTRACT 1_001_003
#define TEMPAUTH_CONTRACT 1_001_004
#define SET_ITEMBASE 1_001_005
#define SET_MARKETPLACE 1_001_006
#define SET_OWNER 1_001_007
#define SET_NFT_CONTRACT 1_001_008
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
#define REPAIR 1_002_012
//#define SALVAGE 1_002_013
//#define SCAN 1_002_014
#define STORE 1_002_015
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
#define DEFAULT_SPEED 1_005_006
#define DEFAULT_STRUCTURE 1_005_007
#define DEFAULT_ARMOR 1_005_008
#define DEFAULT_SHIELD 1_005_009
#define DEFAULT_CARGO 1_005_010
#define COCKPITS 1_005_011
#define SLOT 1_005_012
#define SLOTS 1_005_013
#define HANGAR 1_005_014
#define HANGARS 1_005_015
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

//#define FACILITY 1_006_010
//#define HANGAR_MODULE 1_006_011
//#define REFINERY_FACILITY 1_006_012
//#define ASSEMBLE_FACILITY 1_006_013
//#define ADMINISTRATIVE_FACILITY 1_006_014
//#define OBSERVATORY_FACILITY 1_006_015
//#define TRADE_HUB 1_006_016

#define SHIP 1_006_018
#define STATION 1_006_019
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

struct PROPERTYINFO {
    long //hash,
	//size,
	//type,
	defaultArmor,
	defaultCargo,
	defaultCockpits,
	defaultShield,
	defaultSlots,
	defaultSpeed,
	defaultStructure,
	freeCargoSpace,
	freeHangars,
	freeSlots;
} currentINFO;

//struct SLOTINFO {
//    long hash,
//	category,
//	size,
//	type,
//	slot;
//} currentSlot;

// struct HANGARINFO {
    // long hash,
	// size,
	// type;
// } currentHangar;

//struct ARTICLEINFO {
//    long category,
//	type,
//	size,
//	slot;
//} currentArticle;

constructor();

void constructor(void) {
    // this function will be called only once on first activation.
	gameVoteContract = 1111; //TODO: set initial ID
	status = 0;
	currentPOLL.hash = 0;
	currentINFO.freeHangars = 0;
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

void getPropertyDetails()
{
	currentINFO.defaultArmor = getExtMapValue(objectSymbol, DEFAULT_ARMOR, itemBaseContract);
	currentINFO.defaultCargo = getExtMapValue(objectSymbol, DEFAULT_CARGO, itemBaseContract);
	currentINFO.defaultCockpits = getExtMapValue(objectSymbol, COCKPITS, itemBaseContract);
	currentINFO.defaultShield = getExtMapValue(objectSymbol, DEFAULT_SHIELD, itemBaseContract);
	currentINFO.defaultSlots = getExtMapValue(objectSymbol, SLOTS, itemBaseContract);
	currentINFO.defaultSpeed = getExtMapValue(objectSymbol, DEFAULT_SPEED, itemBaseContract);
	currentINFO.defaultStructure = getExtMapValue(objectSymbol, DEFAULT_STRUCTURE, itemBaseContract);

	currentINFO.freeCargoSpace = currentINFO.defaultCargo;
	currentINFO.freeSlots = currentINFO.defaultSlots;
	currentINFO.freeHangars = 0;
}
//void getSlotDetails(long slotIndex) {
//    currentSlot.hash = getExtMapValue(currentINFO.slotsHash, slotIndex, itemBaseContract);
//	currentSlot.category = getExtMapValue(CATEGORY, currentSlot.hash, itemBaseContract);
//    currentSlot.size = getExtMapValue(SIZE, currentSlot.hash, itemBaseContract);
//    currentSlot.type = getExtMapValue(TYPE, currentSlot.hash, itemBaseContract);
//    currentSlot.slot = getExtMapValue(SLOT, currentSlot.hash, itemBaseContract);
//}
//void getArticleDetails(long articleSymbol) {
//	currentArticle.category = getExtMapValue(CATEGORY, articleSymbol, itemBaseContract);
//    currentArticle.type = getExtMapValue(TYPE, articleSymbol, itemBaseContract);
//    currentArticle.size = getExtMapValue(SIZE, articleSymbol, itemBaseContract);
//    currentArticle.slot = getExtMapValue(SLOT, articleSymbol, itemBaseContract);
//}

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
    case SET_MARKETPLACE:
        marketplaceContract = currentPOLL.parameter;
        setMapValue(MARKETPLACE_CONTRACT, 1, currentPOLL.parameter);
        break;
    case TYPE:
        objectSymbol = currentPOLL.parameter;
        objectType = getExtMapValue(objectSymbol, TYPE, itemBaseContract);
        setMapValue(TYPE, 1, objectSymbol);
        break;
    case SET_OWNER:
        setMapValue(OWNER, 1, currentPOLL.actorID);
        break;
    case SET_NFT_CONTRACT:
        setMapValue(SET_NFT_CONTRACT, 1, currentPOLL.parameter);
        break;
    case AUTHENTICATE:
        setMapValue(TEMPAUTH_CONTRACT, currentPOLL.parameter, SetTimeOut(16));
        break;
    case LOCATION_CONTRACT:
        locationContract = currentPOLL.parameter;
        setMapValue(LOCATION_CONTRACT, 1, currentPOLL.parameter);
        break;
    case BIGBANG_CREATE:
        if (getCodeHashOf(currentTX.sender) != 0 && IsAuthenticated(currentTX.sender) == 1 && objectType == STATION)
        {
            status = 1;
            setMapValue(STATUS, 1, 1);
            getPropertyDetails();
            setMapValue(OWNER, 1, 0);
            locationContract = currentTX.message[2]; // location = ASTEROID
            setMapValue(LOCATION_CONTRACT, 1, currentTX.message[2]);// location = ASTEROID
            setMapValue(SLOT, 1, currentTX.message[3]); // QuaryFac = 5869704833636131171
            setMapValue(SLOT, currentTX.message[3], 1); // QuaryFac = 5869704833636131171
            setMapValue(SLOT, 2, currentTX.message[4]); // MedRfFac = 5576974018634211683
            setMapValue(SLOT, currentTX.message[4], 1); // MedRfFac = 5576974018634211683
            setMapValue(SLOT, 3, currentTX.message[5]); // MedAsFac = 5576973945837871459
            setMapValue(SLOT, currentTX.message[5], 1); // MedAsFac = 5576973945837871459
            setMapValue(SLOT, 4, currentTX.message[6]); // MedHgMod = 5576973975701778276
            setMapValue(SLOT, currentTX.message[6], 1); // MedHgMod = 5576973975701778276
            //setMapValue(SLOT, 5, currentTX.message[7]); // TradeHub = 6085033130354832738
            //setMapValue(SLOT, currentTX.message[7], 1); // TradeHub = 6085033130354832738
            currentINFO.freeHangars = -1;
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
		case REPAIR:
			Repair();
			break;
			//case SCAN:
			//    Scan();
			//    break;
		// case SALVAGE:
			// Salvage();
			// break;
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
    /* build the object itself
    * currentPOLL.actorID = initiatorID (of build order)
    * currentPOLL.mainMethod = GAME_SPECIFIC
    * currentPOLL.subMethod = BUILD
    * currentPOLL.parameter = assembly
    * currentPOLL.parameter2 = SYSTEM (Computer)
    * currentPOLL.parameter3 = Amount (1)
    * currentPOLL.parameter4 = 0

    * build other objects
    * currentPOLL.mainMethod = GAME_SPECIFIC
    * currentPOLL.subMethod = BUILD
    * currentPOLL.parameter = assembly
    * currentPOLL.parameter2 = buildObjectName
    * currentPOLL.parameter3 = amountNQT
    * currentPOLL.parameter4 = outputTargetID
    * currentPOLL.actorID = actorID
    * currentPOLL.targetID = buildPlace
    */

    long symbol = objectSymbol;
    //long tryBuild = 0;

    long hash = 0;

    if (status == 1)
    {
        // build objects with this object
        symbol = currentPOLL.parameter2;
        if (IsServiceGiven(BUILD, currentPOLL.parameter, symbol) != 0)
        {
            long needMaterialCount = getExtMapValue(INVENT, symbol, itemBaseContract);
            long whereAvailable = CheckSubItems(symbol, needMaterialCount, currentPOLL.parameter3, currentPOLL.parameter4);

            switch (whereAvailable)
            {
            case 1:
                // subitems here available for item without contract
                // set into local cargo
                SetItemIntoCargo(currentPOLL.parameter2, currentPOLL.parameter3, 0);
                break;
            case 2:
                // subitems there available for vehicle/item with contract
                if (getCodeHashOf(currentPOLL.parameter4) == getCodeHashOf(contractID) && getExtMapValue(currentPOLL.parameter2, TYPE, itemBaseContract) == SHIP)
                {
                    // remote contract is a vehicle contract and just a ship
                    // dock the ship
                    DockObject(currentPOLL.parameter4);
                }
                else
                {
                    // set into local cargo
                    SetItemIntoCargo(currentPOLL.parameter4, 1, 0);
                }

                // send buildup command to the vehicle/item contract
                SetSendBufferForTargetContract(GAME_SPECIFIC, BUILD, currentPOLL.parameter2, currentPOLL.parameter3, 0, HASH, currentPOLL.hash, currentPOLL.partOfPoll);
                SendBufferWithAmount(currentFee, currentPOLL.parameter4);

                break;
            case 3:
                // subitems here available for vehicle/item with contract
                // send buildup materials to target contract
                SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentPOLL.parameter2, currentPOLL.parameter3, 0, HASH, currentPOLL.hash, currentPOLL.partOfPoll);
                SendBufferWithAmount(currentFee, currentPOLL.parameter4);
                break;
                //case -1:
                    // subitems here not available for item without contract
                //case -2:
                    // subitems there not available for item with contract
                //case -3:
                    // subitems here not available for item with contract
                //case 0:
                //default:
                    // not available
                    //break;

            }

            if (whereAvailable == 1 || whereAvailable == 3)
            {
                // refresh input materials amounts in map 
                for (long i = 1; i <= needMaterialCount; i++)
                {
                    hash = getExtMapValue(GetB3FromHash256(INVENT, symbol, 0, 0), i, itemBaseContract);
                    GetItemOutOfCargo(getExtMapValue(PARAMETER2, hash, gameVoteContract), getExtMapValue(PARAMETER3, hash, gameVoteContract) * (currentPOLL.parameter3 / ONE_WHOLE), 0);
                }
            }

        }
    }
    else
    {
        // build this object itself
        if (CheckSubItems(symbol, getExtMapValue(INVENT, symbol, itemBaseContract), 1, 0) == 1)
        {
            // subitems here available for this contract itself
            status = 1;
            setMapValue(STATUS, 1, 1);
            getPropertyDetails();
        }

    }

}
void Dock()
{
    /* dock the zeptor on the station
    * currentPOLL.actorID = initiatorID
    * currentPOLL.mainMethod = GAME_SPECIFIC
    * currentPOLL.subMethod = DOCK
    * currentPOLL.parameter = hangar module symbol
    * currentPOLL.parameter2 = objectSymbol (ZeptLigh (to get properties))
    * currentPOLL.parameter3 = 0
    * currentPOLL.parameter4 = objectContract (contractID)
    */

    // this object must exist
    if (status == 1)
    {

        if (currentTX.sender == gameVoteContract &&
            currentPOLL.partOfPoll == ACTOR &&
            getCodeHashOf(currentPOLL.parameter) != 0 &&
            IsAuthenticated(currentPOLL.parameter) == 0)
        {
            // target part (from gamevote)
            // temporarily authenticate the toolID
            setMapValue(TEMPAUTH_CONTRACT, currentPOLL.parameter, SetTimeOut(16));
        }
        else if (currentTX.sender == currentPOLL.parameter &&
            currentPOLL.partOfPoll == ACTOR)
        {
            // actor part (from tool)
            setMapValue(TEMPAUTH_CONTRACT, currentPOLL.parameter, 0);

            if (locationContract == currentPOLL.targetID)
            {
                // undock 
                // set the location of this object to the location(star) of the location(asteroid) of the target object(station)
                locationContract = getExtMapValue(LOCATION_CONTRACT, 1, getExtMapValue(LOCATION_CONTRACT, 1, currentPOLL.targetID));
                setMapValue(LOCATION_CONTRACT, 1, locationContract);
            }
            else
            {
                // dock
                // set the location of this object to the target object
                locationContract = currentPOLL.targetID;
                setMapValue(LOCATION_CONTRACT, 1, currentPOLL.targetID);
            }

        }
        else if (currentPOLL.partOfPoll == TARGET)
        {
            // target part (from vehicle with tool)
            // check tool can handle item
            if (IsServiceGiven(DOCK, currentPOLL.parameter, getExtMapValue(TYPE, 1, currentPOLL.actorID)) != 0)
            {
                // check if vehicle is docked
                if (getExtMapValue(getExtMapValue(HANGARS, currentPOLL.parameter4, currentPOLL.targetID), currentPOLL.actorID, currentPOLL.targetID) == 1)
                {
                    // undock
                    UndockObject(currentPOLL.actorID);
                }
                else
                {
                    // dock
                    DockObject(currentPOLL.actorID);
                }

                // send POLL to toolID
                SetSendBufferForTargetContract(HASH, currentPOLL.hash, 0, 0, 0, 0, 0, TARGET);
                SendBufferWithAmount(currentFee, currentPOLL.parameter);
            }
        }

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

    long item = 0;

    // this object must exist
    if (status == 1)
    {
        if (currentTX.sender == gameVoteContract &&
            currentPOLL.partOfPoll == TARGET &&
            getCodeHashOf(currentPOLL.parameter) != 0 &&
            IsAuthenticated(currentPOLL.parameter) == 0)
        {
            // target part (from gamevote)
            // temporarily authenticate the toolID
            setMapValue(TEMPAUTH_CONTRACT, currentPOLL.parameter, SetTimeOut(16));
        }
        else if (currentTX.sender == currentPOLL.parameter &&
            currentPOLL.partOfPoll == TARGET)
        {
            // target part (from tool)
            setMapValue(TEMPAUTH_CONTRACT, currentPOLL.parameter, 0);
            item = getMapValue(SLOT, currentPOLL.parameter3);
            if (item == 0 || item != currentPOLL.parameter2)
            {
                // slot is free OR another item is in slot
                setMapValue(SLOT, currentPOLL.parameter3, currentPOLL.parameter2);
                setMapValue(SLOT, currentPOLL.parameter2, 1);
            }
            else
            {
                // unfit item
                setMapValue(SLOT, currentPOLL.parameter3, 0);
                setMapValue(SLOT, currentPOLL.parameter2, 0);
				
				// send LOCATION_CONTRACT to item
				SetSendBufferForTargetContract(CONTRACT_SPECIFIC, LOCATION_CONTRACT, currentPOLL.targetID, 0, 0, 0, 0, TARGET);
				SendBufferWithAmount(currentFee, currentPOLL.parameter2);

            }
        }
        else if (currentPOLL.partOfPoll == ACTOR)
        {
            // actor part (from vehicle with tool)
            // check tool can handle item
            if (IsServiceGiven(EQUIP, currentPOLL.parameter, getExtMapValue(TYPE, 1, currentPOLL.parameter2)) != 0)
            {
                // check availability of target
                if (getMapValue(getMapValue(HANGARS, currentPOLL.parameter4), currentPOLL.targetID) != 0)
                {
                    // check item is stored here OR some item is fitted in slot there
                    item = getMapValue(getMapValue(STORE, currentPOLL.parameter4), currentPOLL.parameter2);
                    if (item != 0 || getExtMapValue(SLOT, currentPOLL.parameter3, currentPOLL.targetID) != 0)
                    {
                        // get item out of cargo if available
                        if (item != 0)
                        {
                            GetItemOutOfCargo(currentPOLL.parameter2, 1, currentPOLL.parameter4);
							
							// send LOCATION_CONTRACT to item
							SetSendBufferForTargetContract(CONTRACT_SPECIFIC, LOCATION_CONTRACT, currentPOLL.targetID, 0, 0, 0, 0, TARGET);
							SendBufferWithAmount(currentFee, currentPOLL.parameter2);
                        }

                        // send POLL to toolID
                        SetSendBufferForTargetContract(HASH, currentPOLL.hash, 0, 0, 0, 0, 0, TARGET);
                        SendBufferWithAmount(currentFee, currentPOLL.parameter);

                    }
                }
            }
        }
    }
}
void Explode()
{
    // object destroyed/not exist
    status = 0;
    setMapValue(STATUS, 1, 0);
    long count = getExtMapValue(INVENT, objectSymbol, itemBaseContract);
    for (long i = 1; i <= count; i++)
    {
        setMapValue(BUILD, getExtMapValue(PARAMETER2, getExtMapValue(objectSymbol, i, itemBaseContract), gameVoteContract), 0);
    }

}
void Insure()
{
    insurence = currentPOLL.parameter;
}

void Mine()
{
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

    if ((getCodeHashOf(currentPOLL.targetID) != 0 || getCodeHashOf(currentPOLL.parameter4) == 0) && currentPOLL.parameter == contractID)
    {
        SetItemIntoCargo(currentPOLL.parameter2, currentPOLL.parameter3, currentPOLL.parameter4);
    }
}

void Refining()
{
    /* refine the object
    * currentPOLL.actorID = initiatorID, not contract
    * currentPOLL.mainMethod = GAME_SPECIFIC
    * currentPOLL.subMethod = REFINE
    * currentPOLL.parameter = Refinery Facility (MedRfFac)
    * currentPOLL.parameter2 = Refined Materials (GoldBar)
    * currentPOLL.parameter3 = Amount (1)
    * currentPOLL.parameter4 = 0
    */

    // check if service is fully given on this vehicle contract
    if (status != 0)
    {
        if (IsServiceGiven(REFINE, currentPOLL.parameter, currentPOLL.parameter2) != 0)
        {
            long needMaterialCount = getExtMapValue(INVENT, currentPOLL.parameter2, itemBaseContract);
            if (CheckSubItems(currentPOLL.parameter2, needMaterialCount, currentPOLL.parameter3, 0) == 1)
            {
                if (currentPOLL.parameter4 == 0)
                {
                    // set into local cargo
                    SetItemIntoCargo(currentPOLL.parameter2, currentPOLL.parameter3, 0);
                }
                else if (getCodeHashOf(currentPOLL.parameter4) != 0)
                {
                    // send buildup materials to target contract
                    SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, HASH, currentPOLL.hash, 0, 0, 0, currentPOLL.partOfPoll);
                    SendBufferWithAmount(currentFee, currentPOLL.parameter4);
                }

                // refresh input materials amounts in map 
                for (long i = 1; i <= needMaterialCount; i++)
                {
                    long hash = getExtMapValue(GetB3FromHash256(INVENT, currentPOLL.parameter2, 0, 0), i, itemBaseContract);
                    GetItemOutOfCargo(getExtMapValue(PARAMETER2, hash, gameVoteContract), getExtMapValue(PARAMETER3, hash, gameVoteContract) * (currentPOLL.parameter3 / ONE_WHOLE), 0);
                }
            }
        }
    }
}
void Repair()
{

}

/*void salvage()
{
	
	* salvage other vehicle
	* mainmethod                            =gamespecific
	* submethod                             =salvage
	* parameter             :targetid       =salvager
	* parameter2            :salvagearea    =storage/slot/build
	* parameter3            :item/slot      =cocinstu/4
	* parameter4            :ownerid        =gamer/provider
	* parameter5/actorid    :toolid         =smalsalv
	* parameter6/targetid   :salvageid      =zeptligh
	

	// todo: first salvage cargo, second salvage fitting, last salvage vehicle systems

	if (status == -1)
	{
		long fitteditemid = getmapvalue(slot, currentpoll.parameter3);
		long itemid = 0;
		long symbol = 0;

		if (fitteditemid != 0)
		{
			// parameter3 is a fitted article
			symbol = getextmapvalue(type, 1, fitteditemid);
		}
		else
		{
			symbol = getextmapvalue(type, 1, currentpoll.parameter3);
			if (symbol == 0)
			{
				// parameter3 is not a article
				symbol = currentpoll.parameter3;
			}
			else
			{
				// parameter3 is a stored article
				itemid = currentpoll.parameter3;
			}

		}

		if (isservicegiven(salvage, currentpoll.actorid, symbol) == 1)
		{
			long itemsymbolid = 0;
			// ready for salvaging
			if (currentinfo.freecargospace == currentinfo.defaultcargo &&
				currentinfo.freeslots == currentinfo.defaultslots)
			{
				// no cargo and no fitting left, ready for salvaging systems of vehicle
				setmapvalue(build, symbol, 0);

				itemsymbolid = symbol;

				// set status = 0 if no systems left by checking subitems of vehicle
				checksubitems(objectsymbol, getextmapvalue(invent, objectsymbol, itembasecontract), 1, 0);

			}
			else if (fitteditemid != 0) // first salvage cargo + fitting
			{
				// unfit item
				setmapvalue(slot, currentpoll.parameter3, 0);
				setmapvalue(slot, fitteditemid, 0);
				currentinfo.freeslots += 1;

				itemsymbolid = fitteditemid;

				// send location_contract to item
				setsendbufferfortargetcontract(contract_specific, location_contract, currentpoll.parameter, 0, 0, 0, 0, target);
				sendbufferwithamount(currentfee, fitteditemid);
			}
			else if (itemid == 0)
			{
				// get stored item (without contract)
				getitemoutofcargo(symbol, 1, currentpoll.actorid);
				
				itemsymbolid = symbol;
			}
			else
			{
				// get stored article
				getitemoutofcargo(itemid, 1, currentpoll.actorid);

				itemsymbolid = itemid;

				// send location_contract to item
				setsendbufferfortargetcontract(contract_specific, location_contract, currentpoll.parameter, 0, 0, 0, 0, target);
				sendbufferwithamount(currentfee, itemid);
			}

			// send store command to salvager
			setsendbufferfortargetcontract(game_specific, store, itemsymbolid, 1, 0, 0, 0, target);
			sendbufferwithamount(currentfee, currentpoll.parameter);
		}
	}
}*/
	

void Store()
{
    // check if item is one with contract
    if (getCodeHashOf(currentPOLL.parameter) != 0 && currentPOLL.parameter2 > 1)
    {
        currentPOLL.parameter2 = 1;
    }

    if (status == 0)
    {
        // check if sender is location
        if (currentTX.sender == locationContract)
        {
            setMapValue(BUILD, currentPOLL.parameter, getMapValue(BUILD, currentPOLL.parameter) + currentPOLL.parameter2);
        }
    }
    else
    {

        /* incoming message
        * currentPOLL.mainMethod = GAME_SPECIFIC
        * currentPOLL.subMethod = STORE
        * currentPOLL.parameter = ITEM/ID (ELEMENT,ARTICLE,...)
        * currentPOLL.parameter2 = amount (123)
        * currentPOLL.parameter3 = 0 (FROM)
        * currentPOLL.parameter4 = 0 (TO)
        * currentPOLL.actorID = 10000 (account)
        * currentPOLL.targetID = 7777 (vehicle contract)
        * currentPOLL.partOfPoll = (ACTOR/TARGET)
        */

        if (currentPOLL.parameter3 == 0 || currentPOLL.parameter4 == 0)
        {
            if (currentPOLL.partOfPoll == ACTOR && getCodeHashOf(currentPOLL.targetID) != 0)
            {
                // currentPOLL.parameter = Element (IRON)
                // currentPOLL.parameter2 = Amount (123)

                GetItemOutOfCargo(currentPOLL.parameter, currentPOLL.parameter2, currentPOLL.parameter4);
            }
            else if (currentPOLL.partOfPoll == TARGET)
            {
                SetItemIntoCargo(currentPOLL.parameter, currentPOLL.parameter2, 0);
            }

        }
        else
        {
            // check if enough amount of item is stored and owned by source OWNER
            if (getMapValue(GetB3FromHash256(STORE, currentPOLL.parameter3, 0, 0), currentPOLL.parameter) >= currentPOLL.parameter2)
            {
                GetItemOutOfCargo(currentPOLL.parameter, currentPOLL.parameter2, currentPOLL.parameter3);
                SetItemIntoCargo(currentPOLL.parameter, currentPOLL.parameter2, currentPOLL.parameter4);
            }
        }
    }

}

// support methods
long CheckSubItems(long symbol, long needMaterialCount, long wishAmountNQT, long targetID)
{
    long returner = 0;
    long kind = 1;
    long hash = GetB3FromHash256(INVENT, symbol, 0, 0);
    long subSymbol = 0;
    long needAmountNQT = 0;
    long isAmountNQT = 0;
    long matLeft = needMaterialCount;

    // check input materials amounts
    for (long i = 1; i <= needMaterialCount; i++)
    {
        long voteHash = getExtMapValue(hash, i, itemBaseContract);
        subSymbol = getExtMapValue(PARAMETER2, voteHash, gameVoteContract);
        needAmountNQT = getExtMapValue(PARAMETER3, voteHash, gameVoteContract);

        if (needAmountNQT <= 0)
        {
            matLeft--;
            continue;
        }

        isAmountNQT = 0;

        if (status == 0)
        {
            isAmountNQT = getMapValue(BUILD, subSymbol);

            if (isAmountNQT < needAmountNQT)
            {
                break;
            }
        }
        else
        {
            if (targetID != 0 && getCodeHashOf(targetID) != 0)
            {
                isAmountNQT = getExtMapValue(BUILD, subSymbol, targetID);
                kind = 2;
            }

            if (isAmountNQT <= 0)
            {
                isAmountNQT = getMapValue(GetWichHash(STORE), subSymbol);
                if (kind == 2)
                {
                    kind = 3;
                }
            }
        }

        if (isAmountNQT > 0 && isAmountNQT >= needAmountNQT * (wishAmountNQT / ONE_WHOLE))
        {
            matLeft--;
        }

    }

    if (matLeft <= 0)
    {
        returner = kind;
    }

	// if (status == -1 && matLeft == needMaterialCount)
	// {
		// // complete salvaged / no longer exists
		// status = 0;
		// setMapValue(STATUS, 1, 0);
		// returner = -1;
	// }

    return returner;

}

long IsAuthenticated(long sender)
{
    if (sender == gameVoteContract || sender == itemBaseContract || sender == marketplaceContract || sender == locationContract || getMapValue(SLOT, sender) == 1 || GetTimeIsUp(getMapValue(TEMPAUTH_CONTRACT, sender)) == 0)// || getExtMapValue(LOCATION_CONTRACT, 1, sender) == contractId)
    {
        return 1;
    }

    return 0;
}
long GetB3FromHash256(long a1, long a2, long a3, long a4) {
	Set_A1_A2(a1, a2);
	Set_A3_A4(a3, a4);
	SHA256_A_To_B();
	return Get_B3();
}

long IsServiceGiven(long service, long toolID, long productSymbol)
{
    // check facility is installed (on this contract)
    // check service is given by installed facility (on itembase)
    // check if product can be handled by service
    // check if product can made with facility
    // check if product size is less or equal to the facility size
    // check getcodehash of actorID is 0 (no contract)

    long toolSymbol = getExtMapValue(TYPE, 1, toolID);

    long test0 = getMapValue(SLOT, toolID);
    long test1 = getExtMapValue(toolSymbol, SERVICES, itemBaseContract);
    long test2 = getExtMapValue(test1, service, itemBaseContract);
    long test3 = getExtMapValue(productSymbol, HANDLED_BY, itemBaseContract);
    long test4 = getExtMapValue(test3, service, itemBaseContract);
    long test5 = getExtMapValue(productSymbol, SIZE, itemBaseContract);
    long test6 = getExtMapValue(toolSymbol, SIZE, itemBaseContract);

    if (getMapValue(SLOT, toolID) != 0 &&
        getExtMapValue(getExtMapValue(toolSymbol, SERVICES, itemBaseContract), service, itemBaseContract) != 0 &&
        getExtMapValue(getExtMapValue(productSymbol, HANDLED_BY, itemBaseContract), service, itemBaseContract) != 0 &&
        getExtMapValue(productSymbol, SIZE, itemBaseContract) <= getExtMapValue(toolSymbol, SIZE, itemBaseContract))
    {
        switch (service)
        {
        case BUILD:
        case REFINE:
            if (getExtMapValue(getExtMapValue(productSymbol, MANUFACTURED_BY, itemBaseContract), toolSymbol, itemBaseContract) != 0)
            {
                return 1;
            }
            break;
        case EQUIP:
        case DOCK:
            return 1;

            //default:
                //break;
        }

    }

    return 0;
}

void SetItemIntoCargo(long item, long amount, long owner)
{
    long ownerHash = GetWichHash(STORE);
    if (owner == 0)
    {
        owner = GetWich();
    }
    else
    {
        ownerHash = GetB3FromHash256(STORE, owner, 0, 0);
    }

    if (currentINFO.freeCargoSpace > 0 && amount > 0)
    {
        long itemAmount = 0;
        if (currentINFO.freeCargoSpace > amount)
        {
            itemAmount = amount;
            currentINFO.freeCargoSpace -= amount;
        }
        else
        {
            itemAmount = currentINFO.freeCargoSpace;
            currentINFO.freeCargoSpace = 0;
        }

        //long wichHash = GetWichHash(STORE);
        setMapValue(STORE, owner, ownerHash);
        setMapValue(ownerHash, item, getMapValue(ownerHash, item) + itemAmount);

    }

}
void GetItemOutOfCargo(long item, long amount, long owner)
{
    if (owner == 0)
    {
        owner = GetWichHash(STORE);
    }
    else
    {
        owner = GetB3FromHash256(STORE, owner, 0, 0);
    }

    long amountAvailable = getMapValue(owner, item);
    if (amountAvailable > amount)
    {
        amountAvailable = amountAvailable - amount;
        currentINFO.freeCargoSpace += amount;
    }
    else
    {
        currentINFO.freeCargoSpace += amountAvailable;
        amountAvailable = 0;
    }

    setMapValue(owner, item, amountAvailable);

}

void DockObject(long objectContract)
{
    if ((objectType != STATION && currentINFO.freeHangars > 0) || objectType == STATION)
    {
        long ownerID = getExtMapValue(OWNER, 1, objectContract);

        long wichHash = GetWichHash(HANGAR);
        setMapValue(HANGARS, GetWich(), wichHash);
        setMapValue(wichHash, objectContract, 1);

        if (objectType != STATION)
        {
            // reduce hangar places
            currentINFO.freeHangars -= 1;
        }
    }

}
void UndockObject(long objectContract)
{
    long wichHash = GetWichHash(HANGAR);
    if (getMapValue(wichHash, objectContract) == 1)
    {
        setMapValue(wichHash, objectContract, 0);
        if (objectType != STATION)
        {
            // add hangar places
            currentINFO.freeHangars += 1;
        }
    }
}

long GetWich()
{
    if (objectType == STATION)
    {
        // item available only to the owner
        if (getCodeHashOf(currentPOLL.actorID) == 0)
        {
            // actorID is an account
            return currentPOLL.actorID;
        }
        else if (getCodeHashOf(currentPOLL.parameter4) == 0)
        {
            // actorID is an contract and parameter4 is an account
            return currentPOLL.parameter4;
        }

    }

    // items available to anyone on the vehicle
    return contractID;
}
long GetWichHash(long type) {
	return GetB3FromHash256(type, GetWich(), 0, 0);
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
