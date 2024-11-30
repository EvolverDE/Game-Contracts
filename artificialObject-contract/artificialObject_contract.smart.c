
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
#pragma maxConstVars 1
#pragma optimizationLevel 2
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
#define CREATE_STATION 1_001_010

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
#define MINING 1_002_010
#define REFINE 1_002_011
#define REPAIR 1_002_012
//#define SCAN 1_002_013
#define STORE 1_002_014
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
#define CATEGORY 1_005_001
#define TYPE 1_005_002
#define SIZE 1_005_003
//#define PROPERTIES 1_005_004
#define DEFAULT_SPEED 1_005_005
#define DEFAULT_STRUCTURE 1_005_006
#define DEFAULT_ARMOR 1_005_007
#define DEFAULT_SHIELD 1_005_008
#define DEFAULT_CARGO 1_005_009
#define COCKPITS 1_005_010
#define SLOT 1_005_011
#define SLOTS 1_005_012
#define HANGAR 1_005_013
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
#define HANGAR_MODULE 1_006_011
#define REFINERY_FACILITY 1_006_012
#define ASSEMBLE_FACILITY 1_006_013
//#define ADMINISTRATIVE_FACILITY 1_006_014
//#define OBSERVATORY_FACILITY 1_006_015
//#define TRADE_HUB 1_006_016

#define SHIP 1_006_017
#define STATION 1_006_018
// #define ASTEROID 1_006_019
// #define MOON 1_006_020
// #define PLANET 1_006_021
// #define STAR 1_006_022

// slot and hangar types
#define INTERNAL 1_007_001
#define EXTERNAL 1_007_002

// slot types
// #define AMMO_SLOT 1_007_003
#define GUN_SLOT 1_007_004
#define TURRET_SLOT 1_007_005
#define PULPIT_SLOT 1_007_006
#define LAUNCHER_SLOT 1_007_007
//#define ENGINE_SLOT 1_007_008
#define SHIELD_SLOT 1_007_009
#define CARGO_SLOT 1_007_010
#define SCANNER_SLOT 1_007_011
#define FACILITY_SLOT 1_007_012

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
#define REFINED 1_009_001
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
long objectName = 0; // Article = MngStati
long objectType = 0; // smallShip; mediumStation
long originalNFTID = 0;
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
    long hash,
	size,
	type,
	defaultSpeed,
	defaultStructure,
	defaultArmor,
	defaultShield,
	defaultCargo,
	freeCargoSpace,
	freeHangars,
	cockpits,
	slotsHash;
} currentINFO;

struct SLOTINFO {
    long hash,
	category,
	size,
	type,
	slot;
} currentSlot;

// struct HANGARINFO {
    // long hash,
	// size,
	// type;
// } currentHangar;

struct ARTICLEINFO {
    long category,
	type,
	size,
	slot;
} currentArticle;

constructor();

void constructor(void) {
    // this function will be called only once on first activation.
	gameVoteContract = 1111; //TODO: set initial ID
	status = 0;
	currentPOLL.hash = 0;
	currentArticle.category = 0;
	currentINFO.freeHangars = 0;
	contractID = GetcontractID();
	gameVoteContract = 1111;
	status = 0;
	setMapValue(GAMEVOTE_CONTRACT, 1, 1111);
	setMapValue(STATUS, 1, 0);
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
        currentPOLL.actorID = getExtMapValue(ACTOR_ID, hashValue, pollSaveID);
        currentPOLL.targetID = getExtMapValue(TARGET_ID, hashValue, pollSaveID);
        currentPOLL.mainMethod = getExtMapValue(MAINMETHOD, hashValue, pollSaveID);
        currentPOLL.subMethod = getExtMapValue(SUBMETHOD, hashValue, pollSaveID);
        currentPOLL.parameter = getExtMapValue(PARAMETER, hashValue, pollSaveID);
        currentPOLL.parameter2 = getExtMapValue(PARAMETER2, hashValue, pollSaveID);
        currentPOLL.parameter3 = getExtMapValue(PARAMETER3, hashValue, pollSaveID);
        currentPOLL.parameter4 = getExtMapValue(PARAMETER4, hashValue, pollSaveID);
	}
	
    currentPOLL.partOfPoll = currentTX.message[7];
	
}

