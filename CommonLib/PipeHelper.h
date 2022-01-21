#pragma once
#include <stdio.h>
#include <io.h>
#include <CBinaryString.h>
#include <fcntl.h>
#include <wtypes.h>
//HRead�ǂݎ�����o�C�g����Ԃ��B
//���̊֐��̓t�@�C���̏I���œǂݎ�������ƁA0 ��Ԃ��܂��B 
// Fd���L���łȂ��ꍇ�A�t�@�C�����ǂݎ��p�ɊJ����Ă��Ȃ��ꍇ�A
// �܂��̓t�@�C�������b�N����Ă���ꍇ�́A�u�p�����[�^�[�̌��؁v
// �Ő�������Ă���悤�ɁA�����ȃp�����[�^�[�n���h���[���Ăяo����܂��B
//  ���s�̌p���������ꂽ�ꍇ�A���̊֐���-1 ��Ԃ��A errno ��
//  EBADF�ɐݒ肵�܂��B
//HWrite���������ꍇ �A_write �������܂ꂽ�o�C�g�����Ԃ���܂��B 
// �f�B�X�N�Ɏc���Ă�����ۂ̗̈悪�A�֐����f�B�X�N�ɏ�����������
// ���Ă���o�b�t�@�[�̃T�C�Y��菬�����ꍇ �A_write �͎��s���A�o�b
// �t�@�[�̓��e�̓f�B�X�N�Ƀt���b�V������܂���B �߂�l -1 �̓G���[
// �������܂��B �����ȃp�����[�^�[���n�����ƁA�u�p�����[�^�[�̌��؁v
// �Ő�������Ă���悤�ɁA���̊֐��͖����ȃp�����[�^�[ �n���h���[��
// �Ăяo���܂��B ���s�̌p����������Ă���ꍇ�A�֐��� -1 ��Ԃ��Aerrno
// ��EBADF�Ƃ��� 3 �̒l�ɐݒ肳��܂��B����́A�t�@�C���L�q�q��������
// ���邩�A�t�@�C�����������ݗp�ɊJ����܂���BENOSPC�B����́A�����
// ���߂Ƀf�o�C�X�ɏ\���ȗ̈悪�c���Ă��Ȃ��ꍇ���Ӗ����܂��B�܂���
// EINVAL�B����́A�o�b�t�@�[�� null �|�C���^�[���������AUnicode ���[�h
// �Ńt�@�C���ɏ������܂���̃o�C�g�����n���ꂽ�ꍇ���Ӗ����܂��B
//
namespace PipeHelper 
{

	int StdRead(CBinaryString& bd);
	int StdWrite(const CBinaryString& bd);
	int HRead(HANDLE hPipeIn, CBinaryString&bd);
	int HWrite(HANDLE hPipeOut,const CBinaryString&bd);
}
