#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <queue>
#include <chrono>
#include <unordered_map>
#include <mutex>
#include <thread>

namespace Euclid_Prover {

	#define IOSTREAM_INCLUDED // Comment out to disable all std::cout messages. //

	#ifdef IOSTREAM_INCLUDED
	std::vector<std::string> TraceCallStackStdStrVec{ "EuclidProver" };
	#endif

	void __stdlog__(const std::initializer_list<std::string>& msg, const bool AddNewlineFlag = true)
	{
		// Check if the iostream library is included.
        #ifdef IOSTREAM_INCLUDED
		auto it = msg.begin();
		auto IT = msg.end();
		std::cout << *it;
		++it; // Advance the iterator by 1 position //
		for (it; it != IT; ++it)
		{
			std::cout << *it;
		}
		if (AddNewlineFlag)
			std::cout << std::endl;
        #endif
	};

	void __stdtracein__(const std::string& msg)
	{
		// Check if the iostream library is included.
	    #ifdef IOSTREAM_INCLUDED
		TraceCallStackStdStrVec.emplace_back(msg);
		const std::size_t I = TraceCallStackStdStrVec.size();
		std::cout << TraceCallStackStdStrVec[0];
		for (std::size_t i = 1; i < I; ++i)
		{
			std::cout << " >> " << TraceCallStackStdStrVec[i];
		}
		std::cout << std::endl;
	    #endif
	};

	void __stdtraceout__(const std::string& msg)
	{
		// Check if the iostream library is included.
	#ifdef IOSTREAM_INCLUDED
		TraceCallStackStdStrVec.pop_back();
		const std::size_t I = TraceCallStackStdStrVec.size();
		std::string buff{ TraceCallStackStdStrVec[0] };
		for (std::size_t i = 1; i < I; ++i)
		{
			buff.append(" >> " + TraceCallStackStdStrVec[i]);
		}
		std::cout << buff << " << " << msg << std::endl;
		std::cout << buff << std::endl;
	#endif
	};

	/**
		AUTHOR:
			Seagat2011 (https://github.com/Seagat2011 )

		TITLE:
			EuclidProver.h

		DESCRIPTION:
			Term rewrite engine.

		TEST CASE 246:
			nanoseconds elapsed: +5 min

		REFERENCES
			OpenAI GPT-4-32k-0314 ( { max_tokens:32000, temperature:1.0, top_p:1.0, N:1,
				stream:false, logprobs:NULL, echo:false, stop:NULL, presence_penalty:0,
				frequency_penalty:0, best_of:1, logit_bias:NULL } )

		COMPATIBILITY
			Windows 11+ x86i64
	*/
	
	void PrintPath(const std::vector<std::vector<std::vector<std::string>>>& OutProofStep3DStdStrVec)
	{
		__stdtracein__("PrintPath");
	    for (const std::vector<std::vector<std::string>>& x : OutProofStep3DStdStrVec)
	    {
	        __stdlog__({ "{" }, false);
    	    for (const std::vector<std::string>& y : x)
    	    {
    	        __stdlog__({ "{" }, false);
    	       for (const std::string& z : y)
        	    {
        	        __stdlog__({ " ", z, " " }, false);
        	    } 
    	        __stdlog__({ "}, " }, false);
    	    }
	        __stdlog__({ "}" });
	    }
		__stdtraceout__("PrintPath");
	}
	
	std::string ComputeHash(const std::vector<std::string>& Theorem_Src)
	{
		__stdtracein__("ComputeHash");
		std::string retbuff {"{"};
		
	       for (const std::string& z : Theorem_Src)
    	    {
    	        retbuff += z;
    	    } 
            retbuff += "}";
            
        retbuff += "}";
	    
		__stdtraceout__("ComputeHash");
		return retbuff;
	}

