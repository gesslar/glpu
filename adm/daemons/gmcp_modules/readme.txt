These GMCP modules are called from the GMCP_D and are intended to handle
outbound GMCP data. They are not intended to be called directly.

The function GMCP_D::send_gmcp() is used to parse out and make decisions
before passing the information to the appropriate module in this directory.

The file name is the GMCP package name, and the function name is either
the subpackage or the command being sent to the client.

In this way, you can build a GMCP package that is modular and easy to
add/remove/maintain without having to manipulate the GMCP_D directly.

The module is responsible for generating the inormation, or can pass along
a payload from the GMCP_D to the player, and subsequently the client.
