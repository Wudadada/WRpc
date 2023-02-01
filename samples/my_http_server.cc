#include "wqd/http/http_server.h"
#include "wqd/log.h"

wqd::Logger::ptr g_logger = WQD_LOG_ROOT();
wqd::IOManager::ptr worker;
void run() {
    g_logger->setLevel(wqd::LogLevel::INFO);
    wqd::Address::ptr addr = wqd::Address::LookupAnyIPAddress("0.0.0.0:8020");
    if(!addr) {
        WQD_LOG_ERROR(g_logger) << "get address error";
        return;
    }

    wqd::http::HttpServer::ptr http_server(new wqd::http::HttpServer(true, worker.get()));
    //wqd::http::HttpServer::ptr http_server(new wqd::http::HttpServer(true));
    bool ssl = false;
    while(!http_server->bind(addr, ssl)) {
        WQD_LOG_ERROR(g_logger) << "bind " << *addr << " fail";
        sleep(1);
    }

    if(ssl) {
        //http_server->loadCertificates("/home/apps/soft/wqd/keys/server.crt", "/home/apps/soft/wqd/keys/server.key");
    }

    http_server->start();
}

int main(int argc, char** argv) {
    wqd::IOManager iom(1);
    worker.reset(new wqd::IOManager(4, false));
    iom.schedule(run);
    return 0;
}