void getPropertyDetails(void) {
    currentINFO.defaultSpeed = getExtMapValue(DEFAULT_SPEED, objectName, itemBaseContract);
    currentINFO.defaultStructure = getExtMapValue(DEFAULT_STRUCTURE, objectName, itemBaseContract);
    currentINFO.defaultArmor = getExtMapValue(DEFAULT_ARMOR, objectName, itemBaseContract);
    currentINFO.defaultShield = getExtMapValue(DEFAULT_SHIELD, objectName, itemBaseContract);
    currentINFO.defaultCargo = getExtMapValue(DEFAULT_CARGO, objectName, itemBaseContract);
    currentINFO.cockpits = getExtMapValue(COCKPITS, objectName, itemBaseContract);
    currentINFO.slotsHash = getExtMapValue(SLOTS, objectName, itemBaseContract);
	
    currentINFO.freeCargoSpace = currentINFO.defaultCargo;
	currentINFO.freeHangars = 0;
	
}
void getSlotDetails(long slotIndex) {
    currentSlot.hash = getExtMapValue(currentINFO.slotsHash, slotIndex, itemBaseContract);
	currentSlot.category = getExtMapValue(CATEGORY, currentSlot.hash, itemBaseContract);
    currentSlot.size = getExtMapValue(SIZE, currentSlot.hash, itemBaseContract);
    currentSlot.type = getExtMapValue(TYPE, currentSlot.hash, itemBaseContract);
    currentSlot.slot = getExtMapValue(SLOT, currentSlot.hash, itemBaseContract);
}

