TEMPLATE = app

QT += qml quick widgets

QMAKE_CXXFLAGS = -std=c++11

SOURCES += main.cpp \
	BeachPlayer.cpp \
	Engine.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
	BeachPlayer.hpp \
	Macros.hpp \
	Engine.hpp

RC_FILE = IconFile.rc

OTHER_FILES += \
	IconFile.rc \
	AppIcon.ico
