
// MIT License
//
// Copyright (c) 2019 degski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <array>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <list>
#include <map>
#include <random>
#include <sax/iostream.hpp>
#include <string>
#include <type_traits>
#include <vector>

#include <multi_array.hpp>

#include <plf/plf_nanotimer.h>
#include <sax/prng.hpp>
#include <sax/uniform_int_distribution.hpp>

int main ( ) {

    sax::Rng rng;

    sax::Matrix<int, 32, 32, -16, -16> v{};

    for ( auto & e : v ) {
        e = std::uniform_int_distribution<int> ( -1000, +1000 ) ( rng );
    }

    plf::nanotimer timer;

    std::uint64_t x = 0;

    timer.start ( );

    for ( int i = 0; i < 100'000'000; ++i ) {
        x += v.at ( std::uniform_int_distribution<int> ( -16, +15 ) ( rng ),
                    std::uniform_int_distribution<int> ( -16, +15 ) ( rng ) );
    }

    double t = timer.get_elapsed_ms ( );

    std::cout << x << nl;
    std::cout << t << nl;

    return EXIT_SUCCESS;
}

/*

    -fsanitize=address

    C:\Program Files\LLVM\lib\clang\9.0.0\lib\windows\clang_rt.asan_cxx-x86_64.lib
    C:\Program Files\LLVM\lib\clang\9.0.0\lib\windows\clang_rt.asan-preinit-x86_64.lib
    C:\Program Files\LLVM\lib\clang\9.0.0\lib\windows\clang_rt.asan-x86_64.lib

*/
