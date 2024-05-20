import {join} from 'path';

export const Context = {
    ContractPath: join(__dirname + '/gamevote-contract.smart.c'),
    SenderAccount1: 10n,
    SenderAccount2: 20n,
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
        Reject: 0n,
        Deposit: 1n,
        Action: 2n,
        VotePoints: 3n,
        Withdrawal: 4n,
    }
}
