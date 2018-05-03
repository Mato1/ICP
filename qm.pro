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
            ui/block_w.h              \
            ui/block_body_w.h         \
            ui/ports_w.h              \
            ui/port_w.h               \
            ui/resizable_frame.h    \
            ui/schema_w.h           \
            ui/vyraz_w.h            \
            ui/connection_w.h       \
 
SOURCES += main_window.cpp          \
           main.cpp                 \
           model/block.cpp          \
           model/schema.cpp         \
           model/vyraz.cpp          \
           ui/block_w.cpp       \
           ui/block_body_w.cpp        \
           ui/port_w.cpp        \
           ui/ports_w.cpp       \
           ui/resizable_frame.cpp   \
           ui/schema_w.cpp            \
           ui/vyraz_w.cpp              \
           ui/connection_w.cpp          \