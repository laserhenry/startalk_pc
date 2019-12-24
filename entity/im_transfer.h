//
// Created by lihaibin on 2019-07-03.
//

#ifndef QTALK_V2_IM_TRANSFER_H
#define QTALK_V2_IM_TRANSFER_H

#include <string>

namespace QTalk {
    namespace Entity {
        struct ImTransfer
        {
            std::string userId{};
            std::string userName{};
        };
    }
}

#endif //QTALK_V2_IM_TRANSFER_H
