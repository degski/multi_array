
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

#pragma once

#include <cassert> // assert
#include <cstddef> // std::size_t
#include <cstdint> // int
#include <cstring> // std::memcpy

#include <tuple>
#include <type_traits>
#include <utility> // std::forward

// This library invokes Undefined Behaviour (UB), as it exploits the pre-calculation of intermediate
//  pointers [pointing outside the array m_data] in order to gain efficiency when dealing with
//  off-zero indices (the fat() and frat() member functions).

#define MA_ASSERT_1                                                                                                                \
    assert ( i_ >= BaseI );                                                                                                        \
    assert ( i_ < I + BaseI );
#define MA_ASSERT_2                                                                                                                \
    MA_ASSERT_1                                                                                                                    \
    assert ( j_ >= BaseJ );                                                                                                        \
    assert ( j_ < J + BaseJ );
#define MA_ASSERT_3                                                                                                                \
    MA_ASSERT_2                                                                                                                    \
    assert ( k_ >= BaseK );                                                                                                        \
    assert ( k_ < K + BaseK );
#define MA_ASSERT_4                                                                                                                \
    MA_ASSERT_3                                                                                                                    \
    assert ( l_ >= BaseL );                                                                                                        \
    assert ( l_ < L + BaseL );

namespace sax {

namespace detail {

template<typename T>
using is_valid_multi_array_type =
    std::enable_if_t<std::conjunction<std::is_default_constructible<T>, std::is_trivially_copyable<T>>::value, T>;
}

template<typename T, int I, int BaseI = 0, typename = detail::is_valid_multi_array_type<T>>
class Vector {

    T m_data[ I ];

    [[nodiscard]] static constexpr int rebase ( ) noexcept { return -BaseI; }
    [[nodiscard]] static constexpr int reverse_rebase ( ) noexcept { return I - 1 + BaseI; }

    public:
    using value_type             = T;
    using pointer                = value_type *;
    using const_pointer          = value_type const *;
    using reference              = value_type &;
    using const_reference        = value_type const &;
    using rv_reference           = value_type &&;
    using size_type              = std::size_t;
    using signed_size_type       = std::make_signed_t<size_type>;
    using difference_type        = signed_size_type;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = pointer;
    using const_reverse_iterator = const_pointer;

    Vector ( ) noexcept : m_data{ T{} } {}
    Vector ( Vector const & v_ ) noexcept { std::memcpy ( m_data, v_.m_data, size ( ) * sizeof ( T ) ); }
    Vector ( Vector && v_ ) noexcept = delete;
    template<typename... Args>
    constexpr Vector ( Args... a_ ) noexcept : m_data{ std::forward<Args> ( a_ )... } {}

    ~Vector ( ) = default;

    Vector & operator= ( Vector const & rhs_ ) noexcept {
        std::memcpy ( m_data, rhs_.m_data, size ( ) * sizeof ( T ) );
        return *this;
    }
    Vector & operator= ( Vector && rhs_ ) noexcept = delete;

    [[nodiscard]] bool operator== ( Vector const & rhs_ ) noexcept {
        return std::memcmp ( m_data, rhs_.m_data, size ( ) * sizeof ( T ) ) == 0;
    }
    [[nodiscard]] bool operator!= ( Vector const & rhs_ ) noexcept { return not operator== ( rhs_ ); };

    using extents_type = std::tuple<int>;

    [[nodiscard]] reference fat ( int const i_ ) noexcept {
        MA_ASSERT_1
        return ( m_data + rebase ( ) )[ i_ ];
    }

    [[nodiscard]] value_type fat ( int const i_ ) const noexcept {
        MA_ASSERT_1
        return ( m_data + rebase ( ) )[ i_ ];
    }

    [[nodiscard]] constexpr reference at ( int const i_ ) noexcept {
        MA_ASSERT_1
        return m_data[ rebase ( ) + i_ ];
    }

