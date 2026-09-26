## [Summary]

StormByte-String is the C++26 text module of the StormByte suite.

It is not Base, Buffer, Config, Crypto, Database, Logger, Multimedia, Network or System. Those live in their own repos. This module depends on Base 2.0.0 or later.

Public headers under `StormByte/string/` cover `String` and `WString`: owned UTF-8 and wide text on `CString` / `WCString`, safe to use across a DLL boundary. They are not `std::string` / `std::wstring`.

If you landed here from a release link and have not read the tree:

- What this module is, how to build it, and examples: [README.md](https://github.com/StormBytePP/StormByte-String/blob/master/README.md)
- Since 1.0.0 license: original source in this repository is dual-licensed, LGPL v3 or later **or** a commercial license from the copyright holder. The grant does not cover other StormByte modules or `thirdparty/` (including the bundled Base tree). [LICENSE](https://github.com/StormBytePP/StormByte-String/blob/master/LICENSE)

## [Unreleased]

[Unreleased]: https://github.com/StormBytePP/StormByte-String/compare/1.0.0...HEAD

## [1.0.0] - 2026-09-26

### Added

- **String** — owned UTF-8 text on `CString`. Null vs empty, contiguous const iterators, implicit `std::string_view`, explicit `std::string`, `operator<<`, content `==` / `!=` / `<=>`, `swap`, `std::hash`. Covered by `StringTests`.
- **WString** — wide counterpart on `WCString`. Same shape with `wchar_t` / `std::wstring_view` / `std::wstring` / `std::wostream`. Covered by `WStringTests`.
- **Conversion** — explicit `String` ↔ `WString` through UTF-8. Ill-formed input becomes U+FFFD.
- **ToUpper / ToLower** — ASCII and Latin-1 Supplement. Other code points copied as-is. Static and instance.
- **SanitizeNewlines** — CR LF → LF. Static and instance.
- **RemoveWhitespace** — drops `isspace` / `iswspace`. Static and instance.
- **IsInteger** — optional sign plus digits. Static and instance.
- **Split** — whitespace tokens into the caller’s `vector`. Static and instance.
- **Explode** — delimiter tokens into the caller’s `queue`, empty fields kept. Static and instance.
- **License** — original source in this repository is dual-licensed: GNU LGPL v3 or later, or a commercial license from the copyright holder. The grant applies only to original StormByte-String source in this repository. It does not cover other StormByte modules or third-party material (including `thirdparty/` and the bundled Base tree). No patent rights are granted.

### Changed

- **String / WString.** `size`, `length` and `operator[]` take and return `Size`. That is a code-unit count, not a byte count.
- **Serializable.** The wire size of `String` and `WString` is `ByteSize`, the same as Base.
- **Split / Explode.** The fill is `STORMBYTE_FORCE_INLINE` in the caller, including the overloads that return the container. `inline` on an exported class can still be a call into the String DLL. `operator std::string` and `operator std::wstring` use the same macro so that copy stays on the caller heap.
- **String / WString observers.** `starts_with`, `ends_with`, `contains`, `find`, `rfind`, `find_first_of`, `find_last_of`, `find_first_not_of`, `find_last_not_of`, `substr`, `compare`, `front` and `back`. Same shape as `std::string` / `std::wstring`, indexes are `Size`, not-found is `npos`. They do not throw and they do not mutate. `substr` returns an owned copy.

[1.0.0]: https://github.com/StormBytePP/StormByte-String/releases/tag/1.0.0
