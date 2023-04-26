

/*

  AUTHOR
      Seagat2011 www.gitub.com/Seagat2011
      eterna.cmu.edu/web/player/90270/
      fold.it/port/user/1992490

  VERSION
      Major.Minor.Bugfix.Patch
      12.0.0.0

  DESCRIPTION
    Theorem prover written in C++20. Ported from JavaScript (A grammar reduction term-rewriting system)
    for use in the Unreal Engine 5.2 Core Lib.

  UPDATES (C++20)
    +_lhs/_rhs RouteSource_EnumClass support for all subnet route maps (Improved branch Performance)
    +Specialization templates - for the primary "Auto" template function declaration (Improved branch Performance)
    +Lockless stack manager: RecursionLimiter (Eliminates mutex/semaphore Performance penalty)
    -Multithread support (Increased latency, reduced scalability)

  UPDATES (JavaScript)
    +Negative proof assertions ~=
    +_AXIOM_.optimizeCallGraph
    +Improved ProofComplete search performance
    +Prove via Auto (PASS)
    +Axiom._eval => Axiom._reduce
    +Axiom.{_reduce,_expand} => eventListener(s)
    +solutionEditor => contentEditable
    +Prove via Reduce (PASS)
    +Prove via Expand (PASS)
    +scoping functionality
    +LibreOffice math library support
    -Axiom._eval eventListener

  NOTES:
    Term rewrites are performed with the aid of a compiler (ie. via LEMMA SUBSTITUTION); SEE TEST CASES

    Substitution methods:
    1. (direct) AXIOMATIC: 1 + 1 = 2
    2. (helper) LEMMA SUBSTITUTION: 1 <==> 1 / 1
    Note: Lemma substitutions rewrite axioms, which can introduce recursion, stack overflow, and other bugs:
    Consider "{ PlayerCharacterSideKick } IsIn { StyxBoat }" - the IsIn operator may or may not link unrelated categories,
    whereas "{ PlayerCharacterSideKick } IsIn_StyxBoat " is guaranteed to converge !!!

  Usage Example. // Lemma substitutions //

    { { a } raised { 2 } } plus { 2ab } plus { b raised { 2 } } <== ( { a } plus { b } ) raised { 2 }
    ( { a } plus { b } ) raised { 2 } minus { 2ab } = { c } raised { 2 } <== ( { a } plus { b } ) raised { 2 } = { { c } raised { 2 } } plus { 2ab }
    { { a } raised { 2 } } plus { 2ab } minus { 2ab } plus { b raised { 2 } } ==> { { a } raised { 2 } } plus { { b } raised { 2 } }
    ( { a } plus { b } ) raised { 2 } = { { c } raised { 2 } } plus { 2ab }
    Prove { { a } raised { 2 } } plus { { b } raised { 2 } } = { c } raised { 2 }

  Usage Example. // Lemma substitution //

    // Axiom
    { PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsIn { StyxRiver } // Current Game State
    { PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand } and { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
    { PlayerCharacterSideKick } IsIn { EuropaLand } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
    { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
    { PlayerCharacterSideKick } IsNotIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
    .
    . [Other available non-relevant Game States, in the current environment, for the PlayerCharacterSideKick to choose from ]
    .
    { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled }

    // Lemma
    { PlayerCharacterSideKick } IsIn { StyxBoat } <== { StyxBoat } IsNotIn { StyxRiver } // These are connectives, and axiom helpers
    { PlayerCharacterSideKick } IsOn { Vehicle } <== { VehicleDriveDisabled }
    { PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } <== { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle }
    { PlayerCharacterSideKick } IsNotIn { StyxBoat } ==> { StyxBoat } IsNotIn { StyxRiver }

    // Theorem
    Prove { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled }

    // Output (Proof-Steps)
    { PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsIn { StyxRiver }
    { PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver }
    { PlayerCharacterSideKick } IsNotIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver }
    { PlayerCharacterSideKick } IsIn { EuropaLand } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
    { PlayerCharacterSideKick } IsNotIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
    { PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand } and { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
    { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
    { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled }

  REFERENCES
      OpenAI GPT-4-32k-0314 ( { max_tokens:32000, temperature:1.0, top_p:1.0, N:1, 
            stream:false, logprobs:NULL, echo:false, stop:NULL, presence_penalty:0, 
            frequency_penalty:0, best_of:1, logit_bias:NULL } )

  COMPATIBILITY
      Windows 11+ x86i64

*/
#include <iostream>
#include <thread>
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <chrono>
#include <vector>
#include <string>
#include <unordered_map>

