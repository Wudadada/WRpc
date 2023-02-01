#include "wqd/http/http_server.h"
#include "wqd/log.h"

static wqd::Logger::ptr g_logger = WQD_LOG_ROOT();

#define XX(...) #__VA_ARGS__

void run() {
    g_logger->setLevel(wqd::LogLevel::INFO);
    //wqd::http::HttpServer::ptr server(new wqd::http::HttpServer(true, worker.get(), wqd::IOManager::GetThis()));
    wqd::http::HttpServer::ptr server(new wqd::http::HttpServer(true));
    wqd::Address::ptr addr = wqd::Address::LookupAnyIPAddress("0.0.0.0:8020");
    while(!server->bind(addr)) {
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    sd->addServlet("/wqd/xx", [](wqd::http::HttpRequest::ptr req
                ,wqd::http::HttpResponse::ptr rsp
                ,wqd::http::HttpSession::ptr session) {
            rsp->setBody(req->toString());
            return 0;
    });

    sd->addGlobServlet("/wqd/*", [](wqd::http::HttpRequest::ptr req
                ,wqd::http::HttpResponse::ptr rsp
                ,wqd::http::HttpSession::ptr session) {
            rsp->setBody("Glob:\r\n" + req->toString());
            return 0;
    });

    sd->addGlobServlet("/wqdx/*", [](wqd::http::HttpRequest::ptr req
                ,wqd::http::HttpResponse::ptr rsp
                ,wqd::http::HttpSession::ptr session) {
            rsp->setBody(XX(<html>
<head><title>404 Not Found</title></head>
<body>
<center><h1>404 Not Found</h1></center>
<hr><center>nginx/1.16.0</center>
</body>
</html>
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
));
            return 0;
    });

    server->start();
}

int main(int argc, char** argv) {
    wqd::IOManager iom(8, true, "main");
    iom.schedule(run);
    return 0;
}
