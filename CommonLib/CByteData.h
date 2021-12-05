#pragma once
#include <cstddef>
#include <corecrt_memcpy_s.h>
#include <stdlib.h>
#include "CByteDataAtom.h"

//RAWデータを格納するクラス。文字列と違い途中に０が有ってもそこで終端としない。
//その為、格納する際はサイズを指定する必要がある。
//データの連結はバッファーサイズが自動で拡張される。
//c_str()によって、NULL終端文字列として出力することも出来る。
class CByteData
{
protected:
	CByteDataAtom *m_pBDA;
public:
	CByteData(const CByteData& OtherObj);
	CByteData(size_t size = 1024);
	CByteData(const char8_t* const pData, size_t nSize);
	CByteData& operator =(const CByteData& ObjectCByteData);
	CByteData operator +( CByteData ObjectCByteData) const;
	explicit operator const char8_t* ()  noexcept;//暗黙の型変換を行わない。null終端ストリング
	virtual ~CByteData();
	CByteData SetByteData(const char8_t* const pData, size_t size);
	const char8_t* c_str() const;
	CByteData& SetBufReSize(const size_t size);
	size_t GetBufSize()const;
	CByteData& SetDataSize(const size_t size);
	size_t GetDataSize()const;
	char8_t* GetBuffer();
};

