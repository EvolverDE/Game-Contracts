/**
 * @author evolver
 * this contract implements a item database contract
 * features:
 * - store item properties and inventions
 * still in developement
 */
 
#include APIFunctions

#program name ItemBaseContract
#program description This contract serves as a database for item properties and inventions
#program activationAmount .5

#pragma maxAuxVars 3
#pragma maxConstVars 3
#pragma optimizationLevel 3
#pragma version 2.2.1

// enumeration substitute
const long ONE_WHOLE = 100000000;

// contract methods
#define CONTRACT_SPECIFIC 1_000_001
#define GAME_SPECIFIC 1_000_002

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
#define REFINE = 1_002_011
#define REPAIR = 1_002_012
#define SCAN = 1_002_013
#define STORE = 1_002_014
#define TREAT = 1_002_015

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

// item base specific
// item properties
#define SLOTS  1_005_012
#define HANGARS 1_005_014

// contract attributes
// basic contract
long authIDs[10];
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
	currentPOLL.actorID = 0;
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
        //currentPOLL.providerID = getExtMapValue(PROVIDER_ID, hashValue, pollSaveID);
        //currentPOLL.pollAmount = getExtMapValue(DEPOSITMENT, currentPOLL.providerID, pollSaveID);
        //currentPOLL.actorID = getExtMapValue(ACTOR_ID, hashValue, pollSaveID);
        //currentPOLL.targetID = getExtMapValue(TARGET_ID, hashValue, pollSaveID);
        currentPOLL.mainMethod = getExtMapValue(MAINMETHOD, hashValue, pollSaveID);
        currentPOLL.subMethod = getExtMapValue(SUBMETHOD, hashValue, pollSaveID);
        currentPOLL.parameter = getExtMapValue(PARAMETER, hashValue, pollSaveID);
        currentPOLL.parameter2 = getExtMapValue(PARAMETER2, hashValue, pollSaveID);
        currentPOLL.parameter3 = getExtMapValue(PARAMETER3, hashValue, pollSaveID);
        currentPOLL.parameter4 = getExtMapValue(PARAMETER4, hashValue, pollSaveID);
    }

    //currentPOLL.partOfPoll = currentTX.message[1];

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
    authIDs[currentPOLL.parameter] = currentPOLL.actorID;
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
        case DESCRIBE:
            Describe();
            break;
        case INVENT:
            Invent();
            break;
        default:
            break;
    }

}

// sub methods
void Describe(void)
{
    /* Set Properties of a object
     * currentPOLL.mainMethod = GAME_SPECIFIC
     * currentPOLL.subMethod = ACT
     * currentPOLL.parameter = property (DEFAULTSPEED, DEFAULTSTRUCTURE, ...)
     * currentPOLL.parameter2 = itemName (ZeptoLit)
     * property (DEFAULTSPEED):
     * currentPOLL.parameter3 = value (350 m/s)
     * property (DEFAULTSTRUCTURE):
     * currentPOLL.parameter3 = value (500 hitpoints)
     * property (DEFAULTARMOR):
     * currentPOLL.parameter3 = value (1000 hitpoints)
     * property (DEFAULTSHIELD):
     * currentPOLL.parameter3 = value (0 hitpoints)
     * property (DEFAULTCARGO):
     * currentPOLL.parameter3 = value (6 m3)
     * property (SLOTS):
     * currentPOLL.parameter3 = value (5 amount)
     * property (HANGAR):
     * currentPOLL.parameter3 = value (1 amount)
     */

    switch (currentPOLL.parameter)
    {
        case SLOTS:
        case HANGARS:

            if (currentPOLL.parameter4 == 0)
            {
                long nestingsHash = GetB3FromHash256(currentPOLL.parameter, currentPOLL.parameter2, currentPOLL.parameter3, 0);

                setMapValue(currentPOLL.parameter, currentPOLL.parameter2, nestingsHash);
                setMapValue(nestingsHash, 0, currentPOLL.parameter3);
            }
            else
            {

                long nestingAmount = getMapValue(currentPOLL.parameter2, 0);

                if (currentPOLL.parameter3 > nestingAmount || currentPOLL.parameter3 < 1)
                {
                    /* Set Nesting Properties
                     * currentPOLL.mainMethod = GAME_SPECIFIC
                     * currentPOLL.subMethod = ACT
                     * currentPOLL.parameter = property (SLOTS/HANGAR)
                     * currentPOLL.parameter2 = nestingHash
                     * currentPOLL.parameter3 = nestingProperty (SIZE, TYPE)
                     * currentPOLL.parameter4 = value (SIZE: 1=small; 2=medium; 3=heavy / TYPE: INTERNAL; WEAPON; DRONE)
                     */

                    setMapValue(currentPOLL.parameter3, currentPOLL.parameter2, currentPOLL.parameter4);

                }
                else
                {
                    /* Set Nesting Properties
                     * currentPOLL.mainMethod = GAME_SPECIFIC
                     * currentPOLL.subMethod = ACT
                     * currentPOLL.parameter = property (SLOTS/HANGAR)
                     * currentPOLL.parameter2 = nestingsHash
                     * currentPOLL.parameter3 = nestingNumber (1...n)
                     * currentPOLL.parameter4 = slotName (SmaExSlo = 6011568028608851055)
                     */

                    //long nestingHash = GetB3FromHash256(currentPOLL.parameter, currentPOLL.parameter2, currentPOLL.parameter3, 0);
                    setMapValue(currentPOLL.parameter2, currentPOLL.parameter3, currentPOLL.parameter4);
                }
            }

            break;
        default:
            setMapValue(currentPOLL.parameter, currentPOLL.parameter2, currentPOLL.parameter3);
            break;
    }
}
void Invent(void)
{
    /* Set build needs of object
     * currentPOLL.mainMethod = GAME_SPECIFIC
     * currentPOLL.subMethod = INVENT
     * currentPOLL.parameter = main (ZeptoLit)
     * currentPOLL.parameter2 = sub (Computer)
     * currentPOLL.parameter3 = subAmount (1)
     * currentPOLL.parameter4 = subIndex (0...n)
     */

    //getInventDetails();

    long count = getMapValue(INVENT, currentPOLL.parameter);

    if (count == 0)
    {
        setMapValue(INVENT, currentPOLL.parameter, 1);
        setMapValue(currentPOLL.parameter, 1, currentPOLL.hash);
    }
    else
    {
        if(currentPOLL.parameter4 > count)
        {
            setMapValue(INVENT, currentPOLL.parameter, currentPOLL.parameter4);
        }

        if (currentPOLL.parameter2 == 0)
        {
            setMapValue(INVENT, currentPOLL.parameter, CleanUpMap(currentPOLL.parameter, currentPOLL.parameter4));
        }
        else
        {
            setMapValue(currentPOLL.parameter, currentPOLL.parameter4, currentPOLL.hash);
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

long GetB3FromHash256(long a1, long a2, long a3, long a4)
{
    Set_A1_A2(a1, a2);
    Set_A3_A4(a3, a4);
    SHA256_A_To_B();
    return Get_B3();
}

long CleanUpMap(long key1, long itemIndex)
{
    long finished = 1;
    do
    {
        finished = 1;
        long nextItem = getMapValue(key1, itemIndex + 1);
        if (nextItem != 0)
        {
            setMapValue(key1, itemIndex, nextItem);
            itemIndex++;
            finished = 0;
        }
        else
        {
            setMapValue(key1, itemIndex, 0);
        }

    } while (finished == 0);

    return itemIndex;
}