	bool SubnetFound
	(
		const std::vector<std::string>& Theorem_Src,
		const std::vector<std::string>& Axiom_Src,
		const std::vector<std::string>& Axiom_Dest,
		std::vector<std::string>& Theorem_Dest
	)
	{
		__stdtracein__("SubnetFound");
		const std::size_t& th_size = Theorem_Src.size();
		const std::size_t& ax_size = Axiom_Src.size();

		bool SubnetFoundFlag{};
		
		if (ax_size <= th_size)
		{
			std::size_t i_ax{};

			for (const std::string& th_tok : Theorem_Src)
			{
			    const bool MatchFoundFlag = (!SubnetFoundFlag && (Axiom_Src[i_ax] == th_tok));
				if (MatchFoundFlag) {
					++i_ax;
					const bool LocalSubnetFoundFlag = ( i_ax == ax_size );
					if (LocalSubnetFoundFlag)
					{
						for (const std::string ax_tok : Axiom_Dest) {
							Theorem_Dest.emplace_back(ax_tok);
						}
                        __stdlog__({"SubnetFound!"});
						SubnetFoundFlag = true;
						i_ax = 0;
						continue;
					}
				} else {
					i_ax = 0;
					Theorem_Dest.emplace_back(th_tok);
				}
			}
		}
		__stdtraceout__("SubnetFound");
		return SubnetFoundFlag;
	};

