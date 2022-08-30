# Network Test Procedure

This documents the procedures for executing the tests in this collection
on both Linux and RTEMS. Some tests can run without intervention or
special setup. Others are manual. Some require special services to
be available.

## Automated Tests

A subset of the networking tests may be executed without manual 
interactive actions. The command __make runtests__ or 
__make CONFIG_INC=@CONFIG_FILE@ runtests__ will execute them on
the native host or a simulator. If the target is an embedded board
without a runner script, it may be necessary to run them one at
a time manually.

The following tests may be executed with no manual actions required.

- devctl_socket
- fcntl_socket
- getaddrinfo01
- getnameinfo01
- ioctl_socket
- send_recv
- sockopt01

The 'send_recv' test attempts to perform at http "get" transaction with 
devel.rtems.org by IP address. The target board

## Manual/Interactive Tests

- tcp_echo_client
- tcp_echo_server
- udp_echo_server
- mcast_listener
- mcast_writer
- select01

These are discussed in the following subsections.

### TCP Echo (Client and Server)

__tcp_echo_client__ and __tcp_echo_server__ are a mated pair. One is 
run on the target system and the other on the system under test. For
complete testing, the client and server should be run on the target.
 __tcp_echo_server__ should be invoked first as in the following example
specifying the number of times to echo and the port to listen on:

```
tcp_echo_server 5 22001
```

__tcp_client_client__ is interactive and is invoked with the port to
connect to as shown below:

```
tcp_echo_client 22001
```

### UDP Echo (Server)

__udp_echo_server__ is executed on the system under test. Use a 
program such as __netcat__ to interact with it.

### Multicast (Client and Server)

__mcast_server__ and __mcast_client__ are a mated pair. One is 
run on the target system and the other on the system under test.
It will be necessary to address routing and firewall configuration
for the two systems to communicate multicast.

### Select01

__select01__ waits for connections. __telnet__ from the host system
to the system under test. There can be multiple __telnet__ sessions.

