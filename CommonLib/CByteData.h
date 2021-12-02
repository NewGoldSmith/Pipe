#pragma once
#include <cstddef>
#include <corecrt_memcpy_s.h>

//RAWデータを格納するクラス。文字列と違い途中に０が有ってもそこで終端としない。
//その為、格納する際はサイズを指定する必要がある。
//データの連結はバッファーサイズが自動で拡張される。
//c_str()によって、NULL終端文字列として出力することも出来る。
//参照カウンタは使っていないので、コピーコンストラクタはそれなりに高コスト。
class CByteData
{
protected:
	char8_t* m_pData;
	int m_DataSize;
	int m_BufSize;
	CByteData();
public:
	CByteData(const CByteData& OtherObj);
	CByteData(int size = 1024);
	CByteData(const char8_t* const pData, int nSize);
	CByteData& operator =(const CByteData& ObjectCByteData);
	CByteData& operator +(const CByteData& ObjectCByteData);
	explicit operator const char8_t* ()  noexcept;//暗黙の型変換を行わない。null終端ストリング
	virtual ~CByteData();
	CByteData SetByteData(const char8_t* const pData, int size);
	const char8_t* c_str() const;
	CByteData& SetBufReSize(const int size);
	int GetBufSize()const;
	CByteData& SetDataSize(const int size);
	int GetDataSize()const;
	char8_t* GetBuffer();
};