    auto Prove = [] () -> void
    {
        __stdtracein__("Prove");
        
        bool QED{};

        constexpr int LHS = 0;
        constexpr int RHS = 1;
        constexpr int opcode_field = 2;
        constexpr int opcode = 0;
        constexpr int axiom_label = 1;
        constexpr int guid_UInt64 = 2;

        const std::string axiom_name = "Axiom_";
        const std::string theorem_name = "Theorem_";

        class OPCODESTRUCT {
        public:
            const std::string lhs_reduce = "lhs_reduce ";
            const std::string lhs_expand = "lhs_expand ";
            const std::string rhs_reduce = "rhs_reduce ";
            const std::string rhs_expand = "rhs_expand ";
        };

        OPCODESTRUCT OpcodeStruct;

        using PROOF_STEP = std::vector<std::vector<std::vector<std::string>>>;
        using PROOF_REWRITE = std::vector<std::vector<std::string>>;

        // input //
        
        std::vector<std::vector<std::vector<std::string>>> InProofLibrary3DStdStrVec =
        
        {
            // {{lhs},{rhs}} // 
            {{"1", "+", "1", "+", "1", "+", "1"},{"4"}},
            {{"1", "+", "1"},{"2"}},
            {{"2", "+", "2"},{"4"}}
        };
        
        PROOF_STEP OutProofStep3DStdStrVec;

        OutProofStep3DStdStrVec.emplace_back(InProofLibrary3DStdStrVec[0]);
        OutProofStep3DStdStrVec[0].push_back ({ "root", theorem_name, "0" });
        
        PrintPath(OutProofStep3DStdStrVec);

        /*
            // internal representation //
            std::vector<
            std::vector<
            std::vector<std::string>>> OutProofStep3DStdStrVec =
            {
                // {{opcode},{lhs},{rhs}} //
                {{"1","+","1","+","1","+","1"},{"4"},{"root","Theorem_","0"}}
            };
        */

		bool bFastForwardFlag{};
		
        std::unordered_map<std::string, PROOF_STEP>
        LHSRouteHistoryMap, RHSRouteHistoryMap;
        
        std::priority_queue<PROOF_STEP> Tasks_Thread, FastForwardTask_Thread;

        Tasks_Thread.push(OutProofStep3DStdStrVec);

        // *** Core Proof Engine (Loop) *** //
        while (!Tasks_Thread.empty() && !QED)
        {
            PROOF_STEP TempProofStep3DStdStrVec { !bFastForwardFlag ? Tasks_Thread.top() : FastForwardTask_Thread.top() };
            
            !bFastForwardFlag ? Tasks_Thread.pop() : FastForwardTask_Thread.pop() ;
            
		    bFastForwardFlag = false;
            
            const PROOF_REWRITE& Theorem{ TempProofStep3DStdStrVec.back() };
            
            const std::size_t I = InProofLibrary3DStdStrVec.size();
            for(std::size_t i = 1; i < I; ++i)
            {
                const
                std::vector<
                std::vector<std::string>>& Axiom = InProofLibrary3DStdStrVec[i];
                
                PROOF_REWRITE
                Theorem_0000{Theorem},
                Theorem_0001{Theorem},
                Theorem_0002{Theorem},
                Theorem_0003{Theorem},
                Theorem_0004{Theorem},
                Theorem_0005{Theorem},
                Theorem_0006{Theorem},
                Theorem_0007{Theorem};
                
                Theorem_0000[LHS].clear();
                Theorem_0001[LHS].clear();
                
                Theorem_0002[RHS].clear();
                Theorem_0003[RHS].clear();
                
                bool lhs_reduce_flag;
                bool lhs_expand_flag;
                bool rhs_reduce_flag;
                bool rhs_expand_flag;
    
                if (lhs_reduce_flag = SubnetFound(Theorem[LHS], Axiom[LHS], Axiom[RHS], Theorem_0000[LHS]))
                {
                    __stdlog__({"lhs_reduce_ via ",axiom_name, std::to_string(i)});
                    Theorem_0000[opcode_field][opcode] = OpcodeStruct.lhs_reduce + " via: ";
                    Theorem_0000[opcode_field][axiom_label] = axiom_name;
                    Theorem_0000[opcode_field][guid_UInt64] = std::to_string(i);
                    
                    PROOF_STEP TempProofStep3DStdStrVec_0000 { TempProofStep3DStdStrVec };
                    TempProofStep3DStdStrVec_0000.emplace_back(Theorem_0000);

					// Commit for later fast-forward //
					const std::string HashKeyStdStr = ComputeHash(Theorem_0000[LHS]);
					if (LHSRouteHistoryMap.find(HashKeyStdStr) == LHSRouteHistoryMap.end())
						LHSRouteHistoryMap.emplace(HashKeyStdStr, TempProofStep3DStdStrVec_0000);

					// Attempt fast-forward //
					if (RHSRouteHistoryMap.find(HashKeyStdStr) != RHSRouteHistoryMap.end()) {
						//std::cout << "Proof found in Module_0000 via Fast-Forward (FF)" << " {" << Theorem_0000[LHS].str() << ", " << Theorem_0000[LHS].str() << "}" << std::endl;
						__stdlog__({ "Proof found in Module_0000 via Fast-Forward (FF)" });
						auto iProofStep2DStdStrVec = RHSRouteHistoryMap[HashKeyStdStr].begin();
						const auto iProofStepEND = RHSRouteHistoryMap[HashKeyStdStr].end();
						const auto& LHS_SOLU = Theorem_0000[LHS];
						// Skip root datum //
						for (++iProofStep2DStdStrVec; iProofStep2DStdStrVec != iProofStepEND; ++iProofStep2DStdStrVec){
						    (*iProofStep2DStdStrVec)[LHS] = LHS_SOLU;
							TempProofStep3DStdStrVec_0000.emplace_back(*iProofStep2DStdStrVec);
						}

						// Is the FF queue still empty? //
						if (FastForwardTask_Thread.empty()) {
							bFastForwardFlag = true;
							FastForwardTask_Thread.push(TempProofStep3DStdStrVec_0000);
							QED = true;
							break;
						}
					}
                    
                    Tasks_Thread.push(TempProofStep3DStdStrVec_0000);
                }
                
                if (lhs_expand_flag = SubnetFound(Theorem[LHS], Axiom[RHS], Axiom[LHS], Theorem_0001[LHS]))
                {
                    __stdlog__({"lhs_expand_ via ",axiom_name, std::to_string(i)});
                    Theorem_0001[opcode_field][opcode] = OpcodeStruct.lhs_expand + " via: ";
                    Theorem_0001[opcode_field][axiom_label] = axiom_name;
                    Theorem_0001[opcode_field][guid_UInt64] = std::to_string(i);
                    
                    PROOF_STEP TempProofStep3DStdStrVec_0001 { TempProofStep3DStdStrVec };
                    TempProofStep3DStdStrVec_0001.emplace_back(Theorem_0001);

					// Commit for later fast-forward //
					const std::string HashKeyStdStr = ComputeHash(Theorem_0001[LHS]);
					if (LHSRouteHistoryMap.find(HashKeyStdStr) == LHSRouteHistoryMap.end())
						LHSRouteHistoryMap.emplace(HashKeyStdStr, TempProofStep3DStdStrVec_0001);

					// Attempt fast-forward //
					if (RHSRouteHistoryMap.find(HashKeyStdStr) != RHSRouteHistoryMap.end()) {
						//std::cout << "Proof found in Module_0001 via Fast-Forward (FF)" << " {" << Theorem_0000[LHS].str() << ", " << Theorem_0000[LHS].str() << "}" << std::endl;
						__stdlog__({ "Proof found in Module_0001 via Fast-Forward (FF)" });
						auto iProofStep2DStdStrVec = RHSRouteHistoryMap[HashKeyStdStr].begin();
						const auto iProofStepEND = RHSRouteHistoryMap[HashKeyStdStr].end();
						const auto& LHS_SOLU = Theorem_0001[LHS];
						// Skip root datum //
						for (++iProofStep2DStdStrVec; iProofStep2DStdStrVec != iProofStepEND; ++iProofStep2DStdStrVec){
						    (*iProofStep2DStdStrVec)[LHS] = LHS_SOLU;
							TempProofStep3DStdStrVec_0001.emplace_back(*iProofStep2DStdStrVec);
						}

						// Is the FF queue still empty? //
						if (FastForwardTask_Thread.empty()) {
							bFastForwardFlag = true;
							FastForwardTask_Thread.push(TempProofStep3DStdStrVec_0001);
							QED = true;
							break;
						}
					}
                    
                    Tasks_Thread.push(TempProofStep3DStdStrVec_0001);
                }
                
                if (rhs_reduce_flag = SubnetFound(Theorem[RHS], Axiom[LHS], Axiom[RHS], Theorem_0002[RHS]))
                {
                    __stdlog__({"rhs_reduce_ via ",axiom_name, std::to_string(i)});
                    Theorem_0002[opcode_field][opcode] = OpcodeStruct.rhs_reduce + " via: ";
                    Theorem_0002[opcode_field][axiom_label] = axiom_name;
                    Theorem_0002[opcode_field][guid_UInt64] = std::to_string(i);
                    
                    PROOF_STEP TempProofStep3DStdStrVec_0002 { TempProofStep3DStdStrVec };
                    TempProofStep3DStdStrVec_0002.emplace_back(Theorem_0002);

					// Commit for later fast-forward //
					const std::string HashKeyStdStr = ComputeHash(Theorem_0002[RHS]);
					if (RHSRouteHistoryMap.find(HashKeyStdStr) == RHSRouteHistoryMap.end())
						RHSRouteHistoryMap.emplace(HashKeyStdStr, TempProofStep3DStdStrVec_0002);

					// Attempt fast-forward //
					if (LHSRouteHistoryMap.find(HashKeyStdStr) != LHSRouteHistoryMap.end()) {
						//std::cout << "Proof found in Module_0002 via Fast-Forward (FF)" << " {" << Theorem_0000[LHS].str() << ", " << Theorem_0000[LHS].str() << "}" << std::endl;
						__stdlog__({ "Proof found in Module_0002 via Fast-Forward (FF)" });
						auto iProofStep2DStdStrVec = LHSRouteHistoryMap[HashKeyStdStr].begin();
						const auto iProofStepEND = LHSRouteHistoryMap[HashKeyStdStr].end();
						const auto& RHS_SOLU = Theorem_0002[RHS];
						// Skip root datum //
						for (++iProofStep2DStdStrVec; iProofStep2DStdStrVec != iProofStepEND; ++iProofStep2DStdStrVec){
						    (*iProofStep2DStdStrVec)[RHS] = RHS_SOLU;
							TempProofStep3DStdStrVec_0002.emplace_back(*iProofStep2DStdStrVec);
						}
						
						// Is the FF queue still empty? //
						if (FastForwardTask_Thread.empty()) {
							bFastForwardFlag = true;
							FastForwardTask_Thread.push(TempProofStep3DStdStrVec_0002);
							QED = true;
							break;
						}
					}
					
                    Tasks_Thread.push(TempProofStep3DStdStrVec_0002);
                }
                
                if (rhs_expand_flag = SubnetFound(Theorem[RHS], Axiom[RHS], Axiom[LHS], Theorem_0003[RHS]))
                {
                    __stdlog__({ "rhs_expand_ via ",axiom_name, std::to_string(i) });
                    Theorem_0003[opcode_field][opcode] = OpcodeStruct.rhs_expand + " via: ";
                    Theorem_0003[opcode_field][axiom_label] = axiom_name;
                    Theorem_0003[opcode_field][guid_UInt64] = std::to_string(i);
                    
                    PROOF_STEP TempProofStep3DStdStrVec_0003 { TempProofStep3DStdStrVec };
                    TempProofStep3DStdStrVec_0003.emplace_back(Theorem_0003);

					// Commit for later fast-forward //
					const std::string HashKeyStdStr = ComputeHash(Theorem_0003[RHS]);
					if (RHSRouteHistoryMap.find(HashKeyStdStr) == RHSRouteHistoryMap.end())
						RHSRouteHistoryMap.emplace(HashKeyStdStr, TempProofStep3DStdStrVec_0003);

					// Attempt fast-forward //
					if (LHSRouteHistoryMap.find(HashKeyStdStr) != LHSRouteHistoryMap.end()) {
						//std::cout << "Proof found in Module_0003 via Fast-Forward (FF)" << " {" << Theorem_0000[LHS].str() << ", " << Theorem_0000[LHS].str() << "}" << std::endl;
						__stdlog__({ "Proof found in Module_0003 via Fast-Forward (FF)" });
						auto iProofStep2DStdStrVec = LHSRouteHistoryMap[HashKeyStdStr].begin();
						const auto iProofStepEND = LHSRouteHistoryMap[HashKeyStdStr].end();
						const auto& RHS_SOLU = Theorem_0003[RHS];
						// Skip root datum //
						for (++iProofStep2DStdStrVec; iProofStep2DStdStrVec != iProofStepEND; ++iProofStep2DStdStrVec){
						    (*iProofStep2DStdStrVec)[RHS] = RHS_SOLU;
							TempProofStep3DStdStrVec_0003.emplace_back(*iProofStep2DStdStrVec);
						}

						// Is the FF queue still empty? //
						if (FastForwardTask_Thread.empty()) {
							bFastForwardFlag = true;
							FastForwardTask_Thread.push(TempProofStep3DStdStrVec_0003);
							QED = true;
							break;
						}
					}
					
                    Tasks_Thread.push(TempProofStep3DStdStrVec_0003);
                }
                
                // Advanced Rewrites //
                
                if (lhs_reduce_flag && rhs_reduce_flag)
                {
                    __stdlog__({ "lhs_reduce + rhs_reduce_ via ",axiom_name, std::to_string(i) });
                    Theorem_0004[opcode_field][opcode] = OpcodeStruct.lhs_reduce + " + " + OpcodeStruct.rhs_reduce + " via: ";
                    Theorem_0004[opcode_field][axiom_label] = axiom_name;
                    Theorem_0004[opcode_field][guid_UInt64] = std::to_string(i);
                    
                    Theorem_0004[LHS] = Theorem_0000[LHS];
                    Theorem_0004[RHS] = Theorem_0002[RHS];
                    
                    PROOF_STEP TempProofStep3DStdStrVec_0004 { TempProofStep3DStdStrVec };
                    TempProofStep3DStdStrVec_0004.emplace_back(Theorem_0004);
                    
                    Tasks_Thread.push(TempProofStep3DStdStrVec_0004);
                }
                
                if (lhs_reduce_flag && rhs_expand_flag)
                {
                    __stdlog__({ "lhs_expand_ + rhs_expand_ via ",axiom_name, std::to_string(i) });
                    Theorem_0005[opcode_field][opcode] = OpcodeStruct.lhs_reduce + " + " + OpcodeStruct.rhs_expand + " via: ";
                    Theorem_0005[opcode_field][axiom_label] = axiom_name;
                    Theorem_0005[opcode_field][guid_UInt64] = std::to_string(i);
                    
                    Theorem_0005[LHS] = Theorem_0000[LHS];
                    Theorem_0005[RHS] = Theorem_0003[RHS];
                    
                    PROOF_STEP TempProofStep3DStdStrVec_0005 { TempProofStep3DStdStrVec };
                    TempProofStep3DStdStrVec_0005.emplace_back(Theorem_0005);
                    
                    Tasks_Thread.push(TempProofStep3DStdStrVec_0005);
                }
                
                if (lhs_expand_flag && rhs_reduce_flag)
                {
                    __stdlog__({ "lhs_expand_ + rhs_reduce_ via ",axiom_name, std::to_string(i) });
                    Theorem_0006[opcode_field][opcode] = OpcodeStruct.lhs_expand + " + " + OpcodeStruct.rhs_reduce + " via: ";
                    Theorem_0006[opcode_field][axiom_label] = axiom_name;
                    Theorem_0006[opcode_field][guid_UInt64] = std::to_string(i);
                    
                    Theorem_0006[LHS] = Theorem_0001[LHS];
                    Theorem_0006[RHS] = Theorem_0002[RHS];
                    
                    PROOF_STEP TempProofStep3DStdStrVec_0006 { TempProofStep3DStdStrVec };
                    TempProofStep3DStdStrVec_0006.emplace_back(Theorem_0006);
                    
                    Tasks_Thread.push(TempProofStep3DStdStrVec_0006);
                }
                
                if (lhs_expand_flag && rhs_expand_flag)
                {
                    __stdlog__({ "lhs_expand_ + rhs_expand_ via ",axiom_name, std::to_string(i) });
                    Theorem_0007[opcode_field][opcode] = OpcodeStruct.lhs_expand + " + " + OpcodeStruct.rhs_expand + " via: ";
                    Theorem_0007[opcode_field][axiom_label] = axiom_name;
                    Theorem_0007[opcode_field][guid_UInt64] = std::to_string(i);
                    
                    Theorem_0007[LHS] = Theorem_0001[LHS];
                    Theorem_0007[RHS] = Theorem_0003[RHS];
                    
                    PROOF_STEP TempProofStep3DStdStrVec_0007 { TempProofStep3DStdStrVec };
                    TempProofStep3DStdStrVec_0007.emplace_back(Theorem_0007);
                    
                    Tasks_Thread.push(TempProofStep3DStdStrVec_0007);
                }
                
            } // end for(...InProofLibrary3DStdStrVec)
            
        } // end while  (!Tasks_Thread.empty() && !QED)
        // *** End Core Proof Engine (Loop) *** //

        if (QED)
        {
            OutProofStep3DStdStrVec = bFastForwardFlag ? FastForwardTask_Thread.top() : Tasks_Thread.top() ;
            PrintPath(OutProofStep3DStdStrVec);
            __stdlog__({ "\n\nQ.E.D.\n" });
        }
        __stdtraceout__("Prove");
    }; // end Prove(...)
	
} // end namespace Proof

#include <string>
#include <chrono>
//#include <Euclid.h>

int main()
{
    Euclid_Prover::__stdtracein__("Main");

    const auto start_time_chrono = std::chrono::high_resolution_clock::now();

    Euclid_Prover::Prove();

    std::cout << std::endl;
    const auto end_time_chrono = std::chrono::high_resolution_clock::now();
    const auto duration_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>( end_time_chrono - start_time_chrono ).count();
    std::cout << "Total Duration (nanoseconds): " << duration_chrono << std::endl;

    Euclid_Prover::__stdtraceout__("Main");

    //std::string Hold_for_user_input{};
    //std::cin >> Hold_for_user_input;

    return EXIT_SUCCESS;
}