enum class MaxCPUCores : int
{
    _1,
    _Max,
    _GPU_Enabled
};

enum class RouteGoal_EnumClass : int
{
    _lhs,
    _rhs,
    _root
};

enum class Indirection_EnumClass : int
{
    _auto,
    _reduce,
    _expand
};

struct AxiomProto_Struct
{
    uint64_t LHSPrimaryKey_UInt64{};
    uint64_t RHSPrimaryKey_UInt64{};

    std::vector<std::string> LHS_StdStrVec;
    std::vector<std::string> RHS_StdStrVec;

    std::unordered_map<RouteGoal_EnumClass,
        std::unordered_map<uint64_t, bool>>
        LHSCallHistory;

    std::unordered_map<RouteGoal_EnumClass,
        std::unordered_map<uint64_t, bool>>
        RHSCallHistory;

    std::unordered_map<RouteGoal_EnumClass,
        std::unordered_map<uint64_t, bool>>
        LHSCallGraph_UInt64Map;

    std::unordered_map<RouteGoal_EnumClass,
        std::unordered_map<uint64_t, bool>>
        RHSCallGraph_UInt64Map;

    bool SubnetFound_LHS(const uint64_t& PKeyFind) const noexcept
    {
        const bool ret = (LHSPrimaryKey_UInt64 % PKeyFind) == 0;
        return ret;
    }

    bool SubnetFound_RHS(const uint64_t& PKeyFind) const noexcept
    {
        const bool ret = (RHSPrimaryKey_UInt64 % PKeyFind) == 0;
        return ret;
    }

    bool bParseStrict_Flag{};

    uint64_t guid{};
};

struct Theorem_Struct : public AxiomProto_Struct
{
    uint64_t TotalProofsFound_UInt64{};
    uint64_t MaxAllowedProofs_UInt64 = 1;

    bool ProofFound_Flag{};

    std::vector<uint64_t> ProofStack_VecUInt64;

    std::vector<std::string> ProofString_StdStrVec;

    Indirection_EnumClass Indir_EnumClass = Indirection_EnumClass::_auto;

    bool UpdatePrimaryKey_LHS(const uint64_t& PKeyFind, const uint64_t& PKeyReplace) noexcept
    {
        bool ReturnStatus_Flag{};
        LHSPrimaryKey_UInt64 = LHSPrimaryKey_UInt64 / PKeyFind * PKeyReplace;
        ReturnStatus_Flag = true;
        return ReturnStatus_Flag;
    }

    bool UpdatePrimaryKey_RHS(const uint64_t& PKeyFind, const uint64_t& PKeyReplace) noexcept
    {
        bool ReturnStatus_Flag{};
        RHSPrimaryKey_UInt64 = RHSPrimaryKey_UInt64 / PKeyFind * PKeyReplace;
        ReturnStatus_Flag = true;
        return ReturnStatus_Flag;
    }
};

struct Axiom_Struct : public AxiomProto_Struct
{

};

struct NextRound_Struct
{
    bool Round_Flag {};
    bool init_flag {};
    
    uint8_t iCurrentRound_UInt8 = 0;
    uint8_t iNextRound_UInt8 = 1;

    uint8_t NextRound_UInt8(const bool UpdateRound_Flag = true)
    {
        if(UpdateRound_Flag && init_flag)
            std::swap(iCurrentRound_UInt8,iNextRound_UInt8);
            
        init_flag = true;

        return iNextRound_UInt8;
    };

    uint8_t CurrentRound_UInt8()
    {
        return iCurrentRound_UInt8;
    };
};

uint64_t GUID{};

