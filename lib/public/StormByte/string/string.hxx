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

#include <StormByte/cstring.hxx>
#include <StormByte/string/visibility.h>

#include <compare>
#include <cstddef>
#include <functional>
#include <iterator>
#include <ostream>
#include <queue>
#include <string>
#include <string_view>
#include <vector>

/**
 * @namespace StormByte
 * @brief Root namespace of the StormByte suite.
 */
namespace StormByte {
	/**
	 * @namespace String
	 * @brief String module: owned UTF-8 and wide text on top of @ref StormByte::CString / @ref StormByte::WCString.
	 */
	namespace String {
		/**
		 * @class WString
		 * @brief Wide counterpart of @ref String. Defined in wstring.hxx.
		 */
		class WString;

		/**
		 * @class String
		 * @brief Owned UTF-8 text composed of @ref StormByte::CString.
		 *
		 * Not a `std::string`. Iterators are constant and contiguous so
		 * algorithms that read a range of `char` work. In-place mutating
		 * algorithms do not: helpers return a new @ref String.
		 *
		 * `operator std::string_view` is implicit and inline. `operator
		 * std::string` is explicit and inline (caller heap).
		 *
		 * Conversion to @ref WString is explicit and runs in the module
		 * (UTF-8 → wide). Conversion from @ref WString copies UTF-8 in
		 * the module.
		 *
		 * `ToUpper` / `ToLower` map only ASCII `A–Z` / `a–z`. Other
		 * well-formed UTF-8 code points are copied. Ill-formed bytes are
		 * copied one-by-one so a sequence is never split in the middle
		 * of a valid character.
		 */
		class STORMBYTE_STRING_PUBLIC String {
			public:
				using value_type = char;	///< Byte type
				using const_iterator = const char*;	///< Contiguous observer
				using const_reverse_iterator = std::reverse_iterator<const_iterator>;	///< Reverse observer

				/**
				 * @name Life
				 * @{
				 */

				/**
				 * @brief Null text.
				 */
				String() noexcept;

				/**
				 * @brief Copies a C string.
				 * @param str Source; may be null.
				 */
				explicit String(const char* str) noexcept;

				/**
				 * @brief Copies a view into an owned NUL-terminated buffer.
				 * @param str Source.
				 */
				explicit String(std::string_view str) noexcept;

				/**
				 * @brief Takes an owned buffer.
				 * @param text Buffer.
				 */
				explicit String(CString text) noexcept;

				/**
				 * @brief UTF-8 from wide text.
				 * @param other Wide source.
				 */
				explicit String(const WString& other) noexcept;

				/**
				 * @brief Copy constructor.
				 * @param other Text to copy.
				 */
				String(const String& other) noexcept;

				/**
				 * @brief Move constructor.
				 * @param other Text to take. @p other becomes null.
				 */
				String(String&& other) noexcept;

				/**
				 * @brief Releases the buffer.
				 */
				~String() noexcept = default;

				/**
				 * @brief Copy assignment.
				 * @param other Text to copy.
				 * @return *this.
				 */
				String& operator=(const String& other) noexcept;

				/**
				 * @brief Move assignment.
				 * @param other Text to take. @p other becomes null.
				 * @return *this.
				 */
				String& operator=(String&& other) noexcept;

				/** @} */

				/**
				 * @name Range
				 * @{
				 */

				/**
				 * @brief First character, or null.
				 * @return Iterator.
				 */
				inline const_iterator begin() const noexcept {
					return data();
				}

				/**
				 * @brief One past the last character, or null.
				 * @return Iterator.
				 */
				inline const_iterator end() const noexcept {
					const char* text = data();
					return text ? text + size() : nullptr;
				}

				/**
				 * @brief First character, or null.
				 * @return Iterator.
				 */
				inline const_iterator cbegin() const noexcept {
					return begin();
				}

				/**
				 * @brief One past the last character, or null.
				 * @return Iterator.
				 */
				inline const_iterator cend() const noexcept {
					return end();
				}

				/**
				 * @brief Reverse begin.
				 * @return Reverse iterator.
				 */
				inline const_reverse_iterator rbegin() const noexcept {
					return const_reverse_iterator(end());
				}

				/**
				 * @brief Reverse end.
				 * @return Reverse iterator.
				 */
				inline const_reverse_iterator rend() const noexcept {
					return const_reverse_iterator(begin());
				}

				/**
				 * @brief Reverse begin.
				 * @return Reverse iterator.
				 */
				inline const_reverse_iterator crbegin() const noexcept {
					return rbegin();
				}

