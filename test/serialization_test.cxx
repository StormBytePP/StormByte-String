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

#include <StormByte/binary_data.hxx>
#include <StormByte/string/serializable.hxx>
#include <StormByte/test_handlers.h>

#include <cstdint>
#include <cstring>
#include <string>

using StormByte::BinaryData;
using StormByte::Serializable;
using StormByte::String::String;
using StormByte::String::WString;

namespace {
	BinaryData Truncate(const BinaryData& buf, std::size_t new_size) {
		if (new_size >= static_cast<std::size_t>(buf.size().Value()))
			return buf;
		return BinaryData(buf.data(), StormByte::Size{ static_cast<std::uint64_t>(new_size) });
	}
}

// -------------------
// String
// -------------------

int test_string_shares_std_string_wire() {
	const String owned("StormByte");
	const std::string text = "StormByte";
	ASSERT_TRUE("test_string_shares_std_string_wire",
		Serializable<String>(owned).Serialize() == Serializable<std::string>(text).Serialize());
	RETURN_TEST("test_string_shares_std_string_wire", 0);
}

int test_serialize_string() {
	const String data("Hello, StormByte!");
	auto buffer = Serializable<String>(data).Serialize();
	if (buffer.empty())
		RETURN_TEST("test_serialize_string", 1);
	auto expected = Serializable<String>::Deserialize(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_string", 1);
	}
	ASSERT_TRUE("test_serialize_string", data == expected.value());
	ASSERT_EQUAL("test_serialize_string", Serializable<String>::Size(data), static_cast<std::size_t>(buffer.size().Value()));
	RETURN_TEST("test_serialize_string", 0);
}

int test_serialize_string_empty() {
	const String data("");
	auto buffer = Serializable<String>(data).Serialize();
	auto expected = Serializable<String>::Deserialize(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_string_empty", 1);
	}
	ASSERT_TRUE("test_serialize_string_empty", expected.value() == String(""));
	ASSERT_TRUE("test_serialize_string_empty", static_cast<bool>(expected.value()));
	RETURN_TEST("test_serialize_string_empty", 0);
}

int test_serialize_string_huge_size() {
	auto clean = Serializable<String>(String("safe")).Serialize();
	if (clean.size() < StormByte::Size{ sizeof(std::uint64_t) })
		RETURN_TEST("test_serialize_string_huge_size", 1);
	auto buf = clean;
	std::uint64_t huge = static_cast<std::uint64_t>(-1);
	std::memcpy(buf.data(), &huge, sizeof(huge));
	auto result = Serializable<String>::Deserialize(buf);
	if (result) {
		std::cerr << "test_serialize_string_huge_size: huge size was accepted\n";
		RETURN_TEST("test_serialize_string_huge_size", 1);
	}
	RETURN_TEST("test_serialize_string_huge_size", 0);
}

int test_serialize_string_null_is_empty_wire() {
	const String missing;
	const String empty("");
	auto a = Serializable<String>(missing).Serialize();
	auto b = Serializable<String>(empty).Serialize();
	auto c = Serializable<std::string>(std::string()).Serialize();
	ASSERT_TRUE("test_serialize_string_null_is_empty_wire", a == b);
	ASSERT_TRUE("test_serialize_string_null_is_empty_wire", a == c);
	auto expected = Serializable<String>::Deserialize(a);
	if (!expected)
		RETURN_TEST("test_serialize_string_null_is_empty_wire", 1);
	ASSERT_TRUE("test_serialize_string_null_is_empty_wire", expected.value() == String(""));
	RETURN_TEST("test_serialize_string_null_is_empty_wire", 0);
}

int test_serialize_string_truncated() {
	auto buffer = Serializable<String>(String("TruncationTest")).Serialize();
	const auto n = static_cast<std::size_t>(buffer.size().Value());
	for (std::size_t len = 0; len < n; ++len) {
		auto truncated = Truncate(buffer, len);
		auto result = Serializable<String>::Deserialize(truncated);
		if (result) {
			std::cerr << "test_serialize_string_truncated: size " << len << " accepted\n";
			RETURN_TEST("test_serialize_string_truncated", 1);
		}
	}
	RETURN_TEST("test_serialize_string_truncated", 0);
}

// -------------------
// WString
// -------------------

