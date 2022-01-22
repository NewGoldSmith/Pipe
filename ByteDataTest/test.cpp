//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <string.h>
//int TextToHexTextA(char* pstrDest, unsigned int szDest, char* pstrSource);
//int main()
//{
//	char strSource[] { "A\r\n" };
//	char strDest[256]{};
//	int len = TextToHexTextA(strDest, 256, strSource);
//}
//int TextToHexTextA(char* pstrDest, unsigned int szDest, char* pstrSource)
//{
//	unsigned int SourceLen = strlen(pstrSource);
//	char str[3]{};
//	int TotalLen = 0;
//	int LenWriten = 0;
//	int i = 0;
//	do {
//
//		LenWriten = sprintf_s(str, 3, "%02X", (unsigned char)pstrSource[i]);
//		_memccpy(pstrDest + TotalLen, str, sizeof(char), LenWriten);
//		TotalLen += LenWriten;
//		pstrDest[TotalLen] = ' ';
//		TotalLen++;
//		i++;
//	} while (i < SourceLen && (TotalLen + 3) < szDest);
//	pstrDest[TotalLen - 1] = '\0';
//	return TotalLen - 1;
//}