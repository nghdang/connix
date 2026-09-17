Rule Engine
------------

.. sw-req:: Rule Engine
    :id: SW_REQ_RULE_BASED
    :desc: The Connix SHALL support rules that pair triggering events with
        actions and operate within the connection cycle that they trigger.
    :rationale: Reduces manual intervention while keeping connection lifetime
        controlled by the connection-flow requirements.

.. sw-req:: Rule Definition
    :id: SW_REQ_RULE_BASED_DEFINITION
    :links: SW_REQ_RULE_BASED
    :desc: Connix SHALL accept a rule definition containing one triggering
        event and one send or receive action when the configuration is loaded.
    :rationale: Provides a simple, predictable automation unit.

Fit Criterion: A user-defined rule contains exactly one supported event and
one send or receive action.

.. sw-req:: Rule Cycle Scope
    :id: SW_REQ_RULE_BASED_CYCLE_SCOPE
    :links: SW_REQ_RULE_BASED
    :desc: Connix SHALL execute a matching rule's action only during the
        connect/send/receive/close cycle that triggered it and SHALL NOT chain
        the action into a later cycle.
    :rationale: Prevents rules from extending connection lifetime across
        triggers.

Fit Criterion: A matching rule performs its action only within the current
cycle and leaves no action or open connection pending for a later cycle.
