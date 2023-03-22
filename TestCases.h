#pragma once

#include "__x86i64Int.h"
#include <cassert>
#define _DEBUG
#ifdef _DEBUG
	class x86i64IntTestCases
	{
	public:
		x86i64IntTestCases()
		{
			std::__x86i64Int a{ "191" };
			std::cout << a << std::endl;
			std::__x86i64Int b{ "119" };
			std::cout << b << std::endl;
			std::__x86i64Int c = a + b;
			assert(c == a + b);
			std::cout << c << std::endl;
			std::__x86i64Int d = a * b;
			assert(d == a * b);
			std::cout << d << std::endl; 
			std::__x86i64Int e = a / b;
			std::cout << e.quotient() << std::endl;
			std::cout << e.remainder() << std::endl;
			std::__x86i64Int _a_{};
			a = 12n;
			assert(a == 12n);
			//std::__x86i64Int aa( 12 ); // Error
			//std::__x86i64Int aaa{ 0 }; // Error
			std::__x86i64Int aaaa{ "12" };
			b = 0n;
			assert(b == 0n);
			c = std::__x86i64Int{ 12n };
			std::__x86i64Int ccc = std::__x86i64Int{ "12" };
			d = std::__x86i64Int{ 12n };
			std::__x86i64Int f{ 0n };
			a = b = 12n;
			assert(a == 12n);
			assert(b == 12n);
			assert(a + b == std::__x86i64Int{ "24" });
			assert(c * d == std::__x86i64Int{ "144" });
			assert(a - b == std::__x86i64Int{ "0" });
			assert(a++ == std::__x86i64Int{ "12" });
			assert(a == std::__x86i64Int{ "13" });
			assert(a-- == std::__x86i64Int{ "13" });
			assert(a == std::__x86i64Int{ "12" });
			++a;
			assert(--a == std::__x86i64Int{ "12" });
			assert(a % b == std::__x86i64Int{ "0" });
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
			std::cout << a.quotient() << std::endl;
			std::cout << a.remainder() << std::endl;
			assert(a.quotient() == 1n);
			assert(a.remainder() == 01n);
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
