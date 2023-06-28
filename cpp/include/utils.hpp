#ifndef ILRD_HRD28_UTILS_HPP
#define ILRD_HRD28_UTILS_HPP
namespace ilrd
{

#define UNUSED(x) (void)(x)

#if __cplusplus < 201103L
#define MODERN(STATEMENT)
#define NULL_PTR NULL
#define NOEXCEPT throw()
#define OVERRIDE
#else
#define MODERN(STATEMENT) STATEMENT
#define NULL_PTR nullptr
#define NOEXCEPT noexcept
#define OVERRIDE override
#endif

#ifdef NDEBUG
#define DEBUG_ONLY(x) (x)
#else
#define DEBUG_ONLY(x)
#endif

}//namespace ilrd
#endif  //ILRD_HRD28_UTILS_HPP