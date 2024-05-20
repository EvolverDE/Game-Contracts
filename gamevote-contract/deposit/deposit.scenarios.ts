import {Context} from '../context';
import {TransactionObj} from 'signum-smartc-testbed';

export const Deposit: TransactionObj[] = [
    {
        blockheight: 2,
        amount: 20_0000_0000n,
        sender: Context.SenderAccount1,
        recipient: Context.ThisContract,
        messageArr: [Context.Methods.Deposit]
    },
]