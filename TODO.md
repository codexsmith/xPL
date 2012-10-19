
For Determinators:
------------------
- Make sure we use the value parser from the RulesManager in all places (like xplconditions...)
- Handle groups
- Make determinator execution happen in order, and in a separate thread
- Implement stop/suspend/delay actions

For more XHCP:
--------------
- Group support
- RELOAD / occasional save of determinators/globals
- Figure out what 'settings' are
- runrule may be wrong - ignores conditions


For Configuration Manager:
--------------------------
- Revise the test application in the SDK, and get it doing some configuration stuff
- build long-term storage (xml?)
- Implement an independent thread to handle config

