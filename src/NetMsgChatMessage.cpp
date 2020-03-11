#include "NetMsgChatMessage.h"

#include <sstream>

#include "GLViewFontModule.h"
#include "ManagerGLView.h"

using namespace Aftr;

NetMsgMacroDefinition(NetMsgChatMessage);

NetMsgChatMessage::NetMsgChatMessage()
{
    msg = "";
}

bool NetMsgChatMessage::toStream(NetMessengerStreamBuffer& os) const
{
    os << msg;

    return true;
}

bool NetMsgChatMessage::fromStream(NetMessengerStreamBuffer& is)
{
    is >> msg;

    return true;
}

void NetMsgChatMessage::onMessageArrived()
{
    // call updateCamera in GLView
    ManagerGLView::getGLView<GLViewFontModule>()->newChatMessage(msg);
}

std::string NetMsgChatMessage::toString() const
{
    std::stringstream ss;
    ss << "Chat Message: \"" << msg << "\"";
    return ss.str();
}