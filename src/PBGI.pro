TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../dependencies/include
QMAKE_CXXFLAGS += -DGLM_FORCE_RADIANS

win32 {
INCLUDEPATH += C:/Coding/Librarys/include
LIBS        += -LC:/Coding/Librarys/lib/32
LIBS        += -lglew32 -lGLFW3 -lopengl32 -lgdi32 -mwindows
}

linux {
LIBS        +=  -lGLEW -lglfw3 -lGL -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread
}

SOURCES += \
    demo.cpp \
    main.cpp \
    Engine/camera.cpp \
    Engine/fast2dquad.cpp \
    Engine/object.cpp \
    Utils/abstractshader.cpp \
    Utils/clock.cpp \
    Utils/computeshader.cpp \
    Utils/fbo.cpp \
    Utils/fragmentshader.cpp \
    Utils/geometryshader.cpp \
    Utils/loadfile.cpp \
    Utils/shaderprogram.cpp \
    Utils/vertexshader.cpp \
    GI/uvmapper.cpp \
    Engine/texture.cpp \
    Engine/scenemanager.cpp \
    GI/radiosity.cpp \
    Engine/light.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    demo.h \
    Engine/camera.h \
    Engine/fast2dquad.h \
    Engine/object.h \
    Utils/abstractshader.h \
    Utils/clock.h \
    Utils/computeshader.h \
    Utils/fbo.h \
    Utils/fragmentshader.h \
    Utils/geometryshader.h \
    Utils/loadfile.h \
    Utils/shaderprogram.h \
    Utils/utils.h \
    Utils/vertexshader.h \
    GI/uvmapper.h \
    Engine/texture.h \
    Engine/scenemanager.h \
    GI/radiosity.h \
    Engine/light.h

DISTFILES += \
    ../release/Data/Shaders/basic.fsh \
    ../release/Data/Shaders/depth.fsh \
    ../release/Data/Shaders/postFX.fsh \
    ../release/Data/Shaders/basic.vsh \
    ../release/Data/Shaders/depth.vsh \
    ../release/Data/Shaders/postFX.vsh \
    ../release/Data/Shaders/Utils/basic.fsh \
    ../release/Data/Shaders/Utils/dbg.fsh \
    ../release/Data/Shaders/Utils/basic.vsh \
    ../release/Data/Shaders/Utils/dbg.vsh \
    ../release/Data/Shaders/GI/buffers.fsh \
    ../release/Data/Shaders/GI/buffers.gsh \
    ../release/Data/Shaders/GI/buffers.vsh \
    ../release/Data/Shaders/GI/radiosity.csh \
    ../release/Data/Shaders/GI/timelerp.csh \
    ../release/Data/Shaders/GI/copy.csh \
    ../release/Data/Shaders/sky.fsh \
    ../release/Data/Shaders/sky.vsh

