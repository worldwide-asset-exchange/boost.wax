#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/system.hpp>

using namespace std;
using namespace eosio;

static const symbol STAKE_SYMBOL = symbol("WAX", 8);
static const uint64_t MAX_RAM_BOOST = 4096 - 1611;

CONTRACT booster : public contract {
  public:
    using contract::contract;

    ACTION boost(name from, name to, asset cpu, asset net);
    ACTION updateboost(name from, name to, asset cpu_to, asset net_to);
    ACTION unboost(name from, name to);

    // deprecated
    ACTION boosterdel (name booster);

    ACTION reg(name contract, uint64_t cpu_us_per_user, uint64_t net_words_per_user, bool use_allow_list, vector<name> allowed_contracts);
    ACTION dereg(name contract);

    ACTION noop() {};
    ACTION boostram(name from, name to, uint64_t bytes);

    struct stakeargs {
      name from;
      name receiver;
      asset stake_net_quantity;
      asset stake_cpu_quantity;
      bool transfer;
    };

    struct unstakeargs {
      name from;
      name receiver;
      asset unstake_net_quantity;
      asset unstake_cpu_quantity;
    };

    void stake(name to, asset cpu, asset net);
    void unstake(name to, asset cpu, asset net);

    bool can_boost(name booster);
    uint64_t get_time();

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

    TABLE boosters {
      name booster;
      auto  primary_key() const { return booster.value; }
    };
    typedef multi_index<name("boosters"), boosters> boosters_table;

    TABLE contracts {
      name          contract;
      uint64_t      cpu_us_per_user;
      uint64_t      net_words_per_user;
      bool          use_allow_list;
      vector<name>  allowed_contracts;
      bool          allow_ram_boosts;
      auto primary_key() const { return contract.value; }
    };
    typedef multi_index<name("contracts"), contracts> contracts_table;

    // ramboosts are scoped buy buyer account
    TABLE ramboosts {
      name     user;
      uint64_t bytes;
      auto  primary_key() const { return user.value; }
    };
    typedef multi_index<name("ramboosts"), boosts> boosts_table;
};