void getArticleDetails(long articleSymbol) {
	currentArticle.category = getExtMapValue(CATEGORY, articleSymbol, itemBaseContract);
    currentArticle.type = getExtMapValue(TYPE, articleSymbol, itemBaseContract);
    currentArticle.size = getExtMapValue(SIZE, articleSymbol, itemBaseContract);
    currentArticle.slot = getExtMapValue(SLOT, articleSymbol, itemBaseContract);
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
				 else if(currentTX.sender == locationContract && currentTX.message[0] == CONTRACT_SPECIFIC && status == 0 && currentTX.message[1] == CREATE_STATION)
				 {
					 // if status = 0 and sender = locationContract then just create this object
					 currentPOLL.mainMethod = CONTRACT_SPECIFIC;
					 currentPOLL.subMethod = CREATE_STATION;
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
void ContractSpecific(void) {
	/*
		* currentPOLL.actorID = owner / authID
		* currentPOLL.mainMethod = CONTRACT_SPECIFIC
		* currentPOLL.subMethod = SET_ITEMBASE
		* currentPOLL.parameter = itemBaseContract
		* currentPOLL.parameter2 = 
		* currentPOLL.parameter3 = 
		* currentPOLL.parameter4 = 
	*/
	
	if (itemBaseContract == 0 && currentPOLL.subMethod != SET_ITEMBASE) {
		return;
	}
	
	switch (currentPOLL.subMethod) {
		case SET_ITEMBASE:
			itemBaseContract = currentPOLL.parameter;
			setMapValue(ITEMBASE_CONTRACT, 1, currentPOLL.parameter);
			break;
		case SET_MARKETPLACE:
			marketplaceContract = currentPOLL.parameter;
			setMapValue(MARKETPLACE_CONTRACT, 1, currentPOLL.parameter);
			break;
		case TYPE:
			objectName = currentPOLL.parameter;
			objectType = getExtMapValue(TYPE, objectName, itemBaseContract);
			setMapValue(TYPE, objectType, objectName);
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
		case CREATE_STATION:
			if (getCodeHashOf(currentTX.sender) != 0 && currentTX.sender == locationContract && objectType == STATION){
				status = 1;
				setMapValue(STATUS, 1, 1);
				getPropertyDetails();
				setMapValue(SLOT, 1, 5576974018634211683); // MedRfFac = 5576974018634211683
				setMapValue(SLOT, 2, 5576973945837871459); // MedAsFac = 5576973945837871459
				setMapValue(SLOT, 3, 5576973975701778276); // MedHgMod = 5576973975701778276
				setMapValue(SLOT, 4, 6085033130354832738); // TradeHub = 6085033130354832738
				currentINFO.freeHangars = -1;
			}
			break;
	}
}
void GameSpecific(void) {
	switch (currentPOLL.subMethod) {
		
		case BUILD:
			Build();
			break;
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
		case MINING:
			Mining();
			break;
		case REFINE:
			Refining();
			break;
		case REPAIR:
			Repair();
			break;
		case STORE:
			Store();
			break;
		default:
			break;
	}
}

void Build(void) 
{
	/* build the object itself
	* currentPOLL.actorID = initiatorID (of build order)
	* currentPOLL.mainMethod = GAME_SPECIFIC
	* currentPOLL.subMethod = BUILD
	* currentPOLL.parameter = assembly slot (1)
	* currentPOLL.parameter2 = SYSTEM (Computer)
	* currentPOLL.parameter3 = Amount (1)
	* currentPOLL.parameter4 = 0
	*/

	long objectID = objectName;
	long tryBuild = 0;

	long hash = 0;

	if (status == 1)
	{
		// build objects with this object
		objectID = currentPOLL.parameter2;
		getArticleDetails(getMapValue(SLOT, currentPOLL.parameter));
		if (currentArticle.type == ASSEMBLE_FACILITY && currentArticle.size > getExtMapValue(SIZE, currentPOLL.parameter2, itemBaseContract) && getCodeHashOf(currentPOLL.actorID) == 0)
		{
			tryBuild = 1;
		}
	}
	else
	{
		// build object itself
		tryBuild = 1;
	}

	if (tryBuild == 1)
	{
		long needMaterialCount = getExtMapValue(INVENT, objectID, itemBaseContract);
		getArticleDetails(currentPOLL.parameter2);

		if (currentArticle.category == REFINED)
		{
			// get slot of refinery
			currentPOLL.parameter = GetSlotNumberOfType(currentArticle.type);
			Refining();
		}
		else if (currentArticle.type >= SHIP && currentPOLL.parameter4 != 0)
		{
			// SetItemIntoHangar
			if (getCodeHashOf(currentPOLL.parameter4) != 0 && status != 0)
			{
				if(CheckSubItems(objectID, needMaterialCount, currentPOLL.parameter3, currentPOLL.parameter4) == 1){
					//long wichHash = GetWichHash();
					//setMapValue(HANGAR, GetWich(), wichHash);
					//setMapValue(wichHash, currentPOLL.parameter2, currentPOLL.parameter4);

					//currentPOLL.parameter = 3; // slotnumber of hangar module
					//Dock();
					DockObject(currentPOLL.parameter4);
					// send build to target contract
					SetSendBufferForTargetContract(GAME_SPECIFIC, BUILD, currentPOLL.parameter2, currentPOLL.parameter3, 0, HASH, currentPOLL.hash, currentPOLL.partOfPoll);
					SendBufferWithAmount(currentFee, currentPOLL.parameter4);
				}
			}
		}
		else
		{
			if (CheckSubItems(objectID, needMaterialCount, currentPOLL.parameter3, currentPOLL.parameter4) == 1)
			{
				// Build materials ok
				if (status == 0)
				{
					status = 1;
					setMapValue(STATUS, 1, 1);
					setMapValue(OWNER, 1, currentPOLL.actorID);
					getPropertyDetails();
				}
				else
				{
					if (currentPOLL.parameter4 == 0)
					{
						// set into local cargo
						SetItemIntoCargo(currentPOLL.parameter2, currentPOLL.parameter3, 0);
					}
					else if(getCodeHashOf(currentPOLL.parameter4) != 0)
					{
						// send buildup materials to target contract
						SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentPOLL.parameter2, currentPOLL.parameter3, 0, HASH, currentPOLL.hash, currentPOLL.partOfPoll);
						SendBufferWithAmount(currentFee, currentPOLL.parameter4);
					}

					// refresh input materials amounts in map 
					for (long i = 1; i <= needMaterialCount; i++)
					{
						hash = getExtMapValue(objectID, i, itemBaseContract);
						GetItemOutOfCargo(getExtMapValue(PARAMETER2, hash, gameVoteContract), getExtMapValue(PARAMETER3, hash, gameVoteContract) * (currentPOLL.parameter3 / ONE_WHOLE), 0);
					}

				}
			}
			
		}
	}
}

void Dock(void) 
{
	/* dock the zeptor on the station
	* currentPOLL.actorID = initiatorID
	* currentPOLL.mainMethod = GAME_SPECIFIC
	* currentPOLL.subMethod = DOCK
	* currentPOLL.parameter = hangar module number (1..n)
	* currentPOLL.parameter2 = objectSymbol (ZeptLigh (to get properties))
	* currentPOLL.parameter3 = 0
	* currentPOLL.parameter4 = objectContract (contractID)
	*/

	/* Hangarmodul in slot einbauen (entwerfen, Eigenschaften festlegen)
	* wenn hangarmodul in station eingebaut -> unendlich viele hangarslots
	* wenn hangarmodul in mobile plattform (schlachtschiff, miningplattform, etc.) -> hangarslots beschränkt
	* beim docken überprüfen, ob es ein hangarmodul im angegebenen slot des objekts (station, schlachtschiff, miningplattform etc.) gibt
	*/

	// this object must exist
	if (status == 1)
	{
		// dock the source object to a hangarslot of this objects hangar module
		// the target must match the hangar module with a size less than or equal to the module itself 
		getArticleDetails(getExtMapValue(SLOT, currentPOLL.parameter, currentPOLL.targetID));
		if (currentArticle.type == HANGAR_MODULE && currentArticle.size >= getExtMapValue(SIZE, currentPOLL.parameter2, itemBaseContract))
		{
			if (currentPOLL.partOfPoll == ACTOR)
			{
				if(currentPOLL.targetID == currentTX.sender)
				{
					// undock 
					// set the location of this object to the location of the target object
					locationContract = getExtMapValue(LOCATION_CONTRACT, 1, currentPOLL.targetID);
					setMapValue(LOCATION_CONTRACT, 1, locationContract);
				} else {
					// dock
					// set the location of this object to the target object
					locationContract = currentPOLL.targetID;
					setMapValue(LOCATION_CONTRACT, 1, currentPOLL.targetID);
				}
			
			} else {
				//long dockedObject = getExtMapValue(GetWichHash(HANGAR), currentPOLL.parameter4, currentPOLL.targetID);
				if(getExtMapValue(GetWichHash(HANGAR), currentPOLL.parameter4, currentPOLL.targetID) == 1)
				{ 
					// undock
					UndockObject(currentPOLL.parameter4);
				} else {
					// dock
					DockObject(currentPOLL.parameter4);
				}

				SetSendBufferForTargetContract(HASH, currentPOLL.hash, 0, 0, 0, 0, 0, ACTOR);
				SendBufferWithAmount(currentFee, currentPOLL.parameter4);

			}
		}

	}
}
void Equip(void)
{
	/* equip the object
	 * currentPOLL.actorID = owner / authID
	 * currentPOLL.mainMethod = GAME_SPECIFIC
	 * currentPOLL.subMethod = EQUIP
	 * currentPOLL.parameter = articleSymbol
	 * currentPOLL.parameter2 = slotIndex (1...n)
	 * currentPOLL.parameter3 = 0
	 * currentPOLL.parameter4 = 0
	 */
	
	// this object must exist
	if (status == 1)
	{
		// check HANGAR
		if(getExtMapValue(getExtMapValue(HANGARS, currentPOLL.parameter4, currentPOLL.actorID), currentPOLL.targetID, currentPOLL.actorID) == 1)
		{
			// check article available
			// long temp1 = getExtMapValue(STORE, currentPOLL.actorID, locationContract);
			// long temp2 = getExtMapValue(currentPOLL.parameter, getExtMapValue(STORE, currentPOLL.actorID, locationContract), locationContract);

			if (getExtMapValue(currentPOLL.parameter, getExtMapValue(STORE, currentPOLL.parameter4, currentPOLL.actorID), currentPOLL.actorID) / ONE_WHOLE >= 1 || getMapValue(currentPOLL.parameter, getMapValue(STORE, currentPOLL.parameter4)) / ONE_WHOLE >= 1)
			{
				// check if cargo is remote
				if (contractID == currentPOLL.actorID && currentPOLL.actorID != currentPOLL.targetID)
				{
					setMapValue(TEMPAUTH_CONTRACT, currentPOLL.targetID, SetTimeOut(16));
				}
				else
				{
					// check article fits slot
					long slotIndex = GetArticleMatchSlotIndex(currentPOLL.parameter, currentPOLL.parameter2);
					if (slotIndex != 0)
					{
						// equip article to slotIndex
						setMapValue(SLOT, slotIndex, currentPOLL.parameter);
						// get article out of cargo
						if(currentPOLL.actorID == currentPOLL.targetID)
						{
							// get article out of this objects store  (local cargo)
							GetItemOutOfCargo(currentPOLL.parameter, 1, 0);
						}
						else
						{
							// get article out of the locations store (remote cargo)
							SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentPOLL.parameter, ONE_WHOLE, 0, HASH, currentPOLL.hash, ACTOR);
							SendBufferWithAmount(currentFee, locationContract);
						}
					}
				}
			}
		}
	}
	
}
void Explode(void) 
{
	// object destroyed/not exist
	status = 0;
	setMapValue(STATUS, 1, 0);
	long count = getExtMapValue(INVENT, objectName, itemBaseContract);
	for (int i = 1; i <= count; i++)
	{
		setMapValue(BUILD, getExtMapValue(PARAMETER2, getExtMapValue(objectName, i, itemBaseContract), gameVoteContract), 0);
	}
}
void Insure(void)
{
	insurence = currentPOLL.parameter;
}
	
