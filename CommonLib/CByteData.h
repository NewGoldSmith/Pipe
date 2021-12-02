#pragma once
#include <cstddef>
#include <corecrt_memcpy_s.h>

//RAW�f�[�^���i�[����N���X�B������ƈႢ�r���ɂO���L���Ă������ŏI�[�Ƃ��Ȃ��B
//���ׁ̈A�i�[����ۂ̓T�C�Y���w�肷��K�v������B
//�f�[�^�̘A���̓o�b�t�@�[�T�C�Y�������Ŋg�������B
//c_str()�ɂ���āANULL�I�[������Ƃ��ďo�͂��邱�Ƃ��o����B
//�Q�ƃJ�E���^�͎g���Ă��Ȃ��̂ŁA�R�s�[�R���X�g���N�^�͂���Ȃ�ɍ��R�X�g�B
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
	explicit operator const char8_t* ()  noexcept;//�Öق̌^�ϊ����s��Ȃ��Bnull�I�[�X�g�����O
	virtual ~CByteData();
	CByteData SetByteData(const char8_t* const pData, int size);
	const char8_t* c_str() const;
	CByteData& SetBufReSize(const int size);
	int GetBufSize()const;
	CByteData& SetDataSize(const int size);
	int GetDataSize()const;
	char8_t* GetBuffer();
};

