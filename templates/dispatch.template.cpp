// funcproxy.template.cpp
//
// Defines a template for dispatchFunction to be filled in by rpcgenerate
//  - dispatchFunction is stub exclusive
//  - leaves Python format strings for where things should be filled out
//
// by: Justin Jo and Charles Wan

void dispatchFunction() {{
stringstream debugStream;
StatusCode funcnameCode;

try {{
// read funcname length then name
int funcnamelen;
readAndThrow(RPCSTUBSOCKET, (char *)&funcnamelen, 4);
char funcname[funcnamelen];
readAndThrow(RPCSTUBSOCKET, funcname, funcnamelen);

if (funcname[funcnamelen - 1] != '\0') {{ // check funcname null termed
  funcnameCode = no_null_term_found;
  RPCSTUBSOCKET->write((char *)&funcnameCode, 4);
  throw RPCException("dispatchFunction: Function name not null terminated");
}}

// debug for func request
debugStream << "dispatchFunction: Received function request for "
            << funcname << "()";
c150debug->printf(C150APPLICATION, debugStream.str().c_str());
*GRADING << debugStream.str() << endl;
debugStream.str("");

// branch to check funcname validity
if (!RPCSTUBSOCKET->eof()) {{
{funcBranches} else {{
  // nonexisting function requested
  debugStream << "dispatchFunction: Unknown function " << funcname
              << "() requested";
  c150debug->printf(C150APPLICATION, debugStream.str().c_str());
  *GRADING << debugStream.str() << endl;
  debugStream.str("");

  funcnameCode = nonexistent_func;
  RPCSTUBSOCKET->write((char *)&funcnameCode, 4);
}}
}}
}} catch (RPCException e) {{
  // something went wrong with dispatch, but shouldnt end server
  c150debug->printf(C150APPLICATION,
    "Caught %s",
    e.formattedExplanation().c_str());
}}
}}
