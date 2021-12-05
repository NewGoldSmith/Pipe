#pragma once
#include <cstddef>
#include <corecrt_memcpy_s.h>
#include <stdlib.h>
#include "CByteDataAtom.h"

//RAW�f�[�^���i�[����N���X�B������ƈႢ�r���ɂO���L���Ă������ŏI�[�Ƃ��Ȃ��B
//���ׁ̈A�i�[����ۂ̓T�C�Y���w�肷��K�v������B
//�f�[�^�̘A���̓o�b�t�@�[�T�C�Y�������Ŋg�������B
//c_str()�ɂ���āANULL�I�[������Ƃ��ďo�͂��邱�Ƃ��o����B
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
	explicit operator const char8_t* ()  noexcept;//�Öق̌^�ϊ����s��Ȃ��Bnull�I�[�X�g�����O
	virtual ~CByteData();
	CByteData SetByteData(const char8_t* const pData, size_t size);
	const char8_t* c_str() const;
	CByteData& SetBufReSize(const size_t size);
	size_t GetBufSize()const;
	CByteData& SetDataSize(const size_t size);
	size_t GetDataSize()const;
	char8_t* GetBuffer();
};

