/**
 * @author hansmeuller
 * i´m doing my part
 */
 
#pragma maxAuxVars 3
#pragma maxConstVars 3
#pragma optimizationLevel 1
#pragma version 2.2.1

// enumeration substitute
#define ONESIGNA 100000000

// sub methods
#define ACT 0
#define BUILD 1
#define DOCK 2
#define EQUIP 3
#define EXPLODE 4
#define MINING 5
#define REPAIR 6
#define SCAN 7
#define STORE 8
#define TREAT 9

// map flags
#define ACTION 5

// index for slots array
#define WEAPON_SLOT_1 1
#define WEAPON_SLOT_2 2
#define ENGINE_SLOT 3
#define SHIELD_SLOT 4
#define CARGO_SLOT 5

// Systems required to build up the ship (1XX = Refined Material; 2XX = Common Material; 3XX = Advanced Material; 4XX = Component; 5XX = SYSTEM; 6XX = Article)
#define COCKPIT_INSTRUMENT 500 // 4 pieces required
#define COMPUTER 501 // 1 pieces required
#define MONITOR 502 // 1 pieces required
#define SMALL_ENERGY_GENERATOR 503 // 1 pieces required
#define SMALL_ENGINE 504 // 2 pieces required
#define SMALL_EXTERNAL_SLOT 505 // 2 pieces required
#define SMALL_SHIP_HULL 506 // 1 pieces required

// contract attributes
// basic contract 
long gamevoteContract = 0;
long currentFee = ONESIGNA;
long sendBuffer[8];

// advanced
long objectType = 603 // Article = ZeptorLight
long owner = 0;
long insurence = 0;
long location = 0;

// build
long shipBuildComponents[7];
long status = 0;

// specific
long defaultSpeed = 350;
long defaultStructure = 500;
long defaultArmor = 1000;
long defaultShield = 0;
long slots[5];

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
	currentTX.timestamp = get_Timestamp_For_Tx_In_A();
	currentTX.sender = getSender(currentTX.txId);
	readMessage(currentTX.txId, 0, currentTX.message);
	readMessage(currentTX.txId, 1, currentTX.message + 4);
}

void main(void) {
	do {
		A_To_Tx_After_Timestamp(currentTX.timestamp);
		if (Get_A1() == 0) {
			break;
		}
		getTxDetails();

		switch (currentTX.message[0]) {
			case ACT:
				Act();
				break;
			case BUILD:
				Build();
				break;
			case DOCK:
				Dock();
				break;	
			case EQUIP:
				
				break;
			case EXPLODE:
				
				break;
			case MINING:
				Mine();
				break;
			case REPAIR:
				
				break;
			case SCAN:
				
				break;
			case STORE:
				
				break;
			case TREAT:
				
				break;
			case WITHDRAWALING:
				// TODO: needed?
				break;
			default:
				break;
		}
    } while (true);
}


// main methods
void Act(void) {
	// TODO:
	
    // Define actions for the ship
	// currentTX.sender = gamevoteContractID or ownerID or creatorID
    // currentTX.message[0] = method (ACT)
    // currentTX.message[1] = command (build, equip, docking, mining, storing, scanning, repairing)
    // currentTX.message[2] = targetID
    // currentTX.message[3] = free
    // currentTX.message[4] = free
    // currentTX.message[5] = free
    // currentTX.message[6] = free
    // currentTX.message[7] = free
	
    // ### outgoing ###
    // message[0] = method (ACT)
    // message[1] = command (fire)
    // message[2] = parameter (weapon slot)
    // message[3] = damage
    // message[4] = free
    // message[5] = free
    // message[6] = free
    // message[7] = free
	
	/*
    setMapValue(0, slots[currentTX.message[2]], getMapValue(0, slots[currentTX.message[2]]) - 1);
    SetSendBufferForTargetContract(ACT, currentTX.message[1], currentTX.message[2], currentTX.message[3], 0, 0, 0, 0);
    SendMessageSC(currentTX.message[3]);

    sendAmount(100_0000_0000, currentTX.message[4]);
	*/
}

void Build() {
    // build the ship
    // currentTX.message[0] = method (BUILD)
    // currentTX.message[1] = parameter (System [Computer, Small Ship Hull, Small Engine, etc.])
    // currentTX.message[2] = parameter (Amount of System)
    // currentTX.message[3] = free
    // currentTX.message[4] = free
    // currentTX.message[5] = free
    // currentTX.message[6] = free
    // currentTX.message[7] = free
	
	
	
	// shipBuildComponents[0] = Amount of COCKPIT_INSTRUMENT
	// shipBuildComponents[1] = Amount of COMPUTER
	// shipBuildComponents[2] = Amount of MONITOR
	// shipBuildComponents[3] = Amount of SMALL_ENERGY_GENERATOR
	// shipBuildComponents[4] = Amount of SMALL_ENGINE
	// shipBuildComponents[5] = Amount of SMALL_EXTERNAL_SLOT
	// shipBuildComponents[6] = Amount of SMALL_SHIP_HULL
	
	long checkedComponent = CheckBuildComponent(currentTX.message[1])
	
	if(checkedComponent > 0) {
		shipBuildComponents[checkedComponent -1] = shipBuildComponents[checkedComponent -1] + currentTX.message[2];
		BuildUpShip();
	}
	
}

