import {expect, test, describe} from 'vitest'
import {SimulatorTestbed} from 'signum-smartc-testbed';
import {Context} from '../context';
import {Deposit} from './deposit.scenarios';

describe('Game Vote Contract - Deposit', () => {
    test('should deposit as expected', () => {
        const testbed = new SimulatorTestbed(Deposit)
            .loadContract(Context.ContractPath)
            .runScenario();
            expect(testbed.getContractMapValue(Context.SenderAccount1, Context.MapKeys2.Deposit)).toEqual(100_0000_0000n)
            expect(testbed.getContractMapValue(0n, 0n)).toEqual(Context.SenderAccount1)
            expect(testbed.getContractMapValue(Context.SenderAccount2, Context.MapKeys2.Deposit)).toEqual(100_0000_0000n)
            expect(testbed.getContractMapValue(1n, 1n)).toEqual(Context.SenderAccount2)
            expect(testbed.getContractMapValue(Context.SenderAccount3, Context.MapKeys2.Deposit)).toEqual(100_0000_0000n)
            expect(testbed.getContractMapValue(2n, 2n)).toEqual(Context.SenderAccount3)
            expect(testbed.getContractMapValue(Context.SenderAccount4, Context.MapKeys2.Deposit)).toEqual(100_0000_0000n)
            expect(testbed.getContractMapValue(3n, 3n)).toEqual(Context.SenderAccount4)
            expect(testbed.getContractMapValue(Context.SenderAccount5, Context.MapKeys2.Deposit)).toEqual(100_0000_0000n)
            expect(testbed.getContractMapValue(4n, 4n)).toEqual(Context.SenderAccount5)
            expect(testbed.getContractMapValue(Context.SenderAccount6, Context.MapKeys2.Deposit)).toEqual(100_0000_0000n)
            expect(testbed.getContractMapValue(5n, 5n)).toEqual(Context.SenderAccount6)
            expect(testbed.getContractMapValue(Context.SenderAccount7, Context.MapKeys2.Deposit)).toEqual(100_0000_0000n)
            expect(testbed.getContractMapValue(6n, 6n)).toEqual(Context.SenderAccount7)
            expect(testbed.getContractMapValue(Context.SenderAccount8, Context.MapKeys2.Deposit)).toEqual(100_0000_0000n)
            expect(testbed.getContractMapValue(7n, 7n)).toEqual(Context.SenderAccount8)
            expect(testbed.getContractMapValue(Context.SenderAccount9, Context.MapKeys2.Deposit)).toEqual(100_0000_0000n)
            expect(testbed.getContractMapValue(8n, 8n)).toEqual(Context.SenderAccount9)
            expect(testbed.getContractMapValue(Context.SenderAccount10, Context.MapKeys2.Deposit)).toEqual(100_0000_0000n)
            expect(testbed.getContractMapValue(9n, 9n)).toEqual(Context.SenderAccount10)
    })
})
