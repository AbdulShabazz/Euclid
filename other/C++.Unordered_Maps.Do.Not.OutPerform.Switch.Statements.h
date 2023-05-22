#include <iostream>
#include <cstdlib>
#include <utility>
#include <chrono>
#include <vector>
#include <thread>
#include <unordered_map>
#include <functional>

void LHS_Reduce()
{
    std::this_thread::sleep_for(std::chrono::nanoseconds(2));
}

void LHS_Expand()
{
    std::this_thread::sleep_for(std::chrono::nanoseconds(2));
}

void RHS_Reduce()
{
    std::this_thread::sleep_for(std::chrono::nanoseconds(2));
}

void RHS_Expand()
{
    std::this_thread::sleep_for(std::chrono::nanoseconds(2));   
}

int main()
{
    const int& lhs_reduce = 0x00; 
    const int& lhs_expand = 0x01; 
    const int& rhs_reduce = 0x02; 
    const int& rhs_expand = 0x03; 

    std::unordered_map<int, std::function<void()>> OP;
    OP[0x00] = [](){ LHS_Reduce(); };
    OP[0x01] = [](){ LHS_Expand(); };
    OP[0x02] = [](){ RHS_Reduce(); };
    OP[0x03] = [](){ RHS_Expand(); };
    
    std::vector<int> buff{ 0x00, 0x01, 0x02, 0x03 };
	
	auto start_time_chrono = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1e4; ++i)
    {
        for(const int& val : buff)
        {
            switch(val)
            {
                case 0x00:
                {
                    LHS_Reduce();
                    break;
                }
                case 0x01:
                {
                    LHS_Expand();
                    break;
                }
                case 0x02:
                {
                    RHS_Reduce();
                    break;
                }
                case 0x03:
                {
                    RHS_Expand();
                    break;
                }
            }
        }
    }
	auto end_time_chrono = std::chrono::high_resolution_clock::now();
	auto duration_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_chrono - start_time_chrono).count();
	std::cout << "Total Duration (nanoseconds): " << duration_chrono << std::endl;
	
	start_time_chrono = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1e4; ++i)
    {
        for(const int& val : buff)
        {
            // Example usage
            OP[val](); 
        }
    }
	end_time_chrono = std::chrono::high_resolution_clock::now();
	duration_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_chrono - start_time_chrono).count();
	std::cout << "Total Duration (nanoseconds): " << duration_chrono << std::endl;
	
    return EXIT_SUCCESS;
}
