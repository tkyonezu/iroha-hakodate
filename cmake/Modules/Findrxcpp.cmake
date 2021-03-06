add_library(rxcpp INTERFACE IMPORTED)

find_path(rxcpp_INCLUDE_DIR rxcpp/rx.hpp)
mark_as_advanced(rxcpp_INCLUDE_DIR)

find_package(PackageHandleStandardArgs REQUIRED)
find_package_handle_standard_args(rxcpp DEFAULT_MSG
    rxcpp_INCLUDE_DIR
    )

if (NOT rxcpp_FOUND)
  externalproject_add(reactive_extensions_rxcpp
      GIT_REPOSITORY https://github.com/Reactive-Extensions/rxcpp
      GIT_TAG 1b2e0589f19cb34d8cd58803677701dcf2161876
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND "" # remove install step
      UPDATE_COMMAND "" # remove update step
      TEST_COMMAND "" # remove test step
      )
  externalproject_get_property(reactive_extensions_rxcpp source_dir)
  set(rxcpp_INCLUDE_DIR ${source_dir}/Rx/v2/src)
  file(MAKE_DIRECTORY ${rxcpp_INCLUDE_DIR})

  add_dependencies(rxcpp reactive_extensions_rxcpp)
endif ()

set_target_properties(rxcpp PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${rxcpp_INCLUDE_DIR}
    )
