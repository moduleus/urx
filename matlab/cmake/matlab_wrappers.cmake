###################################################################################################
# add_matlab_enum
###################################################################################################

function(add_matlab_enum)
  cmake_parse_arguments(ADD_MATLAB_ENUM "" "NAMESPACE;NAME" "VALUES;INT_VALUES"
                        ${ARGN})

  # Get enumeration class name
  set(MATLAB_ENUM_NAME ${ADD_MATLAB_ENUM_NAME})

  # Create matlab module path
  string(REPLACE "." "/+" module_path "${ADD_MATLAB_ENUM_NAMESPACE}")
  string(CONCAT module_path "+" "${module_path}")

  # Generate enumerations values matlab code
  set(MATLAB_ENUM_VALUES_CODE "")
  list(LENGTH ADD_MATLAB_ENUM_VALUES values_count)
  math(EXPR values_last_index "${values_count} - 1")

  foreach(index RANGE ${values_last_index})
    list(GET ADD_MATLAB_ENUM_VALUES ${index} value)
    list(GET ADD_MATLAB_ENUM_INT_VALUES ${index} int_value)

    set(MATLAB_ENUM_VALUES_CODE
        "${MATLAB_ENUM_VALUES_CODE}    ${value}(${int_value})")

    if(NOT ${index} EQUAL ${values_last_index})
      set(MATLAB_ENUM_VALUES_CODE "${MATLAB_ENUM_VALUES_CODE}\n")
    endif()
  endforeach()

  # Finally generate file from template
  set(input_config_file "${PROJECT_SOURCE_DIR}/matlab/template/Enum.m.in")
  set(output_config_file
      "${CMAKE_CURRENT_BINARY_DIR}/${module_path}/${MATLAB_ENUM_NAME}.m")
  configure_file(${input_config_file} ${output_config_file})

  # Generate install command
  install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${module_path}/${MATLAB_ENUM_NAME}.m"
    DESTINATION "${MATLAB_INSTALL_LIBDIR}/${module_path}")
endfunction()

###################################################################################################
# add_matlab_struct
###################################################################################################

