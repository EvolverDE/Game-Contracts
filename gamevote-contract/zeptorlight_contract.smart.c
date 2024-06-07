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
