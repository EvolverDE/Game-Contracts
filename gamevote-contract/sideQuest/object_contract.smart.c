/**
	* @author hansmeuller
	* i´m doing my part
*/

#include APIFunctions

#program name miningStationContract
#program description this is a mining station
#program activationAmount .5

#pragma maxAuxVars 4
#pragma maxConstVars 3
#pragma optimizationLevel 1
#pragma version 2.2.1

#define ONE_WHOLE 100000000

// contract methods
//#define DEPOSITING 1_000_000
#define CONTRACT_SPECIFIC 1_000_001
#define GAME_SPECIFIC 1_000_002
//#define WITHDRAWALING 1_000_003

// sub CONTRACT_SPECIFIC methods
#define SET_ITEMBASE 1_001_001
#define SET_LOCATION 1_001_002
#define SET_OWNER 1_001_003
#define SET_NFTID 1_001_004
#define AUTHENTICATION 1_001_005
#define CREATE_STATION 1_001_006

// sub GAME_SPECIFIC methods 
#define ACT 1_002_001
#define BUILD 1_002_002
#define DELIVER 1_002_003
#define DESCRIBE 1_002_004
#define DOCK 1_002_005
#define EQUIP 1_002_006
#define EXPLODE 1_002_007
#define INSURE 1_002_008
#define INVENT 1_002_009
#define MINING 1_002_010
#define REFINE 1_002_011
#define REPAIR 1_002_012
#define SCAN 1_002_013
#define STORE 1_002_014
#define TREAT 1_002_015

// (ext)map flags
// standard
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
#define GUN 1_006_001
#define TURRET 1_006_002
#define PULPIT 1_006_003
#define LAUNCHER 1_006_004
#define ENGINE 1_006_005
#define SHIELD 1_006_006
#define CARGO 1_006_007
#define DRONE 1_006_008

#define FACILITY 1_006_009
#define ASSEMBLE_FACILITY 1_006_010
#define REFINERY_FACILITY 1_006_011
#define TRADE_HUB 1_006_012
#define ADMINISTRATIVE_FACILITY 1_006_013

#define SHIP 1_006_014
#define STATION 1_006_015

// Slot and Hangar types
#define INTERNAL 1_007_001
#define EXTERNAL 1_007_002

// Slot types
#define AMMO_SLOT 1_007_003
#define GUN_SLOT 1_007_004
#define TURRET_SLOT 1_007_005
#define PULPIT_SLOT 1_007_006
#define LAUNCHER_SLOT 1_007_007
#define ENGINE_SLOT 1_007_008
#define SHIELD_SLOT 1_007_009
#define CARGO_SLOT 1_007_010
#define FACILITY_SLOT 1_007_011

// Hangar types
#define DRONE_HANGAR 1_007_012
#define SHIP_HANGAR 1_007_013
#define STATION_HANGAR 1_007_014

// sizes and weights
#define SMALL 1_008_017
#define MEDIUM 1_008_018
#define LARGE 1_008_019
#define XLARGE 1_008_020
#define CAPITAL 1_008_021

#define LIGHT 1_008_022
#define HEAVY 1_008_023


// item tree
//#define ELEMENT 1_100_000
#define REFINED 1_100_001
//#define COMMON 1_100_002
//#define ADVANCED 1_100_003
//#define COMPONENT 1_100_004
//#define SYSTEM 1_100_005
//#define ARTICLE 1_100_006

// zeptorlight contract specific
#define OWNER 1_009_001
#define STATUS 1_009_002
//#define AMOUNT 1_009_003

// extContract flags
#define ACTOR 1_010_001
#define TARGET 1_010_002

// contract attributes
// basic contract 
long authIDs[10];
long sendBuffer[8];
long currentFee = ONE_WHOLE;

// advanced
long contractID = 0;
long itemBaseID = 0;
long location = 0;
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
	cockpits,
	slotsHash,
	hangarsHash;
} currentINFO;

struct SLOTINFO {
    long hash,
	category,
	size,
	type,
	slot;
} currentSlot;

