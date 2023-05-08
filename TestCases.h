#pragma once

#include "__x86i64Int.h"
#include <cassert>
#include <exception>
#define _DEBUG
#ifdef _DEBUG
	class x86i64IntTestCases
	{
	public:
		x86i64IntTestCases()
		{
			std::__x86i64Int a{ "191" };
			std::cout << "a: " << (a) << std::endl;
			std::__x86i64Int b{ "119" };
			std::cout << "b: " << b << std::endl;
			std::__x86i64Int c = a + b;
			std::cout << "c = a + b: " << c << std::endl;
			std::__x86i64Int d = a * b;
			std::cout << "d = a * b: " << d << std::endl;
			std::__x86i64Int e = a / b;
			std::cout << "e = a / b, \ne.quotient(): " << e.quotient() << ", \ne.remainder() : " << e.remainder() << std::endl;
			std::__x86i64Int _a_{};
			a = 12n;
			std::cout << "a = 12n: " << b << std::endl;
			try 
			{
				// std::__x86i64Int aa( 12 ); // Compile Error
			}
			catch (const std::exception& e)
			{
				std::cout << "std::__x86i64Int aa( 12 ) // Compiler Error: " << e.what() << std::endl;
			}
			try
			{
				// std::__x86i64Int aa(0); // Runtime Error
			}
			catch (const std::exception& e)
			{
				std::cout << "std::__x86i64Int aa( 0 ) // Runtime Error: " << e.what() << std::endl;
			}
			std::__x86i64Int aaaa{ "12" };
			b = 0n;
			c = std::__x86i64Int{ 12n };
			std::__x86i64Int ccc = std::__x86i64Int{ "12" };
			d = std::__x86i64Int{ 12n };
			std::__x86i64Int f{ 0n };
			a = b = 12n;
			std::cout << "a = b = 12n, a: " << a << ", b: " << b << std::endl;
			std::cout << "a + b // 24 : " << a + b << std::endl;
			std::cout << "c * d // 144 : " << c * d << std::endl;
			std::cout << "a - b // 0 : " << a - b << std::endl;
			std::cout << "a++ // 12 : " << a++ << std::endl;
			std::cout << "a // 13 : " << a << std::endl;
			std::cout << "a-- // 13 : " << a-- << std::endl;
			std::cout << "a // 12: " << a << std::endl;
			std::cout << "++a // 13 : " << ++a << std::endl;
			std::cout << "a // 13 : " << a << std::endl;
			std::cout << "--a // 12 : " << --a << std::endl;
			std::cout << "a // 12 : " << a << std::endl;
			std::cout << "a %= b // 0 : " << (a %= b) << std::endl;
			a = 12n;
			a += b;
			std::cout << "a = 12n; a += b; // 24n : " << a << std::endl;
			a -= b;
			std::cout << "a -= b // 12n : " << a << std::endl;
			a *= b;
			std::cout << "a *= b // 144: " << a << std::endl;
			std::cout << "a == std::__x86i64Int{ \"144\" }) // True: " << (a == std::__x86i64Int{ "144" }) << std::endl;
			a /= b;
			std::cout << "(a /= b) // a.quotient() == 1n: " << a.quotient() << std::endl;
			std::cout << "(a /= b) // a.remainder() == 0n: " << a.remainder() << std::endl;
			std::cout << "a < b == false : " << (a < b) << std::endl;
			std::cout << "a > b == false : " << (a > b) << std::endl;
			std::cout << "a <= b == true : " << (a <= b) << std::endl;
			std::cout << "a >= b == true : " << (a >= b) << std::endl;
			std::cout << "a == b && b == 12n : " << (a == b && b == 12n) << std::endl;
			std::cout << "a != b == false : " << (a != b) << std::endl;
			std::cout << "a & b == 12 // 12 : " << (a & b) << std::endl;
		};
	};
#endif
