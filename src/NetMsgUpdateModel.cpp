#include "NetMsgUpdateCamera.h"

#include <sstream>

#include "GLViewFontModule.h"
#include "ManagerGLView.h"

using namespace Aftr;

NetMsgMacroDefinition(NetMsgUpdateCamera);

NetMsgUpdateCamera::NetMsgUpdateCamera()
{
    id = 0;
    displayMatrix = Mat4();
    position = Vector(0, 0, 0);
}

bool NetMsgUpdateCamera::toStream(NetMessengerStreamBuffer& os) const
{
    os << id;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            os << displayMatrix[i * 4 + j];
        }
    }
    os << position.x << position.y << position.z;

    return true;
}

bool NetMsgUpdateCamera::fromStream(NetMessengerStreamBuffer& is)
{
    is >> id;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            is >> displayMatrix[i * 4 + j];
        }
    }
    displayMatrix[15] = 1;
    is >> position.x >> position.y >> position.z;

    return true;
}

void NetMsgUpdateCamera::onMessageArrived()
{
    // call updateCamera in GLView
    ManagerGLView::getGLView<GLViewFontModule>()->updateCamera(displayMatrix, position);
}

std::string NetMsgUpdateCamera::toString() const
{
    //std::stringstream ss;
    //ss << "UpdateCamera | " << displayMatrix << " | " << position;
    //return ss.str();
    return "";
}