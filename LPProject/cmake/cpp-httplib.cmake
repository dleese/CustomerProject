function(CppHttpLib)
  # Check if httplib is already available to avoid redundant fetches
  if(TARGET httplib)
    return()
  endif()

  include(FetchContent)
  
  FetchContent_Declare(
    httplib 
    SYSTEM
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
    GIT_TAG a609330e4c6374f741d3b369269f7848255e1954 # v0.14.1
    GIT_SHALLOW TRUE
  )
  
  FetchContent_MakeAvailable(httplib)
  
endfunction()
