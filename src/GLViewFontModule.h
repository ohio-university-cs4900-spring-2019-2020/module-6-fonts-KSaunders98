#pragma once

#include <memory>

#include "GLView.h"

namespace Aftr {
class Camera;
class NetMessengerClient;
class WOFTGLString;
class WOGUILabel;

/**
   \class GLViewFontModule
   \author Scott Nykl 
   \brief A child of an abstract GLView. This class is the top-most manager of the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

class GLViewFontModule : public GLView {
public:
    static GLViewFontModule* New(const std::vector<std::string>& outArgs);
    virtual ~GLViewFontModule();
    virtual void updateWorld(); ///< Called once per frame
    virtual void loadMap(); ///< Called once at startup to build this module's scene
    virtual void onResizeWindow(GLsizei width, GLsizei height);
    virtual void onMouseDown(const SDL_MouseButtonEvent& e);
    virtual void onMouseUp(const SDL_MouseButtonEvent& e);
    virtual void onMouseMove(const SDL_MouseMotionEvent& e);
    virtual void onKeyDown(const SDL_KeyboardEvent& key);
    virtual void onKeyUp(const SDL_KeyboardEvent& key);

    void updateCamera(const Mat4& displayMatrix, const Vector& position);
    void newChatMessage(const std::string& msg);

protected:
    GLViewFontModule(const std::vector<std::string>& args);
    virtual void onCreate();

    std::unique_ptr<NetMessengerClient> netClient;
    std::string playerString;
    WO* cameraWO;
    WOFTGLString* stringWO;
    WOGUILabel* chatLabel;
    std::string chatMessage;
    bool typing;
};

/** \} */

} //namespace Aftr
