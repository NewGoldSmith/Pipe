//Copyright (c) 2020 2021, Gold Smith
//Released under the MIT license
//https�@://opensource.org/licenses/mit-license.php

#pragma once
#include <cstddef>
#include <corecrt_memcpy_s.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <CBinaryStringCore.h>


//Binary�f�[�^���i�[�A�g���~���O�A�A���A�����Aascii�e�L�X�g�Ƒ��ݕϊ�
//����N���X�B������ƈႢ�r����'\0'���L���Ă������ŏI�[�Ƃ��Ȃ��B
//���ׁ̈A�i�[����ۂ̓f�[�^�T�C�Y���w�肷��K�v������B
// �e�k���^�[�~�l�[�e�B�b�g��������i�[�A���o�����\�B
// ���̏ꍇ�A�T�C�Y�̎w��͕s�v�B
//obj2.Detach(obj1)�ŁA�Q�Ƃ̃`�F�[������O���Ċi�[�\�B
//obj2.Detach(obj2)�ŁA��ŎQ�ƃ`�F�[������؂藣�����Ƃ��ł���B
//GetBuffer�Ńo�b�t�@�[�ɒ��ڏ������ނ��Ƃ��o���邪�A�����z����Q�Ƃ���S�Ă�
// �I�u�W�F�N�g�̃f�[�^���ς��B
//�������A�����������A���P�[�g���Ȃ��̂ō����B
// �Q�Ƃ̓���Ƃ��Ă͓K�؂������ӁB
// �X���b�h�Z�[�t�ł͂Ȃ��̂ŁA�K�v�ɉ����đ΍􂪕K�v�B
//GetBuffer�ł́A�Q�ƃJ�E���g�͑����Ȃ��B
//�f�[�^�̘A���̓o�b�t�@�[�T�C�Y�������Ŋg�������B
//c_str()�ɂ���āANULL�I�[������Ƃ��ďo�͂��邱�Ƃ��o����B
//c_str()�̓r����'\0'���������ꍇ�A�����ŏI���Ƃ���֐�������̂Œ��ӁB
// c_strw()��wchar_t*�ɃL���X�g���邾���Ȃ̂ŁA���ӁB
// �ϊ��֐��͍��̂Ƃ��떢�����BCBWinCov.h�̕ϊ��֐����g���B
// SpaceHexTextA�Ƃ́A"FF 0D OA"�̗l�ȃX�y�[�X�Ő؂藣�����e�L�X�g�`����
// ����B
// �e�L�X�g�L�q�Ńo�C�i���z����i�[���鎞�Ɏg���B
// BinaryDataToSpaceHexTextA�ŁA�o�C�i���z���SpaceHexText�`���ɕς���B
// SpaceHexTextToBinaryDataA�ŁASpaceHexText�`�����A�o�C�i���z��ɕϊ��ł�
// ��B
// �܂�A"FF OD OA"��{0xFF,0x0d,0x0a}�ɕϊ��ł���B
// RemoveSpaceHexText�ŁA�X�y�[�X�������ł���Bascii�����񂵂����M�ł��Ȃ�
// �󋵂Ńo�C�i���z��𑗐M�������ꍇ�A�f�[�^��Z���o����B
// ���̏ꍇ�A��M���ŁAHexTextToBinaryDataA�ŁA�o�C�i���z��𕜌�����B
//TrimPosition�Ńo�C�i���z�񂩂�w�肳�ꂽ�͈͂�؂藣���A�߂�l�Ƃ��ĕԂ��B
// �n�܂�̃|�W�V�����ƏI���̃|�W�V�������w�肷��B
// ���̃o�C�g��͌��Ԃ��l�߂�B
//TrimFirstCode�́A�擪���猟�����f�~���^�ɂ�����ƃf�~���^�܂ł̔z��
// �Ԃ����B�f�~���^�́A�\�[�X�A�Ԃ�l�A�ǂ���ɂ��c��Ȃ��B
//�R�[�h����v���Ȃ��ꍇ�A�󕶎����Ԃ���A�\�[�X�͂��̂܂܂��ێ������B
//FindFirst�n�́A����������̈�v�����C���f�b�N�X��Ԃ��B
// ���������ȗ������ꍇ�͐擪���猟������B
// �������́A�iDataSize-�������̃T�C�Y�j�ȉ��łȂ���΂Ȃ�Ȃ��B
// ������Ȃ������ꍇ-1�B
// �Ⴆ�΁A�擪�Ō��������ꍇ�A�Ԃ�l��0�B
// FindLast�n�́A�I�[����O�������֑���������̈�v�����C���f�b�N�X��Ԃ��B
// �������́A�iDataSize-�������̃T�C�Y�j�ȉ��łȂ���΂Ȃ�Ȃ��B
//BFormat�Ƃ́A"BS 3 "+{0xFF,0x00,0xF3}�̗l�ȕ����O�R�[�h���܂߂邱�Ƃ�
// �o����t�H�[�}�b�g�ł���B
// '\0'���I�[�����łȂ�Binary String��z�肵�Ă���B
// ��������emBFormat::BS�ɂ���ƁA�����R�[�h�O�̃R�[�h���܂܂��{����
// �o�C�i���z��������t�H�[�}�b�g�ɂȂ�B
// ��������emBFormat::HTS�ɂ���ƁA[0-9][A-F]�̊Ԃ̕����R�[�h���g�p�����
// HexText�`�����g����B
// TrimFirstBFormat�͐擪��BFormat�ł��邱�Ƃ�z�肵�Ă���B
// �؂������z��𖳕ϊ��ŕԂ��B���ɑ����c��̔z��͐擪�ɋl�߂���B
// MakeBFormat�ŁABFormat�ɕϊ��ł���B
// UnMakeBFormat�ŁABFormat����ʏ�̔z��ɕϊ�����B�ϊ�������A
// ���̎c��̔z������^�[���l�Ƃ��ĕԂ��B
// empty�󕶎��񂩂ǂ����Ԃ��B
// SetDebugMark�B���I�m�ۂ����f�[�^�z��̈�̍ŏ��̂W�o�C�g�Ƀ}�[�N��
// �������ށB�f�[�^��ǂ����̃f�o�b�N�p�B
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

