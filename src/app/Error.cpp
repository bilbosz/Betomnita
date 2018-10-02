#include "app/Error.hpp"

#include "app/Application.hpp"

#include <chrono>

namespace App
{
    static const std::chrono::high_resolution_clock::time_point s_start = std::chrono::high_resolution_clock::now();

    void Error::Breakpoint()
    {
#if defined _WIN32
        __debugbreak();
#elif defined __linux__
        __builtin_trap();
#else
        ::abort();
#endif
    }

    void Error::Message( const std::wstring& type, const std::string& file, uint32_t line, const std::wstring& message, bool abort )
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto diff = std::chrono::duration_cast< std::chrono::microseconds >( now - s_start );

        std::wostringstream out;
        out << L"[" << std::setw( 10 ) << std::setprecision( 4 ) << std::fixed << diff.count() / 1'000'000.0 << L"] <" << type << L"> " << file.c_str() << L":" << line
            << L": " << message;
        std::wcerr << out.str() << std::endl;
#ifdef _DEBUG
        if( abort )
        {
            Breakpoint();
        }
#endif
    }
}