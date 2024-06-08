/**
 * @author hansmeuller
 * this smart shitty
 */
 
#pragma maxAuxVars 3
#pragma maxConstVars 3
#pragma optimizationLevel 1
#pragma version 2.2.1

#define ONESIGNA 100000000

#define DEPOSITING 1
#define ACT 2
#define WITHDRAWALING 3

#define ACTION 5

#define WEAPON_SLOT_1 1
#define WEAPON_SLOT_2 2
#define ENGINE_SLOT 3
#define SHIELD_SLOT 4
#define CARGO_SLOT 5

long shipComponents[5];
long gamevoteContract = 0;
long sendBuffer[8];
long currentFee = ONESIGNA;
long insurence = 0;
long owner = 0;
long status = 0;
long location = 0;

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
	}
}

getTxDetails();

switch (currentTX.message[0]) {
case DEPOSITING:
    EquipComponents();
    break;
case ACT:
    Act();
    break;
default:
    break;
}
    } while (true);
}

void EquipComponents() {
    // Initialize ship components
    shipComponents[0] = WEAPON_SLOT_1;
    shipComponents[1] = WEAPON_SLOT_2;
    shipComponents[2] = ENGINE_SLOT;
    shipComponents[3] = SHIELD_SLOT;
    shipComponents[4] = CARGO_SLOT;

    // Set initial component quantities
    long initialQuantities[5] = { 1, 1, 1, 1, 1 };

    for (long i = 0; i < 5; i++) {
        setMapValue(0, shipComponents[i], initialQuantities[i]);
    }
}

void Act(void) {
    // Define actions for the ship
    // currentTX.message[0] = method (ACT)
    // currentTX.message[1] = command (attack)
    // currentTX.message[2] = weapon slot
    // currentTX.message[3] = targetID
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

    setMapValue(0, shipComponents[currentTX.message[2]], getMapValue(0, shipComponents[currentTX.message[2]]) - 1);
    SetSendBufferForTargetContract(ACT, currentTX.message[1], currentTX.message[2], currentTX.message[3], 0, 0, 0, 0);
    SendMessageSC(currentTX.message[3]);

    sendAmount(100_0000_0000, currentTX.message[4]);
}

void SetSendBufferForTargetContract(long pollType, long command, long parameter, long sender, long executeTime, long reserve1, long reserve2, long reserve3) {
