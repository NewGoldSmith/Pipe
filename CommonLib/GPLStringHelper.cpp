#include "GPLStringHelper.h"

//#pragma warning(disable : 4996)


namespace GPLStringHelper {
	size_t CommonCovt(void* pStrDst, const size_t DstSize, const void* pStrSrc, const size_t SrcLen, const char* pstrDstType, const char* pstrSrcType)
	{
		iconv_t conv; // conversion descriptor
		//char8_t  buf_src[BUFSIZ] = {};
		//char8_t buf_dst[BUFSIZ];
		//char8_t* p_src_buf = buf_src;
		//char8_t* p_dst_buf = buf_dst;
		size_t src_len = SrcLen;
		size_t dst_len = DstSize;
//		memcpy_s(buf_src, BUFSIZ, pStrSrc, SrcLen);
		if ((conv = iconv_open(pstrDstType, pstrSrcType)) == (iconv_t)-1)
		{
			_ftprintf_s(stderr, _T("error: %s: %s\n"), _T("__FUNCTION__"), _T("iconv open"));
			return 0;
		}
		if (iconv(conv, (char**)&pStrSrc, &src_len, (char**)&pStrDst, &dst_len) == (size_t)-1)
		{
			_ftprintf_s(stderr, _T("error: %s: %s\n"), _T("__FUNCTION__"), _T("iconv"));
			return 0;
		}
//		memcpy_s(pStrDst, dst_len, buf_dst, BUFSIZ - dst_len);
		if (iconv_close(conv) == -1) {
			_ftprintf_s(stderr, _T("error: %s: %s\n"), _T("__FUNCTION__"), _T("iconv_close"));
			return 0;
		}
		return DstSize - dst_len;
	}

	std::u16string U8toU16LE(std::u8string const& src)
	{
		char16_t u16buf[BUFSIZ] = {};
		size_t i = CommonCovt(u16buf, BUFSIZ, src.c_str(), src.length() * sizeof(char8_t), "UTF-16LE", "UTF-8");
		return u16buf;
	}


	std::u8string U16LEtoU8(std::u16string const& src)
	{
		char8_t u8buf[BUFSIZ] = {};
		size_t i = CommonCovt(u8buf, BUFSIZ, src.c_str(), src.length() * sizeof(wchar_t), "UTF-8", "UTF-16LE");
		return u8buf;
	}

	std::string U8toCP932(std::u8string const& src)
	{
		char dstbuf[BUFSIZ] = {};
		size_t i = CommonCovt(dstbuf, BUFSIZ, src.c_str(), src.length() * sizeof(char8_t), "CP932", "UTF-8");
		return dstbuf;
	}

	std::u8string CP932toU8(std::string const& src)
	{
		char8_t dstbuf[BUFSIZ] = {};
		size_t i = CommonCovt(dstbuf, BUFSIZ, src.c_str(), src.length() * sizeof(char8_t),  "UTF-8","CP932");
		return dstbuf;
	}

	std::basic_string<wchar_t> U8toW(const std::u8string& src)
	{
		wchar_t dstbuf[BUFSIZ] = {};
		size_t i = CommonCovt(dstbuf, BUFSIZ, src.c_str(), src.length() * sizeof(char8_t), "UTF-16LE", "UTF-8");
		return dstbuf;
	}

	std::basic_string<char8_t> WtoU8(const std::basic_string<wchar_t>& src)
	{
		char8_t u8buf[BUFSIZ] = {};
		size_t i = CommonCovt(u8buf, BUFSIZ, src.c_str(), src.length() * sizeof(wchar_t), "UTF-8", "UTF-16LE");
		return u8buf;
	}

}