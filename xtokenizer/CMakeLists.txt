# Copyright (C) Kumo inc. and its affiliates.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###########################################################################
# below are examples, after initialization, modify it to your own.
###########################################################################
###kmcmake_example

if (RUST_SUPPORT)


    add_custom_command(
            OUTPUT ${TOKENIZERS_RUST_LIB}
            COMMAND
            ${CMAKE_COMMAND} -E env
            CARGO_TARGET_DIR=${XTOKENIZERS_CARGO_TARGET_DIR}
            ${CARGO_EXTRA_ENVS}
            ${CARGO_EXECUTABLE} build ${XTOKENIZERS_CARGO_FLAGS}
            WORKING_DIRECTORY ${XTOKENIZERS_CARGO_SOURCE_PATH}
            COMMENT "Building Rust tokenizer ..."
            VERBATIM
    )

    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libtoken_hg.a
            COMMAND
            ${CMAKE_COMMAND} -E copy
            ${TOKENIZERS_RUST_LIB}
            ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS ${TOKENIZERS_RUST_LIB}
            COMMENT "Copying Rust library to build dir..."
            VERBATIM
    )

    add_custom_target(token_hg_copy ALL
            DEPENDS
            ${TOKENIZERS_RUST_LIB}
            ${CMAKE_CURRENT_BINARY_DIR}/libtoken_hg.a
    )

    add_library(token_hg STATIC IMPORTED GLOBAL)
    set_target_properties(token_hg PROPERTIES
            IMPORTED_LOCATION ${TOKENIZERS_RUST_LIB}
    )
    add_dependencies(token_hg token_hg_copy)

    add_custom_target(token_hg_build
            DEPENDS ${TOKENIZERS_RUST_LIB}
    )
    install(FILES ${TOKENIZERS_RUST_LIB}
            DESTINATION lib
            RENAME libtoken_hg.a
    )
endif ()

kmcmake_cc_library(
        NAMESPACE ${PROJECT_NAME}
        NAME xtokenizer
        SOURCES
        sentencepiece.cc
        huggingface.cc
        tokenizer.cc
        CXXOPTS
        ${KMCMAKE_CXX_OPTIONS}
        PLINKS
        ${TOKENIZERS_RUST_LIB}
        ${KMCMAKE_DEPS_LINK}
        PUBLIC
)

###kmcmake_example