/*
==============================================================
 Name        : drive.h
 Copyright   : Copyright (C) ����c��w�}�C�N���}�E�X�N���u
 Description : ���s�֘A�̊֐������ł��D

  �X�V����
 2015/12/11		�R��	�R�����g��SLA7078�p�̃}�N����ǉ�
 2016/2/24		�R��	2016�N�x�p�Ƀs���ݒ��ύX
==============================================================
*/

/*�w�b�_�t�@�C���̓ǂݍ���*/
#include "port.h"

#ifndef DRIVE_H_										//�Ή��t�@�C���ň�x���ǂݍ��܂�Ă��Ȃ��ꍇ�ȉ����`

	#define DRIVE_H_									//�ǂݍ��񂾂��Ƃ�\��

/*============================================================
		�e��萔��ϐ��錾
============================================================*/
	//----�h���C�oIC�֘A----
	#define MDR_CLK_L	P0_8		//�N���b�N�[�q��
	#define MDR_CLK_R	P1_9		//�N���b�N�[�q�E
// ��OK

	#define MDR_CW_L	P2_2		//IC���z��H��CW/CCW��
	#define MDR_CW_R	P3_5		//IC���z��H��CW/CCW�E
	#define MDR_REF		P2_9		//���t�@�����X

	#define MDR_M1		P0_6		//�㎥�؂�ւ����W�b�N1
	#define MDR_M2		P3_4		//�㎥�؂�ւ����W�b�N2
	#define MDR_M3		P2_1		//�㎥�؂�ւ����W�b�N3

	/* �㎥�؂�ւ����W�b�N
	�ESLA7078MPRT
	  M1 | M2 | M3 |�㎥���[�h
	   L |  L |  L | 2���㎥(Mode8�Œ�)
	   L |  H |  L | 2���㎥(ModeF�Œ�)
	   H |  L |  L | 1-2���㎥(2����)
	   H |  H |  L | 1-2���㎥(ModeF�Œ�)
	   L |  L |  H |  W1-2���i 4�����j
	   L |  H |  H | 4W1-2���i16�����j
	   H |  L |  H | 2W1-2���i 8�����j
	   H |  H |  H | �X���[�v���[�h2

	�ESLA7073MPRT
	  M1 | M2 | �㎥���[�h
	   L |  L | 4W1-2���i16�����j
	   L |  H | 2W1-2���i 8�����j
	   H |  L |  W1-2���i 4�����j
	   H |  H |   1-2���i 2�����j
	*/



	//----��������֘A----
	#define MT_FWD_L	1			//CW/CCW�őO�ɐi�ޏo��(��)
	#define MT_BACK_L	0			//CW/CCW�Ō��ɐi�ޏo��(��)
	#define MT_FWD_R	1			//CW/CCW�őO�ɐi�ޏo��(�E)
	#define MT_BACK_R	0		//CW/CCW�Ō��ɐi�ޏo��(�E)

	#define FORWARD	0x00			//�O�i����
	#define BACK	0x11			//���
	#define TURN_L	0x01			//��]����(��)
	#define TURN_R	0x10			//��]����(�E)

	//----�����]���p�萔----
	#define DIR_TURN_R90	0x01	//�E90�x��]
	#define DIR_TURN_L90	0xff	//��90�x��]
	#define DIR_TURN_180	0x02	//180�x��]


	//====�ϐ�====
#ifdef EXTERN										//�Ή��t�@�C����EXTERN����`����Ă���ꍇ
	/*�O���[�o���ϐ��̒�`*/
	const uint16_t table[] = {
		#include "table.h"
	};												//table.h�ɓ\��t�����l��ێ�����z��
	volatile uint16_t t_cnt_l, t_cnt_r;					//�e�[�u���J�E���^
	volatile uint16_t minindex, maxindex;				//�Œᑬ�x�E�ő呬�x
	volatile uint16_t interval_l, interval_r;			//���E�C���^�[�o��
	volatile uint16_t pulse_l, pulse_r;					//���E�p���X�J�E���^
	int16_t	dl, dr;										//��ᐧ���
#else												//�Ή��t�@�C����EXTERN����`����Ă��Ȃ��ꍇ
	/*�O���[�o���ϐ��̐錾*/
	extern const uint16_t table[];					//table.h�ɓ\��t�����l��ێ�����z��
	extern volatile uint16_t t_cnt_l, t_cnt_r;			//�e�[�u���J�E���^
	extern volatile uint16_t minindex, maxindex;		//�Œᑬ�x�E�ő呬�x
	extern volatile uint16_t interval_l, interval_r;	//���E�C���^�[�o��
	extern volatile uint16_t pulse_l, pulse_r;			//���E�p���X�J�E���^
	extern int16_t	dl, dr;								//��ᐧ���
#endif

/*============================================================
		�֐��v���g�^�C�v�錾
============================================================*/
	//====���s�n====
	//----��֐�----
	void driveA(uint16_t, unsigned char);	//�������s
	void driveD(uint16_t, unsigned char);	//�������s
	void driveAD(uint16_t, unsigned char);	//���������s
	void driveU(uint16_t, unsigned char);	//�������s(�O�̑��x���ێ�)
	void driveC(uint16_t, unsigned char);	//�葬���s(���炩���ߌ��܂������x)
	void set_dir(unsigned char);		//�i�ޕ����̐ݒ�

	void drive_start();
	void drive_stop(unsigned char);

	//----�ȈՏ�ʊ֐�----
	void half_sectionA();		//���������
	void half_sectionD();		//���������
	void a_section();			//����������
	void a_sectionU();			//��������
	void turn_R90();			//�E90��]
	void turn_L90();			//��90��]
	void turn_180();			//180�x��]
	void set_position();		//�ʒu���킹

	//----���s����----
	void test_drive(char *mode);	//���s����

#endif /* DRIVE_H_ */
