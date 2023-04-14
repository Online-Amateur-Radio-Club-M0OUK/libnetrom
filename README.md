# libnetrom

## Client

To create a NET/ROM client, you need to instantiate the libnetrom::Client class with the constructor that takes the callsign of the NET/ROM interface to bind to.

The call to connect() takes the callsign to make a NET/ROM connection to. A response of false from this method indicates an error with the processing of the connection, and a call to getErrno() to get the error information may be informative. A reponse of true indicates that a connection is in progress.

The method connected() should be polled to determine if the connection has completed. This is indicated by the return of the address of a libnetrom::Socket object which should be used for interaction with the remote station. A response of nullptr indicates that either an error occurred, or the connection hasn't completed yet. A check on the errno value via getErrno() will indicate the error, or not.

A call to close() will stop a connection that is currently in progress.

## Server

To create a NET/ROM client, you need to instantiate the libnetrom::Server class with the constructor that takes the callsign of the NET/ROM interface to bind to.

The call to bind() starts the server listening for incoming NET/ROM connections. A return value of true indicates that this was succesful, a false indicates some form of failure and the error number returned by getErrno() may be useful.

The accept() method should be polled for any incoming connections. It will return a new libnetrom::Socket object for any incoming connections, or a nullptr if no incoming connection is found, or an error has occurred. A call to getErrno() will indicate if an error has occurred.

Once the server needs to exit, call to close() should be made.

## Socket

This is the main class for interacting with a NET/ROM connection. Once a valid libnetrom::Socket object has been obtained, the callsign of the remote station can be obtained via a call to getPeer().

Interaction should be done via the read() and write() methods which are 8-bit clean. The write() method segments any data to be sent into 236 byte packets which is the maximum amount of data in one NET/ROM packet.

The call to read() is non-blocking. It will return -2 if an error has occurred, and getErrno() should be called to find the reason. A return value of -1 indicates that no data was available. A return value of 0 indicates that the connection has ended. A positive value indicates that data has been received.

At the end of the connection, close() should be called.

