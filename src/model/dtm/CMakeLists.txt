set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_SSE2 -DDSFMT_MEXP=19937")  # for dSFMT

set(SOURCE_FILES ${BigTopicModel_SOURCE_DIR}/third_party/dSFMT/dSFMT.cpp
    main.cpp lcorpus.cpp lcorpus.h random.h aliastable.h utils.h pdtm.cpp pdtm.h)

include_directories(${BigTopicModel_SOURCE_DIR}/src/util)
include_directories(${BigTopicModel_SOURCE_DIR}/src/model)
include_directories(${BigTopicModel_SOURCE_DIR}/src/engine)
include_directories(${BigTopicModel_SOURCE_DIR}/third_party)

add_executable(dtm ${SOURCE_FILES})
target_link_libraries(dtm ${MPI_CXX_LIBRARIES} ${GLOG} ${GFLAGS})

add_executable(dtm_fmt dtm_formatter.cpp)
target_link_libraries(dtm_fmt ${GLOG} ${GFLAGS})
