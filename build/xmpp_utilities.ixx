export module strophe.utilities;

import <memory_resource>;
import <string>;

export namespace xmpp
{
    std::pmr::string create_jid(
        std::string_view node,
        std::string_view domain,
        std::string_view resource,
        std::pmr::memory_resource* allocator
    );

    std::string_view jid_bare(std::string_view jid);
    std::string_view jid_node(std::string_view jid);
    std::string_view jid_domain(std::string_view jid);
    std::string_view jid_resource(std::string_view jid);

    std::pmr::string uuid_gen(std::pmr::memory_resource *allocator);
}
