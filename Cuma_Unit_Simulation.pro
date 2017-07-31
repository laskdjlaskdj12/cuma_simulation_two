QT += core network sql testlib
QT -= gui

CONFIG += c++11

TARGET = Cuma_Unit_Simulation
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Cuma_Main_Unit/cuma_main.cpp \
    Cuma_Main_Unit/Cuma_File/cuma_file.cpp \
    Cuma_Unit_Base/cuma_unit_base.cpp \
    Test/cuma_main_unit_test.cpp \
    Cuma_Debug/cuma_debug.cpp \
    Cuma_Main_Unit/Cuma_Peer_Server/cuma_peer_server.cpp \
    Cuma_Main_Unit/Cuma_Peer_Client/cuma_peer_client.cpp \
    Cuma_Main_Unit/Cuma_Peer_Protocol/cuma_peer_protocol.cpp \
    Test/cuma_file_test.cpp \
    Test/cuma_client_test.cpp \
    Test/cuma_server_test.cpp \
    Test/cuma_test_env.cpp \
    Test/cuma_unit_base_test.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    Cuma_Main_Unit/cuma_main.h \
    Cuma_Main_Unit/Cuma_File/cuma_file.h \
    Cuma_Unit_Base/cuma_unit_base.h \
    Test/cuma_main_unit_test.h \
    Cuma_Debug/cuma_debug.h \
    Test/cuma_file_test.h \
    Cuma_Main_Unit/Cuma_Peer_Protocol/cuma_peer_protocol.h \
    Cuma_Main_Unit/Cuma_File/cuma_file_info_struct.h \
    Test/cuma_client_test.h \
    Test/cuma_server_test.h \
    Test/cuma_test_env.h \
    Test/cuma_unit_base_test.h

STATECHARTS +=
