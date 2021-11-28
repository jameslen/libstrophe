export module strophe;

export import strophe.context;
export import strophe.connection;

namespace xmpp
{
    export void initialize();
    export void shutdown();

    export bool version_check(int major, int minor);

    
}