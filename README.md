/* Author: Siddharth Manu
 * Date: 26-Oct-2014
 */

As part of a system programming course, this is a simple client/server program. Over the next 3-4 months,
this repository will be updated to make a distributed program which handles large work requests, which it
shares with multiple client threads.

How to run?
-----------

1. Build the client & server using "make"
2. Run the server using ./caps_server
3. There are two ways to run a client.
	a. telnet localhost <port_num>, where PORT_NUM is 4444 by default (as specified in caps.h)
	b. Run the client ./caps_client