void Mining(void)
{
	if ((getCodeHashOf(currentPOLL.targetID) != 0 || getCodeHashOf(currentPOLL.actorID) == 0) && currentPOLL.parameter4 == contractID)
	{
		SetItemIntoCargo(currentPOLL.parameter, currentPOLL.parameter2, 0);
	}
}
void Refining(void)
{
	/* refine the object
	* currentPOLL.actorID = initiatorID, not contract
	* currentPOLL.mainMethod = GAME_SPECIFIC
	* currentPOLL.subMethod = REFINE
	* currentPOLL.parameter = refinery slot (2)
	* currentPOLL.parameter2 = Refined Materials (GoldBar)
	* currentPOLL.parameter3 = Amount (1)
	* currentPOLL.parameter4 = 0
	*/

	if (status != 0)
	{
		getArticleDetails(getMapValue(SLOT, currentPOLL.parameter));
		if (currentArticle.type == REFINERY_FACILITY && currentArticle.type == getExtMapValue(TYPE, currentPOLL.parameter2, itemBaseContract) && currentArticle.size > getExtMapValue(SIZE, currentPOLL.parameter2, itemBaseContract) && getCodeHashOf(currentPOLL.actorID) == 0)
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
					long hash = getExtMapValue(currentPOLL.parameter2, i, itemBaseContract);
					GetItemOutOfCargo(getExtMapValue(PARAMETER2, hash, gameVoteContract), getExtMapValue(PARAMETER3, hash, gameVoteContract) * (currentPOLL.parameter3 / ONE_WHOLE), 0);
				}
			}
		}
	}
}
void Repair(void)
{
	// TODO: Implement and Test
}

