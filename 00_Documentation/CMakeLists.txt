file(GLOB DOCS *.md)


add_custom_target(Documentation SOURCES ${DOCS} ../README.md)
