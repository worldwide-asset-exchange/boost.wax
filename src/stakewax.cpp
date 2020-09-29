#include <stakewax.hpp>
#include <eosio/system.hpp>

uint64_t getTime(){
  return (int64_t)eosio::current_time_point().time_since_epoch().count() / 1000;
}

void requireWhitelist(name from){
  check((from == "wax.tycoon"_n ||
    from == "gpk.topps"_n ||
    from == "market.place"_n ||
    from == "bhiyveuahwmz"_n ||
    from == "res.pink"_n ||
    from == "shatner"_n ||
    from == "unbox.heroes"_n ||
    from == "4tioi.waa"_n
    ), "dapp not whitelisted.");
}

ACTION stakewax::boost(name from, name to, asset cpu, asset net){
  require_auth(from);
  requireWhitelist(from);

  boosts_table _boosts(get_self(), get_self().value);
  auto itr = _boosts.find(to.value);

  if(itr != _boosts.end())
    return;

  check(cpu.amount <= 50000000000, "max 50 WAX cpu");
  check(net.amount <= 1000000000, "max 10 WAX net");

  _boosts.emplace(get_self(), [&](auto& msg) {
      msg.user = to;
      msg.requester = from;
      msg.amount_cpu = cpu;
      msg.amount_net = net;
      msg.time = getTime();
  });

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

ACTION stakewax::unboost(name from, name to){
  require_auth(from);
  requireWhitelist(from);
}

EOSIO_DISPATCH(stakewax, (boost)(unboost));
