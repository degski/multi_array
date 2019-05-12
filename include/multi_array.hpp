
// MIT License
//
// Copyright (c) 2018, 2019 degski
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
#include <cstdint> // std::intptr_t
#include <cstring> // std::memcpy

#include <array>
#include <type_traits>
#include <utility> // std::forward

// This library invokes Undefined Behaviour (UB), as it exploits the pre-calculation of intermediate
//  pointers [pointing outside the array m_data] in order to gain efficiency when dealing with
//  off-zero indices.

namespace sax {

namespace detail {

template<typename T>
using is_valid_multi_array_type =
    std::enable_if_t<std::conjunction<std::is_default_constructible<T>, std::is_trivially_copyable<T>>::value, T>;
}

template<typename T, std::intptr_t I, std::intptr_t BaseI = 0, typename = detail::is_valid_multi_array_type<T>>
class Vector {

    T m_data[ I ];

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
    Vector ( const Vector & v_ ) noexcept { std::memcpy ( m_data, v_.m_data, I * sizeof ( T ) ); }
    Vector ( Vector && v_ ) noexcept { std::memcpy ( m_data, v_.m_data, I * sizeof ( T ) ); }
    template<typename... Args>
    Vector ( Args... a_ ) noexcept : m_data{ std::forward<Args> ( a_ )... } {}

    using extents_type = std::array<std::intptr_t, 1>;

    [[nodiscard]] constexpr reference at ( const std::intptr_t i_ ) noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        return ( m_data - BaseI )[ i_ ];
    }

    [[nodiscard]] constexpr value_type at ( const std::intptr_t i_ ) const noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        return ( m_data - BaseI )[ i_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr reference rat ( const std::intptr_t i_ ) noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        return ( m_data + I - 1 + BaseI )[ -i_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr value_type rat ( const std::intptr_t i_ ) const noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        return ( m_data + I - 1 + BaseI )[ -i_ ];
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
    [[nodiscard]] static constexpr extents_type extents ( ) noexcept { return extents_type{ I }; }
};

template<typename T, std::intptr_t I, std::intptr_t J, std::intptr_t BaseI = 0, std::intptr_t BaseJ = 0,
         typename = detail::is_valid_multi_array_type<T>>
class Matrix {

    T m_data[ I * J ];

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
    Matrix ( const Matrix & m_ ) noexcept { std::memcpy ( m_data, m_.m_data, I * J * sizeof ( T ) ); }
    Matrix ( Matrix && m_ ) noexcept { std::memcpy ( m_data, m_.m_data, I * J * sizeof ( T ) ); }
    template<typename... Args>
    Matrix ( Args... a_ ) noexcept : m_data{ std::forward<Args> ( a_ )... } {}

    using extents_type = std::array<std::intptr_t, 2>;

    [[nodiscard]] constexpr const_reference ref ( const std::intptr_t i_, const std::intptr_t j_ ) const noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        return ( m_data - BaseJ - BaseI * J )[ j_ + i_ * J ];
    }

    [[nodiscard]] constexpr reference at ( const std::intptr_t i_, const std::intptr_t j_ ) noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        return ( m_data - BaseJ - BaseI * J )[ j_ + i_ * J ];
    }

