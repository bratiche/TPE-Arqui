# TPE-Arqui

##Compilation
´´
cd Toolchain
make
cd ..
make
´´

##Run
### Set up newtork
To set up ´br0´ bridge connection and 3 tap interfaces (´tap0´, ´tap1´ and ´tap2´):
´´
sudo ./connection.sh
´´
### Run up to 3 instances
´´´
sudo ./run.sh
sudo ./run1.sh
sudo ./run2.sh
