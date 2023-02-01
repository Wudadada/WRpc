#ifndef __WQD_HTTP_SERVLETS_CONFIG_SERVLET_H__
#define __WQD_HTTP_SERVLETS_CONFIG_SERVLET_H__

#include "wqd/http/servlet.h"

namespace wqd {
namespace http {

class ConfigServlet : public Servlet {
public:
    ConfigServlet();
    virtual int32_t handle(wqd::http::HttpRequest::ptr request
                   , wqd::http::HttpResponse::ptr response
                   , wqd::http::HttpSession::ptr session) override;
};

}
}

#endif
