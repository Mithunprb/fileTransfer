# FileTransfer
file transfer using shell based client and server using socks  library in C

### Source code
* [clinet.c](client.c)<br>
* [server.c](server.c)
* Outputs:
  * client  
  ```python
  % ./client
  file descriptor 3 received

  Please enter file name to receive:
  server.c
  ```
  * server
  ```python
  % ./server
  file descriptor 3 received

  Successfully binded!

  Waiting for file name...
  client.c
  ```
