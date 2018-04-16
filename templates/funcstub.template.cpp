// funcstub.template.cpp
//
// Defines a template for an rpc stub function to be filled in by rpcgenerate
//  - leaves Python format strings for where things should be filled out
//    - e.g. {funcname} 
//
// by: Justin Jo and Charles Wan

void _{funcname}() {{
stringstream debugStream;

// reag args size then all args bytes
debugStream << "Receiving arguments for {funcname}()";
logDebug(debugStream, C150APPLICATION, true);

int argsSize;
readAndThrow(RPCSTUBSOCKET, (char *)&argsSize, 4);
char argsBytes[argsSize];
readAndThrow(RPCSTUBSOCKET, argsBytes, argsSize);

// use string streawm to deconstruct args bytes into args
stringstream ss;
ss << string(argsBytes, argsSize);
StatusCode argsCode = good_args; // assume that args are good for now

{declareArgs}

try {{
{readArgs}
argsCode = checkArgs(ss);
}} catch (RPCException e) {{ // should only be from extractString
  argsCode = scrambled_args;
}}

// send args code
RPCSTUBSOCKET->write((char *)&argsCode, 4);
if (argsCode != good_args) {{
  c150debug->printf(C150APPLICATION,
    "stub.{funcname}: Error %d occurred when parsing args",
    argsCode);
  return; // if bad args, stop here
}}

// call real func with args
debugStream << "Calling {funcname}()";
logDebug(debugStream, C150APPLICATION, true);
{callFunction} // must declare a result variable res, if return value exists

{% begin result %}
// send result size back then result
debugStream << "Sending result of call to {funcname}()";
logDebug(debugStream, C150APPLICATION, true);

int resSize = 0;
{resSizeAccumulate}
RPCSTUBSOCKET->write((char *)&resSize, 4);

{sendRes}
{% end result %}
}}