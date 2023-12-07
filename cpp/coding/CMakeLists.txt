# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.5)

# 项目信息,项目名称Demo2
project (cpp LANGUAGES CXX)

# C++ 17
set(CMAKE_CXX_STANDARD 17)   

#查找当前目录下所有源文件
#并将名称保存到DIR_SRCS变量
aux_source_directory(. DIR_SRCS)

#打开全局moc
set(CMAKE_AUTOMOC ON)
#打开全局uic
set(CMAKE_AUTOUIC ON)
#打开全局rcc，如果没有使用qrc，此句可以去掉
set(CMAKE_AUTORCC ON)

#设置工程包含当前目录，使用*.ui文件时，需要加上这句，否则找不到头文件
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#指定Qt路径和启用当前目录(按需设置)
SET(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)
#SET(CMAKE_PREFIX_PATH D:/qt/qt/5.9.3/msvc2017_64/lib/cmake)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#引入Qt库
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Sql REQUIRED)
FIND_PACKAGE(Qt5Xml REQUIRED)
FIND_PACKAGE(Qt5WebEngineWidgets REQUIRED)
# FIND_PACKAGE(Qt5Network REQUIRED)
# FIND_PACKAGE(Qt5Concurrent REQUIRED)

# 添加资源文件  
set(RESOURCE_FILES kxmlreader.qrc)  
qt5_add_resources(RESOURCE_SOURCES ${RESOURCE_FILES})  

#创建工程文件
add_executable(cpp ${DIR_SRCS} ${RESOURCE_SOURCES})

#添加Qt5依赖项
target_link_libraries(cpp Qt5::Widgets Qt5::Sql Qt5::Xml Qt5::WebEngineWidgets)