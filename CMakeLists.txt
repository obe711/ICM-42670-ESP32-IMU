set(COMPONENT_SRCS
    src/icm42670.c
    src/icm42670-esp32.c
)

set(COMPONENT_ADD_INCLUDEDIRS
    inc/
    src/
)

set(COMPONENT_REQUIRES driver)

register_component()
