Building Block View
###################

1st Level Building Block View
=============================

.. uml:: building_block_view_1st_level.puml
   :align: center
   :caption: 1st Level Building Block View

.. list-table:: 1st Level Building Block View
   :header-rows: 1

   * - Building Block
     - Description
   * - Interface Layer
     - Provide the boundary between the application and the outside world.
   * - Application Layer
     - Contain the business logic and orchestrate the flow of data between the other layers.
   * - Service Layer
     - Provide the implementation for the business services and exchange data with client applications.

2nd Level Building Block View
=============================

.. uml:: building_block_view_2nd_level.puml
   :align: center
   :caption: 2nd Level Building Block View

.. list-table:: 2nd Level Building Block View
   :header-rows: 1

   * - Layer
     - Building Block
     - Description
   * - Interface Layer
     - GUI
     - Provide the graphical user interface for the non-technical users.
   * - Interface Layer
     - CLI
     - Provide the command line interface for the technical users.
   * - Application Layer
     - Application
     - Contain the business logic and orchestrate the flow of data between the other layers.
   * - Application Layer
     - Domain
     - Contain the domain model and business rules.
   * - Service Layer
     - Configuration
     - Provide the users configuration for the application during startup.
   * - Service Layer
     - Persistence
     - Read and write data to the file system.
   * - Service Layer
     - Communication
     - Exchange data with the client applications.

Communication
=============

.. include:: communication/node_layer/node_layer.inc

.. include:: communication/connection_layer/connection_layer.inc

.. include:: communication/transport_layer/transport_layer.inc
