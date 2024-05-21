import {expect, test, describe} from 'vitest'
import {SimulatorTestbed} from 'signum-smartc-testbed';
import {Context} from '../context';
import {Withdrawal} from './withdrawal.scenarios';

describe('Game Vote Contract - Withdrawal', () => {
    test('should withdraw as expected', () => {
        const testbed = new SimulatorTestbed(Withdrawal)
            .loadContract(Context.ContractPath)
            .runScenario();
            expect(testbed.getContractMapValue(Context.SenderAccount1, Context.MapKeys2.Deposit)).toEqual(0n)
            expect(testbed.getContractMapValue(0n, 0n)).toEqual(Context.SenderAccount1)
    })
})
