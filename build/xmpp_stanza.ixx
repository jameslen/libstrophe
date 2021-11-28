export module strophe.stanza;

import <map>;
import <memory>;
import <memory_resource>;
import <string>;

namespace xmpp
{
    export enum stanza_type
    {
        unknown,
        text,
        tag
    };

    export class stanza
    {
    public:
        explicit stanza(
            std::pmr::memory_resource* allocator
        );

        stanza(
            std::string_view str,
            std::pmr::memory_resource* allocator
        );

        std::shared_ptr<stanza> copy();

        bool is_text();
        bool is_tag();

        std::pmr::string to_text();

        std::shared_ptr<stanza> get_children();
        std::shared_ptr<stanza> get_child_by_name();
        std::shared_ptr<stanza> get_child_by_ns();
        std::shared_ptr<stanza> get_child_by_name_and_ns();

        std::shared_ptr<stanza> get_next();

        bool add_child(std::shared_ptr<stanza> child, bool do_clone = false);

        std::string_view get_attribute(std::string_view name);
        size_t get_attribute_count();

        std::pmr::map<std::pmr::string, std::pmr::string> get_attributes();

        std::pmr::string get_text();
        std::string_view get_text_ptr();
        std::string_view get_name();

        bool set_attribute(std::string_view key, std::string_view value);
        bool set_name(std::string_view name);
        bool set_text(std::string_view text);

        bool delete_attribute(std::string_view key);

        std::string_view get_ns();
        std::string_view get_type();
        std::string_view get_id();
        std::string_view get_to();
        std::string_view get_from();
        bool set_ns(std::string_view ns);
        bool set_id(std::string_view id);
        bool set_type(std::string_view type);
        bool set_to(std::string_view to);
        bool set_from(std::string_view from);

        // TODO:
        ///* allocate and initialize a stanza in reply to another */
        //xmpp_stanza_t *xmpp_stanza_reply(xmpp_stanza_t *stanza);
        //xmpp_stanza_t *
        //xmpp_stanza_reply_error(xmpp_stanza_t *stanza, const char *error_type, const char *condition, const char *text);

        ///* stanza subclasses */
        //xmpp_stanza_t *xmpp_message_new(xmpp_ctx_t *ctx, const char *type, const char *to, const char *id);
        //char *xmpp_message_get_body(xmpp_stanza_t *msg);
        //int xmpp_message_set_body(xmpp_stanza_t *msg, const char *text);

        //xmpp_stanza_t *xmpp_iq_new(xmpp_ctx_t *ctx, const char *type, const char *id);
        //xmpp_stanza_t *xmpp_presence_new(xmpp_ctx_t *ctx);
        //xmpp_stanza_t *xmpp_error_new(xmpp_ctx_t *ctx, xmpp_error_type_t type, const char *text);

    private:
        std::pmr::memory_resource *m_allcator;
        stanza_type m_type;

        std::shared_ptr<stanza> m_prev;
        std::shared_ptr<stanza> m_next;
        std::shared_ptr<stanza> m_children;
        std::weak_ptr<stanza> m_parent;

        std::pmr::string data;

        std::pmr::map<std::pmr::string, std::pmr::string> attributes;
    };
}