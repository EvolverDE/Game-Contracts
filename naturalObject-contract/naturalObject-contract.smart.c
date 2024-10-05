/**
 * @author evolver
 * this smart contract implements a natural object contract
 * features:
 * - only responds to authenticated IDs (like gameVote-contract as initial contract)
 * - represents the data structure of an natural in-game object
 * - interact with different game logic methods (like mining and scan)
 * - interacts with other game contracts (like artificial object contracts)
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
#define GAMEVOTE_CONTRACT 1_001_000
//#define ITEMBASE_CONTRACT 1_001_001
#define LOCATION_CONTRACT 1_001_002
#define TEMPAUTH_CONTRACT 1_001_003
//#define SET_ITEMBASE 1_001_004
//#define SET_OWNER 1_001_005
//#define SET_NFT_CONTRACT 1_001_006
#define AUTHENTICATE 1_001_007
#define CREATE_STATION 1_001_008

// sub GAME_SPECIFIC methods 
//#define ACT 1_002_001
//#define BUILD 1_002_002
//#define DELIVER 1_002_003
//#define DESCRIBE 1_002_004
//#define DOCK 1_002_005
//#define EQUIP 1_002_006
//#define EXPLODE 1_002_007
//#define INSURE 1_002_008
//#define INVENT 1_002_009
#define MINING 1_002_010
//#define REFINE 1_002_011
//#define REPAIR 1_002_012
#define SCAN 1_002_013
//#define STORE 1_002_014
//#define TREAT 1_002_015

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
//#define SIZE 1_005_003
//#define PROPERTIES 1_005_004
//#define DEFAULT_SPEED 1_005_005
//#define DEFAULT_STRUCTURE 1_005_006
//#define DEFAULT_ARMOR 1_005_007
//#define DEFAULT_SHIELD 1_005_008
//#define DEFAULT_CARGO 1_005_009
//#define COCKPITS 1_005_010
//#define SLOT 1_005_011
//#define SLOTS 1_005_012
//#define HANGAR 1_005_013
//#define HANGARS 1_005_014

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

//#define SHIP 1_006_017
//#define STATION 1_006_018
#define ASTEROID 1_006_019
//#define MOON 1_006_020
#define PLANET 1_006_021
//#define STAR 1_006_022

// slot and hangar types
#define INTERNAL 1_007_001
#define EXTERNAL 1_007_002

// slot types
#define AMMO_SLOT 1_007_003
#define GUN_SLOT 1_007_004
#define TURRET_SLOT 1_007_005
#define PULPIT_SLOT 1_007_006
#define LAUNCHER_SLOT 1_007_007
#define ENGINE_SLOT 1_007_008
#define SHIELD_SLOT 1_007_009
#define CARGO_SLOT 1_007_010
#define SCANNER_SLOT 1_007_011
#define FACILITY_SLOT 1_007_012

// hangar types
#define DRONE_HANGAR 1_007_013
#define SHIP_HANGAR 1_007_014
#define STATION_HANGAR 1_007_015

// sizes and weights
//#define SMALL 1_008_001
//#define MEDIUM 1_008_002
//#define LARGE 1_008_003
//#define XLARGE 1_008_004
//#define CAPITAL 1_008_005

//#define LIGHT 1_008_006
//#define HEAVY 1_008_007

// item tree
#define ELEMENT 1_100_000
//#define REFINED 1_100_001
//#define COMMON 1_100_002
//#define ADVANCED 1_100_003
//#define COMPONENT 1_100_004
//#define SYSTEM 1_100_005
//#define ARTICLE 1_100_006

// artificialObject contract specific
//#define OWNER 1_009_001
#define STATUS 1_009_002
//#define AMOUNT 1_009_003

// extContract flags
//#define ACTOR 1_010_001
//#define TARGET 1_010_002

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
long sendBuffer[8];
long currentFee = ONE_WHOLE;

// advanced
long status = 0;
long gameVoteContract = 0;
long locationContract = 0;
long objectName = 0; // Article = sol00001
long objectType = 0; // solarsystem; asteroid; ...

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
	//TODO: set initial ID
	currentPOLL.hash = 0;
	gameVoteContract = 1111;
	status = 0;
	setMapValue(GAMEVOTE_CONTRACT, 0, 1111);
	setMapValue(STATUS, 0, 0);
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

    currentPOLL.partOfPoll = currentTX.message[7];

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
            if(currentTX.amount > 1000_0000_0000)
			{
				GenerateElements();
			}
            break;
        }
        else
        {
             if (getCodeHashOf(currentTX.sender) != 0)
			 {
				 // currentTX.sender is a contract

				 if (currentTX.message[0] == HASH)
				 {
					 getPollDetails(currentTX.message[1], gameVoteContract);
				 }
				 else if (firstStart == 1 && currentTX.message[0] == 18657918865985095) // 18657918865985095 = BIGBANG
				 {
					 long station = getExtMapValue(currentTX.message[0], 0, currentTX.sender);
					 if (getCodeHashOf(station) != 0)
					 {
						 objectType = currentTX.message[1];
						 status = 1;
						 firstStart = 0;
						 if (objectType == ASTEROID)
						 {
							 locationContract = currentTX.message[2];
							 setMapValue(LOCATION_CONTRACT, 0, locationContract);

							 SetSendBufferForTargetContract(CONTRACT_SPECIFIC, CREATE_STATION, 0, 0, 0, 0, 0, 0);
							 SendBufferWithAmount(ONE_WHOLE, station);

						 }
						 setMapValue(STATUS, 0, 1);
						 setMapValue(TYPE, 0, objectType);
					 }
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
    * currentPOLL.subMethod = SETOWNER
    * currentPOLL.parameter = 
    * currentPOLL.parameter2 = 
    * currentPOLL.parameter3 = 
    * currentPOLL.parameter4 = 
    */

    switch (currentPOLL.subMethod)
    {
        case TYPE:
			objectName = currentPOLL.parameter;
			objectType = PLANET;
			break;
		case AUTHENTICATE:
			setMapValue(TEMPAUTH_CONTRACT, currentPOLL.actorID, SetTimeOut(16));
			break;
		case LOCATION_CONTRACT:
			locationContract = currentPOLL.parameter;
			setMapValue(LOCATION_CONTRACT, 0, currentPOLL.parameter);
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
		case SCAN:
			Scan();
			break;
        default:
            break;
    }

}

