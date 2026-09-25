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

#include <StormByte/string/serializable.hxx>

#include <string>

namespace StormByte {
	template class STORMBYTE_STRING_INSTANTIATE Serializable<String::String>;
	template class STORMBYTE_STRING_INSTANTIATE Serializable<String::WString>;

	std::size_t Detail::Codec<String::String>::Size(const String::String& data) noexcept {
		return Detail::Codec<std::string>::Size(static_cast<std::string>(data));
	}

	std::vector<std::byte> Detail::Codec<String::String>::Write(const String::String& data) noexcept {
		return Detail::Codec<std::string>::Write(static_cast<std::string>(data));
	}

	Expected<String::String, DeserializeError> Detail::Codec<String::String>::Read(std::span<const std::byte> data) noexcept {
		auto payload = Detail::Codec<std::string>::Read(data);
		if (!payload)
			return Unexpected(payload.error());
		return String::String(payload.value());
	}

	std::size_t Detail::Codec<String::WString>::Size(const String::WString& data) noexcept {
		return Detail::Codec<std::wstring>::Size(static_cast<std::wstring>(data));
	}

	std::vector<std::byte> Detail::Codec<String::WString>::Write(const String::WString& data) noexcept {
		return Detail::Codec<std::wstring>::Write(static_cast<std::wstring>(data));
	}

	Expected<String::WString, DeserializeError> Detail::Codec<String::WString>::Read(std::span<const std::byte> data) noexcept {
		auto payload = Detail::Codec<std::wstring>::Read(data);
		if (!payload)
			return Unexpected(payload.error());
		return String::WString(payload.value());
	}
}