void Store(void)
{
	if (status == 0)
	{
		if (currentPOLL.partOfPoll == TARGET)
		{
			setMapValue(BUILD, currentPOLL.parameter, getMapValue(BUILD, currentPOLL.parameter) + currentPOLL.parameter2);
		}
	}
	else
	{
		/* incoming message 
		* currentPOLL.mainMethod = GAME_SPECIFIC
		* currentPOLL.subMethod = STORE
		* currentPOLL.parameter = ITEM (ELEMENT,ARTICLE,...)
		* currentPOLL.parameter2 = amount (123)
		* currentPOLL.parameter3 = 0 (FROM)
		* currentPOLL.parameter4 = 0 (TO)
		* currentPOLL.actorID = 10000 (account)
		* currentPOLL.targetID = 7777 (artificial contract)
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
			if (getMapValue(currentPOLL.parameter, GetB3FromHash256(STORE, currentPOLL.parameter3, 0, 0)) >= currentPOLL.parameter2)
			{
				GetItemOutOfCargo(currentPOLL.parameter, currentPOLL.parameter2, currentPOLL.parameter3);
				SetItemIntoCargo(currentPOLL.parameter, currentPOLL.parameter2, currentPOLL.parameter4);
			}
		}
	}
}

// support methods
long CheckSubItems(long objectID, long needMaterialCount, long wishAmount, long targetID)
{
	long hash = 0;
	long material = 0;
	long materialNeedAmount = 0;
	long materialAvailableAmount = 0;
	long materialCount = 0;

	// check input materials amounts
	for (long i = 1; i <= needMaterialCount; i++)
	{
		hash = getExtMapValue(objectID, i, itemBaseContract);
		material = getExtMapValue(PARAMETER2, hash, gameVoteContract);
		materialNeedAmount = getExtMapValue(PARAMETER3, hash, gameVoteContract);
		materialAvailableAmount = 0;
		long atLeastNQT = materialNeedAmount;
		long atLeast = atLeastNQT / ONE_WHOLE;

		if (atLeastNQT <= 0)
		{
			continue;
		}

		if (atLeastNQT / ONE_WHOLE == 0)
		{
			atLeast = ONE_WHOLE / atLeastNQT;
			//atLeastNQT = atLeast * ONE_WHOLE;
			wishAmount = wishAmount / atLeastNQT * atLeastNQT;

			if (wishAmount * atLeast < atLeastNQT)
			{
				wishAmount = atLeastNQT;
			}

		}
		else
		{
			if (wishAmount < ONE_WHOLE)
			{
				wishAmount = ONE_WHOLE;
			}
		}

		if (status == 0)
		{
			materialAvailableAmount = getMapValue(BUILD, material);
		}
		else
		{
			// check if in buildup
			if (targetID != 0 && currentArticle.type == SHIP)
			{
				materialAvailableAmount = getExtMapValue(BUILD, material, targetID);
			}
			else
			{
				materialAvailableAmount = getMapValue(material, GetWichHash(STORE));// GetAmountAvailable(material);
			}
		}

		if (materialAvailableAmount > 0 && materialAvailableAmount >= materialNeedAmount * (wishAmount / ONE_WHOLE))
		{
			materialCount++;
			continue;
		}

	}

	if (materialCount >= needMaterialCount)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

long IsAuthenticated(long sender)
{
	if(sender == gameVoteContract || sender == itemBaseContract || sender == marketplaceContract || sender == locationContract || GetTimeIsUp(getMapValue(TEMPAUTH_CONTRACT, sender)) == 0)
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

long GetArticleMatchSlotIndex(long articleSymbol, long desiredSlotIndex)
{
    getArticleDetails(articleSymbol);
    getSlotDetails(desiredSlotIndex);

    if (currentSlot.size == currentArticle.size)
    {
        switch (currentSlot.slot)
        {
            case INTERNAL:
                switch (currentArticle.slot)
                {
                    case INTERNAL:
                    case SHIELD_SLOT:
                    case CARGO_SLOT:
                    case FACILITY_SLOT:
                        if (currentSlot.type == 0 || currentSlot.type == currentArticle.type)
                        {
                            return desiredSlotIndex;
                        }
                        break;
                }
                break;
            case EXTERNAL:
                switch (currentArticle.slot)
                {
                    case EXTERNAL:
                    case SCANNER_SLOT:
                    case GUN_SLOT:
                    case TURRET_SLOT:
                    case PULPIT_SLOT:
                    case LAUNCHER_SLOT:
                        if (currentSlot.type == 0 || currentSlot.type == currentArticle.type)
                        {
                            return desiredSlotIndex;
                        }
                        break;
                }
                break;
            default:
                if (currentArticle.slot == currentSlot.slot)
                {
                    return desiredSlotIndex;
                }
                break;

        }

    }

    return 0;

}

long GetSlotNumberOfType(long searchType)
{
    long slotAmount = getExtMapValue(currentINFO.slotsHash, 0, itemBaseContract);

    for(long i = 1; i <= slotAmount; i++)
    {
        getSlotDetails(i);

        if (currentSlot.type == searchType)
        {
            return i;
        }

    }

    return 0;
}

void SetItemIntoCargo(long item, long amount, long owner)
{
    long ownerHash = GetWichHash(STORE);
    if(owner == 0)
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
        setMapValue(item, ownerHash, getMapValue(item, ownerHash) + itemAmount);

    }

}
void GetItemOutOfCargo(long item, long amount, long owner)
{
    if(owner == 0)
    {
        owner = GetWichHash(STORE);
    }
    else
    {
        owner = GetB3FromHash256(STORE, owner, 0, 0);
    }

    long amountAvailable = getMapValue(item, owner);
    if(amountAvailable > amount)
    {
        amountAvailable = amountAvailable - amount;
        currentINFO.freeCargoSpace += amount;
    }
    else
    {
        currentINFO.freeCargoSpace += amountAvailable;
        amountAvailable = 0;
    }
    
    setMapValue(item, owner, amountAvailable);
    
}

void DockObject(long objectContract)
{
    if((objectType != STATION && currentINFO.freeHangars > 0) || objectType == STATION)
    {
        long wichHash = GetWichHash(HANGAR);
        setMapValue(HANGARS, GetWich(), wichHash);
        setMapValue(wichHash, objectContract, 1);

        if(objectType != STATION)
        {
            // reduce hangar places
            currentINFO.freeHangars -= 1;
        }
    }

}
void UndockObject(long objectContract)
{
    long wichHash = GetWichHash(HANGAR);
    if(getMapValue(wichHash, objectContract) == 1)
    {
        setMapValue(wichHash, objectContract, 0);
        if (objectType != STATION)
        {
            // add hangar places
            currentINFO.freeHangars += 1;
        }
    }
}


long GetWich(void) {
	if (objectType == STATION) {
		return currentPOLL.actorID;
	}
	
	return objectName;
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
