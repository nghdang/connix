Solution Strategy
#################

Connix Core
===========

`connix-core` will be implemented by using a Clean Architecture approach. The core will be divided
into three main layers: the domain layer, the application layer, and the infrastructure layer. Each
layer will have its own responsibilities and will communicate with the other layers through
well-defined interfaces.

Connix
======

`connix` will be implemented by wrapping the `connix-core` library and providing a command-line
interface (CLI) for users to interact with the system. The CLI will be built with two running modes:

- **Interactive mode**: Using Read-Eval-Print Loop (REPL) to allow users to interact with the system
  in a conversational manner.
- **Batch mode**: Allowing users to execute commands in a non-interactive manner, suitable for
  automation and scripting.

Connix GUI
==========

`connix-gui` will be implemented by wrapping the `connix-core` library and providing a graphical
user interface (GUI) for users to interact with the system. The GUI will be built using Qt framework
and will provide a user-friendly interface for users to perform various tasks.
