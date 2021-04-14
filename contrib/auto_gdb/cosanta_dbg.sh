#!/bin/bash
# use testnet settings,  if you need mainnet,  use ~/.cosantacore/cosantad.pid file instead
cosanta_pid=$(<~/.cosantacore/testnet3/cosantad.pid)
sudo gdb -batch -ex "source debug.gdb" cosantad ${cosanta_pid}
