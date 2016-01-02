TEMPLATE = subdirs

SUBDIRS += common server client

server.depends = common
client.depends = common

CONFIG += ordered

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/common/release/ -lcommon
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/common/debug/ -lcommon
#else:unix: LIBS += -L$$OUT_PWD/common/ -lcommon

#INCLUDEPATH += $$PWD/common
#DEPENDPATH += $$PWD/common