// sub methods

void Mining(void)
{
     if(status == 1)
	 {
		 long available = getMapValue(ELEMENT, currentPOLL.parameter);
		 long miningQuantity = currentPOLL.parameter2 / ONE_WHOLE;
		 if (available >= miningQuantity)
		 {
			 setMapValue(ELEMENT, currentPOLL.parameter, available - miningQuantity);
			 maxQuantity -= miningQuantity;
	 
			 SetSendBufferForTargetContract(HASH, currentPOLL.hash, 0, 0, 0, 0, 0, currentPOLL.partOfPoll);
			 SendBufferWithAmount(ONE_WHOLE, currentPOLL.parameter4);
	 
			 // 123 * (12369_2500_0000 * 10000 / 18517_4999_9866) * 10000 - 1_0000_0000 = ~81_1517_0000
			 sendAmount(currentPOLL.parameter2 * (Get_Current_Balance() * 10000 / maxQuantity) * 10000 - currentFee, currentPOLL.providerID);
		 }
	 }
}
void Scan()
{
	if (status == 0 && currentTX.sender == gameVoteContract)
	{
		setMapValue(TEMPAUTH_CONTRACT, currentPOLL.targetID, SetTimeOut(15));

		status = 1;
		setMapValue(STATUS, 0, 1);
	}
	else if (status == 1 && IsAuthenticated(currentTX.sender) == 1)
	{
		locationContract = getExtMapValue(LOCATION_CONTRACT, 0, currentTX.sender);
		setMapValue(LOCATION_CONTRACT, 0, locationContract);

	}
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
	if (sender == gameVoteContract || GetTimeIsUp(getMapValue(TEMPAUTH_CONTRACT, sender)) == 0)
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
