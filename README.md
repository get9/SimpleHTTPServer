================================================================================
Simple HTTP Server
================================================================================
Sean Karlage
CS 485G Networking

* Description
This is a simple HTTP server implemented in C to experiment with network
programming capabilities and the POSIX networking library. It is not secure, nor
robust against known attacks. Case in point, don't use this server to run any
sensitive things. Actually, you probably shouldn't use this server period.

Currently it only handles GET requests and the following content types:
    - text/plain (.txt, anything that's not one of the below)
    - text/html (.html)
    - image/jpeg (.jpg, .jpeg)
    - image/gif (.gif)

If a file is not found on the server, it will return a 404. Connections are
closed after every HTTP transaction.

* Build Process
The server can be built with a simple:

    $ make

at the root of the project directory. It has been tested on the multilab and
should build in that environment. To run the server, execute:

    $ ./myserver <port> <http_root>

where the port to run the server on goes where <port> is and the HTTP root
directory goes where <http_root> is. 

* Caveats
    - The server does not handle filenames with spaces in the name currently; if
      such a file is processed, the server will return a 404 response
    - There is a double free or corruption somewhere in the code, but I haven't
      figured out where yet.
