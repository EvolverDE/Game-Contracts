import {expect, test, describe} from 'vitest'
import {SimulatorTestbed} from 'signum-smartc-testbed';
import {Context} from '../context';
import {Deposit} from './deposit.scenarios';

describe('Game Vote Contract - Deposit', () => {
    test('should deposit as expected', () => {
        const testbed = new SimulatorTestbed(Deposit)
            .loadContract(Context.ContractPath)
            .runScenario();
        const deposit = testbed.getContractMapValue(Context.SenderAccount1, Context.MapKeys2.Deposit);
        const indexedAccount = testbed.getContractMapValue(0n, 0n);
        expect(deposit).toEqual(19_5000_0000n)
        expect(indexedAccount).toEqual(Context.SenderAccount1)
    })
})
