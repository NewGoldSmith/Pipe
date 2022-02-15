//Copyright (c) 2020 2021, Gold Smith
//Released under the MIT license
//https　://opensource.org/licenses/mit-license.php

#pragma once
#include <cstddef>
#include <corecrt_memcpy_s.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <CBinaryStringCore.h>


//Binaryデータを格納、トリミング、連結、検索、asciiテキストと相互変換
//するクラス。文字列と違い途中に'\0'が有ってもそこで終端としない。
//その為、格納する際はデータサイズを指定する必要がある。
// 各ヌルターミネーティット文字列も格納、取り出しが可能。
// その場合、サイズの指定は不要。
//obj2.Detach(obj1)で、参照のチェーンから外して格納可能。
//obj2.Detach(obj2)で、後で参照チェーンから切り離すこともできる。
//GetBufferでバッファーに直接書き込むことが出来るが、同じ配列を参照する全ての
// オブジェクトのデータが変わる。
//しかし、メモリをリアロケートしないので高速。
// 参照の動作としては適切だが注意。
// スレッドセーフではないので、必要に応じて対策が必要。
//GetBufferでは、参照カウントは増えない。
//データの連結はバッファーサイズが自動で拡張される。
//c_str()によって、NULL終端文字列として出力することも出来る。
//c_str()の途中に'\0'があった場合、そこで終わりとする関数もあるので注意。
// c_strw()はwchar_t*にキャストするだけなので、注意。
// 変換関数は今のところ未実装。CBWinCov.hの変換関数を使う。
// SpaceHexTextAとは、"FF 0D OA"の様なスペースで切り離したテキスト形式で
// ある。
// テキスト記述でバイナリ配列を格納する時に使う。
// BinaryDataToSpaceHexTextAで、バイナリ配列をSpaceHexText形式に変える。
// SpaceHexTextToBinaryDataAで、SpaceHexText形式を、バイナリ配列に変換でき
// る。
// つまり、"FF OD OA"を{0xFF,0x0d,0x0a}に変換できる。
// RemoveSpaceHexTextで、スペースを除去できる。ascii文字列しか送信できない
// 状況でバイナリ配列を送信したい場合、データを短く出来る。
// その場合、受信側で、HexTextToBinaryDataAで、バイナリ配列を復元する。
//TrimPositionでバイナリ配列から指定された範囲を切り離し、戻り値として返す。
// 始まりのポジションと終わりのポジションを指定する。
// 元のバイト列は隙間を詰める。
//TrimFirstCodeは、先頭から検索しデミリタにあたるとデミリタまでの配列が
// 返される。デミリタは、ソース、返り値、どちらにも残らない。
//コードが一致しない場合、空文字が返され、ソースはそのままが維持される。
//FindFirst系は、第二引数からの一致したインデックスを返す。
// 第二引数を省略した場合は先頭から検索する。
// 第二引数は、（DataSize-第一引数のサイズ）以下でなければならない。
// 見つからなかった場合-1。
// 例えば、先頭で見つかった場合、返り値は0。
// FindLast系は、終端から前方方向へ第二引数からの一致したインデックスを返す。
// 第二引数は、（DataSize-第一引数のサイズ）以下でなければならない。
//BFormatとは、"BS 3 "+{0xFF,0x00,0xF3}の様な文字外コードを含めることが
// 出来るフォーマットである。
// '\0'が終端文字でないBinary Stringを想定している。
// 第一引数をemBFormat::BSにすると、文字コード外のコードが含まれる本当の
// バイナリ配列を扱うフォーマットになる。
// 第一引数をemBFormat::HTSにすると、[0-9][A-F]の間の文字コードが使用される
// HexText形式が使われる。
// TrimFirstBFormatは先頭がBFormatであることを想定している。
// 切り取った配列を無変換で返す。後ろに続く残りの配列は先頭に詰められる。
// MakeBFormatで、BFormatに変換できる。
// UnMakeBFormatで、BFormatから通常の配列に変換する。変換した後、
// 後ろの残りの配列をリターン値として返す。
// empty空文字列かどうか返す。
// SetDebugMark。動的確保したデータ配列領域の最初の８バイトにマークを
// 書き込む。データを追う時のデバック用。
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
	std::vector<std::wstring> SplitW(const std::wstring wstr, const std::wstring wseparator);
	const static int DefAlloc = 1024 - 10 ;
public:
	CBinaryString();
	CBinaryString(const CBinaryString &OtherObj);
	explicit CBinaryString(size_t size);
	CBinaryString(const char DataSZ[]);
	CBinaryString(const wchar_t DataSZ[]);
	explicit CBinaryString(const char8_t strSZ[]);
	explicit CBinaryString(const std::string str);
	explicit CBinaryString(const std::wstring wstr);
	CBinaryString& operator =(const CBinaryString & OtherObj);
	CBinaryString& operator =(const char* strSz);
	CBinaryString& operator =(const std::string str);
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
	explicit operator const wchar_t* () noexcept;
	explicit operator std::string();
	virtual ~CBinaryString();
	CBinaryString &SetTextStringA(const char* pDataSZ);
	CBinaryString& SetTextStringW(const wchar_t* pDataSZ);
	CBinaryString &SetBinaryString8(const char8_t* pData, size_t size);
	CBinaryString& SetBinaryStringW(const wchar_t* pData, size_t wsize);
	CBinaryString& Clear();
	const char* c_strA() const noexcept;
	const char8_t* c_str8() const noexcept;
	const wchar_t* c_strw() const noexcept;
	CBinaryString& SetBufReSize(const size_t size= DefAlloc);
	CBinaryString& SetBufReSizeW(const size_t wsize= DefAlloc);
	CBinaryString& Discard();
	unsigned int GetBufSize()const noexcept;
	unsigned int GetBufSizeW()const noexcept;
	CBinaryString& SetDataSize(const unsigned int size);
	CBinaryString& SetDataSizeW(const unsigned int wsize);
	unsigned int GetDataSize()const noexcept;
	unsigned int GetDataSizeW()const noexcept;
	char8_t* GetBuffer8()const noexcept;
	wchar_t* GetBufferW()const noexcept;
	char* GetBufferA() const noexcept;
	CBinaryString& Detach(const CBinaryString & OtherObj);
	CBinaryString& Detach();
	void RemoveSpaceHexTextA();
	void InsertSpaceHexTextA();
	CBinaryString BinaryDataToSpaceHexTextA();
	CBinaryString BinaryDataToSpaceHexTextW();
	CBinaryString SpaceHexTextToBinaryDataA();
	CBinaryString SpaceHexTextToBinaryDataW();
	CBinaryString HexTextToBinaryDataA();
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
	void SetDebugMark(const char* pstr,int size);
};

