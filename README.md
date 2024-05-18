# Game-Contracts
 just a collection of SmartC-Contracts wich used inGame

# Development

A secure and fast way to develop Smart Contracts in Signum is using the TDD approach, i.e. Test-Driven Development. This allows to programmatically verify the Smart Contracts functionality, while also speed up development and foremost confidence and security.

Each contract "lives" in its own folder aside its test cases, as proposed [here](https://github.com/ohager/signum-smartc-testbed-starter).

## Installation

1. Install NodeJS 18+
2. In this projects root folder simply run `npm install`
3. Run `npm test` to run the tests

## Implementing Tests

It's recommended to create a folder for each "method" of the contract you want to test. A test needs always

1. A collection of (simulated) Transactions, called _Scenarios_
2. A collection of Test cases (see ([how to write test with Vitest](https://vitest.dev/guide/#writing-tests)))

It turned out that it's also good to keep contract specific constants, i.e. map keys, method ids, accounts, etc in a separate file, which you call `globals.ts`, `constants.ts`, or `context.ts`

In your test you have to instantiate the testbed, like this 

```ts
const testbed = new SimulatorTestbed(Scenario) // instantiate with a Scenario
    .loadContract(Context.ContractPath)
    .runScenario();
```

Once executed, the testbed object maintains a completely emulated blockchain environment for the given scenario, with access to
 - transactions
 - contracts (and their inner state and maps)
 - accounts
 - etc.

> When debugging while testing one can inspect all these objects.

For complete methods and object of the testbed look [here](https://ohager.github.io/signum-smartc-testbed/classes/SimulatorTestbed.html)


### Running Code in Web Simulator

Sometimes it's necessary for _in-situ_ debugging to run the contract in the [Web Simulator](https://deleterium.info/sc-simulator/).

1. Copy the contract code into the simulator
2. use [`toSimulatorTransactions`](https://ohager.github.io/signum-smartc-testbed/functions/toSimulatorTransactions.html) to convert your scenario into copy&pastable JSON for the Simulator.

Now, you can debug the contract interactively (which is not supported by the testbed)