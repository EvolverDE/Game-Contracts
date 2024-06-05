# Game Vote Contract

This smart contract ...

|                    |                         |
|--------------------|-------------------------|
| Code Hash          |                         |
| Testnet Account Id |                         |
| Mainnet Account Id |                         |

## Data Stack

| Name                | Stack Index (0-based) | Initializable | Values | Description |
|---------------------|-----------------------|---------------|--------|-------------|
| contractProvider    | 4                     | no            |        | <TODO>      |
| maxGlobalVotePoints | 5                     | no            |        | <TODO>      |

## Methods:

| Method        | Arg0 | Arg1         | Arg2    | Arg3      | Role/Permission | Description                |
|---------------|------|--------------|---------|-----------|-----------------|----------------------------|
| Deposit       | 1    | Recipient Id |         |           | Everybody       | Deposits Signa on contract |
| Act           | 2    | Method       | Command | Parameter | one in the map  | initiate a act poll        |
| Withdraw      | 3    | 2nd Key      | Value   |           | one in the map  | initiate a withdrawal poll |
| Vote for Poll | 4    | PollsterID   | Vote    |           | one in the map  | vote for a initiated poll  |


### Act

Deposits Signa on Contract (maybe maximum of 100 signa?)

### Withdrawal

Initiates an Act Poll to deregister from contract and get the deposit back

### Vote for Poll

to vote a poll, one in the map has to pay the votefee and set the parameter to PollsterID and vote

### Mathematics
- voteTarget = depositFromPollsterID / maxProviders / 50%
- voteTarget = 100 signa / 10 providers / 2(50%) = 20 signa
- voters votePoints increments
- voteFee == voteTarget -> accept, and the voteFee will be send to targetContract if entire Poll is accepted
- voteFee != voteTarget -> reject, and the voteFee will increase voters deposit by voteFeeValue


## Maps

Initiates an Act Poll to induce changes on a target contract

| Description              | Key 1                              | Key 2                              | Value                                   |
|--------------------------|------------------------------------|------------------------------------|-----------------------------------------|
| iteration of providerIDs | 0 (incr int)                       | 0 (incr int)                       | providerID                              |
| deposit of providerID    | providerID                         | 1(DEPOSIT)                         | 100(signa)                              |
| poll of providerID       | providerID(pollCreator/PollsterID) | 2(ACTION), 3(WITHDRAWAL)           | 0(no poll active) or > 0(poll active)   |
| vote for poll            | 0 (incr int)                       | providerID(pollCreator/PollsterID) | vote(match or not)                      |
| prevent double voting    | voterID                            | providerID(pollCreator/PollsterID) | vote(alreadyVoted != 0; notVoted == 0)  |
| votePoints of voterID    | voterID                            | 4(VOTEPOINTS)                      | 0(int)                                  |

