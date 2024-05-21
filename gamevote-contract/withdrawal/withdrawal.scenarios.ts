import {Context} from '../context';
import {TransactionObj} from 'signum-smartc-testbed';

export const Withdrawal: TransactionObj[] = [
    { blockheight: 2, sender: Context.SenderAccount1, recipient: Context.ThisContract, amount: 100_5000_0000n, messageArr: [Context.Methods.Deposit] },
    { blockheight: 4, sender: Context.SenderAccount2, recipient: Context.ThisContract, amount: 100_5000_0000n, messageArr: [Context.Methods.Deposit] },
    { blockheight: 6, sender: Context.SenderAccount3, recipient: Context.ThisContract, amount: 100_5000_0000n, messageArr: [Context.Methods.Deposit] },
    { blockheight: 8, sender: Context.SenderAccount4, recipient: Context.ThisContract, amount: 100_5000_0000n, messageArr: [Context.Methods.Deposit] },
    { blockheight: 10, sender: Context.SenderAccount5, recipient: Context.ThisContract, amount: 100_5000_0000n, messageArr: [Context.Methods.Deposit] },
    { blockheight: 12, sender: Context.SenderAccount6, recipient: Context.ThisContract, amount: 100_5000_0000n, messageArr: [Context.Methods.Deposit] },
    { blockheight: 14, sender: Context.SenderAccount7, recipient: Context.ThisContract, amount: 100_5000_0000n, messageArr: [Context.Methods.Deposit] },
    { blockheight: 16, sender: Context.SenderAccount8, recipient: Context.ThisContract, amount: 100_5000_0000n, messageArr: [Context.Methods.Deposit] },
    { blockheight: 18, sender: Context.SenderAccount9, recipient: Context.ThisContract, amount: 100_5000_0000n, messageArr: [Context.Methods.Deposit] },
    { blockheight: 20, sender: Context.SenderAccount10, recipient: Context.ThisContract, amount: 100_5000_0000n, messageArr: [Context.Methods.Deposit] },

    { blockheight: 24, sender: Context.SenderAccount1, recipient: Context.ThisContract, amount: 5000_0000n, messageArr: [Context.Methods.Withdraw] },

    { blockheight: 30, sender: Context.SenderAccount2, recipient: Context.ThisContract, amount: 21_5000_0000n, messageArr: [Context.Methods.VoteForPoll, Context.SenderAccount1] },
    { blockheight: 35, sender: Context.SenderAccount3, recipient: Context.ThisContract, amount: 21_5000_0000n, messageArr: [Context.Methods.VoteForPoll, Context.SenderAccount1] },
    { blockheight: 40, sender: Context.SenderAccount4, recipient: Context.ThisContract, amount: 21_5000_0000n, messageArr: [Context.Methods.VoteForPoll, Context.SenderAccount1] },
    { blockheight: 45, sender: Context.SenderAccount5, recipient: Context.ThisContract, amount: 21_5000_0000n, messageArr: [Context.Methods.VoteForPoll, Context.SenderAccount1] },
    { blockheight: 50, sender: Context.SenderAccount6, recipient: Context.ThisContract, amount: 21_5000_0000n, messageArr: [Context.Methods.VoteForPoll, Context.SenderAccount1] },
    { blockheight: 55, sender: Context.SenderAccount7, recipient: Context.ThisContract, amount: 21_5000_0000n, messageArr: [Context.Methods.VoteForPoll, Context.SenderAccount1] }
]