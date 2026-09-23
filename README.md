# StormByte-String

![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey)
![C++26](https://img.shields.io/badge/C%2B%2B-26-00599C?logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.28+-064F8C?logo=cmake&logoColor=white)
![License: LGPL v3 or commercial](https://img.shields.io/badge/License-LGPL_v3_or_commercial-blue.svg)
[![CI](https://github.com/StormBytePP/StormByte-String/actions/workflows/ci.yml/badge.svg)](https://github.com/StormBytePP/StormByte-String/actions/workflows/ci.yml)
[![Sponsor](https://img.shields.io/badge/Sponsor-StormBytePP-ea4aaa?logo=githubsponsors)](https://github.com/sponsors/StormBytePP)

This repository is **StormByte-String**.

It is not a replacement for `std::string`, `std::wstring` or ICU. Those already do their job better inside a single binary.

The point of this library is a **small, owned string that is safe to return and store across a DLL / shared-object boundary**. `std::string` / `std::wstring` are not: allocator, layout and CRT can differ on each side of the link on Windows. `String` / `WString` own a `CString` / `WCString` allocated by this module. Views (`string_view`, iterators, `data()`) and copies into `std::string` / `std::wstring` are inline in the **caller**, so the caller’s heap is the caller’s heap.

If the text never leaves the module that created it, use `std::string`.

It depends on [StormByte](https://github.com/StormBytePP/StormByte) (Base) 2.0.0 or later.

## What this module does

- **String** — UTF-8 text on `CString`. Contiguous const iterators, implicit `std::string_view`, explicit `std::string`, `operator<<`.
- **WString** — wide text on `WCString`. Same shape with `wchar_t` / `std::wstring_view` / `std::wstring` / `std::wostream`.
- **Conversion** — explicit `String` ↔ `WString` (UTF-8; ill-formed input becomes U+FFFD).
- **Ordering** — content `==` / `!=` / `<=>`, `swap`, `std::hash`. A default object is null; `""` / `L""` is valid empty text. Null is not equal to empty.
- **Algorithms** — `begin` / `end` / `data` / `size` so `<algorithm>` and `std::ranges` run on the object.

On top of that, the types carry operations that show up constantly when text crosses a module boundary: `ToUpper` / `ToLower`, `SanitizeNewlines`, `RemoveWhitespace`, `IsInteger`, `Split` and `Explode`. Each one exists as a static and as an instance method. That list is not a Unicode toolkit and is not frozen; later releases can add more of the same kind.

`ToUpper` / `ToLower` map ASCII and Latin-1 Supplement. Any other code point is copied as-is.

## The rest of the suite

| Module | Role | API |
| --- | --- | --- |
| [Base](https://github.com/StormBytePP/StormByte) | Foundation every other module links | [/StormByte](https://dev.stormbyte.org/StormByte) |
| [Buffer](https://github.com/StormBytePP/StormByte-Buffer) | FIFO, SharedFIFO, Ring, Producer/Consumer and multi-stage pipelines | [/StormByte-Buffer](https://dev.stormbyte.org/StormByte-Buffer) |
| [Config](https://github.com/StormBytePP/StormByte-Config) | Human-readable text and versioned binary documents (groups, lists, raw bytes) | [/StormByte-Config](https://dev.stormbyte.org/StormByte-Config) |
| [Crypto](https://github.com/StormBytePP/StormByte-Crypto) | Hash, compress, encrypt, sign and key agreement — Crypto++ never leaves the private tree | [/StormByte-Crypto](https://dev.stormbyte.org/StormByte-Crypto) |
| [Database](https://github.com/StormBytePP/StormByte-Database) | One API over SQLite, PostgreSQL and MariaDB | [/StormByte-Database](https://dev.stormbyte.org/StormByte-Database) |
| [Logger](https://github.com/StormBytePP/StormByte-Logger) | Stream logger with levels, headers, human-readable sizes and redaction (`ThreadedLog`) | [/StormByte-Logger](https://dev.stormbyte.org/StormByte-Logger) |
| [Multimedia](https://github.com/StormBytePP/StormByte-Multimedia) | Decode, encode and containers without raw FFmpeg types; codecs enabled only if present | [/StormByte-Multimedia](https://dev.stormbyte.org/StormByte-Multimedia) |
| [Network](https://github.com/StormBytePP/StormByte-Network) | Framed packets, Client/Server, IPv4/IPv6 TCP and Buffer pipelines (compress/encrypt) | [/StormByte-Network](https://dev.stormbyte.org/StormByte-Network) |
| **String** | This repository | [/StormByte-String](https://dev.stormbyte.org/StormByte-String) |
| [System](https://github.com/StormBytePP/StormByte-System) | Processes, pipes and environment variables across Linux, Windows and macOS | [/StormByte-System](https://dev.stormbyte.org/StormByte-System) |

## Table of Contents

- [What this module does](#what-this-module-does)
- [The rest of the suite](#the-rest-of-the-suite)
- [Installation](#installation)
- [Usage](#usage)
- [String](#string)
- [WString](#wstring)
- [Conversion](#conversion)
- [Case](#case)
- [Newlines and whitespace](#newlines-and-whitespace)
- [Integer](#integer)
- [Split](#split)
- [Explode](#explode)
- [Algorithms](#algorithms)
- [Contributing](#contributing)
- [License](#license)

## Installation

Needs a C++26 compiler, CMake 3.28 or newer, and Base 2.0.0.

```sh
git clone --recurse-submodules https://github.com/StormBytePP/StormByte-String.git
cd StormByte-String
cmake -S . -B build
cmake --build build
```

Shared library is the default. Static:

```sh
cmake -S . -B build -DSTORMBYTE_STRING_SHARED=OFF
```

Headers:

```cpp
#include <StormByte/string/string.hxx>
#include <StormByte/string/wstring.hxx>
```

## Usage

Namespace root for the types is `StormByte::String`.

A default `String` / `WString` is null (`operator bool` is false, `data()` is null). Constructed from `""` / `L""` it is valid and empty.

### String

```cpp
#include <StormByte/string/string.hxx>
#include <iostream>
#include <string>

using StormByte::String::String;

int main() {
	String text("hello");
	if (text)
		std::cout << text << " " << text.size() << std::endl;

	const std::string_view view = text;
	const std::string copy = text;

	String missing;
	String empty("");
	if (!missing && empty && missing != empty)
		std::cout << "null is not empty" << std::endl;
}
```

### WString

```cpp
#include <StormByte/string/wstring.hxx>
#include <iostream>

using StormByte::String::WString;

int main() {
	WString text(L"wide");
	std::wcout << text << L" " << text.size() << std::endl;
}
```

### Conversion

```cpp
#include <StormByte/string/string.hxx>
#include <StormByte/string/wstring.hxx>
#include <iostream>

using StormByte::String::String;
using StormByte::String::WString;

int main() {
	String utf8("café");
	WString wide(utf8);
	String back(wide);
	WString also = static_cast<WString>(utf8);
	if (back == utf8 && also == wide)
		std::cout << "round-trip" << std::endl;
}
```

### Case

```cpp
#include <StormByte/string/string.hxx>
#include <iostream>

using StormByte::String::String;

int main() {
	std::cout << String::ToUpper("café") << std::endl;
	std::cout << String("CAFÉ").ToLower() << std::endl;
}
```

That prints `CAFÉ` and `café`. `ß`, Greek, Cyrillic and anything outside Latin-1 stay unchanged.

### Newlines and whitespace

```cpp
#include <StormByte/string/string.hxx>
#include <iostream>

using StormByte::String::String;

int main() {
	std::cout << String::SanitizeNewlines("a\r\nb\n") << std::endl;
	std::cout << String(" a\tb\n").RemoveWhitespace() << std::endl;
}
```

`SanitizeNewlines` turns CR LF into LF. Other bytes are kept. `RemoveWhitespace` drops what `isspace` / `iswspace` reports.

### Integer

```cpp
#include <StormByte/string/string.hxx>
#include <iostream>

using StormByte::String::String;

int main() {
	std::cout << String::IsInteger("42") << " "
	          << String("-3").IsInteger() << " "
	          << String::IsInteger("1a") << std::endl;
}
```

An optional leading `+` / `-` plus digits. Empty, `"-"`, whitespace and letters fail.

### Split

Whitespace-separated tokens. Leading and trailing space is skipped. The output container is the caller’s.

```cpp
#include <StormByte/string/string.hxx>
#include <iostream>
#include <vector>

using StormByte::String::String;

int main() {
	std::vector<String> tokens;
	String::Split("  a  bb\tc ", tokens);
	for (const String& token : tokens)
		std::cout << token << std::endl;

	const auto also = String("x y").Split();
}
```

### Explode

Tokens on a delimiter. Empty fields stay in the queue.

```cpp
#include <StormByte/string/string.hxx>
#include <iostream>
#include <queue>

using StormByte::String::String;

int main() {
	std::queue<String> parts;
	String::Explode("a,,b", ',', parts);
	while (!parts.empty()) {
		std::cout << "[" << parts.front() << "]" << std::endl;
		parts.pop();
	}
}
```

That prints `[a]`, `[]`, `[b]`.

### Algorithms

```cpp
#include <StormByte/string/string.hxx>
#include <algorithm>
#include <iostream>
#include <ranges>

using StormByte::String::String;

int main() {
	const String text("mississippi");
	if (std::ranges::find(text, 'p') != text.end())
		std::cout << std::ranges::count(text, 'i') << std::endl;

	const std::string_view view = text;
	if (std::ranges::equal(view, std::string_view("mississippi")))
		std::cout << "view" << std::endl;
}
```

`WString` is the same with `wchar_t` literals (`L"…"`, `L','`, `std::wstring_view`).

## Contributing

Issues and pull requests belong on this repository. Fork and open a PR against `master`.

Read [CONTRIBUTING.md](CONTRIBUTING.md) before you send a patch (copyright assignment and review rules). Coding rules are in [CODING_STYLE.md](CODING_STYLE.md).

## License

Since 1.0.0, original source in this repository is dual-licensed: GNU Lesser General Public License v3 or later, or a commercial license from the copyright holder (David C. Manuelda <StormByte@gmail.com>).

The grant applies only to original StormByte-String source in this repository. It does not cover other StormByte modules or third-party material shipped here (including everything under `thirdparty/`, and in particular the bundled StormByte Base tree), which remains under its own license. Neither license grants patent rights.

See [LICENSE](LICENSE) for the dual-license notice and [COPYING.LGPLv3](COPYING.LGPLv3) for the full GNU LGPL version 3 text. Also <https://www.gnu.org/licenses/lgpl-3.0.html>.

## Support

StormByte is developed in spare time. Sponsorship is optional and does not buy features, priority or support.

- [GitHub Sponsors](https://github.com/sponsors/StormBytePP)
- [PayPal](https://paypal.me/StormBytePP)