int test_serialize_wstring() {
	const WString data(L"Hello, StormByte!");
	auto buffer = Serializable<WString>(data).Serialize();
	if (buffer.empty())
		RETURN_TEST("test_serialize_wstring", 1);
	auto expected = Serializable<WString>::Deserialize(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_wstring", 1);
	}
	ASSERT_TRUE("test_serialize_wstring", data == expected.value());
	ASSERT_EQUAL("test_serialize_wstring", Serializable<WString>::Size(data), static_cast<std::size_t>(buffer.size().Value()));
	RETURN_TEST("test_serialize_wstring", 0);
}

int test_serialize_wstring_empty() {
	const WString data(L"");
	auto buffer = Serializable<WString>(data).Serialize();
	auto expected = Serializable<WString>::Deserialize(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_wstring_empty", 1);
	}
	ASSERT_TRUE("test_serialize_wstring_empty", expected.value() == WString(L""));
	RETURN_TEST("test_serialize_wstring_empty", 0);
}

int test_serialize_wstring_non_bmp() {
	const WString data(L"\U0001F4A9");
	auto buffer = Serializable<WString>(data).Serialize();
	auto expected = Serializable<WString>::Deserialize(buffer);
	if (!expected) {
		std::cerr << expected.error()->what() << std::endl;
		RETURN_TEST("test_serialize_wstring_non_bmp", 1);
	}
	ASSERT_TRUE("test_serialize_wstring_non_bmp", data == expected.value());
	RETURN_TEST("test_serialize_wstring_non_bmp", 0);
}

int test_serialize_wstring_null_is_empty_wire() {
	const WString missing;
	const WString empty(L"");
	auto a = Serializable<WString>(missing).Serialize();
	auto b = Serializable<WString>(empty).Serialize();
	auto c = Serializable<std::wstring>(std::wstring()).Serialize();
	ASSERT_TRUE("test_serialize_wstring_null_is_empty_wire", a == b);
	ASSERT_TRUE("test_serialize_wstring_null_is_empty_wire", a == c);
	auto expected = Serializable<WString>::Deserialize(a);
	if (!expected)
		RETURN_TEST("test_serialize_wstring_null_is_empty_wire", 1);
	ASSERT_TRUE("test_serialize_wstring_null_is_empty_wire", expected.value() == WString(L""));
	RETURN_TEST("test_serialize_wstring_null_is_empty_wire", 0);
}

int test_serialize_wstring_truncated() {
	auto buffer = Serializable<WString>(WString(L"TruncationTest")).Serialize();
	const auto n = static_cast<std::size_t>(buffer.size().Value());
	for (std::size_t len = 0; len < n; ++len) {
		auto truncated = Truncate(buffer, len);
		auto result = Serializable<WString>::Deserialize(truncated);
		if (result) {
			std::cerr << "test_serialize_wstring_truncated: size " << len << " accepted\n";
			RETURN_TEST("test_serialize_wstring_truncated", 1);
		}
	}
	RETURN_TEST("test_serialize_wstring_truncated", 0);
}

int test_wstring_shares_std_wstring_wire() {
	const WString owned(L"StormByte");
	const std::wstring text = L"StormByte";
	ASSERT_TRUE("test_wstring_shares_std_wstring_wire",
		Serializable<WString>(owned).Serialize() == Serializable<std::wstring>(text).Serialize());
	RETURN_TEST("test_wstring_shares_std_wstring_wire", 0);
}

int main() {
	int result = 0;

	// -------------------
	// String
	// -------------------
	result += test_string_shares_std_string_wire();
	result += test_serialize_string();
	result += test_serialize_string_empty();
	result += test_serialize_string_huge_size();
	result += test_serialize_string_null_is_empty_wire();
	result += test_serialize_string_truncated();

	// -------------------
	// WString
	// -------------------
	result += test_serialize_wstring();
	result += test_serialize_wstring_empty();
	result += test_serialize_wstring_non_bmp();
	result += test_serialize_wstring_null_is_empty_wire();
	result += test_serialize_wstring_truncated();
	result += test_wstring_shares_std_wstring_wire();

	if (result == 0)
		std::cout << "All tests passed!" << std::endl;
	else
		std::cout << result << " tests failed." << std::endl;
	return result;
}