struct HANGARINFO {
    long hash,
	size,
	type;
} currentHangar;

struct ARTICLEINFO {
    long category,
	type,
	size,
	slot;
} currentArticle;

constructor();

void constructor(void) {
    // this function will be called only once on first activation.
	authIDs[0] = 1111; //TODO: set initial ID
	currentPOLL.hash = 0;
	currentArticle.category = 0;
	contractID = GetContractID();
}

long GetContractID(){
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

void getPropertyDetails(void) {
    currentINFO.defaultSpeed = getExtMapValue(DEFAULT_SPEED, objectName, itemBaseID);
    currentINFO.defaultStructure = getExtMapValue(DEFAULT_STRUCTURE, objectName, itemBaseID);
    currentINFO.defaultArmor = getExtMapValue(DEFAULT_ARMOR, objectName, itemBaseID);
    currentINFO.defaultShield = getExtMapValue(DEFAULT_SHIELD, objectName, itemBaseID);
    currentINFO.defaultCargo = getExtMapValue(DEFAULT_CARGO, objectName, itemBaseID);
    currentINFO.cockpits = getExtMapValue(COCKPITS, objectName, itemBaseID);
    currentINFO.slotsHash = getExtMapValue(SLOTS, objectName, itemBaseID);
    currentINFO.hangarsHash = getExtMapValue(HANGARS, objectName, itemBaseID);
	
    currentINFO.freeCargoSpace = currentINFO.defaultCargo;
	
}
void getSlotDetails(long slotIndex) {
    currentSlot.hash = getExtMapValue(currentINFO.slotsHash, slotIndex, itemBaseID);
    currentSlot.size = getExtMapValue(SIZE, currentSlot.hash, itemBaseID);
    currentSlot.type = getExtMapValue(TYPE, currentSlot.hash, itemBaseID);
    currentSlot.slot = getExtMapValue(SLOT, currentSlot.hash, itemBaseID);
}
void getHangarDetails(long hangarIndex) {
    currentHangar.hash = getExtMapValue(currentINFO.hangarsHash, hangarIndex, itemBaseID);
    currentHangar.size = getExtMapValue(SIZE, currentSlot.hash, itemBaseID);
    currentHangar.type = getExtMapValue(TYPE, currentSlot.hash, itemBaseID);
}
void getArticleDetails(long articleSymbol)
{
    currentArticle.type = getExtMapValue(TYPE, articleSymbol, itemBaseID);
    currentArticle.size = getExtMapValue(SIZE, articleSymbol, itemBaseID);
    currentArticle.slot = getExtMapValue(SLOT, articleSymbol, itemBaseID);
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
				if (currentTX.message[0] == CONTRACT_SPECIFIC && currentTX.message[1] == CREATE_STATION)
				{
					currentPOLL.mainMethod = CONTRACT_SPECIFIC;
					currentPOLL.subMethod = CREATE_STATION;
				}
				else if (currentTX.message[0] == GAME_SPECIFIC)
				{

					//SetSendBufferForTargetContract(GAME_SPECIFIC, BUILD, 0, 0, 0, 0, currentPOLL.actorID, currentPOLL.parameter4);
					//SendBufferWithAmount(ONE_WHOLE, currentPOLL.parameter4);

					/* build the object itself
					* currentPOLL.actorID = initiatorID (of build order)
					* currentPOLL.mainMethod = GAME_SPECIFIC
					* currentPOLL.subMethod = BUILD
					* currentPOLL.parameter = assembly slot (1)
					* currentPOLL.parameter2 = SYSTEM (Computer)
					* currentPOLL.parameter3 = Amount (1)
					* currentPOLL.parameter4 = 0
					*/

					currentPOLL.hash = 0;
					currentPOLL.mainMethod = GAME_SPECIFIC;
					currentPOLL.subMethod = currentTX.message[1];
					currentPOLL.parameter = currentTX.message[2];
					currentPOLL.parameter2 = currentTX.message[3];
					currentPOLL.parameter3 = currentTX.message[4];
					currentPOLL.parameter4 = currentTX.message[5];
					currentPOLL.actorID = currentTX.message[6];
					currentPOLL.targetID = currentTX.message[7];

					if(currentPOLL.targetID == contractID)
					{
						currentPOLL.partOfPoll = TARGET;
					}

				}
				else
				{
					getPollDetails(currentTX.message[0], currentTX.sender);
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
	/*
	 * currentPOLL.actorID = owner / authID
	 * currentPOLL.mainMethod = CONTRACT_SPECIFIC
	 * currentPOLL.subMethod = SET_ITEMBASE
	 * currentPOLL.parameter = itemBaseID
	 * currentPOLL.parameter2 = 
	 * currentPOLL.parameter3 = 
	 * currentPOLL.parameter4 = 
	 */

	if (itemBaseID == 0 && currentPOLL.subMethod != SET_ITEMBASE)
	{
		return;
	}

	switch (currentPOLL.subMethod)
	{
		case SET_ITEMBASE:
			itemBaseID = currentPOLL.parameter;
			authIDs[1] = currentPOLL.parameter;
			break;
		case TYPE:
			objectName = currentPOLL.parameter;
			objectType = getExtMapValue(TYPE, objectName, itemBaseID);
			break;
		case SET_OWNER:
			//owner = currentPOLL.actorID;
			setMapValue(OWNER, 0, currentPOLL.actorID);
			break;
		case SET_NFTID:
			originalNFTID = currentPOLL.parameter;
			break;
		case AUTHENTICATION:
			authIDs[currentPOLL.parameter] = currentPOLL.actorID;
			break;
		case SET_LOCATION:
			location = currentPOLL.parameter;
			authIDs[9] = currentPOLL.parameter;
			break;
		case CREATE_STATION:
			if (getCodeHashOf(currentTX.sender) != 0 && currentTX.sender == location && objectType == STATION)
			{
				setMapValue(STATUS, 0, 1);
				getPropertyDetails();
				setMapValue(SLOT, 1, 5576974018634211683); // MedRfFac = 5576974018634211683
				setMapValue(SLOT, 2, 5576973945837871459); // MedAsFac = 5576973945837871459
			}
			break;
	}
}
void GameSpecific(void)
{
	switch (currentPOLL.subMethod)
	{
		case ACT:
			Act();
			break;
		case BUILD:
			Build();
			break;
		case DELIVER:
			Deliver();
			break;
		case DESCRIBE:
			Describe();
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
		case INVENT:
			Invent();
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
		case SCAN:
			Scan();
			break;
		case STORE:
			Store();
			break;
		case TREAT:
			Treat();
			break;
		default:
			break;
	}
}

// sub methods
void Act(void)
{

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
	long status = getMapValue(STATUS, 0);
	long tryBuild = 0;

	long hash = 0;

	if (status != 0)
	{
		objectID = currentPOLL.parameter2;
		getArticleDetails(getMapValue(SLOT, currentPOLL.parameter));
		if (currentArticle.type == ASSEMBLE_FACILITY && currentArticle.size > getExtMapValue(SIZE, currentPOLL.parameter2, itemBaseID) && getCodeHashOf(currentPOLL.actorID) == 0)
		{
			tryBuild = 1;
		}
	}
	else
	{
		tryBuild = 1;
	}

	if (tryBuild == 1)
	{
		long needMaterialCount = getExtMapValue(INVENT, objectID, itemBaseID);
		getArticleDetails(currentPOLL.parameter2);

		if (currentArticle.category == REFINED)
		{
			// get slot of refinery
			currentPOLL.parameter = GetSlotNumberOfType(currentArticle.type);
			Refining();
		}
		else if (currentArticle.type >= SHIP && currentPOLL.parameter4 != 0)
		{
			// TODO: SetItemIntoHangar
			if (getCodeHashOf(currentPOLL.parameter4) != 0 && status != 0)
			{
				if(CheckSubItems(objectID, needMaterialCount, currentPOLL.parameter3, currentPOLL.parameter4) == 1){
					long wichHash = GetWichHash();
					setMapValue(HANGAR, GetWich(), wichHash);
					setMapValue(wichHash, currentPOLL.parameter2, currentPOLL.parameter4);

					// send build to target contract
					SetSendBufferForTargetContract(GAME_SPECIFIC, BUILD, currentPOLL.parameter2, currentPOLL.parameter3, 0, 0, currentPOLL.actorID, currentPOLL.parameter4);
					SendBufferWithAmount(ONE_WHOLE, currentPOLL.parameter4);
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
					setMapValue(STATUS, 0, 1);
				}
				else
				{
					if (currentPOLL.parameter4 == 0)
					{
						// set into local cargo
						SetItemIntoCargo(currentPOLL.parameter2, currentPOLL.parameter3);
					}
					else if(getCodeHashOf(currentPOLL.parameter4) != 0)
					{
						// send buildup materials to target contract
						SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentPOLL.parameter2, currentPOLL.parameter3, 0, 0, currentPOLL.actorID, currentPOLL.parameter4);
						SendBufferWithAmount(ONE_WHOLE, currentPOLL.parameter4);
					}

					// refresh input materials amounts in map 
					for (long i = 1; i <= needMaterialCount; i++)
					{
						hash = getExtMapValue(objectID, i, itemBaseID);
						GetItemOutOfCargo(getExtMapValue(PARAMETER2, hash, authIDs[0]), getExtMapValue(PARAMETER3, hash, authIDs[0]) * (currentPOLL.parameter3 / ONE_WHOLE));
					}

				}
			}
	
		}
	}

}

long CheckSubItems(long objectID, long needMaterialCount, long wishAmount, long targetID)
{
	long status = getMapValue(STATUS, 0);

	long hash = 0;
	long material = 0;
	long materialNeedAmount = 0;
	long materialAvailableAmount = 0;
	long materialCount = 0;

	// check input materials amounts
	for (long i = 1; i <= needMaterialCount; i++)
	{
		hash = getExtMapValue(objectID, i, itemBaseID);
		material = getExtMapValue(PARAMETER2, hash, authIDs[0]);
		materialNeedAmount = getExtMapValue(PARAMETER3, hash, authIDs[0]);
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
				materialAvailableAmount = GetAmountAvailable(material);
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

void Deliver(void)
{

}
void Describe(void)
{

}
void Dock(void)
{
	/* dock the zeptor on the station
	* currentPOLL.actorID = initiatorID
	* currentPOLL.mainMethod = GAME_SPECIFIC
	* currentPOLL.subMethod = DOCK
	* currentPOLL.parameter = todo (maybe hangar number (1..n))
	* currentPOLL.parameter2 = todo (what to dock? (small ship, medium ship, big ship))
	* currentPOLL.parameter3 = todo (?)
	* currentPOLL.parameter4 = todo (?)
	*/
	void Dock(void)
	{
		//if activ
		if (getMapValue(STATUS, 0) == 0)
		{
			return;
		}

		long hangarIndex = currentPOLL.parameter; // hangar
		getHangarDetails(hangarIndex);

		// zu beaucoup???
		long shipSize = currentPOLL.parameter2;
		if (currentHangar.size >= shipSize)
		{
			// docking
			setMapValue(HANGAR, hangarIndex, currentPOLL.actorID);
		}
		else
		{
			//try?
		}
	}
}
void Equip(void)
{
	// equip the object
	// currentTX.message[0] = method (ACT)
	// currentTX.message[1] = command (EQUIP)
	// currentTX.message[2] = parameter (gattling20mm = ID)
	// currentTX.message[3] = parameter2 (slotindex = 1...n)
	// currentTX.message[4] = free
	// currentTX.message[5] = free
	// currentTX.message[6] = free
	// currentTX.message[7] = free

	/* equip the object
	 * currentPOLL.actorID = owner / authID
	 * currentPOLL.mainMethod = GAME_SPECIFIC
	 * currentPOLL.subMethod = EQUIP
	 * currentPOLL.parameter = articleSymbol
	 * currentPOLL.parameter2 = slotIndex (1...n)
	 * currentPOLL.parameter3 = 
	 * currentPOLL.parameter4 = 
	 */
	void Equip(void)
	{
		// zu beaucoup???
		long slotIndex = GetArticleMatchSlotIndex(currentPOLL.parameter, currentPOLL.parameter2);

		if (slotIndex == 0)
		{
			return;
		}

		// check slot
		long currentEquipment = getMapValue(SLOTS, slotIndex);

		if (currentEquipment != 0)
		{
			// warning?
		}

		// get 
		setMapValue(SLOTS, slotIndex, currentPOLL.parameter);
	}

	long slotIndex = GetArticleMatchSlotIndex(currentPOLL.parameter, currentPOLL.parameter2);
	if (slotIndex != 0)
	{
		setMapValue(SLOTS, slotIndex, currentPOLL.parameter);
	}
}
void Explode(void)
{
	// station destroyed/not exist
	setMapValue(STATUS, 0, 0);
	long count = getExtMapValue(INVENT, objectName, itemBaseID);
	for (long i = 1; i <= count; i++)
	{
		setMapValue(BUILD, getExtMapValue(PARAMETER2, getExtMapValue(objectName, i, itemBaseID), authIDs[0]), 0);
	}

}
void Insure(void)
{
	insurence = currentPOLL.parameter;
}
void Invent(void)
{

}
void Mining(void)
{

	if (getCodeHashOf(currentPOLL.targetID) != 0 || getCodeHashOf(currentPOLL.actorID) == 0)
	{
		SetItemIntoCargo(currentPOLL.parameter, currentPOLL.parameter2);
	}

	//if (currentPOLL.partOfPoll == ACTOR)
	//{
	//    if (getCodeHashOf(currentPOLL.targetID) != 0)
	//    {
	//        /* mining ELEMENT 
	//         * currentPOLL.actorID = this
	//         * currentPOLL.targetID = contract
	//         * currentPOLL.mainMethod = GAME_SPECIFIC
	//         * currentPOLL.subMethod = MINING
	//         * currentPOLL.parameter = Element (IRON)
	//         * currentPOLL.parameter2 = Amount (123)
	//         * currentPOLL.parameter3 = 0
	//         * currentPOLL.parameter4 = 0
	//         */

	//        SetItemIntoCargo(currentPOLL.parameter, currentPOLL.parameter2);
	//    }
	//}
	//else if (currentPOLL.partOfPoll == TARGET)
	//{
	//    if (getCodeHashOf(currentPOLL.actorID) == 0)
	//    {
	//        /* mining ELEMENT 
	//         * currentPOLL.actorID = account, non contract
	//         * currentPOLL.targetID = this
	//         * currentPOLL.mainMethod = GAME_SPECIFIC
	//         * currentPOLL.subMethod = MINING
	//         * currentPOLL.parameter = Element (IRON)
	//         * currentPOLL.parameter2 = Amount (123)
	//         * currentPOLL.parameter3 = 0
	//         * currentPOLL.parameter4 = 0
	//         */

	//        SetItemIntoCargo(currentPOLL.parameter, currentPOLL.parameter2);
	//    }

	//    // forward to solarsystem ID (as location)

	//    // ### outgoing to TARGET ###
	//    // recipient = targetContractID
	//    // message[0] = currentHash
	//    // message[1] = TARGET (wich part on this poll)
	//    // message[2] = voteContractID (optional)
	//    // message[3] = free
	//    // message[4] = free
	//    // message[5] = free
	//    // message[6] = free
	//    // message[7] = free

	//    SetSendBufferForTargetContract(currentPOLL.hash, TARGET, authIDs[0], 0, 0, 0, 0, 0);
	//    SendBufferWithAmount(currentPOLL.pollAmount, location);

	//}

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

	if (getMapValue(STATUS, 0) != 0)
	{
		getArticleDetails(getMapValue(SLOT, currentPOLL.parameter));
		if (currentArticle.type == REFINERY_FACILITY && currentArticle.type == getExtMapValue(TYPE, currentPOLL.parameter2, itemBaseID) && currentArticle.size > getExtMapValue(SIZE, currentPOLL.parameter2, itemBaseID) && getCodeHashOf(currentPOLL.actorID) == 0)
		{
			long needMaterialCount = getExtMapValue(INVENT, currentPOLL.parameter2, itemBaseID);
			if (CheckSubItems(currentPOLL.parameter2, needMaterialCount, currentPOLL.parameter3, 0) == 1)
			{
				if (currentPOLL.parameter4 == 0)
				{
					// set into local cargo
					SetItemIntoCargo(currentPOLL.parameter2, currentPOLL.parameter3);
				}
				else if (getCodeHashOf(currentPOLL.parameter4) != 0)
				{
					// send buildup materials to target contract
					SetSendBufferForTargetContract(GAME_SPECIFIC, STORE, currentPOLL.parameter2, currentPOLL.parameter3, 0, 0, currentPOLL.actorID, currentPOLL.parameter4);
					SendBufferWithAmount(ONE_WHOLE, currentPOLL.parameter4);
				}

				// refresh input materials amounts in map 
				for (long i = 1; i <= needMaterialCount; i++)
				{
					long hash = getExtMapValue(currentPOLL.parameter2, i, itemBaseID);
					GetItemOutOfCargo(getExtMapValue(PARAMETER2, hash, authIDs[0]), getExtMapValue(PARAMETER3, hash, authIDs[0]) * (currentPOLL.parameter3 / ONE_WHOLE));
				}
			}
		}
	}

}
void Repair(void)
{

}
void Scan(void)
{

}
void Store(void)
{
	if (getMapValue(STATUS, 0) == 0)
	{
		if (currentPOLL.partOfPoll == TARGET)
		{
			setMapValue(BUILD, currentPOLL.parameter, getMapValue(BUILD, currentPOLL.parameter) + currentPOLL.parameter2);
		}
	}
	else
	{
		if (currentPOLL.partOfPoll == ACTOR && getCodeHashOf(currentPOLL.targetID) != 0)
		{
			// currentPOLL.parameter = Element (IRON)
			// currentPOLL.parameter2 = Amount (123)

			GetItemOutOfCargo(currentPOLL.parameter, currentPOLL.parameter2);
		}
		else if (currentPOLL.partOfPoll == TARGET)
		{
			SetItemIntoCargo(currentPOLL.parameter, currentPOLL.parameter2);
		}
	}
	
}
void Treat(void)
{

}

long IsAuthenticated(long sender)
{

	if(sender == itemBaseID || sender == location || sender == location)
	{
		return 1;
	}

	for (long i = 0; i < authIDs.length; i++)
	{
		if (authIDs[i] == sender)
		{
			return 1;
		}
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
	long slotAmount = getExtMapValue(currentINFO.slotsHash, 0, itemBaseID);

	for(long i = 1; i < slotAmount; i++)
	{
		getSlotDetails(i);

		if (currentSlot.type == searchType)
		{
			return i;
		}

	}

	return 0;
}

void SetItemIntoCargo(long item, long amount)
{
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

		long wichHash = GetWichHash();
		setMapValue(STORE, GetWich(), wichHash);
		setMapValue(item, wichHash, getMapValue(item, wichHash) + itemAmount);

	}

}
void GetItemOutOfCargo(long item, long amount)
{

	long amountAvailable = GetAmountAvailable(item);
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
	
	setMapValue(item, GetWichHash(), amountAvailable);
	
}

long GetAmountAvailable(long item)
{
	long x = getMapValue(item, GetWichHash());
	return x;
}

long GetWich(void)
{
	if (objectType == STATION)
	{
		return currentPOLL.actorID;
	}

	return objectName;
}
long GetWichHash(void)
{
	long y = GetB3FromHash256(STORE, GetWich(), 0, 0);
	return y;
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
