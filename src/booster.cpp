#include <booster.hpp>

ACTION booster::reg(name contract, uint64_t cpu_us_per_user, uint64_t net_words_per_user, bool use_allow_list, vector<name> allowed_contracts) {
  require_auth(contract);

  contracts_table _contracts(get_self(), get_self().value);
  auto itr = _contracts.find(contract.value);
  if(itr == _contracts.end()) {
    _contracts.emplace(contract, [&](auto& row) {
      row.contract = contract;
      row.cpu_us_per_user = cpu_us_per_user;
      row.net_words_per_user = net_words_per_user;
      row.use_allow_list = use_allow_list;
      row.allowed_contracts = allowed_contracts;
    });
  } else {
    _contracts.modify(itr, contract, [&](auto &rec) {
      rec.cpu_us_per_user = cpu_us_per_user;
      rec.net_words_per_user = net_words_per_user;
      rec.use_allow_list = use_allow_list;
      rec.allowed_contracts = allowed_contracts;
    });
  }
}

ACTION booster::dereg(name contract) {
  contracts_table _contracts(get_self(), get_self().value);
  auto _contract = _contracts.require_find(contract.value, "contract not found");
  _contracts.erase(_contract);
}

ACTION boster::boostram(name from, name to, uint64_t bytes) {
  require_auth(get_self());

  
}

ACTION booster::boosterdel (name booster) {
  require_auth(get_self());

  boosters_table _boosters(get_self(), get_self().value);
  auto _booster = _boosters.require_find(booster.value, "booster not found");

  _boosters.erase(_booster);
}

ACTION booster::boost(name from, name to, asset cpu, asset net) {
  // if (!can_boost(from)) return;
  //
  // boosts_table _boosts(get_self(), get_self().value);
  // auto itr = _boosts.find(to.value);
  //
  // if(itr != _boosts.end()) return;
  //
  // // not yet boosted
  //
  // check(cpu.symbol == STAKE_SYMBOL, "invalid cpu symbol");
  // check(net.symbol == STAKE_SYMBOL, "invalid net symbol");
  //
  // if (cpu.amount > 2500000000) cpu.amount = 2500000000;
  // if (net.amount > 1000000000) net.amount = 1000000000;
  //
  // _boosts.emplace(get_self(), [&](auto& msg) {
  //     msg.user = to;
  //     msg.requester = from;
  //     msg.amount_cpu = cpu;
  //     msg.amount_net = net;
  //     msg.time = get_time();
  // });
  //
  // stake(to, cpu, net);
}

ACTION booster::unboost(name from, name to) {
  if (!can_boost(from)) return;

  boosts_table _boosts(get_self(), get_self().value);
  auto itr = _boosts.require_find(to.value, "account not boosted");

  unstake(to, itr->amount_cpu, itr->amount_net);

  _boosts.erase(itr);
}

ACTION booster::updateboost(name from, name to, asset cpu_to, asset net_to) {
  if (!can_boost(from)) return;

  boosts_table _boosts(get_self(), get_self().value);
  auto itr = _boosts.require_find(to.value, "account not boosted");

  check(cpu_to.symbol == STAKE_SYMBOL, "invalid cpu symbol");
  check(net_to.symbol == STAKE_SYMBOL, "invalid net symbol");

  asset NO_WAX = asset(0, STAKE_SYMBOL);

  if (cpu_to.amount > 2500000000) cpu_to.amount = 2500000000;
  if (net_to.amount > 1000000000) net_to.amount = 1000000000;

  if (itr->amount_cpu.amount > cpu_to.amount && itr->amount_net.amount > net_to.amount) {
    // if both need unstaking
      unstake(to, asset(itr->amount_cpu.amount - cpu_to.amount, STAKE_SYMBOL), asset(itr->amount_net.amount - net_to.amount, STAKE_SYMBOL));
  } else if (itr->amount_cpu.amount < cpu_to.amount && itr->amount_net.amount < net_to.amount) {
    // if both need staking
      stake(to, asset(cpu_to.amount - itr->amount_cpu.amount, STAKE_SYMBOL), asset(net_to.amount - itr->amount_net.amount, STAKE_SYMBOL));
  } else {
    /**
      CPU HANDLER
    **/
    if (itr->amount_cpu.amount > cpu_to.amount) {
      unstake(to, asset(itr->amount_cpu.amount - cpu_to.amount, STAKE_SYMBOL), NO_WAX);
    } else if (itr->amount_cpu.amount < cpu_to.amount) {
      stake(to, asset(cpu_to.amount - itr->amount_cpu.amount, STAKE_SYMBOL), NO_WAX);
    }

    /**
      NET HANDLER
    **/
    if (itr->amount_net.amount > net_to.amount) {
      unstake(to, NO_WAX, asset(itr->amount_net.amount - net_to.amount, STAKE_SYMBOL));
    } else if (itr->amount_net.amount < net_to.amount) {
      stake(to, NO_WAX, asset(net_to.amount - itr->amount_net.amount, STAKE_SYMBOL));
    }
  }

  _boosts.modify(itr, get_self(), [&](auto &row) {
      row.requester = from;
      row.amount_cpu = cpu_to;
      row.amount_net = net_to;
      row.time = get_time();
  });
}

void booster::stake(name to, asset cpu, asset net) {
  stakeargs args;

  args.from = get_self();
  args.receiver = to;
  args.stake_cpu_quantity = cpu;
  args.stake_net_quantity = net;
  args.transfer = false;

  action issueAction = action(
      permission_level(get_self(), "active"_n),
      "eosio"_n,
      "delegatebw"_n,
      std::move(args)
  );
  issueAction.send();
}

void booster::unstake(name to, asset cpu, asset net) {
  unstakeargs args;

  args.from = get_self();
  args.receiver = to;
  args.unstake_cpu_quantity = cpu;
  args.unstake_net_quantity = net;

  action issueAction = action(
      permission_level(get_self(), "active"_n),
      "eosio"_n,
      "undelegatebw"_n,
      std::move(args)
  );
  issueAction.send();
}

bool booster::can_boost(name booster) {
  require_auth(booster);
  boosters_table _boosters(get_self(), get_self().value);
  auto itr = _boosters.find(booster.value);
  return itr != _boosters.end();
}

uint64_t booster::get_time() {
  return (int64_t) eosio::current_time_point().time_since_epoch().count() / 1000;
}

EOSIO_DISPATCH(booster, (reg)(dereg)(noop)(boost)(unboost)(updateboost)(boosterdel));
