/*******************************************************************************
* Copyright 2020-2022 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions
* and limitations under the License.
*
*
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#ifndef _ONEMATH_BACKENDS_HPP_
#define _ONEMATH_BACKENDS_HPP_

#include <map>
#include <string>

namespace oneapi {
namespace math {

enum class backend {
    mklcpu,
    mklgpu,
    cublas,
    rocsolver,
    cusolver,
    curand,
    netlib,
    armpl,
    rocblas,
    rocrand,
    generic,
    cufft,
    rocfft,
    portfft,
    cusparse,
    rocsparse,
    unsupported
};

typedef std::map<backend, std::string> backendmap;

// clang-format alternate the formatting depending on the parity of the number of backends
// It is disabled to reduce noise
// clang-format off
static backendmap backend_map = { { backend::mklcpu, "mklcpu" },
                                  { backend::mklgpu, "mklgpu" },
                                  { backend::cublas, "cublas" },
                                  { backend::cusolver, "cusolver" },
                                  { backend::curand, "curand" },
                                  { backend::netlib, "netlib" },
                                  { backend::armpl, "armpl" },
                                  { backend::rocblas, "rocblas" },
                                  { backend::rocrand, "rocrand" },
                                  { backend::rocsolver, "rocsolver" },
                                  { backend::generic, "generic" },
                                  { backend::cufft, "cufft" },
                                  { backend::rocfft, "rocfft" },
                                  { backend::portfft, "portfft" },
                                  { backend::cusparse, "cusparse" },
                                  { backend::rocsparse, "rocsparse" },
                                  { backend::unsupported, "unsupported" } };
// clang-format on

} //namespace math
} //namespace oneapi

#endif //_ONEMATH_BACKENDS_HPP_
