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

if (NOT DEFINED CARGO_HOME)
    if ("$ENV{CARGO_HOME}" STREQUAL "")
        set(CARGO_HOME "${USER_HOME}/.cargo")
    else ()
        set(CARGO_HOME "$ENV{CARGO_HOME}")
    endif ()
endif ()

find_program(CARGO_EXECUTABLE cargo
        HINTS "${CARGO_HOME}"
        PATH_SUFFIXES "bin")
mark_as_advanced(CARGO_EXECUTABLE)

set(RUST_SUPPORT OFF)
if (CARGO_EXECUTABLE)
    set(RUST_SUPPORT ON)
endif ()

set(CARGO_EXTRA_ENVS "")

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND TOKENIZERS_C_LINK_LIBS ${CMAKE_DL_LIBS})
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(XTOKENIZERS_CARGO_TARGET aarch64-unknown-linux-gnu)
        set(CARGO_EXTRA_ENVS
                AR_${XTOKENIZERS_CARGO_TARGET}=${TOOLCHAIN_DIR}${XTOKENIZERS_CARGO_TARGET}-ar
                CC_${XTOKENIZERS_CARGO_TARGET}=${TOOLCHAIN_DIR}${XTOKENIZERS_CARGO_TARGET}-gcc
                CXX_${XTOKENIZERS_CARGO_TARGET}=${TOOLCHAIN_DIR}${XTOKENIZERS_CARGO_TARGET}-g++
        )
    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(XTOKENIZERS_CARGO_TARGET x86_64-unknown-linux-gnu)
        # ==================================
    endif ()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(XTOKENIZERS_CARGO_TARGET wasm32-unknown-emscripten)
elseif (CMAKE_SYSTEM_NAME STREQUAL "iOS")
    if (CMAKE_OSX_SYSROOT MATCHES ".*iPhoneSimulator\\.platform.*")
        if (CMAKE_OSX_ARCHITECTURES MATCHES "x86_64")
            set(XTOKENIZERS_CARGO_TARGET x86_64-apple-ios)
        else ()
            set(XTOKENIZERS_CARGO_TARGET aarch64-apple-ios-sim)
        endif ()
    else ()
        set(XTOKENIZERS_CARGO_TARGET aarch64-apple-ios)
    endif ()
    # add extra dependency needed for rust tokenizer in iOS
    find_library(FOUNDATION_LIB Foundation)
    find_library(SECURITY_LIB Security)
    list(APPEND TOKENIZERS_C_LINK_LIBS ${FOUNDATION_LIB} ${SECURITY_LIB})
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(XTOKENIZERS_CARGO_TARGET aarch64-apple-darwin)
    endif ()
    if (CMAKE_OSX_DEPLOYMENT_TARGET)
        set(CARGO_EXTRA_ENVS
                MACOSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
        )
    endif ()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
    if (ANDROID_ABI STREQUAL "arm64-v8a")
        set(XTOKENIZERS_CARGO_TARGET aarch64-linux-android)
    elseif (ANDROID_ABI STREQUAL "armeabi-v7a")
        set(XTOKENIZERS_CARGO_TARGET armv7-linux-androideabi)
    elseif (ANDROID_ABI STREQUAL "x86_64")
        set(XTOKENIZERS_CARGO_TARGET x86_64-linux-android)
    elseif (ANDROID_ABI STREQUAL "x86")
        set(XTOKENIZERS_CARGO_TARGET i686-linux-android)
    endif ()
    set(CARGO_EXTRA_ENVS
            AR_${XTOKENIZERS_CARGO_TARGET}=${ANDROID_TOOLCHAIN_ROOT}/bin/llvm-ar
            CC_${XTOKENIZERS_CARGO_TARGET}=${ANDROID_TOOLCHAIN_ROOT}/bin/${XTOKENIZERS_CARGO_TARGET}${ANDROID_NATIVE_API_LEVEL}-clang
            CXX_${XTOKENIZERS_CARGO_TARGET}=${ANDROID_TOOLCHAIN_ROOT}/bin/${XTOKENIZERS_CARGO_TARGET}${ANDROID_NATIVE_API_LEVEL}-clang++
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(XTOKENIZERS_CARGO_TARGET aarch64-pc-windows-msvc)
    else ()
        set(XTOKENIZERS_CARGO_TARGET x86_64-pc-windows-msvc)
    endif ()
endif ()

set(XTOKENIZERS_CARGO_FLAGS "")
set(XTOKENIZERS_CARGO_TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(XTOKENIZERS_CARGO_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

if (NOT XTOKENIZERS_CARGO_TARGET STREQUAL "")
    list(APPEND XTOKENIZERS_CARGO_FLAGS --target ${XTOKENIZERS_CARGO_TARGET})
    set(XTOKENIZERS_CARGO_BINARY_DIR
            "${XTOKENIZERS_CARGO_BINARY_DIR}/${XTOKENIZERS_CARGO_TARGET}")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(XTOKENIZERS_CARGO_BINARY_DIR "${XTOKENIZERS_CARGO_BINARY_DIR}/debug")
else ()
    list(APPEND XTOKENIZERS_CARGO_FLAGS --release)
    set(XTOKENIZERS_CARGO_BINARY_DIR "${XTOKENIZERS_CARGO_BINARY_DIR}/release")
endif ()

set(XTOKENIZERS_CARGO_SOURCE_PATH ${PROJECT_SOURCE_DIR}/xtokenizer/rust)

if (MSVC)
    set(TOKENIZERS_RUST_LIB "${XTOKENIZERS_CARGO_BINARY_DIR}/xtoken_hg.lib")
else ()
    set(TOKENIZERS_RUST_LIB "${XTOKENIZERS_CARGO_BINARY_DIR}/libxtoken_hg.a")
endif ()



