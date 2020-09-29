CLEOS=cleos -u http://127.0.0.1:8888

clean: clean-build

clean-build:
	-rm *.abi *.wasm

build:
	eosio-cpp -abigen ./src/stakewax.cpp -o stakewax.wasm -I ./include/
