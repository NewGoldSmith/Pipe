#pragma once
#include <stdio.h>
#include <io.h>
#include <CBinaryString.h>
#include <fcntl.h>
#include <wtypes.h>
//HRead読み取ったバイト数を返す。
//この関数はファイルの終わりで読み取りをすると、0 を返します。 
// Fdが有効でない場合、ファイルが読み取り用に開かれていない場合、
// またはファイルがロックされている場合は、「パラメーターの検証」
// で説明されているように、無効なパラメーターハンドラーが呼び出されます。
//  実行の継続が許可された場合、この関数は-1 を返し、 errno を
//  EBADFに設定します。
//HWrite成功した場合 、_write 書き込まれたバイト数が返されます。 
// ディスクに残っている実際の領域が、関数がディスクに書き込もうと
// しているバッファーのサイズより小さい場合 、_write は失敗し、バッ
// ファーの内容はディスクにフラッシュされません。 戻り値 -1 はエラー
// を示します。 無効なパラメーターが渡されると、「パラメーターの検証」
// で説明されているように、この関数は無効なパラメーター ハンドラーを
// 呼び出します。 実行の継続が許可されている場合、関数は -1 を返し、errno
// はEBADFという 3 つの値に設定されます。これは、ファイル記述子が無効で
// あるか、ファイルが書き込み用に開かれません。ENOSPC。これは、操作の
// ためにデバイスに十分な領域が残っていない場合を意味します。または
// EINVAL。これは、バッファーが null ポインターだったか、Unicode モード
// でファイルに書き込まれる奇数のバイト数が渡された場合を意味します。
//
namespace PipeHelper 
{

	int StdRead(CBinaryString& bd);
	int StdWrite(const CBinaryString& bd);
	int HRead(HANDLE hPipeIn, CBinaryString&bd);
	int HWrite(HANDLE hPipeOut,const CBinaryString&bd);
}
