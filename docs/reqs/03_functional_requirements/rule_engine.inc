Rule Engine
-----------

.. sw-req:: Rule Engine
    :id: [$SW_REQ RULE_BASED]
    :desc: Connix SHALL support rules that associate one triggering event with
           one send or receive action and execute the action within the
           connection cycle triggered by that event.

.. sw-req:: Rule Definition
    :id: [$SW_REQ RULE_BASED_DEFINITION]
    :desc: Connix SHALL accept a rule definition containing exactly one
           supported triggering event and exactly one send or receive action
           when the configuration is loaded.

.. sw-req:: Rule Cycle Scope
    :id: [$SW_REQ RULE_BASED_CYCLE_SCOPE]
    :desc: Connix SHALL execute a matching rule's action only during the
           connection cycle triggered by its event and SHALL NOT defer or chain
           the action into a later cycle.
