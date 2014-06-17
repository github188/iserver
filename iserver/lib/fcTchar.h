#ifndef GUARD_FCTCHAR_H
#define GUARD_FCTCHAR_H

#pragma once

#if (defined _UNICODE) || (defined UNICODE)
#define _tstring wstring
#define _tstringstream wstringstream
#define _tcin wcin
#else
#define _tstring string
#define _tstringstream stringstream
#define _tcin cin
#endif

// U--UTF8 T--TCHAR A--Ansi W--wchar--Unicode16
#include <atlbase.h>
#define fcA2U(ASTR) (LPSTR)ATL::CW2A((ATL::CA2W(ASTR)), CP_UTF8)
#define fcW2U(WSTR) (LPSTR)ATL::CW2A((WSTR), CP_UTF8)
#define fcU2A(USTR) (LPSTR)ATL::CW2A(ATL::CA2W((USTR), CP_UTF8))
#define fcU2W(USTR) (LPWSTR)ATL::CA2W((USTR), CP_UTF8)
#define fcA2W(ASTR) (LPWSTR)ATL::CA2W(ASTR)
#define fcW2A(WSTR) (LPSTR)ATL::CW2A(WSTR)
#define fcA2A(ASTR) (LPSTR)(ASTR)
#define fcW2W(WSTR) (LPWSTR)(WSTR)
#if (defined _UNICODE) || (defined UNICODE)
#define fcU2T fcU2W
#define fcT2U fcW2U
#define fcA2T fcA2W
#define fcT2A fcW2A
#define fcW2T fcW2W
#define fcT2W fcW2W
#else
#define fcU2T fcU2A
#define fcT2U fcA2U
#define fcA2T fcA2A
#define fcT2A fcA2A
#define fcW2T fcW2A
#define fcT2W fcA2W
#endif

#endif // end of GUARD_FCTCHAR_H
