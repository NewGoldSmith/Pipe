#pragma once
#include <cstddef>
#include <corecrt_memcpy_s.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "CBinaryStringCore.h"
//Binary�f�[�^���i�[����N���X�B������ƈႢ�r����'\0'���L���Ă�������
// �I�[�Ƃ��Ȃ��B
//���ׁ̈A�i�[����ۂ̓T�C�Y���w�肷��K�v������B
//obj2.Detach(obj1)�ŁA�Q�Ƃ̃`�F�[������O���B
//obj2.Detach(obj2)�ŁA��ŎQ�ƃ`�F�[������؂藣�����Ƃ��ł���B
//GetBuffer�Ńo�b�t�@�[�ɒ��ڏ������ނ��Ƃ��o���邪�A�����z����Q�Ƃ���S�Ă�
// �I�u�W�F�N�g�̃f�[�^���ς��B
// �Q�Ƃ̓���Ƃ��Ă͓K�؂������ӁB
//�������A�����������A���P�[�g���Ȃ��̂ō����B
//GetBuffer�ł́A�Q�ƃJ�E���g�͑����Ȃ��B
//�f�[�^�̘A���̓o�b�t�@�[�T�C�Y�������Ŋg�������B
//c_strA()�ɂ���āANULL�I�[������Ƃ��ďo�͂��邱�Ƃ��o����B
//c_strA()�̓r����'\0'���������ꍇ�A�����ŏI���Ƃ���֐�������̂Œ��ӁB
// c_strw()��wchar_t*�ɃL���X�g���邾���Ȃ̂ŁA���ӁB
// �\�߁Awchar_t�^�̃f�[�^���k���^�[�~�l�[�e�B�b�g�o�C�i���[�Ƃ���
// �i�[���邱�Ƃ����E�߁B
// �ϊ��֐��͍��̂Ƃ��떢�����B
// SpaceHexText�Ƃ́A"FF 0D OA"�̗l�ȃX�y�[�X�Ő؂藣�����e�L�X�g�`���ł���B
// �e�L�X�g�L�q�Ńo�C�i���z����i�[���鎞�Ɏg���B
// BinaryDataToSpaceHexTextA�ŁA�o�C�i���z���SpaceHexText�`���ɕς���B
// SpaceHexTextToBinaryDataA�ŁASpaceHexText�`�����A�o�C�i���z��ɕϊ��ł���B
// �܂�A"FF OD OA"��{0xFF,0x0d,0x0a}�ɕϊ��ł���B
// RemoveSpaceHexText�ŁA�X�y�[�X�������ł���Bascii�����񂵂����M�ł��Ȃ��󋵂Ńo�C�i���z��𑗐M�������ꍇ�A�f�[�^��Z���o����B
// ���̏ꍇ�A���M��ŁAInsertSpaceHexText�ASpaceHexTextToBinaryDataA�ŁA�o�C�i���z��𕜌�����B
//TrimPosition�Ńo�C�g���؂藣���߂�l�Ƃ��ĕԂ��B
// �n�܂�̃|�W�V�����ƏI���̃|�W�V�������w�肷��B
// ���̃o�C�g��͌��Ԃ��l�߂�B
//TrimFirstCode�́A�擪���猟�����f�~���^�ɂ�����ƃf�~���^�܂ł̔z�񂪕Ԃ����B�f�~���^�́A�\�[�X�A�Ԃ�l�A�ǂ���ɂ��c��Ȃ��B
//�R�[�h����v���Ȃ��ꍇ�A�󕶎����Ԃ���A�\�[�X�͂��̂܂܂��ێ������B
//FindFirst�n�́A����������̈�v�����C���f�b�N�X��Ԃ��B
// ���������ȗ������ꍇ�͐擪���猟������B
// �������́A�iDataSize-�������̃T�C�Y�j�ȉ��łȂ���΂Ȃ�Ȃ��B
// ������Ȃ������ꍇ-1�B
// �擪�Ō��������ꍇ�A�Ԃ�l��0�B
// FindLast�n�́A�I�[����O�������֑���������̈�v�����C���f�b�N�X��Ԃ��B
// �������́A�iDataSize-�������̃T�C�Y�j�ȉ��łȂ���΂Ȃ�Ȃ��B
//BFormat�Ƃ́A"BS 3 "+{0xFF,0x00,0xF3}�̗l�ȕ����O�R�[�h���܂߂邱�Ƃ��o����t�H�[�}�b�g�ł���B
// '\0'���I�[�����łȂ�Binary String��z�肵�Ă���B
// ��������emBFormat::BS�ɂ���ƁA�����R�[�h�O�̃R�[�h���܂܂��{���̃o�C�i���z��������t�H�[�}�b�g�ɂȂ�B
// ��������emBFormat::HTS�ɂ���ƁA[0-9][A-F]�̊Ԃ̕����R�[�h���g�p�����HexText�`�����g����B
// TrimFirstBFormat�͐擪��BFormat�ł��邱�Ƃ�z�肵�Ă���B
// �؂������z��𖳕ϊ��ŕԂ��B���ɑ����c��̔z��͐擪�ɋl�߂���B
// MakeBFormat�ŁABFormat�ɕϊ��ł���B
// UnMakeBFormat�ŁABFormat����ʏ�̔z��ɕϊ�����B�ϊ������c���
// �z������^�[���l�Ƃ��ĕԂ��B
// empty�󕶎��񂩂ǂ����Ԃ��B
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