function(add_matlab_struct)
  cmake_parse_arguments(
    ADD_MATLAB_STRUCT "" "NAMESPACE;CPP_NAMESPACE;NAME;CPP_NAME"
    "PROPERTIES_NAME;PROPERTIES_TYPE;PROPERTIES_TAG" ${ARGN})

  set(MATLAB_STRUCT_NAMESPACE "${ADD_MATLAB_STRUCT_NAMESPACE}")

  # Make the module path as a list "egal.operation.convolution" => ["egal", "operation", "convolution"]
  string(REPLACE "." ";" modules_list_spaced "${ADD_MATLAB_STRUCT_NAMESPACE}")
  # ["egal", "operation", "convolution"] => ["Egal", "Operation", "Convolution"]
  uppercase_first_char_list(modules_list_spaced modules_list_uc)

  # Get struct class name
  set(MATLAB_STRUCT_NAME ${ADD_MATLAB_STRUCT_NAME})
  set(CPP_STRUCT_NAME ${ADD_MATLAB_STRUCT_CPP_NAME})

  # Create matlab module path "+egal/+operation/+convolution"
  string(REPLACE ";" "/+" matlab_module_path "${modules_list_spaced}")
  string(CONCAT matlab_module_path "+" "${matlab_module_path}")

  # Create the corresponding mex api name "MexOperationConvolution<MATLAB_STRUCT_NAME>" and sub path "Operation/Convolution"
  list(POP_FRONT modules_list_uc OUTPUT_VARIABLE mex_modules_sublist)
  string(REPLACE ";" "" MEX_STRUCT_LIBRARY "${mex_modules_sublist}")
  string(REPLACE ";" "/" MEX_STRUCT_MODULE_SUBPATH "${mex_modules_sublist}")
  set(MEX_STRUCT_LIBRARY "Mex${MEX_STRUCT_LIBRARY}${MATLAB_STRUCT_NAME}")

  string(REPLACE "::" "/" CPP_STRUCT_MODULE_PATH
                 "${ADD_MATLAB_STRUCT_CPP_NAMESPACE}")
  set(CPP_STRUCT_MODULE_NAMESPACE "${ADD_MATLAB_STRUCT_CPP_NAMESPACE}")

  # Generate struct properties definition in matlab code
  set(MATLAB_STRUCT_PROPERTIES_CODE "")
  set(MATLAB_STRUCT_METHODS_CODE "")
  list(LENGTH ADD_MATLAB_STRUCT_PROPERTIES_NAME props_count)
  math(EXPR props_last_index "${props_count} - 1")

  foreach(index RANGE ${props_last_index})
    list(GET ADD_MATLAB_STRUCT_PROPERTIES_NAME ${index} prop_name)
    list(GET ADD_MATLAB_STRUCT_PROPERTIES_TYPE ${index} prop_type)
    list(GET ADD_MATLAB_STRUCT_PROPERTIES_TAG ${index} prop_tag)

    # Code that defines the property
    set(MATLAB_STRUCT_PROPERTIES_CODE
        "${MATLAB_STRUCT_PROPERTIES_CODE}    ${prop_name} ${prop_type}")

    # Prepare setter and getter function names of dependent properties
    string(REPLACE "_" ";" prop_name_list "${prop_name}")
    uppercase_first_char_list(prop_name_list prop_name_list_uc)
    string(REPLACE ";" "" prop_name_ucc "${prop_name_list_uc}")
    set(prop_getter "get${prop_name_ucc}")
    set(prop_setter "set${prop_name_ucc}")

    # Prepare setter and getter function call code
    set(prop_getter_call_code
        "${MEX_STRUCT_LIBRARY}('${prop_getter}', this.id)")
    set(prop_setter_call_code "${MEX_STRUCT_LIBRARY}('${prop_setter}', this.id")
    if(prop_tag STREQUAL "enumeration")
      # cast to enumeration class type
      set(prop_getter_call_code "${prop_type}(${prop_getter_call_code})")
      # cast to int32 for mex
      set(prop_setter_call_code "${prop_setter_call_code}, int32(val))")
    else()
      set(prop_setter_call_code "${prop_setter_call_code}, val)")
    endif()

    # Code that implements get/set method of property
    set(MATLAB_STRUCT_METHODS_CODE
        "${MATLAB_STRUCT_METHODS_CODE}\
    function val = get.${prop_name}(this)
      val = ${prop_getter_call_code};
    end

    function set.${prop_name}(this, val)
      ${prop_setter_call_code};
    end")

    # Mex cpp code that implements get/set proxy method of current property
    string(TOUPPER ${prop_tag} prop_tag_uc)
    string(
      APPEND
      MEX_STRUCT_METHODS_CODE
      "MEX_DEFINE_RW_${prop_tag_uc}_PROPERTY(${CPP_STRUCT_NAME}, ${prop_name}, ${prop_getter}, ${prop_setter})"
    )

    if(NOT ${index} EQUAL ${props_last_index})
      set(MATLAB_STRUCT_PROPERTIES_CODE "${MATLAB_STRUCT_PROPERTIES_CODE}\n")
      set(MATLAB_STRUCT_METHODS_CODE "${MATLAB_STRUCT_METHODS_CODE}\n\n")
      set(MEX_STRUCT_METHODS_CODE "${MEX_STRUCT_METHODS_CODE}\n")
    endif()
  endforeach()

  # Finally generate matlab file from template
  set(matlab_input_config_file
      "${PROJECT_SOURCE_DIR}/matlab/template/Struct.m.in")
  set(matlab_output_config_file
      "${CMAKE_CURRENT_BINARY_DIR}/${matlab_module_path}/${MATLAB_STRUCT_NAME}.m"
  )
  configure_file(${matlab_input_config_file} ${matlab_output_config_file})

  # Finally generate file from template
  set(mex_input_config_file
      "${PROJECT_SOURCE_DIR}/matlab/template/MexStruct.cpp.in")
  set(mex_output_config_file
      "${CMAKE_CURRENT_BINARY_DIR}/Mex/${MEX_STRUCT_MODULE_SUBPATH}/${MATLAB_STRUCT_NAME}.cpp"
  )
  configure_file(${mex_input_config_file} ${mex_output_config_file})

  # Create mex target
  matlab_add_mex(
    NAME ${MEX_STRUCT_LIBRARY}
    SRC "${CMAKE_CURRENT_BINARY_DIR}/Mex/${MEX_STRUCT_MODULE_SUBPATH}/${MATLAB_STRUCT_NAME}.cpp"
    LINK_TO eGalCuda mexplus R2018a)

  # Create install command target
  install(
    TARGETS ${MEX_STRUCT_LIBRARY}
    LIBRARY
      DESTINATION "${MATLAB_INSTALL_LIBDIR}/${matlab_module_path}/private")

  install(
    FILES
      "${CMAKE_CURRENT_BINARY_DIR}/${matlab_module_path}/${MATLAB_STRUCT_NAME}.m"
    DESTINATION "${MATLAB_INSTALL_LIBDIR}/${matlab_module_path}")
endfunction()

###################################################################################################
# add_matlab_kernel
###################################################################################################

function(add_matlab_kernel)
  cmake_parse_arguments(ADD_MATLAB_KERNEL "" "NAMESPACE;CPP_NAMESPACE" ""
                        ${ARGN})

  set(MATLAB_KERNEL_NAMESPACE "${ADD_MATLAB_KERNEL_NAMESPACE}")

  # Make the module path as a list "egal.operation.convolution" => ["egal", "operation", "convolution"]
  string(REPLACE "." ";" modules_list_spaced "${ADD_MATLAB_KERNEL_NAMESPACE}")
  # ["egal", "operation", "convolution"] => ["Egal", "Operation", "Convolution"]
  uppercase_first_char_list(modules_list_spaced modules_list_uc)

  # Create matlab module path "+egal/+operation/+convolution"
  string(REPLACE ";" "/+" matlab_module_path "${modules_list_spaced}")
  string(CONCAT matlab_module_path "+" "${matlab_module_path}")

  # Create the corresponding mex api name "MexOperationConvolutionKernel" and sub path "Operation/Convolution"
  list(POP_FRONT modules_list_uc OUTPUT_VARIABLE mex_modules_sublist)
  string(REPLACE ";" "" MEX_STRUCT_LIBRARY "${mex_modules_sublist}")
  string(REPLACE ";" "/" MEX_STRUCT_MODULE_SUBPATH "${mex_modules_sublist}")
  set(MEX_STRUCT_LIBRARY "Mex${MEX_STRUCT_LIBRARY}Kernel")

  string(REPLACE "::" "/" CPP_STRUCT_MODULE_PATH
                 "${ADD_MATLAB_KERNEL_CPP_NAMESPACE}")
  set(CPP_STRUCT_MODULE_NAMESPACE "${ADD_MATLAB_KERNEL_CPP_NAMESPACE}")

  # Finally generate matlab file from template
  set(matlab_input_config_file
      "${PROJECT_SOURCE_DIR}/matlab/template/Kernel.m.in")
  set(matlab_output_config_file
      "${CMAKE_CURRENT_BINARY_DIR}/${matlab_module_path}/Kernel.m")
  configure_file(${matlab_input_config_file} ${matlab_output_config_file})

  # Finally generate file from template
  set(mex_input_config_file
      "${PROJECT_SOURCE_DIR}/matlab/template/MexKernel.cpp.in")
  set(mex_output_config_file
      "${CMAKE_CURRENT_BINARY_DIR}/Mex/${MEX_STRUCT_MODULE_SUBPATH}/Kernel.cpp")
  configure_file(${mex_input_config_file} ${mex_output_config_file})

  # Create mex target
  matlab_add_mex(
    NAME ${MEX_STRUCT_LIBRARY}
    SRC "${CMAKE_CURRENT_BINARY_DIR}/Mex/${MEX_STRUCT_MODULE_SUBPATH}/Kernel.cpp"
    LINK_TO eGalCuda mexplus R2018a)

  # Create install command target
  install(
    TARGETS ${MEX_STRUCT_LIBRARY}
    LIBRARY
      DESTINATION "${MATLAB_INSTALL_LIBDIR}/${matlab_module_path}/private")

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${matlab_module_path}/Kernel.m"
          DESTINATION "${MATLAB_INSTALL_LIBDIR}/${matlab_module_path}")
endfunction()
