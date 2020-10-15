# stakewax

This contract boosts the cpu and net for a given account. It is meant to be used from the unboxing contracts to allow clients to perform unboxing without having resources beforehand



## Setup
Run `boosteradd (name booster)` from contract account to add accounts that can boost. 

- booster = The account that will be allowed to boost.

## Boosting
Boost by running `boost(name from, name to, asset cpu, asset net)`:

- from = The account that boosts
- to = The account that gets boosted
- cpu = How much (in WAX) gets staked to that account
- net = How much (in WAX) gets staked to that account

_Boosting an account that was already boosted will result in no additional boosting. **Update boosts** to modify a boost._

## Update Boosts
Does someone have too much wax staked and is inactive? Update the amount they got boosted with `updateboost(name from, name to, asset cpu_to, asset net_to)` to update the amount of WAX staked to that account.

This will set the amount of WAX staked which means it will unstake / stake CPU and/or NET for that account to the new values.

- from = The account that boosts
- to = The account that gets boosted
- cpu_to = How much (in WAX) gets staked to that account
- net_to = How much (in WAX) gets staked to that account


## Unboost
Want to unstaked everything for an inactive account? You can unboost via `unboost(name from, name to)`

- from = The account that boosts
- to = The account that got boosted

## Remove booster
Is a booster using too many resources and should not be allowed to boost? You can remove them by running `boosterdel (name booster)`.

- booster = The account that was allowed to boost