				/**
				 * @brief Reverse end.
				 * @return Reverse iterator.
				 */
				inline const_reverse_iterator crend() const noexcept {
					return rend();
				}

				/**
				 * @brief Contiguous pointer; null when the buffer is null.
				 * @return Pointer to the first byte.
				 */
				inline const char* data() const noexcept {
					return static_cast<const char*>(m_text);
				}

				/**
				 * @brief Byte count; `0` when null or empty.
				 * @return Length.
				 */
				inline std::size_t size() const noexcept {
					return m_text.Length();
				}

				/**
				 * @brief Same as @ref size.
				 * @return Length.
				 */
				inline std::size_t length() const noexcept {
					return size();
				}

				/**
				 * @brief Whether @ref size is zero.
				 * @return Emptiness. A null buffer is empty.
				 */
				inline bool empty() const noexcept {
					return size() == 0;
				}

				/**
				 * @brief Byte at @p index.
				 * @param index Position in `[0, size()]`. `size()` is the trailing NUL.
				 * @return Character.
				 * @note Null or `index > size()` is undefined and `assert`s when assertions are on.
				 */
				inline char operator[](std::size_t index) const noexcept {
					return m_text[index];
				}

				/**
				 * @brief Whether a buffer is held.
				 * @return `false` only for a null @ref CString. `""` is valid and empty.
				 */
				inline explicit operator bool() const noexcept {
					return static_cast<bool>(m_text);
				}

				/** @} */

				/**
				 * @name Conversions
				 * @{
				 */

				/**
				 * @brief Non-owning view of the text.
				 * @return Empty view when the buffer is null.
				 * @note Same lifetime as `std::string::c_str()`.
				 */
				inline operator std::string_view() const noexcept {
					return static_cast<std::string_view>(m_text);
				}

				/**
				 * @brief Copy of the text in the caller’s heap.
				 * @return Empty string when the buffer is null.
				 */
				inline explicit operator std::string() const {
					return static_cast<std::string>(m_text);
				}

				/**
				 * @brief View of the owned buffer.
				 * @return Buffer, or null.
				 * @note Same lifetime as `std::string::c_str()`.
				 */
				inline explicit operator const char*() const noexcept {
					return static_cast<const char*>(m_text);
				}

				/**
				 * @brief Wide text (UTF-8 decoded in the module).
				 * @return Owned @ref WString.
				 */
				explicit operator WString() const noexcept;

				/**
				 * @brief Owned bytes.
				 * @return Internal @ref CString.
				 */
				inline const CString& Bytes() const noexcept {
					return m_text;
				}

				/** @} */

				/**
				 * @name Helpers
				 * @{
				 */

				/**
				 * @brief ASCII-letter lower case; UTF-8 otherwise copied.
				 * @param str Source.
				 * @return New text.
				 */
				static String ToLower(std::string_view str) noexcept;

				/**
				 * @brief ASCII-letter upper case; UTF-8 otherwise copied.
				 * @param str Source.
				 * @return New text.
				 */
				static String ToUpper(std::string_view str) noexcept;

				/**
				 * @brief Turns CR LF into LF.
				 * @param str Source.
				 * @return New text.
				 */
				static String SanitizeNewlines(std::string_view str) noexcept;

				/**
				 * @brief Drops `isspace` bytes.
				 * @param str Source.
				 * @return New text.
				 */
				static String RemoveWhitespace(std::string_view str) noexcept;

				/**
				 * @brief Optional sign plus ASCII digits.
				 * @param str Source.
				 * @return Whether @p str is an integer token.
				 */
				static bool IsInteger(std::string_view str) noexcept;

				/**
				 * @brief Whitespace-separated tokens. @p out is the caller’s container.
				 * @param str Source.
				 * @param[out] out Tokens.
				 */
				static void Split(std::string_view str, std::vector<String>& out) noexcept;

				/**
				 * @brief Tokens on @p delimiter. @p out is the caller’s container.
				 * @param str Source.
				 * @param delimiter Separator.
				 * @param[out] out Tokens, including empty ones.
				 */
				static void Explode(std::string_view str, char delimiter, std::queue<String>& out) noexcept;

				/**
				 * @brief ASCII-letter lower case of this text.
				 * @return New text.
				 */
				inline String ToLower() const noexcept {
					return ToLower(static_cast<std::string_view>(*this));
				}

