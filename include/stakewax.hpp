#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace std;
using namespace eosio;

CONTRACT stakewax : public contract {
  public:
    using contract::contract;

    ACTION boost(name from, name to, asset cpu, asset net);
    ACTION unboost(name from, name to);

    struct stakeargs {
      name from;
      name receiver;
      asset stake_net_quantity;
      asset stake_cpu_quantity;
      bool transfer;
    };

  private:
    TABLE boosts {
      name    user;
      name    requester;
      asset   amount_cpu;
      asset   amount_net;
      uint64_t time;
      auto  primary_key() const { return user.value; }
    };
    typedef multi_index<name("boosts"), boosts> boosts_table;
};
