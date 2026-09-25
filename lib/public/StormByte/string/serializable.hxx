/*
 * Copyright (C) 2024-2026 David C. Manuelda (StormBytePP)
 *
 * This file is part of StormByte-String.
 *
 * StormByte-String original source is dual-licensed:
 *
 * 1. GNU Lesser General Public License v3.0 (or later)
 *    You may redistribute and/or modify this file under the terms of the
 *    GNU Lesser General Public License as published by the Free Software
 *    Foundation, either version 3 of the License, or (at your option)
 *    any later version.
 *
 * 2. Commercial license
 *    Alternatively, this file may be used under the terms of a commercial
 *    license agreement with the copyright holder
 *    (David C. Manuelda <StormByte@gmail.com>).
 *
 * Both licenses apply only to original StormByte-String source in this
 * repository. They do not cover other StormByte modules or any third-party
 * material shipped with this repository (including everything under
 * thirdparty/, and in particular the bundled StormByte Base tree),
 * which remains under its own license.
 *
 * Neither license grants any patent rights. Any patent licenses required
 * to use this software or third-party components must be obtained separately
 * from the patent holders.
 *
 * StormByte-String is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with StormByte-String. If not, see
 * <https://www.gnu.org/licenses/lgpl-3.0.html>.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-StormByte-Commercial
 */

#pragma once

#include <StormByte/serializable.hxx>
#include <StormByte/string/string.hxx>
#include <StormByte/string/visibility.h>
#include <StormByte/string/wstring.hxx>

#include <span>
#include <vector>

/**
 * @namespace StormByte
 * @brief Root namespace of the StormByte suite.
 */
namespace StormByte {
	/**
	 * @namespace StormByte::Detail
	 * @brief Private specialization point for non-trivial types.
	 */
	namespace Detail {
		/**
		 * @brief @ref StormByte::Detail::Codec specialization for @ref StormByte::String::String.
		 *
		 * Same wire as `std::string`: `uint64` byte count (LE) + raw UTF-8 bytes.
		 * A null @ref StormByte::String::String is written as an empty payload.
		 * Decode always yields a non-null buffer (`""` when the payload is empty).
		 */
		template<>
		struct Codec<String::String> {
			/**
			 * @brief Serialized size of @p data.
			 * @param data Value to measure.
			 * @return Size in bytes.
			 */
			static STORMBYTE_STRING_PUBLIC std::size_t Size(const String::String& data) noexcept;

			/**
			 * @brief Encodes @p data.
			 * @param data Value to encode.
			 * @return Blob.
			 */
			static STORMBYTE_STRING_PUBLIC std::vector<std::byte> Write(const String::String& data) noexcept;

			/**
			 * @brief Decodes a @ref StormByte::String::String from the start of @p data.
			 * @param data Input span.
			 * @return Value, or @ref StormByte::DeserializeError.
			 */
			static STORMBYTE_STRING_PUBLIC Expected<String::String, DeserializeError> Read(std::span<const std::byte> data) noexcept;
		};

		/**
		 * @brief @ref StormByte::Detail::Codec specialization for @ref StormByte::String::WString.
		 *
		 * Same UTF-8 wire as `std::wstring`. A null @ref StormByte::String::WString
		 * is written as an empty payload. Decode always yields a non-null
		 * buffer (`L""` when the payload is empty).
		 */
		template<>
		struct Codec<String::WString> {
			/**
			 * @brief Serialized size of @p data.
			 * @param data Value to measure.
			 * @return Size in bytes.
			 */
			static STORMBYTE_STRING_PUBLIC std::size_t Size(const String::WString& data) noexcept;

			/**
			 * @brief Encodes @p data.
			 * @param data Value to encode.
			 * @return Blob.
			 */
			static STORMBYTE_STRING_PUBLIC std::vector<std::byte> Write(const String::WString& data) noexcept;

			/**
			 * @brief Decodes a @ref StormByte::String::WString from the start of @p data.
			 * @param data Input span.
			 * @return Value, or @ref StormByte::DeserializeError.
			 */
			static STORMBYTE_STRING_PUBLIC Expected<String::WString, DeserializeError> Read(std::span<const std::byte> data) noexcept;
		};
	}

	/// @cond
	extern template class STORMBYTE_STRING_PUBLIC Serializable<String::String>;
	extern template class STORMBYTE_STRING_PUBLIC Serializable<String::WString>;
	/// @endcond
}
