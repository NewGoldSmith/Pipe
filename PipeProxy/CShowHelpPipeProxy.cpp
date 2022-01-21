#include "CShowHelpPipeProxy.h"

CShowHelpPipeProxy::CShowHelpPipeProxy() :CShowInfoPrompt()
{
    m_Text =R"(
This program provides a NetPipe proxy. 

Command options. Lowercase letters can be omitted.
/HA             Host address For example "/HA 127.0.0.4"
/HP             Host port    For example "/HP 0"
/PA             Peer address For example "/PA 127.0.0.3"
/PP             Peer port    For example "/PP 50000"
/CLI            Cliant mode.
/SEV            Server mode.
/BackLog        Number of listen backlogs.
/Copyright      Show License.
/ShowStatus     Show running status.
/Help           Show help.

Data stream option.
It is an option to send from the peer connection destination.
quit + CRLF     Program end.
Prompt option.
quit + CRLF     Program end.
)";

    m_TextSize = m_Text.size();
 

}

