project(ElfReader)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.9.9\\5.9.9\\msvc2015\\")
# 用来搜索qt的各种库 不调用find_package可能会导致后边链接不通过
# linux或mac下还需要单独设置上qt的路径 find_package才能找到对应的库
# windows下设置好QT环境变量就不需要上边的set

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

file(GLOB SRC_FILES "./*.cpp" "./qhexedit2/*.cpp")
file(GLOB HEAD_FILES "./*.h" "./qhexedit2/*.h")
file(GLOB UI_FILES "./*.ui")

qt5_wrap_ui(WRAP_FILES ${UI_FILES})
# 如果有图片等资源文件qrc
# set(RCC_FILES resources.qrc)

# 把ui相关文件单建一个文件夹
source_group("Ui" FILES ${UI_FILES} ${WRAP_FILES})
add_executable(ElfReader ${SRC_FILES} ${HEAD_FILES} ${RCC_FILES} ${WRAP_FILES})

target_link_libraries(ElfReader Qt5::Widgets Qt5::Core Qt5::Gui )

set_target_properties(ElfReader PROPERTIES 
        WIN32_EXECUTABLE YES
        LINK_FLAGS "/ENTRY:mainCRTStartup /SAFESEH:NO")
