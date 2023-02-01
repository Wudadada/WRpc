#ifndef __WQD_LIBRARY_H__
#define __WQD_LIBRARY_H__

#include <memory>
#include "module.h"

namespace wqd {

class Library {
public:
    static Module::ptr GetModule(const std::string& path);
};

}

#endif
