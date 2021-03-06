# Copyright: 2011 RobotCub Consortium
# Author: Carlo Ciliberto
# CopyPolicy: Released under the terms of the GNU GPL v2.0.
#

cmake_minimum_required(VERSION 2.6.4)

set (PROJECTNAME boostMIL)

project(${PROJECTNAME})

set(folder_source src/MILClassifier.cpp
                  src/SelectorClassifier.cpp
                  src/StrongClassifier.cpp
                  src/OnlineBoost.cpp
                  src/OnlineSupport.cpp
                  src/AdaBoost.cpp
                  src/AvgClassifier.cpp)

set(folder_header include/iCub/boostMIL/WeakClassifier.h
                  include/iCub/boostMIL/ClassifierInput.h
                  include/iCub/boostMIL/ClassifierFactory.h
                  include/iCub/boostMIL/MILClassifier.h
                  include/iCub/boostMIL/SelectorClassifier.h
                  include/iCub/boostMIL/StrongClassifier.h
                  include/iCub/boostMIL/OnlineBoost.h
                  include/iCub/boostMIL/OnlineSupport.h
                  include/iCub/boostMIL/AdaBoost.h
                  include/iCub/boostMIL/OnlineSupport.h
                  include/iCub/boostMIL/AvgClassifier.h)

source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${YARP_INCLUDE_DIRS})

add_library(${PROJECTNAME} ${folder_source} ${folder_header})

#link other packages, this is required to let cmake know dependencies
#your code will compile without this, but you can have problems linking against
#your lib in modules using it
target_link_libraries(${PROJECTNAME} ${YARP_LIBRARIES})

icub_export_library(${PROJECTNAME} INTERNAL_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
                                   DESTINATION include/iCub/boostMIL
                                   FILES ${folder_header})

