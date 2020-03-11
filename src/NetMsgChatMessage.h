#pragma once

#include <string>

#include "NetMsg.h"

#ifdef AFTR_CONFIG_USE_BOOST

namespace Aftr {
// message for updating other player's camera
class NetMsgChatMessage : public NetMsg {
public:
    NetMsgMacroDeclaration(NetMsgChatMessage);

    NetMsgChatMessage();
    virtual bool toStream(NetMessengerStreamBuffer& os) const;
    virtual bool fromStream(NetMessengerStreamBuffer& is);
    virtual void onMessageArrived();
    virtual std::string toString() const;

    std::string msg;
};
}

#endif