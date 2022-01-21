#pragma once
//#pragma comment(lib, "WS2_32.LIB")
#include <WinSock2.h>
#include <string>
#include <tchar.h>
#include <ws2tcpip.h>
#include <CBinaryString.h>
namespace SocketHelper
{
	void SockPrintErr();
	void ShowSocketStatus(bool show, SOCKET Socket, const std::string& strPrefix);
	void SockPrintMessage(bool show, const std::string& strMessage);
	//�N���XCByteData��p�֐�
	//SockRead
	// �G���[���������Ȃ��ꍇ�A�߂�l�͓ǂݍ��݃o�C�g���ł���A
	//�\�P�b�g������ɕ����Ă��鎞�͂O�B
	//����ȊO�́ASOCKET�QERROR���Ԃ���AWSAGetLastError�֐���
	//����̃G���[�R�[�h���擾���鎖���ł���B
	//SockWrite
	// �G���[���������Ȃ��ꍇ�A send�͑��M���ꂽ���v�o�C�g��
	// ��Ԃ��܂��B
	//����́A len�p�����[�^�[�ő��M���v�����ꂽ���������Ȃ��ꍇ����
	//��܂��B����ȊO�̏ꍇ�́ASOCKET_ERROR�̒l���Ԃ���A
	//WSAGetLastError���Ăяo�����Ƃœ���̃G���[�R�[�h���擾�ł��܂� �B
	int SockRead(const SOCKET Socket, CBinaryString* pByteData);
	int SockWrite(const SOCKET Socket, const CBinaryString* pByteData);

}