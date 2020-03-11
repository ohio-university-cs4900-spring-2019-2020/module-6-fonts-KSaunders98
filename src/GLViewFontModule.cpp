#include "GLViewFontModule.h"

#include "Axes.h" //We can set Axes to on/off with this
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "PhysicsEngineODE.h"
#include "WorldList.h" //This is where we place all of our WOs

//Different WO used by this module
#include "AftrGLRendererBase.h"
#include "Camera.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "CameraChaseActorSmooth.h"
#include "CameraStandard.h"
#include "MGLFTGLString.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WO.h"
#include "WOCar1970sBeater.h"
#include "WOFTGLString.h"
#include "WOGUILabel.h"
#include "WOHumanCal3DPaladin.h"
#include "WOHumanCyborg.h"
#include "WOLight.h"
#include "WONVDynSphere.h"
#include "WONVPhysX.h"
#include "WONVStaticPlane.h"
#include "WOSkyBox.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOWayPointSpherical.h"

using namespace Aftr;

GLViewFontModule* GLViewFontModule::New(const std::vector<std::string>& args)
{
    GLViewFontModule* glv = new GLViewFontModule(args);
    glv->init(Aftr::GRAVITY, Vector(0, 0, -1.0f), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE);
    glv->onCreate();
    return glv;
}

GLViewFontModule::GLViewFontModule(const std::vector<std::string>& args)
    : GLView(args)
{
    //Initialize any member variables that need to be used inside of LoadMap() here.
    //Note: At this point, the Managers are not yet initialized. The Engine initialization
    //occurs immediately after this method returns (see GLViewFontModule::New() for
    //reference). Then the engine invoke's GLView::loadMap() for this module.
    //After loadMap() returns, GLView::onCreate is finally invoked.

    //The order of execution of a module startup:
    //GLView::New() is invoked:
    //    calls GLView::init()
    //       calls GLView::loadMap() (as well as initializing the engine's Managers)
    //    calls GLView::onCreate()

    //GLViewFontModule::onCreate() is invoked after this module's LoadMap() is completed.
    typing = false;
}

void GLViewFontModule::onCreate()
{
    //GLViewFontModule::onCreate() is invoked after this module's LoadMap() is completed.
    //At this point, all the managers are initialized. That is, the engine is fully initialized.

    if (this->pe != NULL) {
        //optionally, change gravity direction and magnitude here
        //The user could load these values from the module's aftr.conf
        this->pe->setGravityNormalizedVector(Vector(0, 0, -1.0f));
        this->pe->setGravityScalar(Aftr::GRAVITY);
    }
    this->setActorChaseType(STANDARDEZNAV); //Default is STANDARDEZNAV mode
    //this->setNumPhysicsStepsPerRender( 0 ); //pause physics engine on start up; will remain paused till set to 1
}

GLViewFontModule::~GLViewFontModule()
{
    //Implicitly calls GLView::~GLView()
}

void GLViewFontModule::updateWorld()
{
    GLView::updateWorld(); //Just call the parent's update world first.
        //If you want to add additional functionality, do it after
        //this call.

    Vector disp = getCamera()->getPosition() - stringWO->getPosition();
    auto angle = atan2(disp.y, disp.x) + Aftr::PI / 2;

    Mat4 m;
    m = Mat4::rotateIdentityMat(Vector(1, 0, 0), Aftr::PI / 2);
    m = m.rotate(Vector(0, 0, 1), angle);
    stringWO->getModel()->setDisplayMatrix(m);

    Vector pos = Vector();
}

void GLViewFontModule::onResizeWindow(GLsizei width, GLsizei height)
{
    GLView::onResizeWindow(width, height); //call parent's resize method.
}

void GLViewFontModule::onMouseDown(const SDL_MouseButtonEvent& e)
{
    GLView::onMouseDown(e);
}

void GLViewFontModule::onMouseUp(const SDL_MouseButtonEvent& e)
{
    GLView::onMouseUp(e);
}

void GLViewFontModule::onMouseMove(const SDL_MouseMotionEvent& e)
{
    GLView::onMouseMove(e);
}

void GLViewFontModule::onKeyDown(const SDL_KeyboardEvent& key)
{
    GLView::onKeyDown(key);
    if (key.keysym.sym == SDLK_0)
        this->setNumPhysicsStepsPerRender(1);

    if (key.keysym.sym == SDLK_RETURN) {
        if (typing) {
            typing = false;
            if (!chatMessage.empty()) {
                std::cout << "message: " << chatMessage << std::endl;
                chatMessage = "";
            }
            chatLabel->setText("Press enter to type a message and enter again to send");
        } else {
            typing = true;
            chatLabel->setText("");
        }
    } else {
        if (typing) {
            // this is a BAD way of getting user text input, but good enough for a demo
            chatMessage += SDL_GetKeyName(key.keysym.sym);
            chatLabel->setText(chatMessage);
        }
    }
}

