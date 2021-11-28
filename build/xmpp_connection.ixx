export module strophe.connection;

import strophe.context;
import strophe.stanza;

import <chrono>;
import <memory>;
import <memory_resource>;
import <span>;
import <string>;

namespace xmpp
{
    export enum class connection_type
    { 
        unknown,
        client,
        component
    };

    export enum class connection_flags : uint32_t
    {
        disable_tls   = (1 << 0),
        mandatory_tls = (1 << 1),
        legacy_ssl    = (1 << 2),
    };

    struct tlscert_t;
    class connection;

    export using certfail_handler = int (*)(const tlscert_t *cert,
                                            std::string_view errormsg);
    export using conn_handler = void (*)(
        connection *conn,
        connection_event_t event,
        int error,
        stream_error_t *stream_error,
        void *userdata
    );

    // TODO: name this better
    export using data_handler = bool (*)(
        connection *conn,
        std::shared_ptr<stanza> data,
        void *userdata
    );

    export class connection
    {
    public:
        explicit connection(context *ctx);
        ~connection();

        connection() = delete;
        connection(const connection&) = delete;
        connection(connection&&) = delete;

        connection &operator=(const connection&) = delete;
        connection &operator=(connection&&) = delete;

        connection_flags get_flags() const;
        bool set_flags(connection_flags flags);

        void set_jid(std::string_view jid);
        std::string_view get_jid() const;
        std::string_view get_bound_jid() const;

        void set_cafile(std::string_view path);
        void set_capath(std::string_view path);
        void set_certfail_handler(certfail_handler hndl);

        tlscert_t *get_peer_cert();
        void set_client_cert(std::string_view cert, std::string_view key);

        unsigned int cert_xmppaddr_num();
        char *cert_xmppaddr(unsigned int n);

        std::string_view get_pass() const;
        void set_pass(std::string_view pass);
          
        context *get_context();

        void disable_tls();
        bool is_secured();

        void set_keepalive(
            std::chrono::seconds timeout, 
            std::chrono::seconds interval
        );
        
        bool is_connecting();
        bool is_connected();
        bool is_disconnected();

        int connect_client(
            std::string_view altdomain, 
            unsigned short altport, 
            conn_handler callback, 
            void *userdata
        );

        int connect_component(
            std::string_view server,
            unsigned short port,
            conn_handler callback,
            void *userdata
        );

        int connect_raw(
            std::string_view altdomain,
            unsigned short altport,
            conn_handler callback,
            void *userdata
        );

        int open_stream_default();
        
        int open_stream(
            std::span<std::string_view> attributes
        );

        int tls_start();

        void disconnect();

        void send(std::shared_ptr<stanza> data);

        void send_error(error_type_t type, char *text);

        void send_raw_string(const char *fmt, ...);
        void send_raw(std::span<std::byte> data);

        void handler_add(
              data_handler handler,
              const char *ns,
              const char *name,
              const char *type,
              void *userdata
        );
        
        void handler_delete(
            data_handler handler
        );

        void id_handler_add(
            data_handler handler,
            const char *id,
            void *userdata
        );

        void id_handler_delete(
            data_handler handler,
            const char *id
        );

    private:
        std::pmr::memory_resource* m_allocator;
        context* m_context;

        std::pmr::string m_jid;
        std::pmr::string m_password;
    };
}