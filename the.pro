QT += core gui widgets multimedia multimediawidgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        backward_button.cpp \
        custom_textbox.cpp \
        forward_button.cpp \
        library.cpp \
        my_videowidget.cpp \
        play_pause.cpp \
        repeat_button.cpp \
        search_button.cpp \
        shuffle_button.cpp \
        the_button.cpp \
        the_player.cpp \
        time_slider.cpp \
        tomeo.cpp

HEADERS += \
    backward_button.h \
    custom_textbox.h \
    forward_button.h \
    library.h \
    my_videowidget.h \
    play_pause.h \
    repeat_button.h \
    search_button.h \
    shuffle_button.h \
    the_button.h \
    the_player.h \
    time_slider.h \
    tomeo.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

