cmake_minimum_required(VERSION 2.8)
project (izakObjFinder)

# The version number.
#set (Tutorial_VERSION_MAJOR 1)
#set (Tutorial_VERSION_MINOR 0)

 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
GET_FILENAME_COMPONENT(PROJECT_ROOT . ABSOLUTE CACHE INTERNAL "Path prefix for the project")
include_directories("${PROJECT_ROOT}/include/" "${PROJECT_ROOT}/MedianFlowTracker/" "${PROJECT_ROOT}/MedianFlowTracker/mftracker")


#########################################################
# FIND OPENCV
#########################################################
find_package( OpenCV REQUIRED )

#########################################################
# FIND PTHREADS
#########################################################
find_package (Threads)


########################################################
# Linking & stuff
########################################################

option(POST_BUILD_COPY_FILES "Copy resource files after build" YES)
MACRO(COPY_FILES TAR DST)
	if(POST_BUILD_COPY_FILES)
		foreach(SRC ${ARGN})
			add_custom_command(TARGET ${TAR} POST_BUILD COMMAND ${CMAKE_COMMAND} -DSRC="${SRC}" -DDST="${DST}" -P "${CMAKE_SOURCE_DIR}/copy.cmake")
		endforeach(SRC)
	endif(POST_BUILD_COPY_FILES)
ENDMACRO(COPY_FILES TAR DST)

set(RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT}/build/)
set(APP_FILES ${PROJECT_ROOT}/Resources/tldConfig.txt)

set(MF_TRACKER_ROOT_DIR MedianFlowTracker/)
set(MF_TRACKER_DIR MedianFlowTracker/mftracker)

set(MEDIAN_FLOW_TRACKER_SRC "${MF_TRACKER_DIR}/bb.cpp" 
							"${MF_TRACKER_DIR}/bb_predict.cpp" 
							"${MF_TRACKER_DIR}/fbtrack.cpp" 
							"${MF_TRACKER_DIR}/lk.cpp" 
							"${MF_TRACKER_DIR}/median.cpp" 
							"${MF_TRACKER_ROOT_DIR}/MedianFlowTracker.cpp"
	)
set(MEDIAN_FLOW_TRACKER_INC "${MF_TRACKER_DIR}/bb.h" 
							"${MF_TRACKER_DIR}/bb_predict.h" 
							"${MF_TRACKER_DIR}/fbtrack.h" 
							"${MF_TRACKER_DIR}/lk.h" 
							"${MF_TRACKER_DIR}/median.h" 
							"${MF_TRACKER_ROOT_DIR}/MedianFlowTracker.h" 
	)

set(IZAK_OBJ_FINDER_SRC
						
						src/boundingbox.cpp 
						src/clusterer.cpp 
						src/detector.cpp 
						src/disjointset.cpp 
						src/feature.cpp 
						src/fern.cpp 
						src/ffclassifier.cpp 
						src/nnclassifier.cpp 
						src/normalizedpatch.cpp 
						src/params.cpp 
						src/Predator.cpp 
						src/tld_util.cpp 
						src/twobitbp.cpp 
						src/featureTracker.cpp
						src/variancefilter.cpp
						)
set(IZAK_OBJ_FINDER_INC
						include/boundingbox.h 
						include/clusterer.h 
						include/detector.h 
						include/disjointset.h 
						include/feature.h 
						include/fern.h 
						include/ffclassifier.h 
						include/nnclassifier.h 
						include/normalizedpatch.h 
						include/params.h 
						include/Predator.h 
						include/tld_util.h 
						include/twobitbp.h 
						include/serializable.h 
						include/featureTracker.h 
						include/variancefilter.h
						)

set(IZAK_DEMO_APP_SRC src/main.cpp src/main_skeleton.cpp )
set(IZAK_DEMO_APP_INC include/main_skeleton.h )


add_library( mftracker STATIC ${MEDIAN_FLOW_TRACKER_SRC} ${MEDIAN_FLOW_TRACKER_INC} )
target_link_libraries( mftracker ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT}  )

add_library( predator STATIC ${IZAK_OBJ_FINDER_SRC} ${IZAK_OBJ_FINDER_INC} )
target_link_libraries( predator ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} mftracker  )

add_executable( izakObjFinder ${IZAK_DEMO_APP_SRC} ${IZAK_DEMO_APP_INC}  )

#set_target_properties( featureObjectTracker PROPERTIES COMPILE_FLAGS "-D_TRACKER_DEMO_MAIN_" )
target_link_libraries( izakObjFinder ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} mftracker predator )

COPY_FILES(izakObjFinder ${RUNTIME_OUTPUT_DIRECTORY} ${APP_FILES})

add_executable( test src/test.cpp  )
target_link_libraries( test ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} mftracker predator )





