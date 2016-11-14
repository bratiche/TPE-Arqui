sudo ip link add br0 type bridge

sudo ip tuntap add mode tap tap0 user $(whoami)
sudo ip tuntap add mode tap tap1 user $(whoami)

sudo ip link set tap0 master br0
sudo ip link set tap1 master br0

sudo ip link set dev br0 up
sudo ip link set dev tap0 up
sudo ip link set dev tap1 up


