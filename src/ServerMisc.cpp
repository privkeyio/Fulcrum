#include "Common.h"
#include "ServerMisc.h"

namespace ServerMisc
{
    const QString AppVersion(VERSION);
    // This server follows the BLAKE2b proof-of-work hardfork, so past the activation height it serves a chain
    // the stock build does not. Say so where clients, crawlers and the peer network can see it: a wallet that
    // cannot tell the two apart has no way to know which rules the data it is given came from. Kept out of
    // VERSION so that macro stays equal to upstream's and release merges do not conflict on it.
    const QString AppSubVersion = QString("%1 %2+blake2b").arg(APPNAME, VERSION);
}
