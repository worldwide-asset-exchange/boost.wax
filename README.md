# boost.wax

This contract registers other contracts that wish to have WAX apply extended management over CPU and Net for their WAX Cloud Wallet users.

Deployed to: [boost.wax](https://wax.bloks.io/account/boost.wax)


## API

* **reg(name contract, uint64_t cpu_us_per_user, uint64_t net_words_per_user, bool use_allow_list, vector<name> allowed_contracts)**
   Register your contract for bandwidth management.  
   * `contract`: the contract account to register. Must also be the account calling this action.  
   * `cpu_us_per_user`: amount of cpu in microseconds to provide your users over a 24 hour period.  
   * `net_words_per_user`: amount of net in microseconds to provide your users over a 24 hour period.  
   * `use_allow_list`: turn allow list enforcement on or off.
   * `allowed_contracts`: vector of contract name that are permitted to be in transactions accompanying your contract. Must have use_allow_list == true for these to be enforced. The idea is to prevent abuse by dapps that might sneak one of your contract actions into their transactions in order to take advantage of your bandwidth quota. By listing the cotnracts you accept in your contract's transactions, you will only pay with your own cpu+net if all transaction contracts are contained in this list.  
   
    Note: your contract must have a permission called **paybw**, and it must be linked to the **boost.wax**#**noop** action. Furthermore, it must have a 1 of 1 authority usuing the account@permission `boost.wax@paybw`. As an example see the [test.wax@paybw permission](https://wax.bloks.io/account/test.wax#keys). When the free tier is exceeded, the WAX backend will sign for your users using this permission if you have sufficient CPU and Net allocated to your contract's account.
   
* **dereg(name contract)**: 
   Deregister your contract from bandwidth management.  
   
* **noop()**: 
   No-op action inserted into WAX Cloud Wallet transactions that satisfy bandwidth management crtieria.  

* **boost(name from, name to, asset cpu, asset net)**: *Deprecated*
* **updateboost(name from, name to, asset cpu_to, asset net_to)**: *Deprecated*
* **unboost(name from, name to)**: *Deprecated*
* **boosterdel (name booster)**: *Deprecated*

## Bandwidth Management

WAX will give every user 5ms of free CPU and 5000 words of free Net over a rolling 24 hour period. Also, all contracts are individually confined to a maximum of 50s of free bandwidth over a 24 hour period (this 50s quota will likely change as chain demand fluctuates). If either of those run out within the 24 hour period it will be up to the contract to pay for bandwidth (the purpose of this registration contract), and after that, it will be up to the user's own resources. By registering your contract here, permissioning it as described above, and adding CPU and Net to your contract, WAX will automatically begin signing noop transactions with your account so as to conveniently use your contract's CPU and Net to cover the cost for your users. Realize that WAX will only utilize your contract's resources once the free tier runs out for your user or your contract.