    [[nodiscard]] constexpr value_type at ( int const i_ ) const noexcept {
        MA_ASSERT_1
        return m_data[ rebase ( ) + i_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] reference frat ( int const i_ ) noexcept {
        MA_ASSERT_1
        return ( m_data + reverse_rebase ( ) )[ -i_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] value_type frat ( int const i_ ) const noexcept {
        MA_ASSERT_1
        return ( m_data + reverse_rebase ( ) )[ -i_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr reference rat ( int const i_ ) noexcept {
        MA_ASSERT_1
        return m_data[ reverse_rebase ( ) - i_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr value_type rat ( int const i_ ) const noexcept {
        MA_ASSERT_1
        return m_data[ reverse_rebase ( ) - i_ ];
    }

    [[nodiscard]] constexpr pointer data ( ) noexcept { return m_data; }
    [[nodiscard]] constexpr const_pointer data ( ) const noexcept { return m_data; }

    [[nodiscard]] constexpr iterator begin ( ) noexcept { return std::begin ( m_data ); }
    [[nodiscard]] constexpr const_iterator begin ( ) const noexcept { return std::cbegin ( m_data ); }
    [[nodiscard]] constexpr const_iterator cbegin ( ) const noexcept { return std::cbegin ( m_data ); }
    [[nodiscard]] constexpr iterator end ( ) noexcept { return std::end ( m_data ); }
    [[nodiscard]] constexpr const_iterator end ( ) const noexcept { return std::cend ( m_data ); }
    [[nodiscard]] constexpr const_iterator cend ( ) const noexcept { return std::cend ( m_data ); }
    [[nodiscard]] constexpr reverse_iterator rbegin ( ) noexcept { return std::rbegin ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin ( ) const noexcept { return std::crbegin ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator crbegin ( ) const noexcept { return std::crbegin ( m_data ); }
    [[nodiscard]] constexpr reverse_iterator rend ( ) noexcept { return std::rend ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator rend ( ) const noexcept { return std::crend ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator crend ( ) const noexcept { return std::crend ( m_data ); }

    [[nodiscard]] static constexpr std::size_t size ( ) noexcept { return I; }
    [[nodiscard]] static constexpr std::size_t capacity ( ) noexcept { return I; }
    [[nodiscard]] static constexpr extents_type extents ( ) noexcept { return { I }; }
};

template<typename T, int I, int J, int BaseI = 0, int BaseJ = 0, typename = detail::is_valid_multi_array_type<T>>
class Matrix {

    T m_data[ I * J ];

    [[nodiscard]] static constexpr int rebase ( ) noexcept { return -BaseJ - BaseI * J; }
    [[nodiscard]] static constexpr int reverse_rebase ( ) noexcept { return I * J - 1 + BaseJ + BaseI * J; }

    public:
    using value_type             = T;
    using pointer                = value_type *;
    using const_pointer          = value_type const *;
    using reference              = value_type &;
    using const_reference        = value_type const &;
    using rv_reference           = value_type &&;
    using size_type              = std::size_t;
    using signed_size_type       = std::make_signed_t<size_type>;
    using difference_type        = signed_size_type;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = pointer;
    using const_reverse_iterator = const_pointer;

    Matrix ( ) noexcept : m_data{ T{} } {}
    Matrix ( Matrix const & m_ ) noexcept { std::memcpy ( m_data, m_.m_data, size ( ) * sizeof ( T ) ); }
    Matrix ( Matrix && m_ ) noexcept = delete;
    template<typename... Args>
    constexpr Matrix ( Args... a_ ) noexcept : m_data{ std::forward<Args> ( a_ )... } {}

    ~Matrix ( ) = default;

    Matrix & operator= ( Matrix const & rhs_ ) noexcept {
        std::memcpy ( m_data, rhs_.m_data, size ( ) * sizeof ( T ) );
        return *this;
    }
    Matrix & operator= ( Matrix && rhs_ ) noexcept = delete;

    [[nodiscard]] bool operator== ( Matrix const & rhs_ ) noexcept {
        return std::memcmp ( m_data, rhs_.m_data, size ( ) * sizeof ( T ) ) == 0;
    }
    [[nodiscard]] bool operator!= ( Matrix const & rhs_ ) noexcept { return not operator== ( rhs_ ); };

    using extents_type = std::tuple<int, int>;

    [[nodiscard]] reference fat ( int const i_, int const j_ ) noexcept {
        MA_ASSERT_2
        return ( m_data + rebase ( ) )[ j_ + i_ * J ];
    }

    [[nodiscard]] value_type fat ( int const i_, int const j_ ) const noexcept {
        MA_ASSERT_2
        return ( m_data + rebase ( ) )[ j_ + i_ * J ];
    }

    [[nodiscard]] constexpr reference at ( int const i_, int const j_ ) noexcept {
        MA_ASSERT_2
        return m_data[ rebase ( ) + j_ + i_ * J ];
    }

    [[nodiscard]] constexpr value_type at ( int const i_, int const j_ ) const noexcept {
        MA_ASSERT_2
        return m_data[ rebase ( ) + j_ + i_ * J ];
    }

    // Reverse at (rat).
    [[nodiscard]] reference frat ( int const i_, int const j_ ) noexcept {
        MA_ASSERT_2
        return ( m_data + reverse_rebase ( ) )[ -j_ - i_ * J ];
    }

    // Reverse at (rat).
    [[nodiscard]] value_type frat ( int const i_, int const j_ ) const noexcept {
        MA_ASSERT_2
        return ( m_data + reverse_rebase ( ) )[ -j_ - i_ * J ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr reference rat ( int const i_, int const j_ ) noexcept {
        MA_ASSERT_2
        return m_data[ reverse_rebase ( ) - j_ - i_ * J ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr value_type rat ( int const i_, int const j_ ) const noexcept {
        MA_ASSERT_2
        return m_data[ reverse_rebase ( ) - j_ - i_ * J ];
    }

    [[nodiscard]] constexpr pointer data ( ) noexcept { return m_data; }
    [[nodiscard]] constexpr const_pointer data ( ) const noexcept { return m_data; }

    [[nodiscard]] constexpr iterator begin ( ) noexcept { return std::begin ( m_data ); }
    [[nodiscard]] constexpr const_iterator begin ( ) const noexcept { return std::cbegin ( m_data ); }
    [[nodiscard]] constexpr const_iterator cbegin ( ) const noexcept { return std::cbegin ( m_data ); }
    [[nodiscard]] constexpr iterator end ( ) noexcept { return std::end ( m_data ); }
    [[nodiscard]] constexpr const_iterator end ( ) const noexcept { return std::cend ( m_data ); }
    [[nodiscard]] constexpr const_iterator cend ( ) const noexcept { return std::cend ( m_data ); }
    [[nodiscard]] constexpr reverse_iterator rbegin ( ) noexcept { return std::rbegin ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin ( ) const noexcept { return std::crbegin ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator crbegin ( ) const noexcept { return std::crbegin ( m_data ); }
    [[nodiscard]] constexpr reverse_iterator rend ( ) noexcept { return std::rend ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator rend ( ) const noexcept { return std::crend ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator crend ( ) const noexcept { return std::crend ( m_data ); }

    [[nodiscard]] static constexpr std::size_t size ( ) noexcept { return I * J; }
    [[nodiscard]] static constexpr std::size_t capacity ( ) noexcept { return I * J; }
    [[nodiscard]] static constexpr extents_type extents ( ) noexcept { return { I, J }; }
};

template<typename T, int I, int J, int BaseI = 0, int BaseJ = 0,
         typename = std::enable_if_t<std::is_default_constructible<T>::value, T>>
using MatrixRM = Matrix<T, I, J, BaseI, BaseJ>;

template<typename T, int J, int I, int BaseJ = 0, int BaseI = 0,
         typename = std::enable_if_t<std::is_default_constructible<T>::value, T>>
using MatrixCM = Matrix<T, J, I, BaseJ, BaseI>;

template<typename T, int I, int J, int K, int BaseI = 0, int BaseJ = 0, int BaseK = 0,
         typename = detail::is_valid_multi_array_type<T>>
class Cube {

    T m_data[ I * J * K ];

    [[nodiscard]] static constexpr int rebase ( ) noexcept { return K * ( -BaseJ - BaseI * J ) - BaseK; }
    [[nodiscard]] static constexpr int reverse_rebase ( ) noexcept { return I * J * K - 1 + BaseJ * K + BaseI * J * K + BaseK; }

    public:
    using value_type             = T;
    using pointer                = value_type *;
    using const_pointer          = value_type const *;
    using reference              = value_type &;
    using const_reference        = value_type const &;
    using rv_reference           = value_type &&;
    using size_type              = std::size_t;
    using signed_size_type       = std::make_signed_t<size_type>;
    using difference_type        = signed_size_type;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = pointer;
    using const_reverse_iterator = const_pointer;

    Cube ( ) noexcept : m_data{ T{} } {}
    Cube ( Cube const & c_ ) noexcept { std::memcpy ( m_data, c_.m_data, size ( ) * sizeof ( T ) ); }
    Cube ( Cube && c_ ) noexcept = delete;
    template<typename... Args>
    constexpr Cube ( Args... a_ ) noexcept : m_data{ std::forward<Args> ( a_ )... } {}

    ~Cube ( ) = default;

    Cube & operator= ( Cube const & rhs_ ) noexcept {
        std::memcpy ( m_data, rhs_.m_data, size ( ) * sizeof ( T ) );
        return *this;
    }
    Cube & operator= ( Cube && rhs_ ) noexcept = delete;

    [[nodiscard]] bool operator== ( Cube const & rhs_ ) noexcept {
        return std::memcmp ( m_data, rhs_.m_data, size ( ) * sizeof ( T ) ) == 0;
    }
    [[nodiscard]] bool operator!= ( Cube const & rhs_ ) noexcept { return not operator== ( rhs_ ); };

    using extents_type = std::tuple<int, int, int>;

    [[nodiscard]] reference fat ( int const i_, int const j_, int const k_ ) noexcept {
        MA_ASSERT_3
        return ( m_data + rebase ( ) )[ K * ( j_ + i_ * J ) + k_ ];
    }

    [[nodiscard]] value_type fat ( int const i_, int const j_, int const k_ ) const noexcept {
        MA_ASSERT_3
        return ( m_data + rebase ( ) )[ K * ( j_ + i_ * J ) + k_ ];
    }

    [[nodiscard]] constexpr reference at ( int const i_, int const j_, int const k_ ) noexcept {
        MA_ASSERT_3
        return m_data[ rebase ( ) + K * ( j_ + i_ * J ) + k_ ];
    }

    [[nodiscard]] constexpr value_type at ( int const i_, int const j_, int const k_ ) const noexcept {
        MA_ASSERT_3
        return m_data[ rebase ( ) + K * ( j_ + i_ * J ) + k_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] reference frat ( int const i_, int const j_, int const k_ ) noexcept {
        MA_ASSERT_3
        return ( m_data + reverse_rebase ( ) )[ K * ( -j_ - i_ * J ) - k_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] value_type frat ( int const i_, int const j_, int const k_ ) const noexcept {
        MA_ASSERT_3
        return ( m_data + reverse_rebase ( ) )[ K * ( -j_ - i_ * J ) - k_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr reference rat ( int const i_, int const j_, int const k_ ) noexcept {
        MA_ASSERT_3
        return m_data[ reverse_rebase ( ) + K * ( -j_ - i_ * J ) - k_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr value_type rat ( int const i_, int const j_, int const k_ ) const noexcept {
        MA_ASSERT_3
        return m_data[ reverse_rebase ( ) + K * ( -j_ - i_ * J ) - k_ ];
    }

    [[nodiscard]] constexpr pointer data ( ) noexcept { return m_data; }
    [[nodiscard]] constexpr const_pointer data ( ) const noexcept { return m_data; }

    [[nodiscard]] constexpr iterator begin ( ) noexcept { return std::begin ( m_data ); }
    [[nodiscard]] constexpr const_iterator begin ( ) const noexcept { return std::cbegin ( m_data ); }
    [[nodiscard]] constexpr const_iterator cbegin ( ) const noexcept { return std::cbegin ( m_data ); }
    [[nodiscard]] constexpr iterator end ( ) noexcept { return std::end ( m_data ); }
    [[nodiscard]] constexpr const_iterator end ( ) const noexcept { return std::cend ( m_data ); }
    [[nodiscard]] constexpr const_iterator cend ( ) const noexcept { return std::cend ( m_data ); }
    [[nodiscard]] constexpr reverse_iterator rbegin ( ) noexcept { return std::rbegin ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin ( ) const noexcept { return std::crbegin ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator crbegin ( ) const noexcept { return std::crbegin ( m_data ); }
    [[nodiscard]] constexpr reverse_iterator rend ( ) noexcept { return std::rend ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator rend ( ) const noexcept { return std::crend ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator crend ( ) const noexcept { return std::crend ( m_data ); }

    [[nodiscard]] static constexpr std::size_t size ( ) noexcept { return I * J * K; }
    [[nodiscard]] static constexpr std::size_t capacity ( ) noexcept { return I * J * K; }
    [[nodiscard]] static constexpr extents_type extents ( ) noexcept { return { I, J, K }; }
};

template<typename T, int I, int J, int K, int L, int BaseI = 0, int BaseJ = 0, int BaseK = 0, int BaseL = 0,
         typename = detail::is_valid_multi_array_type<T>>
class HyperCube {

    T m_data[ I * J * K * L ];

    [[nodiscard]] static constexpr int rebase ( ) noexcept { return L * ( K * ( -BaseJ - BaseI * J ) - BaseK ) - BaseL; }

    public:
    using value_type             = T;
    using pointer                = value_type *;
    using const_pointer          = value_type const *;
    using reference              = value_type &;
    using const_reference        = value_type const &;
    using rv_reference           = value_type &&;
    using size_type              = std::size_t;
    using signed_size_type       = std::make_signed_t<size_type>;
    using difference_type        = signed_size_type;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = pointer;
    using const_reverse_iterator = const_pointer;

    HyperCube ( ) noexcept : m_data{ T{} } {}
    HyperCube ( HyperCube const & h_ ) noexcept { std::memcpy ( m_data, h_.m_data, size ( ) * sizeof ( T ) ); }
    HyperCube ( HyperCube && h_ ) noexcept = delete;
    template<typename... Args>
    constexpr HyperCube ( Args... a_ ) noexcept : m_data{ std::forward<Args> ( a_ )... } {}

    ~HyperCube ( ) = default;

    HyperCube & operator= ( HyperCube const & rhs_ ) noexcept {
        std::memcpy ( m_data, rhs_.m_data, size ( ) * sizeof ( T ) );
        return *this;
    }
    HyperCube & operator= ( HyperCube && rhs_ ) noexcept = delete;

    [[nodiscard]] bool operator== ( HyperCube const & rhs_ ) noexcept {
        return std::memcmp ( m_data, rhs_.m_data, size ( ) * sizeof ( T ) ) == 0;
    }
    [[nodiscard]] bool operator!= ( HyperCube const & rhs_ ) noexcept { return not operator== ( rhs_ ); };

    using extents_type = std::tuple<int, int, int, int>;

    [[nodiscard]] reference fat ( int const i_, int const j_, int const k_, int const l_ ) noexcept {
        MA_ASSERT_4
        return ( m_data + rebase ( ) )[ L * ( K * ( j_ + i_ * J ) + k_ ) + l_ ];
    }

    [[nodiscard]] value_type fat ( int const i_, int const j_, int const k_, int const l_ ) const noexcept {
        MA_ASSERT_4
        return ( m_data + rebase ( ) )[ L * ( K * ( j_ + i_ * J ) + k_ ) + l_ ];
    }

    [[nodiscard]] constexpr reference at ( int const i_, int const j_, int const k_, int const l_ ) noexcept {
        MA_ASSERT_4
        return m_data[ rebase ( ) + L * ( K * ( j_ + i_ * J ) + k_ ) + l_ ];
    }

    [[nodiscard]] constexpr value_type at ( int const i_, int const j_, int const k_, int const l_ ) const noexcept {
        MA_ASSERT_4
        return m_data[ rebase ( ) + L * ( K * ( j_ + i_ * J ) + k_ ) + l_ ];
    }

    [[nodiscard]] constexpr pointer data ( ) noexcept { return m_data; }
    [[nodiscard]] constexpr const_pointer data ( ) const noexcept { return m_data; }

    [[nodiscard]] constexpr iterator begin ( ) noexcept { return std::begin ( m_data ); }
    [[nodiscard]] constexpr const_iterator begin ( ) const noexcept { return std::cbegin ( m_data ); }
    [[nodiscard]] constexpr const_iterator cbegin ( ) const noexcept { return std::cbegin ( m_data ); }
    [[nodiscard]] constexpr iterator end ( ) noexcept { return std::end ( m_data ); }
    [[nodiscard]] constexpr const_iterator end ( ) const noexcept { return std::cend ( m_data ); }
    [[nodiscard]] constexpr const_iterator cend ( ) const noexcept { return std::cend ( m_data ); }
    [[nodiscard]] constexpr reverse_iterator rbegin ( ) noexcept { return std::rbegin ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin ( ) const noexcept { return std::crbegin ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator crbegin ( ) const noexcept { return std::crbegin ( m_data ); }
    [[nodiscard]] constexpr reverse_iterator rend ( ) noexcept { return std::rend ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator rend ( ) const noexcept { return std::crend ( m_data ); }
    [[nodiscard]] constexpr const_reverse_iterator crend ( ) const noexcept { return std::crend ( m_data ); }

    [[nodiscard]] static constexpr std::size_t size ( ) noexcept { return I * J * K * L; }
    [[nodiscard]] static constexpr std::size_t capacity ( ) noexcept { return I * J * K * L; }
    [[nodiscard]] static constexpr extents_type extents ( ) noexcept { return { I, J, K, L }; }
};

} // namespace sax

#undef MA_ASSERT_4
#undef MA_ASSERT_3
#undef MA_ASSERT_2
#undef MA_ASSERT_1
