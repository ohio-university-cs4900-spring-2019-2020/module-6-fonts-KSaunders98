#pragma once

#include <string>

#include "Mat4.h"
#include "NetMsg.h"
#include "Vector.h"

#ifdef AFTR_CONFIG_USE_BOOST

namespace Aftr {
// message for updating other player's camera
class NetMsgUpdateCamera : public NetMsg {
public:
    NetMsgMacroDeclaration(NetMsgUpdateCamera);

    NetMsgUpdateCamera();
    virtual bool toStream(NetMessengerStreamBuffer& os) const;
    virtual bool fromStream(NetMessengerStreamBuffer& is);
    virtual void onMessageArrived();
    virtual std::string toString() const;

    unsigned int id;
    Mat4 displayMatrix;
    Vector position;
};
}

#endif