void GLViewFontModule::onKeyUp(const SDL_KeyboardEvent& key)
{
    GLView::onKeyUp(key);
}

void Aftr::GLViewFontModule::loadMap()
{
    this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
    this->actorLst = new WorldList();
    this->netLst = new WorldList();

    ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
    ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
    ManagerOpenGLState::enableFrustumCulling = false;
    Axes::isVisible = true;
    this->glRenderer->isUsingShadowMapping(false); //set to TRUE to enable shadow mapping, must be using GL 3.2+

    this->cam->setPosition(15, 15, 10);

    std::string shinyRedPlasticCube(ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl");
    std::string wheeledCar(ManagerEnvironmentConfiguration::getSMM() + "/models/rcx_treads.wrl");
    std::string grass(ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl");
    std::string human(ManagerEnvironmentConfiguration::getSMM() + "/models/human_chest.wrl");
    std::string comicSans(ManagerEnvironmentConfiguration::getSMM() + "/fonts/COMIC.ttf");
    std::string cameraModel(ManagerEnvironmentConfiguration::getLMM() + "/models/filmCamera.fbx");

    //SkyBox Textures readily available
    std::vector<std::string> skyBoxImageNames; //vector to store texture paths
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_water+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_dust+6.jpg" );
    skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_mountains+6.jpg");
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_winter+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/early_morning+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_afternoon+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy3+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day2+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_deepsun+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_evening+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning2+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_noon+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_warp+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_Hubble_Nebula+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_gray_matter+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_easter+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_hot_nebula+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_ice_field+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_lemon_lime+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_milk_chocolate+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_solar_bloom+6.jpg" );
    //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_thick_rb+6.jpg" );

    float ga = 0.1f; //Global Ambient Light level for this module
    ManagerLight::setGlobalAmbientLight(aftrColor4f(ga, ga, ga, 1.0f));
    WOLight* light = WOLight::New();
    light->isDirectionalLight(true);
    light->setPosition(Vector(0, 0, 100));
    //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
    //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
    light->getModel()->setDisplayMatrix(Mat4::rotateIdentityMat({ 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD));
    light->setLabel("Light");
    worldLst->push_back(light);

    //Create the SkyBox
    WO* wo = WOSkyBox::New(skyBoxImageNames.at(0), this->getCameraPtrPtr());
    wo->setPosition(Vector(0, 0, 0));
    wo->setLabel("Sky Box");
    wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
    worldLst->push_back(wo);

    ////Create the infinite grass plane (the floor)
    wo = WO::New(grass, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
    wo->setPosition(Vector(0, 0, 0));
    wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
    ModelMeshSkin& grassSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
    grassSkin.getMultiTextureSet().at(0)->setTextureRepeats(5.0f);
    grassSkin.setAmbient(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f)); //Color of object when it is not in any light
    grassSkin.setDiffuse(aftrColor4f(1.0f, 1.0f, 1.0f, 1.0f)); //Diffuse color components (ie, matte shading color of this object)
    grassSkin.setSpecular(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f)); //Specular color component (ie, how "shiney" it is)
    grassSkin.setSpecularCoefficient(10); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
    wo->setLabel("Grass");
    worldLst->push_back(wo);

    cameraWO = WO::New(cameraModel, Vector(0.25f, 0.25f, 0.25f), MESH_SHADING_TYPE::mstFLAT);
    cameraWO->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
    ModelMeshSkin& skin = cameraWO->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
    skin.setAmbient(aftrColor4f(0.5f, 0.1f, 0.5f, 1.0f));
    skin.setDiffuse(aftrColor4f(0.75f, 0.1f, 0.75f, 1.0f));
    skin.setSpecular(aftrColor4f(0.2f, 0.2f, 0.2f, 1.0f));
    worldLst->push_back(cameraWO);

    stringWO = WOFTGLString::New(comicSans, 30);
    stringWO->getModelT<MGLFTGLString>()->setFontColor(aftrColor4f(1.0f, 0.0f, 1.0f, 1.0f));
    stringWO->getModelT<MGLFTGLString>()->setSize(10, 3);
    stringWO->setText("Testing123 yay!");
    stringWO->setPosition(0, 0, 3);

    cameraWO->getChildren().push_back(stringWO);
    cameraWO->setPosition(Vector(10, 10, 10));

    chatLabel = WOGUILabel::New(nullptr);
    chatLabel->setText("Press enter to type a message and enter again to send");
    chatLabel->setColor(200, 200, 200);
    chatLabel->setFontSize(30);
    chatLabel->setPosition(Vector(0.5, 0.15, 0));
    chatLabel->setFontOrientation(FONT_ORIENTATION::foCENTER);
    chatLabel->setFontPath(comicSans);
    worldLst->push_back(chatLabel);
}

void GLViewFontModule::updateCamera(const Mat4& displayMatrix, const Vector& position)
{
    cameraWO->getModel()->setDisplayMatrix(displayMatrix);
    cameraWO->setPosition(position);
}