				/**
				 * @brief ASCII-letter upper case of this text.
				 * @return New text.
				 */
				inline String ToUpper() const noexcept {
					return ToUpper(static_cast<std::string_view>(*this));
				}

				/**
				 * @brief CR LF to LF on this text.
				 * @return New text.
				 */
				inline String SanitizeNewlines() const noexcept {
					return SanitizeNewlines(static_cast<std::string_view>(*this));
				}

				/**
				 * @brief Drops `isspace` bytes from this text.
				 * @return New text.
				 */
				inline String RemoveWhitespace() const noexcept {
					return RemoveWhitespace(static_cast<std::string_view>(*this));
				}

				/**
				 * @brief Whether this text is an integer token.
				 * @return Whether it matches @ref IsInteger(std::string_view).
				 */
				inline bool IsInteger() const noexcept {
					return IsInteger(static_cast<std::string_view>(*this));
				}

				/**
				 * @brief Whitespace-separated tokens. The vector is built in the caller.
				 * @return Tokens.
				 */
				inline std::vector<String> Split() const noexcept {
					std::vector<String> out;
					Split(static_cast<std::string_view>(*this), out);
					return out;
				}

				/**
				 * @brief Tokens on @p delimiter. The queue is built in the caller.
				 * @param delimiter Separator.
				 * @return Tokens, including empty ones.
				 */
				inline std::queue<String> Explode(char delimiter) const noexcept {
					std::queue<String> out;
					Explode(static_cast<std::string_view>(*this), delimiter, out);
					return out;
				}

				/** @} */

				/**
				 * @name Comparison
				 * @{
				 */

				/**
				 * @brief Content equality.
				 * @param other Other text.
				 * @return Whether the texts are equal.
				 */
				inline bool operator==(const String& other) const noexcept {
					return m_text == other.m_text;
				}

				/**
				 * @brief Content inequality.
				 * @param other Other text.
				 * @return Whether the texts differ.
				 */
				inline bool operator!=(const String& other) const noexcept {
					return !(*this == other);
				}

				/**
				 * @brief Content equality with a C string.
				 * @param str May be null.
				 * @return Whether the texts are equal.
				 */
				inline bool operator==(const char* str) const noexcept {
					return m_text == str;
				}

				/**
				 * @brief Content inequality with a C string.
				 * @param str May be null.
				 * @return Whether the texts differ.
				 */
				inline bool operator!=(const char* str) const noexcept {
					return !(*this == str);
				}

				/**
				 * @brief Content order. Null is less than any text.
				 * @param other Other text.
				 * @return Ordering.
				 */
				inline std::strong_ordering operator<=>(const String& other) const noexcept {
					return m_text <=> other.m_text;
				}

				/**
				 * @brief Content order against a C string.
				 * @param str May be null.
				 * @return Ordering.
				 */
				inline std::strong_ordering operator<=>(const char* str) const noexcept {
					return m_text <=> str;
				}

				/** @} */

				/**
				 * @brief Swaps buffers with @p other.
				 * @param other Other text.
				 */
				void swap(String& other) noexcept;

			private:
				CString m_text;	///< Owned bytes
		};

		/**
		 * @brief Writes @p text to @p stream.
		 * @param stream Destination.
		 * @param text Source.
		 * @return @p stream.
		 */
		inline std::ostream& operator<<(std::ostream& stream, const String& text) {
			return stream << text.Bytes();
		}

		/**
		 * @brief Content equality.
		 * @param str C string; may be null.
		 * @param text Text.
		 * @return Whether the texts are equal.
		 */
		inline bool operator==(const char* str, const String& text) noexcept {
			return text == str;
		}

		/**
		 * @brief Content inequality.
		 * @param str C string; may be null.
		 * @param text Text.
		 * @return Whether the texts differ.
		 */
		inline bool operator!=(const char* str, const String& text) noexcept {
			return text != str;
		}

		/**
		 * @brief Swaps two texts.
		 * @param left First text.
		 * @param right Second text.
		 */
		inline void swap(String& left, String& right) noexcept {
			left.swap(right);
		}
	}
}

/**
 * @brief Hash of the text (`0` when the view is empty and the buffer is null).
 */
template<>
struct std::hash<StormByte::String::String> {
	/**
	 * @brief Hashes @p text.
	 * @param text Text.
	 * @return Hash.
	 */
	std::size_t operator()(const StormByte::String::String& text) const noexcept {
		return std::hash<std::string_view>{}(static_cast<std::string_view>(text));
	}
};
