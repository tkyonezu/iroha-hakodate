add_library(optional INTERFACE IMPORTED)

find_path(optional_INCLUDE_DIR nonstd/optional.hpp)
mark_as_advanced(optional_INCLUDE_DIR)

find_package(PackageHandleStandardArgs REQUIRED)
find_package_handle_standard_args(optional DEFAULT_MSG
    optional_INCLUDE_DIR
    )

if (NOT optional_FOUND)
  externalproject_add(martinmoene_optional
      GIT_REPOSITORY https://github.com/martinmoene/optional-lite
      GIT_TAG a0ddabb8b52e1eaaf0dd1515bb85698b747528e4
      CONFIGURE_COMMAND "" # remove configure step
      BUILD_COMMAND "" # remove build step
      INSTALL_COMMAND "" # remove install step
      TEST_COMMAND "" # remove test step
      UPDATE_COMMAND "" # remove update step
      )
  externalproject_get_property(martinmoene_optional source_dir)
  set(optional_INCLUDE_DIR ${source_dir}/include)
  file(MAKE_DIRECTORY ${optional_INCLUDE_DIR})

  add_dependencies(optional martinmoene_optional)
endif ()

set_target_properties(optional PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${optional_INCLUDE_DIR}
    )
