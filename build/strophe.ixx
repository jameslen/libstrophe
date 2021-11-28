module;

#include "strophe.h"

export module strophe;

import <chrono>;
import <memory>;
import <memory_resource>;
import <string>;

namespace xmpp
{
    export void initialize();
    export void shutdown();

    export bool version_check(int major, int minor);

    export enum class log_level
    { 
        debug,
        info,
        warn,
        error
    };

    enum class loop_status
    {
        not_started,
        running,
        quit
    };

    export enum class connection_type
    { 
        unknown,
        client,
        component
    };

    export using log_handler = void (*)(
        void *userdata,
        log_level level,
        const char *area,
        const char *msg
    );

    export struct logger
    {
      log_handler handler;
      void *userdata;
    };

    class connection;

    export class context
    {
    public:
        context(
            logger* logger,
            std::pmr::memory_resource* allocator
        ) noexcept;

        context(const context&) = delete;
        context(context&&) = delete;

        context &operator=(const context&) = delete;
        context &operator-(context&&) = delete;

        ~context() = default;

        void set_verbosity(log_level level) noexcept;

        std::shared_ptr<connection> create_connection();

    private:
        static constexpr size_t c_default_loop_timeout = 1000;

        std::pmr::memory_resource *mem{};
        const xmpp_log_t *log{};
        int verbosity;

        xmpp_rand_t *rand{};
        loop_status loop_status = loop_status::not_started;
        xmpp_connlist_t *connlist{};
        xmpp_handlist_t *timed_handlers{};

        unsigned long timeout = c_default_loop_timeout;

        friend class connection;
    };

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

        uint32_t get_flags() const;
        bool set_flags(uint32_t flags);

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
            cstd::string_view altdomain, 
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

        void send(stanza_t *stanza);

        void send_error(error_type_t type, char *text);

        void send_raw_string(const char *fmt, ...);
        void send_raw(std::span<std::byte> data);
    private:
        std::pmr::memory_resource* m_allocator;
        context* m_context;

        std::pmr::string m_jid;
        std::pmr::string m_password;
    };
}