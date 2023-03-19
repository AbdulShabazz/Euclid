#pragma once

#include "__x86i64Int.h"
#include <cassert>

#ifdef _DEBUG
	class x86i64IntTestCases
	{
	public:
		x86i64IntTestCases()
		{
			std::__x86i64Int _a_{}; // OK
			std::__x86i64Int a{ 12n }; // OK
			//std::__x86i64Int aa{ 12 }; // Error
			std::__x86i64Int aaa{ "0" };
			std::__x86i64Int aaaa{ "12" }; // OK
			std::__x86i64Int b{ 0n };
			std::__x86i64Int c = std::__x86i64Int{ 12n }; // OK
			//std::__x86i64Int cc = std::__x86i64Int{ 12 }; // Error
			std::__x86i64Int ccc = std::__x86i64Int{ "12" }; // OK
			std::__x86i64Int d = std::__x86i64Int{ 12n };
			std::__x86i64Int e = 12n;
			std::__x86i64Int f(0);
			// a = b = 12n
			assert(a == 12n);
			assert(b == 12n);
			assert(a + b == std::__x86i64Int{ "24" });
			assert(c* d == std::__x86i64Int{ "144" });
			std::pair<std::__x86i64Int, std::__x86i64Int> aa = a / b;
			assert(aa.first == std::__x86i64Int{ "1" });
			assert(aa.second == std::__x86i64Int{ "0" });
			assert(a - b == std::__x86i64Int{ "0" });
			assert(a++ == std::__x86i64Int{ "12" });
			assert(a == std::__x86i64Int{ "13" });
			assert(a-- == std::__x86i64Int{ "13" });
			assert(a == std::__x86i64Int{ "12" });
			++a;
			assert(--a == std::__x86i64Int{ "12" });
			assert(a% b == std::__x86i64Int{ "0" });
			a %= b;
			assert(a == std::__x86i64Int{ "0" });
			a = 12n;
			a += b;
			assert(a == std::__x86i64Int{ "24" });
			a -= b;
			assert(a == std::__x86i64Int{ "12" });
			a *= b;
			assert(a == std::__x86i64Int{ "144" });
			a /= b;
			assert(a.first == 1n);
			assert(a.second == 01n);
			assert(a < b == false);
			assert(a > b == false);
			assert(a <= b == true);
			assert(a >= b == true);
			assert(a == b && b == 12n);
			assert(a != b == false);
			assert(a& b == 12n);
			a &= b;
			assert(a == b == 12n);
		};
	};
#endif
