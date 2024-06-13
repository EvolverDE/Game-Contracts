/**
 * @author evolver
 * this smart contract implements a solar system
 */

#include APIFunctions

#program name SolarSystemContract
#program description This contract serves as a actor/target contract and represent a solar system
#program activationAmount .5

#pragma maxAuxVars 3
#pragma maxConstVars 3
#pragma optimizationLevel 3
#pragma version 2.2.1

// enumeration substitute
#define ONESIGNA 100000000

// contract methods
#define DEPOSITING 1
#define WITHDRAWALING 2

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
#define ACTOR 12
#define TARGET 13
#define SUBMETHOD 14
#define PARAMETER 15

// Metals(17)
#define IRON 26
#define ALUMINIUM 13
#define TITAN 22
#define COPPER 29
#define TIN 50
#define SILVER 47
#define GOLD 79
#define COBALT 27
#define LEAD 82
#define PLATINUM 78
#define CHROME 24
#define MERCURY 80
#define WOLFRAM 74
#define URANIUM 92
#define NEODYM 60
#define DYSPROSIUM 66
#define TANTALUM 73

// Semimetals(5)
#define SILICON 14
#define LITHIUM 3
#define CALCIUM 20
#define SODIUM 11
#define STRONTIUM 38

// Nonmetals(8)
#define HYDROGEN 1
#define HELIUM 2
#define OXYGEN 8
#define CARBON 6
#define NITROGEN 7
#define NEON 10
#define SULFUR 16
#define XENON 54

long metals[17];
long semimetals[5];
long nonmetals[8];

long quantities[30];
long gamevoteContract = 0;
long sendBuffer[8];
long currentFee = ONESIGNA;

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
	currentTX.timestamp = Get_Timestamp_For_Tx_In_A();
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
			case DEPOSITING:
				GenerateElements();
				break;
			case ACT:
				Act();
				break;
			case BUILD:
				// so you want to buildup an entire solar system, huh?
				break;
			case DOCK:
				// what are you thinking of docking an entire solar system to?
				break;	
			case EQUIP:
				// there is much room for a dysonsphere...NOT! TODO: maybe for stations?
				break;
			case EXPLODE:
				// are you thinking about a supernova?
				break;
			case MINING:
				// TODO: well, you can actually mine the resources of a solar system, yes.
				break;
			case REPAIR:
				// lets hope its not to late for the earth...
				break;
			case SCAN:
				// solar systems are indifferent to everything...
				break;
			case STORE:
				// TODO: how big is the cargo of a solar system? i think there is much room for stations!
				break;
			case TREAT:
				// maybe one day the solar system collide/handle with another one... but not in this game universe
				break;
			default:
				break;
        }
		
    } while (true);
}

