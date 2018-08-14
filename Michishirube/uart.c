/*
==============================================================
 Name        : uart.c
 Copyright   : Copyright (C) 早稲田大学マイクロマウスクラブ
 Description : 通信用関数たちです．

  更新履歴
 2015/12/10		山上	コメント追加、uart_Init内LCR処理を変更
==============================================================
*/

//※解説の章番号等は日本語版ユーザーマニュアルRev.00.15に準拠

/*ヘッダファイルの読み込み*/
#include "global.h"
#include <stdio.h>
#include <stdarg.h>															// 可変引数リストを使用

//+++++++++++++++++++++++++++++++++++++++++++++++
//uart_Init
//	UARTの初期設定を行う
// 引数1：baudrate・・・通信のボーレート
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void uart_Init(){
	R_PG_SCI_Set_C1();
	R_PG_RSPI_Set_C0();
	R_PG_RSPI_SetCommand_C0();
}

//+++++++++++++++++++++++++++++++++++++++++++++++
//UART_printf
//	UARTで標準出力を行う
//	対応指定子	：%s,%c,%d,%u,%X,%ld,%lu,%lX
//				　数値はリーディングゼロと表示桁数指定可能
// 引数1：format・・・送信する本文
// 引数2以降:・・・可変引数(printf等の本文以降の引数のようなもの)
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++

void uart_printf(const char *format, ...) {
    va_list arg;
    uint8_t buf[64];
    uint8_t *p;

    va_start(arg, format);

    vsprintf((char*)buf, format, arg);
    p = buf;
    while( *p != '\0' ) {
        R_PG_SCI_SendAllData_C1(p, 1);
        p++;
    }
    va_end(arg);
}

void Spi0IntFunc(){
	R_PG_RSPI_StopModule_C0();
}
