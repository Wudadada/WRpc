#ifndef __WQD_HTTP_HTTP_H__
#define __WQD_HTTP_HTTP_H__

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace wqd {
namespace http {

/* Request Methods */
#define HTTP_METHOD_MAP(XX)         \
  XX(0,  DELETE,      DELETE)       \
  XX(1,  GET,         GET)          \
  XX(2,  HEAD,        HEAD)         \
  XX(3,  POST,        POST)         \
  XX(4,  PUT,         PUT)          \
  /* pathological */                \
  XX(5,  CONNECT,     CONNECT)      \
  XX(6,  OPTIONS,     OPTIONS)      \
  XX(7,  TRACE,       TRACE)        \
  /* WebDAV */                      \
  XX(8,  COPY,        COPY)         \
  XX(9,  LOCK,        LOCK)         \
  XX(10, MKCOL,       MKCOL)        \
  XX(11, MOVE,        MOVE)         \
  XX(12, PROPFIND,    PROPFIND)     \
  XX(13, PROPPATCH,   PROPPATCH)    \
  XX(14, SEARCH,      SEARCH)       \
  XX(15, UNLOCK,      UNLOCK)       \
  XX(16, BIND,        BIND)         \
  XX(17, REBIND,      REBIND)       \
  XX(18, UNBIND,      UNBIND)       \
  XX(19, ACL,         ACL)          \
  /* subversion */                  \
  XX(20, REPORT,      REPORT)       \
  XX(21, MKACTIVITY,  MKACTIVITY)   \
  XX(22, CHECKOUT,    CHECKOUT)     \
  XX(23, MERGE,       MERGE)        \
  /* upnp */                        \
  XX(24, MSEARCH,     M-SEARCH)     \
  XX(25, NOTIFY,      NOTIFY)       \
  XX(26, SUBSCRIBE,   SUBSCRIBE)    \
  XX(27, UNSUBSCRIBE, UNSUBSCRIBE)  \
  /* RFC-5789 */                    \
  XX(28, PATCH,       PATCH)        \
  XX(29, PURGE,       PURGE)        \
  /* CalDAV */                      \
  XX(30, MKCALENDAR,  MKCALENDAR)   \
  /* RFC-2068, section 19.6.1.2 */  \
  XX(31, LINK,        LINK)         \
  XX(32, UNLINK,      UNLINK)       \
  /* icecast */                     \
  XX(33, SOURCE,      SOURCE)       \

/* Status Codes */
#define HTTP_STATUS_MAP(XX)                                                 \
  XX(100, CONTINUE,                        Continue)                        \
  XX(101, SWITCHING_PROTOCOLS,             Switching Protocols)             \
  XX(102, PROCESSING,                      Processing)                      \
  XX(200, OK,                              OK)                              \
  XX(201, CREATED,                         Created)                         \
  XX(202, ACCEPTED,                        Accepted)                        \
  XX(203, NON_AUTHORITATIVE_INFORMATION,   Non-Authoritative Information)   \
  XX(204, NO_CONTENT,                      No Content)                      \
  XX(205, RESET_CONTENT,                   Reset Content)                   \
  XX(206, PARTIAL_CONTENT,                 Partial Content)                 \
  XX(207, MULTI_STATUS,                    Multi-Status)                    \
  XX(208, ALREADY_REPORTED,                Already Reported)                \
  XX(226, IM_USED,                         IM Used)                         \
  XX(300, MULTIPLE_CHOICES,                Multiple Choices)                \
  XX(301, MOVED_PERMANENTLY,               Moved Permanently)               \
  XX(302, FOUND,                           Found)                           \
  XX(303, SEE_OTHER,                       See Other)                       \
  XX(304, NOT_MODIFIED,                    Not Modified)                    \
  XX(305, USE_PROXY,                       Use Proxy)                       \
  XX(307, TEMPORARY_REDIRECT,              Temporary Redirect)              \
  XX(308, PERMANENT_REDIRECT,              Permanent Redirect)              \
  XX(400, BAD_REQUEST,                     Bad Request)                     \
  XX(401, UNAUTHORIZED,                    Unauthorized)                    \
  XX(402, PAYMENT_REQUIRED,                Payment Required)                \
  XX(403, FORBIDDEN,                       Forbidden)                       \
  XX(404, NOT_FOUND,                       Not Found)                       \
  XX(405, METHOD_NOT_ALLOWED,              Method Not Allowed)              \
  XX(406, NOT_ACCEPTABLE,                  Not Acceptable)                  \
  XX(407, PROXY_AUTHENTICATION_REQUIRED,   Proxy Authentication Required)   \
  XX(408, REQUEST_TIMEOUT,                 Request Timeout)                 \
  XX(409, CONFLICT,                        Conflict)                        \
  XX(410, GONE,                            Gone)                            \
  XX(411, LENGTH_REQUIRED,                 Length Required)                 \
  XX(412, PRECONDITION_FAILED,             Precondition Failed)             \
  XX(413, PAYLOAD_TOO_LARGE,               Payload Too Large)               \
  XX(414, URI_TOO_LONG,                    URI Too Long)                    \
  XX(415, UNSUPPORTED_MEDIA_TYPE,          Unsupported Media Type)          \
  XX(416, RANGE_NOT_SATISFIABLE,           Range Not Satisfiable)           \
  XX(417, EXPECTATION_FAILED,              Expectation Failed)              \
  XX(421, MISDIRECTED_REQUEST,             Misdirected Request)             \
  XX(422, UNPROCESSABLE_ENTITY,            Unprocessable Entity)            \
  XX(423, LOCKED,                          Locked)                          \
  XX(424, FAILED_DEPENDENCY,               Failed Dependency)               \
  XX(426, UPGRADE_REQUIRED,                Upgrade Required)                \
  XX(428, PRECONDITION_REQUIRED,           Precondition Required)           \
  XX(429, TOO_MANY_REQUESTS,               Too Many Requests)               \
  XX(431, REQUEST_HEADER_FIELDS_TOO_LARGE, Request Header Fields Too Large) \
  XX(451, UNAVAILABLE_FOR_LEGAL_REASONS,   Unavailable For Legal Reasons)   \
  XX(500, INTERNAL_SERVER_ERROR,           Internal Server Error)           \
  XX(501, NOT_IMPLEMENTED,                 Not Implemented)                 \
  XX(502, BAD_GATEWAY,                     Bad Gateway)                     \
  XX(503, SERVICE_UNAVAILABLE,             Service Unavailable)             \
  XX(504, GATEWAY_TIMEOUT,                 Gateway Timeout)                 \
  XX(505, HTTP_VERSION_NOT_SUPPORTED,      HTTP Version Not Supported)      \
  XX(506, VARIANT_ALSO_NEGOTIATES,         Variant Also Negotiates)         \
  XX(507, INSUFFICIENT_STORAGE,            Insufficient Storage)            \
  XX(508, LOOP_DETECTED,                   Loop Detected)                   \
  XX(510, NOT_EXTENDED,                    Not Extended)                    \
  XX(511, NETWORK_AUTHENTICATION_REQUIRED, Network Authentication Required) \

/**
 * @brief HTTP????????????
 */
enum class HttpMethod {
#define XX(num, name, string) name = num,
    HTTP_METHOD_MAP(XX)
#undef XX
    INVALID_METHOD
};

/**
 * @brief HTTP????????????
 */
enum class HttpStatus {
#define XX(code, name, desc) name = code,
    HTTP_STATUS_MAP(XX)
#undef XX
};

/**
 * @brief ???????????????????????????HTTP????????????
 * @param[in] m HTTP??????
 * @return HTTP????????????
 */
HttpMethod StringToHttpMethod(const std::string& m);

/**
 * @brief ???????????????????????????HTTP????????????
 * @param[in] m ?????????????????????
 * @return HTTP????????????
 */
HttpMethod CharsToHttpMethod(const char* m);

/**
 * @brief ???HTTP??????????????????????????????
 * @param[in] m HTTP????????????
 * @return ?????????
 */
const char* HttpMethodToString(const HttpMethod& m);

/**
 * @brief ???HTTP??????????????????????????????
 * @param[in] m HTTP????????????
 * @return ?????????
 */
const char* HttpStatusToString(const HttpStatus& s);

/**
 * @brief ??????????????????????????????
 */
struct CaseInsensitiveLess {
    /**
     * @brief ??????????????????????????????
     */
    bool operator()(const std::string& lhs, const std::string& rhs) const;
};

/**
 * @brief ??????Map??????key???,?????????????????????,??????????????????
 * @param[in] m Map????????????
 * @param[in] key ?????????
 * @param[out] val ?????????????????????
 * @param[in] def ?????????
 * @return
 *      @retval true ????????????, val ???????????????
 *      @retval false ??????????????????????????? val = def
 */
template<class MapType, class T>
bool checkGetAs(const MapType& m, const std::string& key, T& val, const T& def = T()) {
    auto it = m.find(key);
    if(it == m.end()) {
        val = def;
        return false;
    }
    try {
        val = boost::lexical_cast<T>(it->second);
        return true;
    } catch (...) {
        val = def;
    }
    return false;
}

/**
 * @brief ??????Map??????key???,?????????????????????
 * @param[in] m Map????????????
 * @param[in] key ?????????
 * @param[in] def ?????????
 * @return ?????????????????????????????????????????????,?????????????????????
 */
template<class MapType, class T>
T getAs(const MapType& m, const std::string& key, const T& def = T()) {
    auto it = m.find(key);
    if(it == m.end()) {
        return def;
    }
    try {
        return boost::lexical_cast<T>(it->second);
    } catch (...) {
    }
    return def;
}

class HttpResponse;
/**
 * @brief HTTP????????????
 */
class HttpRequest {
public:
    /// HTTP?????????????????????
    typedef std::shared_ptr<HttpRequest> ptr;
    /// MAP??????
    typedef std::map<std::string, std::string, CaseInsensitiveLess> MapType;
    /**
     * @brief ????????????
     * @param[in] version ??????
     * @param[in] close ??????keepalive
     */
    HttpRequest(uint8_t version = 0x11, bool close = true);

    std::shared_ptr<HttpResponse> createResponse();

    /**
     * @brief ??????HTTP??????
     */
    HttpMethod getMethod() const { return m_method;}

    /**
     * @brief ??????HTTP??????
     */
    uint8_t getVersion() const { return m_version;}

    /**
     * @brief ??????HTTP???????????????
     */
    const std::string& getPath() const { return m_path;}

    /**
     * @brief ??????HTTP?????????????????????
     */
    const std::string& getQuery() const { return m_query;}

    /**
     * @brief ??????HTTP??????????????????
     */
    const std::string& getBody() const { return m_body;}

    /**
     * @brief ??????HTTP??????????????????MAP
     */
    const MapType& getHeaders() const { return m_headers;}

    /**
     * @brief ??????HTTP???????????????MAP
     */
    const MapType& getParams() const { return m_params;}

    /**
     * @brief ??????HTTP?????????cookie MAP
     */
    const MapType& getCookies() const { return m_cookies;}

    /**
     * @brief ??????HTTP??????????????????
     * @param[in] v HTTP??????
     */
    void setMethod(HttpMethod v) { m_method = v;}

    /**
     * @brief ??????HTTP?????????????????????
     * @param[in] v ????????????0x11, 0x10
     */
    void setVersion(uint8_t v) { m_version = v;}

    /**
     * @brief ??????HTTP???????????????
     * @param[in] v ????????????
     */
    void setPath(const std::string& v) { m_path = v;}

    /**
     * @brief ??????HTTP?????????????????????
     * @param[in] v ????????????
     */
    void setQuery(const std::string& v) { m_query = v;}

    /**
     * @brief ??????HTTP?????????Fragment
     * @param[in] v fragment
     */
    void setFragment(const std::string& v) { m_fragment = v;}

    /**
     * @brief ??????HTTP??????????????????
     * @param[in] v ?????????
     */
    void setBody(const std::string& v) { m_body = v;}

    /**
     * @brief ??????????????????
     */
    bool isClose() const { return m_close;}

    /**
     * @brief ????????????????????????
     */
    void setClose(bool v) { m_close = v;}

    /**
     * @brief ??????websocket
     */
    bool isWebsocket() const { return m_websocket;}

    /**
     * @brief ????????????websocket
     */
    void setWebsocket(bool v) { m_websocket = v;}

    /**
     * @brief ??????HTTP???????????????MAP
     * @param[in] v map
     */
    void setHeaders(const MapType& v) { m_headers = v;}

    /**
     * @brief ??????HTTP???????????????MAP
     * @param[in] v map
     */
    void setParams(const MapType& v) { m_params = v;}

    /**
     * @brief ??????HTTP?????????Cookie MAP
     * @param[in] v map
     */
    void setCookies(const MapType& v) { m_cookies = v;}

    /**
     * @brief ??????HTTP?????????????????????
     * @param[in] key ?????????
     * @param[in] def ?????????
     * @return ??????????????????????????????,?????????????????????
     */
    std::string getHeader(const std::string& key, const std::string& def = "") const;

    /**
     * @brief ??????HTTP?????????????????????
     * @param[in] key ?????????
     * @param[in] def ?????????
     * @return ??????????????????????????????,?????????????????????
     */
    std::string getParam(const std::string& key, const std::string& def = "");

    /**
     * @brief ??????HTTP?????????Cookie??????
     * @param[in] key ?????????
     * @param[in] def ?????????
     * @return ??????????????????????????????,?????????????????????
     */
    std::string getCookie(const std::string& key, const std::string& def = "");

    
    /**
     * @brief ??????HTTP?????????????????????
     * @param[in] key ?????????
     * @param[in] val ???
     */
    void setHeader(const std::string& key, const std::string& val);

    /**
     * @brief ??????HTTP?????????????????????
     * @param[in] key ?????????
     * @param[in] val ???
     */

    void setParam(const std::string& key, const std::string& val);
    /**
     * @brief ??????HTTP?????????Cookie??????
     * @param[in] key ?????????
     * @param[in] val ???
     */
    void setCookie(const std::string& key, const std::string& val);

    /**
     * @brief ??????HTTP?????????????????????
     * @param[in] key ?????????
     */
    void delHeader(const std::string& key);

    /**
     * @brief ??????HTTP?????????????????????
     * @param[in] key ?????????
     */
    void delParam(const std::string& key);

    /**
     * @brief ??????HTTP?????????Cookie??????
     * @param[in] key ?????????
     */
    void delCookie(const std::string& key);

    /**
     * @brief ??????HTTP?????????????????????????????????
     * @param[in] key ?????????
     * @param[out] val ????????????,val???????????????
     * @return ????????????
     */
    bool hasHeader(const std::string& key, std::string* val = nullptr);

    /**
     * @brief ??????HTTP?????????????????????????????????
     * @param[in] key ?????????
     * @param[out] val ????????????,val???????????????
     * @return ????????????
     */
    bool hasParam(const std::string& key, std::string* val = nullptr);

    /**
     * @brief ??????HTTP?????????Cookie??????????????????
     * @param[in] key ?????????
     * @param[out] val ????????????,val???????????????
     * @return ????????????
     */
    bool hasCookie(const std::string& key, std::string* val = nullptr);

    /**
     * @brief ???????????????HTTP?????????????????????
     * @tparam T ????????????
     * @param[in] key ?????????
     * @param[out] val ?????????
     * @param[in] def ?????????
     * @return ?????????????????????????????????true,????????????val=def
     */
    template<class T>
    bool checkGetHeaderAs(const std::string& key, T& val, const T& def = T()) {
        return checkGetAs(m_headers, key, val, def);
    }

    /**
     * @brief ??????HTTP?????????????????????
     * @tparam T ????????????
     * @param[in] key ?????????
     * @param[in] def ?????????
     * @return ?????????????????????????????????????????????,????????????def
     */
    template<class T>
    T getHeaderAs(const std::string& key, const T& def = T()) {
        return getAs(m_headers, key, def);
    }

    /**
     * @brief ???????????????HTTP?????????????????????
     * @tparam T ????????????
     * @param[in] key ?????????
     * @param[out] val ?????????
     * @param[in] def ?????????
     * @return ?????????????????????????????????true,????????????val=def
     */
    template<class T>
    bool checkGetParamAs(const std::string& key, T& val, const T& def = T()) {
        initQueryParam();
        initBodyParam();
        return checkGetAs(m_params, key, val, def);
    }

    /**
     * @brief ??????HTTP?????????????????????
     * @tparam T ????????????
     * @param[in] key ?????????
     * @param[in] def ?????????
     * @return ?????????????????????????????????????????????,????????????def
     */
    template<class T>
    T getParamAs(const std::string& key, const T& def = T()) {
        initQueryParam();
        initBodyParam();
        return getAs(m_params, key, def);
    }

    /**
     * @brief ???????????????HTTP?????????Cookie??????
     * @tparam T ????????????
     * @param[in] key ?????????
     * @param[out] val ?????????
     * @param[in] def ?????????
     * @return ?????????????????????????????????true,????????????val=def
     */
    template<class T>
    bool checkGetCookieAs(const std::string& key, T& val, const T& def = T()) {
        initCookies();
        return checkGetAs(m_cookies, key, val, def);
    }

    /**
     * @brief ??????HTTP?????????Cookie??????
     * @tparam T ????????????
     * @param[in] key ?????????
     * @param[in] def ?????????
     * @return ?????????????????????????????????????????????,????????????def
     */
    template<class T>
    T getCookieAs(const std::string& key, const T& def = T()) {
        initCookies();
        return getAs(m_cookies, key, def);
    }

    /**
     * @brief ????????????????????????
     * @param[in, out] os ?????????
     * @return ?????????
     */
    std::ostream& dump(std::ostream& os) const;

    /**
     * @brief ?????????????????????
     * @return ?????????
     */
    std::string toString() const;

    void init();
    void initParam();
    void initQueryParam();
    void initBodyParam();
    void initCookies();
private:
    /// HTTP??????
    HttpMethod m_method;
    /// HTTP??????
    uint8_t m_version;
    /// ??????????????????
    bool m_close;
    /// ?????????websocket
    bool m_websocket;

    uint8_t m_parserParamFlag;
    /// ????????????
    std::string m_path;
    /// ????????????
    std::string m_query;
    /// ??????fragment
    std::string m_fragment;
    /// ???????????????
    std::string m_body;
    /// ????????????MAP
    MapType m_headers;
    /// ????????????MAP
    MapType m_params;
    /// ??????Cookie MAP
    MapType m_cookies;
};

/**
 * @brief HTTP???????????????
 */
class HttpResponse {
public:
    /// HTTP????????????????????????
    typedef std::shared_ptr<HttpResponse> ptr;
    /// MapType
    typedef std::map<std::string, std::string, CaseInsensitiveLess> MapType;
    /**
     * @brief ????????????
     * @param[in] version ??????
     * @param[in] close ??????????????????
     */
    HttpResponse(uint8_t version = 0x11, bool close = true);

    /**
     * @brief ??????????????????
     * @return ????????????
     */
    HttpStatus getStatus() const { return m_status;}

    /**
     * @brief ??????????????????
     * @return ??????
     */
    uint8_t getVersion() const { return m_version;}

    /**
     * @brief ?????????????????????
     * @return ?????????
     */
    const std::string& getBody() const { return m_body;}

    /**
     * @brief ??????????????????
     */
    const std::string& getReason() const { return m_reason;}

    /**
     * @brief ??????????????????MAP
     * @return MAP
     */
    const MapType& getHeaders() const { return m_headers;}

    /**
     * @brief ??????????????????
     * @param[in] v ????????????
     */
    void setStatus(HttpStatus v) { m_status = v;}

    /**
     * @brief ??????????????????
     * @param[in] v ??????
     */
    void setVersion(uint8_t v) { m_version = v;}

    /**
     * @brief ?????????????????????
     * @param[in] v ?????????
     */
    void setBody(const std::string& v) { m_body = v;}

    /**
     * @brief ??????????????????
     * @param[in] v ??????
     */
    void setReason(const std::string& v) { m_reason = v;}

    /**
     * @brief ??????????????????MAP
     * @param[in] v MAP
     */
    void setHeaders(const MapType& v) { m_headers = v;}

    /**
     * @brief ??????????????????
     */
    bool isClose() const { return m_close;}

    /**
     * @brief ????????????????????????
     */
    void setClose(bool v) { m_close = v;}

    /**
     * @brief ??????websocket
     */
    bool isWebsocket() const { return m_websocket;}

    /**
     * @brief ????????????websocket
     */
    void setWebsocket(bool v) { m_websocket = v;}

    /**
     * @brief ????????????????????????
     * @param[in] key ?????????
     * @param[in] def ?????????
     * @return ???????????????????????????,????????????def
     */
    std::string getHeader(const std::string& key, const std::string& def = "") const;

    /**
     * @brief ????????????????????????
     * @param[in] key ?????????
     * @param[in] val ???
     */
    void setHeader(const std::string& key, const std::string& val);

    /**
     * @brief ????????????????????????
     * @param[in] key ?????????
     */
    void delHeader(const std::string& key);

    /**
     * @brief ?????????????????????????????????
     * @tparam T ?????????
     * @param[in] key ?????????
     * @param[out] val ???
     * @param[in] def ?????????
     * @return ?????????????????????????????????true,????????????val=def
     */
    template<class T>
    bool checkGetHeaderAs(const std::string& key, T& val, const T& def = T()) {
        return checkGetAs(m_headers, key, val, def);
    }

    /**
     * @brief ???????????????????????????
     * @tparam T ????????????
     * @param[in] key ?????????
     * @param[in] def ?????????
     * @return ?????????????????????????????????????????????,????????????def
     */
    template<class T>
    T getHeaderAs(const std::string& key, const T& def = T()) {
        return getAs(m_headers, key, def);
    }

    /**
     * @brief ?????????????????????
     * @param[in, out] os ?????????
     * @return ?????????
     */
    std::ostream& dump(std::ostream& os) const;

    /**
     * @brief ???????????????
     */
    std::string toString() const;

    void setRedirect(const std::string& uri);
    void setCookie(const std::string& key, const std::string& val,
                   time_t expired = 0, const std::string& path = "",
                   const std::string& domain = "", bool secure = false);
private:
    /// ????????????
    HttpStatus m_status;
    /// ??????
    uint8_t m_version;
    /// ??????????????????
    bool m_close;
    /// ?????????websocket
    bool m_websocket;
    /// ???????????????
    std::string m_body;
    /// ????????????
    std::string m_reason;
    /// ????????????MAP
    MapType m_headers;

    std::vector<std::string> m_cookies;
};

/**
 * @brief ????????????HttpRequest
 * @param[in, out] os ?????????
 * @param[in] req HTTP??????
 * @return ?????????
 */
std::ostream& operator<<(std::ostream& os, const HttpRequest& req);

/**
 * @brief ????????????HttpResponse
 * @param[in, out] os ?????????
 * @param[in] rsp HTTP??????
 * @return ?????????
 */
std::ostream& operator<<(std::ostream& os, const HttpResponse& rsp);

}
}

#endif
