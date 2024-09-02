/**
 * @author evolver
 * this smart contract implements a solar system
 */

#include APIFunctions

#program name SolarSystemContract
#program description This contract serves as a actor/target contract and represent a solar system
#program activationAmount 1.6

#pragma maxAuxVars 3
#pragma maxConstVars 3
#pragma optimizationLevel 3
#pragma version 2.2.1

// enumeration substitute
const long ONE_WHOLE = 100000000;

// contract methods
#define CONTRACT_SPECIFIC 1_000_001
#define GAME_SPECIFIC 1_000_002

// sub CONTRACT_SPECIFIC methods
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

// item tree
#define ELEMENT 1_100_000

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

// contract attributes
// basic contract
long firstStart = 1;
long maxQuantity = 0;
long authIDs[10];
long sendBuffer[8];
long currentFee = ONE_WHOLE;

// specific
long metals[17];
long semimetals[5];
long nonmetals[8];
long quantities[30];

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

constructor();

void constructor(void) {
    // this function will be called only once on first activation.
	authIDs[0] = 1111; //TODO: set initial ID
	currentPOLL.hash = 0;
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
            GenerateElements();
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
                getPollDetails(currentTX.message[0], currentTX.sender);
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
    * currentPOLL.subMethod = SETOWNER
    * currentPOLL.parameter = 
    * currentPOLL.parameter2 = 
    * currentPOLL.parameter3 = 
    * currentPOLL.parameter4 = 
    */

    switch (currentPOLL.subMethod)
    {
        case AUTHENTICATION:
            authIDs[currentPOLL.parameter] = currentPOLL.actorID;
            break;
    }
}
void GenerateElements(void)
{

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

    while (metIndex < metals.length || semiIndex < semimetals.length || nonIndex < nonmetals.length)
    {

        switch (switcher)
        {
            case 0:
                if (metCountdown > 0)
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

    CalculateElementQuantities(Get_Current_Balance()); // currentTX.amount

    for (long i = 0; i < quantities.length; i++)
    {
        if (i >= selectionList.length)
        {
            break;
        }
        // 1_000_000 = Elements, ElementOrderNumber, amount
        setMapValue(ELEMENT, selectionList[i], quantities[i]);
    }

	// by first run, create station with given currentTX.message[0] as contractID of station
    if (firstStart == 1 && getCodeHashOf(currentTX.message[0]) != 0)
    {
        firstStart = 0;
        CreateStartStation(currentTX.message[0]);
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
        case MINING:
            // TODO: well, you can actually mine the resources of a solar system, yes.
            Mining();
            break;
        default:
            break;
    }

}

// sub methods

void CreateStartStation(long startStationID)
{
    SetSendBufferForTargetContract(CONTRACT_SPECIFIC, CREATE_STATION, 0, 0, 0, 0, 0, 0);
    SendBufferWithAmount(ONE_WHOLE, startStationID);
}
void Mining(void)
{
    setMapValue(ELEMENT, currentPOLL.parameter, getMapValue(ELEMENT, currentPOLL.parameter) - currentPOLL.parameter2);
    maxQuantity -= currentPOLL.parameter2;
    
    SetSendBufferForTargetContract(GAME_SPECIFIC, MINING, currentPOLL.parameter, currentPOLL.parameter2, 0, 0, currentPOLL.actorID, currentPOLL.parameter4);
    SendBufferWithAmount(ONE_WHOLE, currentPOLL.parameter4);
    
    // 123 * (12369_2500_0000 * 10000 / 18517_4999_9866) * 10000 - 1_0000_0000 = ~81_1517_0000
    sendAmount(currentPOLL.parameter2 * (Get_Current_Balance() * 10000 / maxQuantity) * 10000 - currentFee, currentPOLL.providerID);

}

// support methods
void CalculateElementQuantities(long signaAmount)
{
    quantities[30];
    long i = 0;
    while (signaAmount > 0)
    {
        quantities[i++] = signaAmount;
        maxQuantity += signaAmount;
        signaAmount /= 3;
    }
    //return quantities;
}
long CalculatePrevalence(long toRound)
{

    long tenth = toRound / 10;
    tenth = tenth * 10;

    long modth = toRound % 10;

    if (modth >= 5)
    {
        tenth += 10;
    }

    if (tenth == 0)
    {
        tenth = 10;
    }

    return tenth / 10;
}
long IsAuthenticated(long sender)
{

    for (long i = 0; i < authIDs.length; i++)
    {
        if (authIDs[i] == sender)
        {
            return 1;
        }
    }

    return 0;
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
