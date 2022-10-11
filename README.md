this program connects two computer peer to peer on the same network.
you have to start server firstly and then client.
after server started client can connect to it with ip address of the server  
## For Downloading
`git clone https://github.com/Avaora/ey_aramam.git`

## For Compiling
`gcc ey_aramam.c -lpthread -o ey_aramam`

## For Installing with make
`make install`

# Usage
## Finding ip address of the server
`ifconfig | grep -A 4 en0`  
inet address is the ip address you looking for
## Example
### firstly you have to start the server by typing this  
type this on the computer you want to connect to
`./ey_aramam`  
### after server started you can connect to it with ip address of the server
type this on the computer you want to request to server for connection
`./ey_aramam 127.0.0.1`  
in this case server's ip address is 127.0.0.1  
and then you are ready to type a message  
enjoy it !!!
