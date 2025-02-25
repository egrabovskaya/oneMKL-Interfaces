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

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

#if __has_include(<sycl/sycl.hpp>)
#include <sycl/sycl.hpp>
#else
#include <CL/sycl.hpp>
#endif
#include "cblas.h"
#include "oneapi/math/detail/config.hpp"
#include "oneapi/math.hpp"
#include "onemath_blas_helper.hpp"
#include "reference_blas_templates.hpp"
#include "test_common.hpp"
#include "test_helper.hpp"

#include <gtest/gtest.h>

using namespace sycl;
using std::vector;

extern std::vector<sycl::device*> devices;

namespace {

int test(device* dev, oneapi::math::layout layout, int N, int incx, int incy, float alpha) {
    // Catch asynchronous exceptions.
    auto exception_handler = [](exception_list exceptions) {
        for (std::exception_ptr const& e : exceptions) {
            try {
                std::rethrow_exception(e);
            }
            catch (exception const& e) {
                std::cout << "Caught asynchronous SYCL exception during SDSDOT:\n"
                          << e.what() << std::endl;
                print_error_code(e);
            }
        }
    };

    queue main_queue(*dev, exception_handler);
    context cxt = main_queue.get_context();
    event done;
    std::vector<event> dependencies;

    // Prepare data.
    auto ua = usm_allocator<float, usm::alloc::shared, 64>(cxt, *dev);
    vector<float, decltype(ua)> x(ua), y(ua);
    float result_ref = float(-1);

    rand_vector(x, N, incx);
    rand_vector(y, N, incy);

    // Call Reference SDSDOT.
    const int N_ref = N, incx_ref = incx, incy_ref = incy;

    result_ref =
        ::sdsdot(&N_ref, (float*)&alpha, (float*)x.data(), &incx_ref, (float*)y.data(), &incy_ref);

    // Call DPC++ SDSDOT.

    auto result_p = (float*)oneapi::math::malloc_shared(64, sizeof(float), *dev, cxt);

    try {
#ifdef CALL_RT_API
        switch (layout) {
            case oneapi::math::layout::col_major:
                done = oneapi::math::blas::column_major::sdsdot(
                    main_queue, N, alpha, x.data(), incx, y.data(), incy, result_p, dependencies);
                break;
            case oneapi::math::layout::row_major:
                done = oneapi::math::blas::row_major::sdsdot(
                    main_queue, N, alpha, x.data(), incx, y.data(), incy, result_p, dependencies);
                break;
            default: break;
        }
        done.wait();
#else
        switch (layout) {
            case oneapi::math::layout::col_major:
                TEST_RUN_BLAS_CT_SELECT(main_queue, oneapi::math::blas::column_major::sdsdot, N,
                                        alpha, x.data(), incx, y.data(), incy, result_p,
                                        dependencies);
                break;
            case oneapi::math::layout::row_major:
                TEST_RUN_BLAS_CT_SELECT(main_queue, oneapi::math::blas::row_major::sdsdot, N, alpha,
                                        x.data(), incx, y.data(), incy, result_p, dependencies);
                break;
            default: break;
        }
        main_queue.wait();
#endif
    }
    catch (exception const& e) {
        std::cout << "Caught synchronous SYCL exception during SDSDOT:\n" << e.what() << std::endl;
        print_error_code(e);
    }

    catch (const oneapi::math::unimplemented& e) {
        return test_skipped;
    }

    catch (const std::runtime_error& error) {
        std::cout << "Error raised during execution of SDSDOT:\n" << error.what() << std::endl;
    }

    // Compare the results of reference implementation and DPC++ implementation.

    bool good = check_equal(*result_p, result_ref, N, std::cout);
    oneapi::math::free_shared(result_p, cxt);

    return (int)good;
}

class SdsdotUsmTests
        : public ::testing::TestWithParam<std::tuple<sycl::device*, oneapi::math::layout>> {};

TEST_P(SdsdotUsmTests, RealSinglePrecision) {
    CHECK_DOUBLE_ON_DEVICE(std::get<0>(GetParam()));
    EXPECT_TRUEORSKIP(test(std::get<0>(GetParam()), std::get<1>(GetParam()), 1357, 2, 3, 2.0));
    EXPECT_TRUEORSKIP(test(std::get<0>(GetParam()), std::get<1>(GetParam()), 1357, -2, -3, 2.0));
    EXPECT_TRUEORSKIP(test(std::get<0>(GetParam()), std::get<1>(GetParam()), 1357, 1, 1, 2.0));
}

INSTANTIATE_TEST_SUITE_P(SdsdotUsmTestSuite, SdsdotUsmTests,
                         ::testing::Combine(testing::ValuesIn(devices),
                                            testing::Values(oneapi::math::layout::col_major,
                                                            oneapi::math::layout::row_major)),
                         ::LayoutDeviceNamePrint());

} // anonymous namespace
