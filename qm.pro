QT += widgets
TARGET = blockbuilder
CONFIG  += qt debug
INCLUDEPATH += src
VPATH += src
HEADERS +=  main_window.h           \
            model/block.h           \
            model/port.h            \
            model/schema.h          \
            model/vyraz.h           \
            model/prepojenie.h      \
            model/data_type.h       \
            ui/block_widget.h       \
            ui/schema_widget.h      \
            ui/resizable_frame.h    \
 
SOURCES += main_window.cpp          \
           model/block.cpp          \
           model/schema.cpp         \
           model/vyraz.cpp          \
           ui/block_widget.cpp      \
           ui/schema_widget.cpp     \
           main.cpp                 \
           ui/resizable_frame.cpp   \