void Dock() {
	
}

void Equip() {

    // equip the ship
    // currentTX.message[0] = method (ACT)
    // currentTX.message[1] = command (EQUIP)
    // currentTX.message[2] = parameter (gattling20mm = ID)
    // currentTX.message[3] = parameter (slotindex = WEAPON_SLOT)
    // currentTX.message[4] = free
    // currentTX.message[5] = free
    // currentTX.message[6] = free
    // currentTX.message[7] = free	
	
	/* Key1 definition:
	 * 0 = Elements (IRON, HYDROGEN, etc.)
	 * 1 = Refined Materials (Iron Bar, Hydrogen Bottle, etc)
	 * 2 = Common Materials (Iron Gear, Fuel, Polymer, etc)
	 * 3 = Advanced Materials (Iron Case, Aluminium Frame, etc)
	 * 4 = Components (Ship Hull Part, Engine Part, Gun Part, etc.)
	 * 5 = Systems (Computer, Small Ship Hull, Small Engine, etc.)
	 * 6 = Equipment (Zeptor Light, Gattling20mm, etc.)
	 */
	
	/* Key2 definition:
	 * 0 = (Equipment)Slot (value = Internal or External)
	 */
	
	/* Value definition:
	 * 1,2,3,... = slotOfEquipment
	 */
	
	long fitSlot = GetSlotType(getExtMapValue(6, 0, currentTX.message[2]), currentTX.message[3]);
	if (fitSlot != 0) {
		slots[fitSlot] = currentTX.message[2]; // Equipment ID
	}
	
	
	
	
	
    // Set initial component quantities
    // long initialQuantities[5] = { 1, 1, 1, 1, 1 };

    // for (long i = 0; i < 5; i++) {
        // setMapValue(0, slots[i], initialQuantities[i]);
    // }
}

void Explode() {
	
}

void Mine() {
	
}

void Repair() {
	
}

void Scan() {
	
}

void Store() {
	
}

void Treat() {
	
}

// sub methods


// support methods
long GetSlotType(long slotOfEquipment, long desiredSlot) {
	
	switch(slotOfEquipment) {
		case WEAPON_SLOT_1:
		case WEAPON_SLOT_2:
			switch(desiredSlot) {
				case WEAPON_SLOT_1:
				case WEAPON_SLOT_2:
					return desiredSlot;
					break; // maybe not needed
			}
			
			break;
		case ENGINE_SLOT:
			if(desiredSlot == ENGINE_SLOT) {
				return desiredSlot;
			}
			break;
		case SHIELD_SLOT:
			if(desiredSlot == SHIELD_SLOT) {
				return desiredSlot;
			}
			break;
		case CARGO_SLOT:
			if(desiredSlot == CARGO_SLOT) {
				return desiredSlot;
			}
			break;
	}
	
	return 0;
	
}

long CheckBuildComponent(long component) {
	
	switch(component) {
		case COCKPIT_INSTRUMENT:
			return 1;
			break;
		case COMPUTER:
			return 2;
			break;
		case MONITOR:
			return 3;
			break;
		case SMALL_ENERGY_GENERATOR:
			return 4;
			break;
		case SMALL_ENGINE:
			return 5;
			break;
		case SMALL_EXTERNAL_SLOT:
			return 6;
			break;
		case SMALL_SHIP_HULL:
			return 7;
			break;
	}
	
	return 0;
	
}

void BuildUpShip() {
	
	if(shipBuildComponents[0] >= 4 && shipBuildComponents[1] >= 1 && shipBuildComponents[2] >= 1 && shipBuildComponents[3] >= 1 && shipBuildComponents[4] >= 2 && shipBuildComponents[5] >= 2 && shipBuildComponents[6] >= 1) {
		status = 1; // ship flightready
	}
	else {
		status = 0; // ship destroyed/not exist
	}
	
}


// contract methods
void SetSendBufferForTargetContract(long pollType, long command, long parameter, long sender, long executeTime, long reserve1, long reserve2, long reserve3) {
    sendBuffer[0] = pollType;
    sendBuffer[1] = command;
    sendBuffer[2] = parameter;
    sendBuffer[3] = sender;
    sendBuffer[4] = executeTime;
    sendBuffer[5] = reserve1;
    sendBuffer[6] = reserve2;
    sendBuffer[7] = reserve3;
}

void SendMessageSC(long recipient) {
    sendAmountAndMessage(currentFee, sendBuffer, recipient);
    sendMessage(sendBuffer + 4, recipient);
}

