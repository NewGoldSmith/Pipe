//Copyright(C) 2021 GoldSmith
//
//This program is free software; you can redistribute itand /or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.If not, see < https://www.gnu.org/licenses/>.

#pragma once
#include <string>
#include <codecvt>
#include <vector>
#include <iconv.h>
#include <tchar.h>
// Installation process.
// Install the English language pack for Visual Studio.
// https://vcpkg.io/en/index.html
// Create an installation directory.
// At the command prompt, "cd installation directory"
// Execute "git clone https://github.com/Microsoft/vcpkg.git"
// Execute "bootstrap-vcpkg.bat".
// Execute "vcpkg --triplet=x64-windows-static install libiconv"
// Execute "vcpkg integrate install" 
// When uninstalling, Execute "vcpkg --triplet = x64-windows-static remove libiconv".
// Execute "vcpkg integrate remove"

namespace GPLStringHelper {

	size_t CommonCovt( void * pStrDst,const size_t DstSize,const void * pStrSrc,const size_t SrcLen,const char* strDstType,const char* strSrcType);
	std::u16string U8toU16LE(std::u8string const& src);
	std::u8string U16LEtoU8(std::u16string const& src);
	std::string U8toCP932(std::u8string const& src);
	std::u8string CP932toU8(std::string const& src);
	std::basic_string<wchar_t> U8toW(const std::u8string &src);
	std::basic_string<char8_t> WtoU8(const std::basic_string<wchar_t>& src);
}