    [[nodiscard]] constexpr value_type at ( const std::intptr_t i_, const std::intptr_t j_ ) const noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        return ( m_data - BaseJ - BaseI * J )[ j_ + i_ * J ];
    }

    // Reverse ref (rref).
    [[nodiscard]] constexpr const_reference rref ( const std::intptr_t i_, const std::intptr_t j_ ) noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        return ( m_data + I * J - 1 + BaseJ + BaseI * J )[ -j_ - i_ * J ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr reference rat ( const std::intptr_t i_, const std::intptr_t j_ ) noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        return ( m_data + I * J - 1 + BaseJ + BaseI * J )[ -j_ - i_ * J ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr value_type rat ( const std::intptr_t i_, const std::intptr_t j_ ) const noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        return ( m_data + I * J - 1 + BaseJ + BaseI * J )[ -j_ - i_ * J ];
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
    [[nodiscard]] static constexpr extents_type extents ( ) noexcept { return extents_type{ I, J }; }
};

template<typename T, std::intptr_t I, std::intptr_t J, std::intptr_t BaseI = 0, std::intptr_t BaseJ = 0,
         typename = std::enable_if_t<std::is_default_constructible<T>::value, T>>
using MatrixRM = Matrix<T, I, J, BaseI, BaseJ>;

template<typename T, std::intptr_t J, std::intptr_t I, std::intptr_t BaseJ = 0, std::intptr_t BaseI = 0,
         typename = std::enable_if_t<std::is_default_constructible<T>::value, T>>
using MatrixCM = Matrix<T, J, I, BaseJ, BaseI>;

template<typename T, std::intptr_t I, std::intptr_t J, std::intptr_t K, std::intptr_t BaseI = 0, std::intptr_t BaseJ = 0,
         std::intptr_t BaseK = 0, typename = detail::is_valid_multi_array_type<T>>
class Cube {

    T m_data[ I * J * K ];

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
    Cube ( const Cube & c_ ) noexcept { std::memcpy ( m_data, c_.m_data, I * J * K * sizeof ( T ) ); }
    Cube ( Cube && c_ ) noexcept { std::memcpy ( m_data, c_.m_data, I * J * K * sizeof ( T ) ); }
    template<typename... Args>
    Cube ( Args... a_ ) noexcept : m_data{ std::forward<Args> ( a_ )... } {}

    using extents_type = std::array<std::intptr_t, 3>;

    [[nodiscard]] constexpr reference at ( const std::intptr_t i_, const std::intptr_t j_, const std::intptr_t k_ ) noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        assert ( k_ >= BaseK );
        assert ( k_ < K + BaseK );
        return ( m_data + K * ( -BaseJ - BaseI * J ) - BaseK )[ K * ( j_ + i_ * J ) + k_ ];
    }

    [[nodiscard]] constexpr value_type at ( const std::intptr_t i_, const std::intptr_t j_, const std::intptr_t k_ ) const
        noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        assert ( k_ >= BaseK );
        assert ( k_ < K + BaseK );
        return ( m_data + K * ( -BaseJ - BaseI * J ) - BaseK )[ K * ( j_ + i_ * J ) + k_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr reference rat ( const std::intptr_t i_, const std::intptr_t j_, const std::intptr_t k_ ) noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        assert ( k_ >= BaseK );
        assert ( k_ < K + BaseK );
        return ( m_data + I * J * K - 1 + BaseJ * K + BaseI * J * K + BaseK )[ K * ( -j_ - i_ * J ) - k_ ];
    }

    // Reverse at (rat).
    [[nodiscard]] constexpr value_type rat ( const std::intptr_t i_, const std::intptr_t j_, const std::intptr_t k_ ) const
        noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        assert ( k_ >= BaseK );
        assert ( k_ < K + BaseK );
        return ( m_data + I * J * K - 1 + BaseJ * K + BaseI * J * K + BaseK )[ K * ( -j_ - i_ * J ) - k_ ];
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
    [[nodiscard]] static constexpr extents_type extents ( ) noexcept { return extents_type{ I, J, K }; }
};

template<typename T, std::intptr_t I, std::intptr_t J, std::intptr_t K, std::intptr_t L, std::intptr_t BaseI = 0,
         std::intptr_t BaseJ = 0, std::intptr_t BaseK = 0, std::intptr_t BaseL = 0, typename = detail::is_valid_multi_array_type<T>>
class HyperCube {

    T m_data[ I * J * K * L ];

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
    HyperCube ( const HyperCube & h_ ) noexcept { std::memcpy ( m_data, h_.m_data, I * J * K * L * sizeof ( T ) ); }
    HyperCube ( HyperCube && h_ ) noexcept { std::memcpy ( m_data, h_.m_data, I * J * K * L * sizeof ( T ) ); }
    template<typename... Args>
    HyperCube ( Args... a_ ) noexcept : m_data{ std::forward<Args> ( a_ )... } {}

    using extents_type = std::array<std::intptr_t, 4>;

    [[nodiscard]] constexpr reference at ( const std::intptr_t i_, const std::intptr_t j_, const std::intptr_t k_,
                                           const std::intptr_t l_ ) noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        assert ( k_ >= BaseK );
        assert ( k_ < K + BaseK );
        assert ( l_ >= BaseL );
        assert ( l_ < L + BaseL );
        return ( m_data + L * ( K * ( -BaseJ - BaseI * J ) - BaseK ) - BaseL )[ L * ( K * ( j_ + i_ * J ) + k_ ) + l_ ];
    }

    [[nodiscard]] constexpr value_type at ( const std::intptr_t i_, const std::intptr_t j_, const std::intptr_t k_,
                                            const std::intptr_t l_ ) const noexcept {
        assert ( i_ >= BaseI );
        assert ( i_ < I + BaseI );
        assert ( j_ >= BaseJ );
        assert ( j_ < J + BaseJ );
        assert ( k_ >= BaseK );
        assert ( k_ < K + BaseK );
        assert ( l_ >= BaseL );
        assert ( l_ < L + BaseL );
        return ( m_data + L * ( K * ( -BaseJ - BaseI * J ) - BaseK ) - BaseL )[ L * ( K * ( j_ + i_ * J ) + k_ ) + l_ ];
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
    [[nodiscard]] static constexpr extents_type extents ( ) noexcept { return extents_type{ I, J, K, L }; }
};

}; // namespace sax
