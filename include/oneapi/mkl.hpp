/*******************************************************************************
* Copyright 2020-2021 Intel Corporation
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

#ifndef ONEMATH_MKL_HPP
#define ONEMATH_MKL_HPP

// Deprecated header is planned to be removed late 2025.
#pragma message("Header `oneapi/mkl.hpp` is deprecated, please use `oneapi/math.hpp` instead")

#include "oneapi/math/types.hpp"

#include "oneapi/math/blas.hpp"
#include "oneapi/math/dft.hpp"
#include "oneapi/math/lapack.hpp"
#include "oneapi/math/rng.hpp"
#include "oneapi/math/sparse_blas.hpp"

#include "mkl/namespace_alias.hpp"

#endif // ONEMATH_MKL_HPP
