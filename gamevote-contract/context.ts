import {join} from 'path';

export const Context = {
    ContractPath: join(__dirname + '/gamevote-contract.smart.c'),
    SenderAccount1: 10n,
    SenderAccount2: 20n,
    SenderAccount3: 30n,
    SenderAccount4: 40n,
    SenderAccount5: 50n,
    SenderAccount6: 60n,
    SenderAccount7: 70n,
    SenderAccount8: 80n,
    SenderAccount9: 90n,
    SenderAccount10: 100n,
    CreatorAccount: 555n,
    ThisContract: 999n,
    ActivationFee: 500_0000n,
    Methods: {
        Deposit: 1n,
        Act: 2n,
        Withdraw: 3n,
        VoteForPoll: 4n
    },
    MapKeys2: {
        Reject: 5n,
        Deposit: 6n,
        Action: 7n,
        VotePoints: 8n,
        Withdrawal: 9n,
    }
}
