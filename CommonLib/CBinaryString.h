#pragma once
#include <cstddef>
#include <corecrt_memcpy_s.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "CBinaryStringCore.h"
//Binaryデータを格納するクラス。文字列と違い途中に'\0'が有ってもそこで
// 終端としない。
//その為、格納する際はサイズを指定する必要がある。
//obj2.Detach(obj1)で、参照のチェーンから外れる。
//obj2.Detach(obj2)で、後で参照チェーンから切り離すこともできる。
//GetBufferでバッファーに直接書き込むことが出来るが、同じ配列を参照する全ての
// オブジェクトのデータが変わる。
// 参照の動作としては適切だが注意。
//しかし、メモリをリアロケートしないので高速。
//GetBufferでは、参照カウントは増えない。
//データの連結はバッファーサイズが自動で拡張される。
//c_strA()によって、NULL終端文字列として出力することも出来る。
//c_strA()の途中に'\0'があった場合、そこで終わりとする関数もあるので注意。
// c_strw()はwchar_t*にキャストするだけなので、注意。
// 予め、wchar_t型のデータをヌルターミネーティットバイナリーとして
// 格納することをお薦め。
// 変換関数は今のところ未実装。
// SpaceHexTextとは、"FF 0D OA"の様なスペースで切り離したテキスト形式である。
// テキスト記述でバイナリ配列を格納する時に使う。
// BinaryDataToSpaceHexTextAで、バイナリ配列をSpaceHexText形式に変える。
// SpaceHexTextToBinaryDataAで、SpaceHexText形式を、バイナリ配列に変換できる。
// つまり、"FF OD OA"を{0xFF,0x0d,0x0a}に変換できる。
// RemoveSpaceHexTextで、スペースを除去できる。ascii文字列しか送信できない状況でバイナリ配列を送信したい場合、データを短く出来る。
// その場合、送信先で、InsertSpaceHexText、SpaceHexTextToBinaryDataAで、バイナリ配列を復元する。
//TrimPositionでバイト列を切り離し戻り値として返す。
// 始まりのポジションと終わりのポジションを指定する。
// 元のバイト列は隙間を詰める。
//TrimFirstCodeは、先頭から検索しデミリタにあたるとデミリタまでの配列が返される。デミリタは、ソース、返り値、どちらにも残らない。
//コードが一致しない場合、空文字が返され、ソースはそのままが維持される。
//FindFirst系は、第二引数からの一致したインデックスを返す。
// 第二引数を省略した場合は先頭から検索する。
// 第二引数は、（DataSize-第一引数のサイズ）以下でなければならない。
// 見つからなかった場合-1。
// 先頭で見つかった場合、返り値は0。
// FindLast系は、終端から前方方向へ第二引数からの一致したインデックスを返す。
// 第二引数は、（DataSize-第一引数のサイズ）以下でなければならない。
//BFormatとは、"BS 3 "+{0xFF,0x00,0xF3}の様な文字外コードを含めることが出来るフォーマットである。
// '\0'が終端文字でないBinary Stringを想定している。
// 第一引数をemBFormat::BSにすると、文字コード外のコードが含まれる本当のバイナリ配列を扱うフォーマットになる。
// 第一引数をemBFormat::HTSにすると、[0-9][A-F]の間の文字コードが使用されるHexText形式が使われる。
// TrimFirstBFormatは先頭がBFormatであることを想定している。
// 切り取った配列を無変換で返す。後ろに続く残りの配列は先頭に詰められる。
// MakeBFormatで、BFormatに変換できる。
// UnMakeBFormatで、BFormatから通常の配列に変換する。変換した残りの
// 配列をリターン値として返す。
// empty空文字列かどうか返す。
//
class CBinaryStringCore;
class CBinaryString
{
	friend CBinaryString operator +(const CBinaryString obj1,const CBinaryString obj2);
	friend CBinaryString operator +(const char* str1, const CBinaryString str2);
	friend CBinaryString operator +(const std::string str1, const CBinaryString str2);
protected:
	CBinaryStringCore *m_pBDA;
	std::vector<std::string> Split(const std::string str, const std::string separator);
public:
	CBinaryString();
	CBinaryString(const CBinaryString &OtherObj);
	explicit CBinaryString(size_t size);
	CBinaryString(const char DataSZ[]);
	CBinaryString(CBinaryString&& OtherObj)noexcept;
	explicit CBinaryString(const char8_t*  pData, size_t nSize);
	explicit CBinaryString(const std::string str);
	CBinaryString& operator =(CBinaryString&& OtherObj)noexcept;
	CBinaryString& operator =(const CBinaryString & OtherObj);
	CBinaryString& operator =(const char* strSz);
	CBinaryString& operator =(const std::string str);
	CBinaryString& operator ()(CBinaryString&& OtherObj);
	CBinaryString& operator ()(const CBinaryString& OtherObj);
	CBinaryString& operator ()(const char* pDataSZ);
	CBinaryString& operator +=(const CBinaryString& OtherObj);
	CBinaryString& operator +=(const char* str);
	CBinaryString operator +(const char* str2)const;
	bool operator ==(const CBinaryString& OtherObj)const;
	bool operator ==(const char* str)const;
	bool operator !=(const CBinaryString& OtherObj)const;
	bool operator <(const CBinaryString& OtherObj)const;
	explicit operator const char8_t* ()  noexcept;
	explicit operator const char* () noexcept;
	explicit operator std::string();
	virtual ~CBinaryString();
	CBinaryString &SetTextStringA(const char* pDataSZ);
	CBinaryString &SetBinaryString8(const char8_t* pData, size_t size);
	CBinaryString& SetBinaryStringW(const wchar_t* pData, size_t sizebyte);
	const char* c_strA() const noexcept;
	const char8_t* c_str8() const noexcept;
	const wchar_t* c_strw() const noexcept;
	CBinaryString& SetBufReSize(const size_t size);
	size_t GetBufSize()const noexcept;
	CBinaryString& SetDataSize(const size_t size);
	size_t GetDataSize()const noexcept;
	char8_t* GetBuffer8() const noexcept;
	char* GetBufferA() const noexcept;
	CBinaryString& Detach(const CBinaryString & OtherObj);
	void RemoveSpaceHexText();
	void InsertSpaceHexText();
	CBinaryString BinaryDataToSpaceHexTextA();
	CBinaryString SpaceHexTextToBinaryDataA();
	CBinaryString TrimFirstCodeA(char demilita);
	CBinaryString TrimFirstCodesA(const char* strdemilita);
	CBinaryString TrimFirstBFormat();
	CBinaryString TrimPosition(unsigned long begin, unsigned long end);
	int FindFirst(const CBinaryString BS, unsigned int InitPos = 0)const;
	int FindFirstA(const char* strFnd,unsigned int InitPos=0)const;
	int FindLast(const CBinaryString BS, unsigned int InitPos = -1)const;
	int FindLastA(const char* strFnd, unsigned int InitPos = -1)const;
	enum class emBFormat { BS, HTS };
	CBinaryString MakeBFormat(emBFormat bFormat=emBFormat::HTS);
	CBinaryString UnMakeBFormat();
	bool empty()const noexcept;
};