void GenerateElements() {
	
	//metals
	metals[0] = IRON;
	metals[1] = ALUMINIUM;
	metals[2] = TITAN;
	metals[3] = COPPER;
	metals[4] = TIN;
	metals[5] = SILVER;
	metals[6] = GOLD;
	metals[7] = COBALT;
	metals[8] = LEAD;
	metals[9] = PLATINUM;
	metals[10] = CHROME;
	metals[11] = MERCURY;
	metals[12] = WOLFRAM;
	metals[13] = URANIUM;
	metals[14] = NEODYM;
	metals[15] = DYSPROSIUM;
	metals[16] = TANTALUM;
	
	//semimetals
	semimetals[0] = SILICON;
	semimetals[1] = LITHIUM;
	semimetals[2] = CALCIUM;
	semimetals[3] = SODIUM;
	semimetals[4] = STRONTIUM;
	
	//nonmetals
	nonmetals[0] = HYDROGEN;
	nonmetals[1] = HELIUM;
	nonmetals[2] = OXYGEN;
	nonmetals[3] = CARBON;
	nonmetals[4] = NITROGEN;
	nonmetals[5] = NEON;
	nonmetals[6] = SULFUR;
	nonmetals[7] = XENON;
	
	long matCount = metals.length + semimetals.length + nonmetals.length;

	long metPrevalence = CalculatePrevalence(metals.length);
	long semiPrevalence = CalculatePrevalence(semimetals.length);
	long nonPrevalence = CalculatePrevalence(nonmetals.length);

	long metCountdown = metPrevalence;
	long semiCountdown = semiPrevalence;
	long nonCountdown = nonPrevalence;

	long metIndex = 0;
	long semiIndex = 0;
	long nonIndex = 0;
	
	long selectionIndex = 0;
	long selectionList[30];
	
	long switcher = 2;

	while (metIndex < metals.length || semiIndex < semimetals.length || nonIndex < nonmetals.length) {
		
		switch(switcher) {
			case 0:
				if(metCountdown > 0)
				{
					if (metIndex < metals.length)
					{
						selectionList[selectionIndex] = metals[metIndex];
						selectionIndex++;
						metIndex++;
					}
					
					metCountdown--;
				}
				else
				{
					metCountdown = metPrevalence;
					switcher = 2;
				}
				break;
			case 1:
				if (semiCountdown > 0)
				{
					if (semiIndex < semimetals.length)
					{
						selectionList[selectionIndex] = semimetals[semiIndex];
						selectionIndex++;
						semiIndex++;
					}
					
					semiCountdown--;
				}
				else
				{
					semiCountdown = semiPrevalence;
					switcher = 0;
				}
				break;
			case 2:
				if (nonCountdown > 0)
				{
					if (nonIndex < nonmetals.length)
					{
						selectionList[selectionIndex] = nonmetals[nonIndex];
						selectionIndex++;
						nonIndex++;
					}
					
					nonCountdown--;
				}
				else
				{
					nonCountdown = nonPrevalence;
					switcher = 1;
				}
				break;
		}
	}

	CalculateElementQuantities(getCurrentBalance()); // currentTX.amount

	for (long i = 0; i < quantities.length; i++)
	{
		if (i >= selectionList.length) {
			break;
		}
		// 0 = Elements, ElementOrderNumber, amount
		setMapValue(0, selectionList[i], quantities[i]);
	}
	
}

void Act(void) {
	// ### income ###
	// currentTX.sender = gamevoteContract or creatorID
	// currentTX.message[0] = contract method (ACT)
	// currentTX.message[1] = sub method (mining)
	// currentTX.message[2] = parameter (123)
	// currentTX.message[3] = endTargetContractID (to process sub method(parameter))
	// currentTX.message[4] = providerID (for reward)
	// currentTX.message[5] = timestamp (vote timeout in the future)
	// currentTX.message[6] = free
	// currentTX.message[7] = free
	
	
	// ### outgoing ###
	// recipientID = endTargetContractID (to process sub method(parameter))
	// message[0] = contract method (ACT)
	// message[1] = sub method (store)
	// message[2] = parameter (IRON)
	// message[3] = parameter (123)
	// message[4] = free
	// message[5] = free
	// message[6] = free
	// message[7] = free
	
	setMapValue(0, currentTX.message[2], getMapValue(0, currentTX.message[2]) - currentTX.message[3]);
	SetSendBufferForTargetContract(ACT, currentTX.message[1], currentTX.message[2], currentTX.message[2], 0, 0, 0, 0);
	SendMessageSC(currentTX.message[1]);
	
	// send reward for contract providing
	sendAmount(100_0000_0000, currentTX.message[4]);
	
}

void CalculateElementQuantities(long signaAmount) {
	quantities[] = 0;
	long i = 0;
	while(signaAmount > 0)
	{
		quantities[i++] = signaAmount;
		signaAmount /= 3;
	}
	//return quantities;
}

long CalculatePrevalence(long toRound) {
	
	long tenth = toRound / 10;
	tenth = tenth * 10;

	long modth = toRound % 10;

	if (modth >= 5)	{
		tenth += 10;
	}

	if(tenth == 0) {
		tenth = 10;
	}

	return tenth / 10;
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

void SendMessageSC(long recipient) {
	sendAmountAndMessage(currentFee, sendBuffer, recipient);
	sendMessage(sendBuffer + 4, recipient);
}