constexpr uint64_t MaxStackSize_UInt64 = std::numeric_limits<unsigned long long>::max();

int main()
{
    const auto start_time_chrono = std::chrono::high_resolution_clock::now();

    /*
    token: [ "1" "2" "4" "+" ]
    prime: [  2   3   5   7 ]
    */

    Theorem_Struct Theorem;

    const uint64_t N = 2; // Update as needed;

    std::array < Axiom_Struct, N > Axioms_Vec;

    std::unordered_map <uint64_t, Axiom_Struct> Axioms_LibraryMap;

    std::vector<std::array<uint64_t, 2>> Axioms_UInt64Vec = // const (global) task list
    {
        {//std::vector<uint64_t> _1p1p1p1e4 =
            5488, // "1 + 1 + 1 + 1"
            5 // "4"
        },//;

        {//std::vector<uint64_t> _1p1e2 =
            28, // "1 + 1"
            3 // "2"
        },//;

        {//std::vector<uint64_t> _2p2e4 =
            63, // "2 + 2"
            5 // "4"
        }
    };

    for (const std::array<uint64_t, 2>& Axiom_i : Axioms_UInt64Vec)
    {
        uint64_t  lhs = Axiom_i[0];
        uint64_t  rhs = Axiom_i[1];

        const uint64_t _lhs = Axiom_i[0];
        const uint64_t _rhs = Axiom_i[1];

        if (_lhs < _rhs)
        {
            lhs = _rhs;
            rhs = _lhs;
        }

        if (GUID < 1)
        {
            Theorem.guid = ++GUID;
            Theorem.LHSPrimaryKey_UInt64 = lhs;
            Theorem.RHSPrimaryKey_UInt64 = rhs;
            continue;
        }

        Axiom_Struct Axiom;
        Axiom.guid = ++GUID;
        Axiom.LHSPrimaryKey_UInt64 = lhs;
        Axiom.RHSPrimaryKey_UInt64 = rhs;

        /*
        Authorize qualifying axiom subnets by adding
        their netlists to the outbound route map
        */

        if (Theorem.SubnetFound_LHS(lhs))
        {
            Theorem.LHSCallGraph_UInt64Map[RouteGoal_EnumClass::_rhs][Axiom.guid] = true;
        }

        if (Theorem.SubnetFound_LHS(rhs))
        {
            Theorem.LHSCallGraph_UInt64Map[RouteGoal_EnumClass::_lhs][Axiom.guid] = true;
        }

        if (Theorem.SubnetFound_RHS(lhs))
        {
            Theorem.RHSCallGraph_UInt64Map[RouteGoal_EnumClass::_rhs][Axiom.guid] = true;
        }

        if (Theorem.SubnetFound_RHS(rhs))
        {
            Theorem.RHSCallGraph_UInt64Map[RouteGoal_EnumClass::_lhs][Axiom.guid] = true;
        }

        Axioms_Vec[Axiom.guid - 2] = Axiom;
    }

    // Todo: Create a filel-handle hash of the proof-statement to be used when and if a proofstep solution posts to a file (stateless)
    // Todo: Fully _reduce / _expand all axioms subnets with updated proofstacks (ie. fast-forwarding) so a matching subnet can be fast-forwarded at runtime.
    for (Axiom_Struct& Axiom_i : Axioms_Vec)
    {
        for (const Axiom_Struct& Axiom_j : Axioms_Vec)
        {
            /*
            Authorize qualifying axiom subnets by adding
            their netlists to the outbound route map
            */
            const bool ijAvoidsACallLoop_Flag = (Axiom_i.guid != Axiom_j.guid); // Avoid CallGraph loops
            if (ijAvoidsACallLoop_Flag)
            {
                if (Axiom_i.SubnetFound_LHS(Axiom_j.LHSPrimaryKey_UInt64))
                {
                    Axiom_i.LHSCallGraph_UInt64Map[RouteGoal_EnumClass::_rhs][Axiom_j.guid] = true;
                }

                if (Axiom_i.SubnetFound_LHS(Axiom_j.RHSPrimaryKey_UInt64))
                {
                    Axiom_i.LHSCallGraph_UInt64Map[RouteGoal_EnumClass::_lhs][Axiom_j.guid] = true;
                }

                if (Axiom_i.SubnetFound_RHS(Axiom_j.LHSPrimaryKey_UInt64))
                {
                    Axiom_i.RHSCallGraph_UInt64Map[RouteGoal_EnumClass::_rhs][Axiom_j.guid] = true;
                }

                if (Axiom_i.SubnetFound_RHS(Axiom_j.RHSPrimaryKey_UInt64))
                {
                    Axiom_i.RHSCallGraph_UInt64Map[RouteGoal_EnumClass::_lhs][Axiom_j.guid] = true;
                }
            }
        }
        //std::cout << Axiom_i.guid << std::endl;
    }

    for (const Axiom_Struct Axiom_i : Axioms_Vec)
    {
        Axioms_LibraryMap[Axiom_i.guid] = std::move(Axiom_i);
    }
    
    //std::cout << Axioms_LibraryMap[2].guid << std::endl;
    //std::cout << Axioms_LibraryMap[3].guid << std::endl;

    NextRound_Struct Round;
    
    bool QED {};

    std::array < std::vector < Theorem_Struct >, 2 > Tasks_Thread = {};
    
    Tasks_Thread[0].push_back(Theorem);
    
    //Theorem.Indir_EnumClass = Indirection_EnumClass::_auto; // _reduce; // _expand;
    
    auto _0000 = Theorem.LHSCallGraph_UInt64Map.find(RouteGoal_EnumClass::_lhs);
    if(_0000 != Theorem.LHSCallGraph_UInt64Map.end())
    {
        std::cout << "Theorem.LHSCallGraph_UInt64Map[RouteGoal_EnumClass::_lhs] = {";
        bool NextCall_Flag {};
        for (const auto& [idx_UInt64, boolType] : _0000->second)
        {   if (NextCall_Flag)
            {
                std::cout << ", ";
                NextCall_Flag = false;
            }
            std::cout << idx_UInt64 << "";
            NextCall_Flag = true;
        }
        std::cout << "}" << std::endl;
    }
    _0000 = Theorem.LHSCallGraph_UInt64Map.find(RouteGoal_EnumClass::_rhs);
    if(_0000 != Theorem.LHSCallGraph_UInt64Map.end())
    {
        std::cout << "Theorem.LHSCallGraph_UInt64Map[RouteGoal_EnumClass::_rhs] = {";
        bool NextCall_Flag {};
        for (const auto& [idx_UInt64, boolType] : _0000->second)
        {   if (NextCall_Flag)
            {
                std::cout << ", ";
                NextCall_Flag = false;
            }
            std::cout << idx_UInt64 << "";
            NextCall_Flag = true;
        }
        std::cout << "}" << std::endl;
    }
    _0000 = Theorem.RHSCallGraph_UInt64Map.find(RouteGoal_EnumClass::_lhs);
    if(_0000 != Theorem.RHSCallGraph_UInt64Map.end())
    {
        std::cout << "Theorem.RHSCallGraph_UInt64Map[RouteGoal_EnumClass::_lhs] = {";
        bool NextCall_Flag {};
        for (const auto& [idx_UInt64, boolType] : _0000->second)
        {   if (NextCall_Flag)
            {
                std::cout << ", ";
                NextCall_Flag = false;
            }
            std::cout << idx_UInt64 << "";
            NextCall_Flag = true;
        }
        std::cout << "}" << std::endl;
    }
    _0000 = Theorem.RHSCallGraph_UInt64Map.find(RouteGoal_EnumClass::_rhs);
    if(_0000 != Theorem.RHSCallGraph_UInt64Map.end())
    {
        std::cout << "Theorem.RHSCallGraph_UInt64Map[RouteGoal_EnumClass::_rhs] = {";
        bool NextCall_Flag {};
        for (const auto& [idx_UInt64, boolType] : _0000->second)
        {   if (NextCall_Flag)
            {
                std::cout << ", ";
                NextCall_Flag = false;
            }
            std::cout << idx_UInt64 << "";
            NextCall_Flag = true;
        }
        std::cout << "}" << std::endl;
    }

    //std::cout << Tasks_Thread[0][0].guid << std::endl;

    constexpr uint64_t MaxAllowedProofs_UInt64{ 1 };
    uint64_t TotalProofsFound_UInt64{};
    
    //  Lambda function to update primary key (theorem subnet)
    /**
    auto update_primary_key = 
    [&](auto&& update_function)
    */
    auto update_primary_key = 
    [](
        const Axiom_Struct& Axiom, 
        const RouteGoal_EnumClass& InRoute_EnumClass,
        auto&& update_function)
    {
        switch (InRoute_EnumClass)
        {
            case RouteGoal_EnumClass::_lhs:
                update_function(
                    Axiom.RHSPrimaryKey_UInt64,
                    Axiom.LHSPrimaryKey_UInt64);
                break;
    
            case RouteGoal_EnumClass::_rhs:
                update_function(
                    Axiom.LHSPrimaryKey_UInt64,
                    Axiom.RHSPrimaryKey_UInt64);
                break;
    
            default:
                break;
        }
    };

    // Lambda function to reduce code duplication
    /** processTheorem =
    [
    &Tasks_Thread
    ] (
    const Theorem_Struct& InTheorem,
    const RouteGoal_EnumClass& InRoute_EnumClass,
    const std::unordered_map <uint64_t, Axiom_Struct>& InAxioms_LibraryMap,
    const bool& callFlag,
    const bool& isLHS,
    const uint8_t& Write) -> void
    */
    auto processTheorem =
    [
        &Tasks_Thread,
        &update_primary_key
    ] (
        const Theorem_Struct& InTheorem,
        const std::unordered_map <uint64_t, Axiom_Struct>& InAxioms_LibraryMap,
        const RouteGoal_EnumClass& InRoute_EnumClass,
        const bool& callFlag,
        const bool& isLHS,
        const uint8_t& Write) -> void
    {
        if (!callFlag)
        {
            // Call requirements not met //
            return;
        }
        //std::cout << "processTheorem" << std::endl;
        
        const auto& callGraph = isLHS ? InTheorem.LHSCallGraph_UInt64Map : InTheorem.RHSCallGraph_UInt64Map;
        
        // Route entry found?
        const auto& it = callGraph.find(InRoute_EnumClass);
        if (it == callGraph.end())
        {
            // No entry found //
            std::cout << "processTheorem: Aborting task... callGraph.find(InRoute_EnumClass) enumeration key not found." << std::endl;
            return;
        }
        
        std::cout << "processTheorem: callGraph.find(InRoute_EnumClass) enumeration key" << std::endl;
        
        const auto& inner_map = it->second;
        for (const auto& [idx_UInt64, boolType] : inner_map)
        //for (const auto& [idx_UInt64, boolType] : InAxioms_LibraryMap)
        {
            // Cache Call history
            const auto& callHistory_0000 = isLHS ? InTheorem.LHSCallHistory : InTheorem.RHSCallHistory;

            //std::cout << std::boolalpha << isLHS << std::endl;
            //std::cout << "{" << idx_UInt64 << ", " << std::boolalpha << boolType << "}" << std::endl;
            /*
            // Is this a valid enumeration key ?
            const auto& it2 = callHistory_0000.find(InRoute_EnumClass);
            if (it2 != callHistory_0000.end())
            {
                // There's no enumeration key entry //
                std::cout << "processTheorem: Exiting callHistory_0000[]... No enumeration key found." << std::endl;
                continue;
            }
            */
            std::cout << "processTheorem: Entering callHistory_0000[]... " << std::endl;
            std::cout << "processTheorem: Creating task to add to the queue..." << std::endl;

            // Clone the struct to make our changes
            Theorem_Struct th = InTheorem;

            const auto& it3 = InAxioms_LibraryMap.find(idx_UInt64);
            if (it3 == InAxioms_LibraryMap.end())
            {
                // Axiom not found //
                std::cout << "processTheorem: Exiting... Axiom not found." << std::endl;
                continue;
            }
            
            // Cache the current axiom 
            const Axiom_Struct& Axiom = it3->second;

            // Make our changes
            if (isLHS)
            {
                update_primary_key(
                    Axiom, 
                    InRoute_EnumClass, 
                    [&th](const auto& rhs, const auto& lhs)
                    { 
                        th.UpdatePrimaryKey_LHS(rhs, lhs); 
                        std::cout << "processTheorem >> update_primary_key[LHS]: theorem {" << th.LHSPrimaryKey_UInt64 << ", " << th.RHSPrimaryKey_UInt64 << "}" << std::endl;
                    });
            } else {
                update_primary_key(
                    Axiom,
                    InRoute_EnumClass,
                    [&th](const auto& lhs, const auto& rhs)
                    { 
                        th.UpdatePrimaryKey_RHS(lhs, rhs); 
                        std::cout << "processTheorem >> update_primary_key[RHS]: theorem {" << th.LHSPrimaryKey_UInt64 << ", " << th.RHSPrimaryKey_UInt64 << "}" << std::endl;
                    });
            }
            
            std::cout << "processTheorem: theorem {" << th.LHSPrimaryKey_UInt64 << ", " << th.RHSPrimaryKey_UInt64 << "}" << std::endl;

            // Update proof step
            th.ProofStack_VecUInt64.push_back(Axiom.guid);
            
            // Cache call history
            auto& callHistory = isLHS ? th.LHSCallHistory : th.RHSCallHistory;

            // Update call history
            callHistory[InRoute_EnumClass][idx_UInt64] = true;

            // Write the task to the tasks thread
            Tasks_Thread[Write].emplace_back(std::move(th));
            
            std::cout << "processTheorem: Tasks_Thread[Write].size(): " << Tasks_Thread[Write].size() << std::endl;
        } // end loop (const auto& [idx_UInt64, boolType] : inner_map)
    }; // end lambda: processTheorem

    uint8_t Read = Round.CurrentRound_UInt8();
    uint8_t Write = Round.NextRound_UInt8();

    while(Tasks_Thread[Read].size())
    {
        for (const Theorem_Struct& InTheorem : Tasks_Thread[Read])
        {
            std::cout << "Main >> Tasks_Thread[Read]: theorem {" << InTheorem.LHSPrimaryKey_UInt64 << ", " << InTheorem.RHSPrimaryKey_UInt64 << "}" << std::endl;
            
            if (InTheorem.LHSPrimaryKey_UInt64 == InTheorem.RHSPrimaryKey_UInt64)
            {
                for (const uint64_t& nID_UInt64 : InTheorem.ProofStack_VecUInt64)
                {
                    std::cout << "Axiom_" << nID_UInt64 << std::endl;
                }
    
                std::cout << "\n";
                std::cout << "Q.E.D." << std::endl;
                std::cout << "\n";
    
              ++TotalProofsFound_UInt64;
    
                if (TotalProofsFound_UInt64 >= MaxAllowedProofs_UInt64)
                {
                    QED = true;
                    break;
                }
            }
    
            const bool CallLHSRouteLHS_Flag =
                InTheorem.LHSCallGraph_UInt64Map.find(RouteGoal_EnumClass::_lhs) !=
                InTheorem.LHSCallGraph_UInt64Map.end();
    
            const bool CallLHSRouteRHS_Flag =
                InTheorem.LHSCallGraph_UInt64Map.find(RouteGoal_EnumClass::_rhs) !=
                InTheorem.LHSCallGraph_UInt64Map.end();
    
            const bool CallRHSRouteLHS_Flag =
                InTheorem.RHSCallGraph_UInt64Map.find(RouteGoal_EnumClass::_lhs) !=
                InTheorem.RHSCallGraph_UInt64Map.end();
    
            const bool CallRHSRouteRHS_Flag =
                InTheorem.RHSCallGraph_UInt64Map.find(RouteGoal_EnumClass::_rhs) !=
                InTheorem.RHSCallGraph_UInt64Map.end();
            
            std::cout << "CallLHSRouteLHS_Flag: " << std::boolalpha << CallLHSRouteLHS_Flag << std::endl;
            std::cout << "CallLHSRouteRHS_Flag: " << std::boolalpha << CallLHSRouteRHS_Flag << std::endl;
            std::cout << "CallRHSRouteLHS_Flag: " << std::boolalpha << CallRHSRouteLHS_Flag << std::endl;
            std::cout << "CallRHSRouteRHS_Flag: " << std::boolalpha << CallRHSRouteRHS_Flag << std::endl;
           
            constexpr bool isLHS_Flag = true;
    
            switch (InTheorem.Indir_EnumClass)
            {
            case Indirection_EnumClass::_reduce:
                processTheorem(InTheorem, Axioms_LibraryMap, RouteGoal_EnumClass::_rhs, CallLHSRouteRHS_Flag,  isLHS_Flag, Write);
                processTheorem(InTheorem, Axioms_LibraryMap, RouteGoal_EnumClass::_rhs, CallRHSRouteRHS_Flag, !isLHS_Flag, Write);
                break;
    
            case Indirection_EnumClass::_expand:
                processTheorem(InTheorem, Axioms_LibraryMap, RouteGoal_EnumClass::_lhs, CallLHSRouteLHS_Flag,  isLHS_Flag, Write);
                processTheorem(InTheorem, Axioms_LibraryMap, RouteGoal_EnumClass::_lhs, CallRHSRouteLHS_Flag, !isLHS_Flag, Write);
                break;
    
            case Indirection_EnumClass::_auto:
            
            default:
                // _reduce
                processTheorem(InTheorem, Axioms_LibraryMap, RouteGoal_EnumClass::_rhs, CallLHSRouteRHS_Flag,  isLHS_Flag, Write);
                processTheorem(InTheorem, Axioms_LibraryMap, RouteGoal_EnumClass::_rhs, CallRHSRouteRHS_Flag, !isLHS_Flag, Write);
                
                // _expand
                processTheorem(InTheorem, Axioms_LibraryMap, RouteGoal_EnumClass::_lhs, CallLHSRouteLHS_Flag,  isLHS_Flag, Write);
                processTheorem(InTheorem, Axioms_LibraryMap, RouteGoal_EnumClass::_lhs, CallRHSRouteLHS_Flag, !isLHS_Flag, Write);
                break;
            }
        } // end loop (const Theorem_Struct& InTheorem : Tasks_Thread[Read])
        
        if (QED)
        {
            break;
        }
        
        std::cout << "Tasks_Thread[Read].size(): " << Tasks_Thread[Read].size() << std::endl;
        std::cout << "Emptying Tasks_Thread[Read] buffer..." << std::endl;
        
        Tasks_Thread[Read] = std::vector<Theorem_Struct>{};
        
        std::cout << "Tasks_Thread[Read].size(): " << Tasks_Thread[Read].size() << std::endl;
        std::cout << "Tasks_Thread[Write].size(): " << Tasks_Thread[Write].size() << std::endl;
        std::cout << "Read Index: " << static_cast<unsigned>(Read) << std::endl;
        std::cout << "Write Index: " << static_cast<unsigned>(Write) << std::endl;
        std::cout << "Updating Read Index..." << std::endl;
        
        Write = Round.NextRound_UInt8();
        Read = Round.CurrentRound_UInt8();
        
        std::cout << "Read Index: " << static_cast<unsigned>(Read) << std::endl;
        std::cout << "Write Index: " << static_cast<unsigned>(Write) << std::endl;
        std::cout << "Tasks_Thread[Read].size(): " << Tasks_Thread[Read].size() << std::endl;
        std::cout << "Tasks_Thread[Write].size(): " << Tasks_Thread[Write].size() << std::endl;
    
    } // end while (Tasks_Thread[Read].size())

    const auto end_time_chrono = std::chrono::high_resolution_clock::now();
    const auto duration_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_chrono - start_time_chrono).count();
    std::cout << "Total Duration (nanoseconds): " << duration_chrono << std::endl;

    return EXIT_SUCCESS;
}
