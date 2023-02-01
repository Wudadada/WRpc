#include "wqd/socket.h"
#include "wqd/log.h"
#include "wqd/iomanager.h"

static wqd::Logger::ptr g_logger = WQD_LOG_ROOT();

void run() {
    wqd::IPAddress::ptr addr = wqd::Address::LookupAnyIPAddress("0.0.0.0:8050");
    wqd::Socket::ptr sock = wqd::Socket::CreateUDP(addr);
    if(sock->bind(addr)) {
        WQD_LOG_INFO(g_logger) << "udp bind : " << *addr;
    } else {
        WQD_LOG_ERROR(g_logger) << "udp bind : " << *addr << " fail";
        return;
    }
    while(true) {
        char buff[1024];
        wqd::Address::ptr from(new wqd::IPv4Address);
        int len = sock->recvFrom(buff, 1024, from);
        if(len > 0) {
            buff[len] = '\0';
            WQD_LOG_INFO(g_logger) << "recv: " << buff << " from: " << *from;
            len = sock->sendTo(buff, len, from);
            if(len < 0) {
                WQD_LOG_INFO(g_logger) << "send: " << buff << " to: " << *from
                    << " error=" << len;
            }
        }
    }
}

int main(int argc, char** argv) {
    wqd::IOManager iom(1);
    iom.schedule(run);
    return 0;
}
