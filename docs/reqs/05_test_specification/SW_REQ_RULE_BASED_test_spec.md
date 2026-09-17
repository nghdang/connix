# Test Specification: SW_REQ_RULE_BASED

**Source:** `docs/reqs/03_functional_requirements/functional_requirements.rst`

## Requirement Summary

This test specification covers the child requirements under the parent `SW_REQ_RULE_BASED`:
`SW_REQ_RULE_BASED_DEFINITION` and `SW_REQ_RULE_BASED_CYCLE_SCOPE`.

**Fit Criterion:** Each child behavior is tested independently: a rule pairs one event with one
send/receive action, and the action remains within the triggering connection cycle.

## Test Design Techniques

- Equivalence Partitioning: action types {send, receive} × event kinds.
- Decision Table: rule definition validity (valid event+action pair vs malformed rule).

## Test Cases

### TC_RULE_01 — Rule pairs message-receipt event with send action
*Type: Positive · Priority: High*

Title: Rule pairs message-receipt event with send action.
Satisfies:
   - SW_REQ_RULE_BASED
   - SW_REQ_RULE_BASED_DEFINITION
Objective: Happy — Verify that rule pairs message-receipt event with send action.
Steps:
   - 1. Establish the precondition: Rule defined: on message receipt matching pattern X, send response Y.
   - 2. Trigger receipt of a message matching pattern X.

Expectations:
   - 1. Response Y is sent within the same cycle as the triggering receipt.

TestData:
   - event=message receipt, action=send(Y).
   - Fixture setup required by the precondition: Rule defined: on message receipt matching pattern X, send response Y.

### TC_RULE_02 — Rule pairs connection-state event with receive action
*Type: Positive · Priority: High*

Title: Rule pairs connection-state event with receive action.
Satisfies:
   - SW_REQ_RULE_BASED
   - SW_REQ_RULE_BASED_DEFINITION
Objective: Happy — Verify that rule pairs connection-state event with receive action.
Steps:
   - 1. Establish the precondition: Rule defined: on connect, wait/receive.
   - 2. Trigger a connect event.

Expectations:
   - 1. Connix performs the receive action as configured immediately following connect, within the same cycle.

TestData:
   - event=connect, action=receive.
   - Fixture setup required by the precondition: Rule defined: on connect, wait/receive.

### TC_RULE_03 — Rule pairs timer event with send action
*Type: Positive · Priority: High*

Title: Rule pairs timer event with send action.
Satisfies:
   - SW_REQ_RULE_BASED
   - SW_REQ_RULE_BASED_DEFINITION
Objective: Happy — Verify that rule pairs timer event with send action.
Steps:
   - 1. Establish the precondition: Rule defined: on timer interval, send heartbeat message.
   - 2. Let the timer interval elapse.

Expectations:
   - 1. The send action executes at the timer trigger, within the cycle it triggers.

TestData:
   - event=timer, action=send(heartbeat).
   - Fixture setup required by the precondition: Rule defined: on timer interval, send heartbeat message.

### TC_RULE_04 — Rule action scoped to a single cycle
*Type: Positive · Priority: High*

Title: Rule action scoped to a single cycle.
Satisfies:
   - SW_REQ_RULE_BASED
   - SW_REQ_RULE_BASED_CYCLE_SCOPE
Objective: Happy — Verify that rule action scoped to a single cycle.
Steps:
   - 1. Establish the precondition: Rule defined and PERIODIC mode active (multiple triggers).
   - 2. Let 3 timer triggers elapse.
   - 3. Confirm the rule's action executes fully and independently in each of the 3 resulting cycles.

Expectations:
   - 1. The action runs 3 separate times, once per cycle; no state/action from one cycle carries into the next.

TestData:
   - mode=PERIODIC, 3 triggers.
   - Fixture setup required by the precondition: Rule defined and PERIODIC mode active (multiple triggers).

### TC_RULE_05 — Rule does not chain across cycles
*Type: Negative · Priority: High*

Title: Rule does not chain across cycles.
Satisfies:
   - SW_REQ_RULE_BASED
   - SW_REQ_RULE_BASED_CYCLE_SCOPE
Objective: Error — Verify that rule does not chain across cycles.
Steps:
   - 1. Establish the precondition: Attempt to configure a rule intended to span multiple cycles (e.g. "wait for event A, then in a later cycle perform action B").
   - 2. Attempt such a multi-cycle rule configuration.

Expectations:
   - 1. Connix rejects the configuration as invalid, or documentation/config schema makes such chaining structurally impossible (single event, single action per rule).

TestData:
   - multi-cycle rule definition attempt.
   - Fixture setup required by the precondition: Attempt to configure a rule intended to span multiple cycles (e.g. "wait for event A, then in a later cycle perform action B").

### TC_RULE_06 — Rule with no matching event produces no action
*Type: Negative · Priority: Medium*

Title: Rule with no matching event produces no action.
Satisfies:
   - SW_REQ_RULE_BASED
   - SW_REQ_RULE_BASED_DEFINITION
Objective: Error — Verify that rule with no matching event produces no action.
Steps:
   - 1. Establish the precondition: Rule defined for a specific message pattern.
   - 2. Send a message that does not match the rule's pattern.

Expectations:
   - 1. No rule action executes; Connix takes no send/receive action beyond the cycle's own base flow.

TestData:
   - non-matching message.
   - Fixture setup required by the precondition: Rule defined for a specific message pattern.

### TC_RULE_07 — Malformed rule definition rejected at configuration load
*Type: Error · Priority: High*

Title: Malformed rule definition rejected at configuration load.
Satisfies:
   - SW_REQ_RULE_BASED
   - SW_REQ_RULE_BASED_DEFINITION
Objective: Error — Verify that malformed rule definition rejected at configuration load.
Steps:
   - 1. Establish the precondition: Config file with a rule missing a required field (e.g. no action specified).
   - 2. Load a configuration containing an incomplete rule.

Expectations:
   - 1. Connix reports a clear configuration error identifying the malformed rule and exits without performing any configured action.

TestData:
   - rule missing `action` field.
   - Fixture setup required by the precondition: Config file with a rule missing a required field (e.g. no action specified).
