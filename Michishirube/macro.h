/*
==============================================================
 Name        : macro.h
 Copyright   : Copyright (C) 早稲田大学マイクロマウスクラブ
 Description : マクロ宣言です．変数型とかで楽に使えるようになります．

  更新履歴
 2015/12/13		山上	コメント追加
==============================================================
*/

#ifndef MACRO_H_												//対応ファイルで一度も読み込まれていない場合以下を定義
	#define MACRO_H_											//読み込んだことを表す

	//旧環境のプログラムを動かすのに必要なマクロ。出てきたら読む程度でいいです。
	#define _BV(bit)			(1<<(bit))						//指定ビット分1を左へビットシフトした値
	#define cbi(addr,bit)	addr &= ~_BV(bit)					//指定レジスタの指定ビットを0にする
	#define sbi(addr,bit)	addr |= _BV(bit)					//指定レジスタの指定ビットを1にする
	#define bit_is_set(addr,bit)	(addr & _BV(bit) ? 1 : 0)	//指定レジスタの指定ビットが1なら1、0なら0
	#define bit_is_clear(addr,bit)	(addr & _BV(bit) ? 0 : 1)	//指定レジスタの指定ビットが0なら1、1なら0
	#define outp(data,addr)	addr = (data)						//指定レジスタを指定データにする
	#define inp(addr)		(addr)								//指定レジスタを取り出す
	#define outb(addr,data)	addr = (data)						//指定レジスタを指定データにする
	#define inb(addr)		(addr)								//指定レジスタを取り出す
	#define PRG_RDB(addr)	(pgm_read_byte(addr))				//プログラムメモリの読み出し？


	//型の置き換え、環境特有の型を一般的な型に置き換える。移植時にはここを変えればいいので楽になる(全部置き換わっていない気もするけど...)
	// 8-bit, int8_t=signed char
/*	typedef char			CHAR;
	typedef unsigned char	UCHAR;
	// 16-bit, SHORTなんてなかった
	typedef int16_t			INT;
	typedef uint16_t		UINT;
	// 32-bit
	typedef int32_t			LONG;
	typedef uint32_t		ULONG;
/*
	// 8-bit
	typedef char			CHAR;
	typedef unsigned char	UCHAR;
	typedef unsigned char	BYTE;

	// 16-bit
	typedef short			SHORT;
	typedef unsigned short	USHORT;
	typedef unsigned short	WORD;

	// 32-bit
	typedef long			LONG;
	typedef unsigned long	ULONG;
	typedef unsigned long	DWORD;
*/
	// ブール演算子
	typedef enum { FALSE = 0, TRUE } BOOL;


	//旧環境のプログラムを動かすのに必要なマクロ。多分使ってないです
	#ifndef	NOP														//待機を表すマクロ
	#define	NOP		asm volatile("nop\n"::)							//アセンブリ言語を直接書いてる？
	#endif
	#ifndef SLEEP													//待機を表すマクロ
	#define	SLEEP	asm volatile("sleep\n"::)						//アセンブリ言語を直接書いてる？
	#endif

	//その他定義
	#ifndef NULL													//NULLが対応ファイルで定義されていない場合
	#define NULL    0												//0として定義する
	#endif

	// Math 2011/03/29
	#define ABS(x)	((x) < 0 ? -(x) : (x))							//絶対値を返すマクロ

#endif /* MACRO_H_ */
