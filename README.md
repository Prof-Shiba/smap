# smap
smap (shiba mapper) is a network scanning tool for both Windows and Linux. It is built using only C and no external libraries.

# How to install
1. Clone the repo and `cd` into it in a terminal. Then run the following:
2. `mkdir build`
3. `cd build`
4. `cmake ..`
5. `make`

# How to use smap
Smap has many features, which are viewable with a simple run of `smap -h`. Keep an eye on it as more and more features are added.
You can run a simple loopback port scan of all ports with `smap 127.0.0.1 -p-`. Or scan an external address, such as nmap's test website: `smap 45.33.32.156 -p 80`, which will only scan port 80.
