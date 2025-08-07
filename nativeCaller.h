/*
    THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
                http://dev-c.com
            (C) Alexander Blade 2015-2024
*/

#pragma once

#include "main.h"
#include "types.h" // This already defines `Void`, so we don't need to.
#include <type_traits>

// Helper function to correctly convert any argument type to UINT64 for the native stack
template<typename T>
inline UINT64 LArg(T value)
{
    if constexpr (std::is_pointer_v<T>)
    {
        return reinterpret_cast<UINT64>(value);
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        return *reinterpret_cast<UINT64*>(&value);
    }
    else
    {
        return static_cast<UINT64>(value);
    }
}

// Single invoke function that correctly handles both void and non-void returns
template <typename R, typename... Args>
R invoke(UINT64 hash, Args... args)
{
    nativeInit(hash);
    (nativePush64(LArg(args)), ...);

    // This check now correctly compares against the `Void` type defined in types.h
    if constexpr (std::is_same_v<R, Void>)
    {
        nativeCall();
        return 0; // Since Void is a DWORD, we must return a value. 0 is fine.
    }
    else
    {
        return *reinterpret_cast<R*>(nativeCall());
    }
}