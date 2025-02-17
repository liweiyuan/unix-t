cmake_minimum_required(VERSION 3.20)
project(unix-t)

include_directories(/usr/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
message(STATUS "include dir: ${CMAKE_SOURCE_DIR}/include")

add_subdirectory(csapp)

add_executable(main main.c)

target_link_libraries(main libapue.a)


##exit
add_executable(aexit exit/aexit.c)
target_link_libraries(aexit libapue.a)
add_executable(environ exit/environ.c)
target_link_libraries(environ libapue.a)
add_executable(skip exit/skip.c)
target_link_libraries(skip libapue.a)
add_executable(rlimit exit/getrlimit.c)
target_link_libraries(rlimit libapue.a)

### io
add_executable(io_char io/char.c)
target_link_libraries(io_char libapue.a)

add_executable(io_line io/line.c)
target_link_libraries(io_line libapue.a)
add_executable(io_read io/read.c)
target_link_libraries(io_read libapue.a)
add_executable(io_text io/text.c)
target_link_libraries(io_text libapue.a)
add_executable(io_temp io/temp.c)
target_link_libraries(io_temp libapue.a)
add_executable(io_stream io/memopen.c)
target_link_libraries(io_stream libapue.a)

### file
add_executable(file_copy file/copy.c file/unix_file.c)
target_link_libraries(file_copy libapue.a)
add_executable(file_dup file/dup.c file/unix_file.c)
target_link_libraries(file_dup libapue.a)
add_executable(file_duo2 file/dup2.c file/unix_file.c)
target_link_libraries(file_duo2 libapue.a)
add_executable(file_double_write file/double_write.c file/unix_file.c)
target_link_libraries(file_double_write libapue.a)
add_executable(file_fsync file/fsync.c file/unix_file.c)
target_link_libraries(file_fsync libapue.a)
add_executable(file_fcntl_copy file/fcntl_copy.c file/unix_file.c)
target_link_libraries(file_fcntl_copy libapue.a)
add_executable(file_fcntl_set_get file/fcntl_get_set.c file/unix_file.c)
target_link_libraries(file_fcntl_set_get libapue.a)
add_executable(file_fcntl_lock file/fcntl_lock.c file/unix_file.c)
target_link_libraries(file_fcntl_lock libapue.a)
add_executable(file_fcntl_nonblock file/fcntl_nonblock.c file/unix_file.c)
target_link_libraries(file_fcntl_nonblock libapue.a)
add_executable(file_seek file/seek.c file/unix_file.c)
target_link_libraries(file_seek libapue.a)
add_executable(file_ioctl file/ioctl.c file/unix_file.c)
target_link_libraries(file_ioctl libapue.a)

### socket
add_executable(gethostent socket/gethostent.c)
target_link_libraries(gethostent libapue.a)

#### ipc
add_executable(ipc_pipe ipc/pipe.c)
target_link_libraries(ipc_pipe libapue.a)

add_subdirectory(dir)