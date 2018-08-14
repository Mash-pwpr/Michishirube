/*
==============================================================
 Name        : auxiliary.c
 Copyright   : Copyright (C) ����c��w�}�C�N���}�E�X�N���u
 Description : �⏕�֐������ł��D�J���܂��傤�D

 �X�V����
 2015/12/4		�R��	�ꕔ�R�����g�ǉ��Amode_select��LED�\��������mode�̏�����ύX
 2015/1/30		�R��	�ꕔ�R�����g�ǉ��A�\���pLED�̐ݒ��ǉ�
==============================================================
*/

/*�w�b�_�t�@�C���̓ǂݍ���*/
#include "global.h"

//������͔̏ԍ����͓��{��Ń��[�U�[�}�j���A��Rev.00.15�ɏ���

//+++++++++++++++++++++++++++++++++++++++++++++++
//wait
//	ms_wait�͏d�����Ďg�p����(�ȉ��Q��)���Ƃ��o���Ȃ�
//	(���삪�r���Ŏ~�܂�)�B���̂��߁A���荞�݊֐����ł�
//	���m�Ȏ��Ԃ͑���ł��Ȃ���while�����[�v��p���đҋ@����
// ����1�Floop�E�E�E�ҋ@���郋�[�v��
// �߂�l�F����
//+++++++++++++++++++++++++++++++++++++++++++++++
void stay(unsigned int loop){
	while(loop--);					//loop��while������
}
//+++++++++++++++++++++++++++++++++++++++++++++++
//ms_wait
//	�~���b�ҋ@����
// ����1�Fms�E�E�E�ҋ@����[�~���b]
// �߂�l�F����
//+++++++++++++++++++++++++++++++++++++++++++++++
void ms_wait(unsigned int ms){

	uint16_t timercnt, i;
	// PDG�œ���N���b�N���A�V�X�e���N���b�N20MHz��32�Ŋ����ĕ������Ă���
	R_PG_Timer_StartCount_CMT_U0_C0(); // �J�E���g�X�^�[�g

	for(i=0; i<ms; i++){
		R_PG_Timer_SetCounterValue_CMT_U0_C0(0); // �J�E���g�l���[���ɖ߂�
		do{
			R_PG_Timer_GetCounterValue_CMT_U0_C0(&timercnt); // �J�E���g�l���擾
		} while(timercnt < 624 ); // 624�J�E���g�ڂ�1ms�o�������ƂɂȂ�
	}

	R_PG_Timer_HaltCount_CMT_U0_C0(); // �J�E���g�I��

}

// --��ms_wait���{���Ɗ��荞�݊֐��ŏd�������ꍇ�̗���--
//	�{��							���荞�݊֐�
//	ms_wait�֐����g�p
//	��
//	SysTick�^�C�}�𓮍�J�n
//	��
//	�J�E���^����萔���܂�܂őҋ@
//	��
//	(�ҋ@�����荞�ݔ���)		��	ms_wait�֐����g�p
//								��
//								(SysTick�^�C�}�𓮍�J�n)
//								��
//								�J�E���^����萔���܂�܂őҋ@
//								��
//								�ҋ@�I��
//								��
//								�J�E���^���~�߂�
//								��
//	�J�E���^����萔���܂�܂őҋ@	��	(���荞�ݏI��)
//	��
//	(SysTick�^�C�}�����삵�Ă��Ȃ�����
//	�J�E���^�����܂炸�ȍ~�ҋ@�̂܂ܓ��삹��)


/*------------------------------------------------------------
		���[�h�Z���N�g
------------------------------------------------------------*/
//+++++++++++++++++++++++++++++++++++++++++++++++
//select_mode
//	���[�h�Z���N�g���s��
// ����1�Fmode�E�E�E���[�h�ԍ����i�[����ϐ��̃A�h���X
// �߂�l�F����
//+++++++++++++++++++++++++++++++++++++++++++++++
void select_mode(char *mode){

	*mode = 0x00;									//�ϐ��̏�����

	//====�ϐ�������====
	uart_printf(" mode: 0\r");						//���[�h��UART�ő��M

	//====���[�h�I������====
	while(!RET){									//����{�^�����������܂Ń��[�v
		//----�C���N�������g----
		if(INC){									//�����{�^���������ꂽ�ꍇ
			*mode += 1;								//���[�h�ԍ����C���N�������g
			*mode = (*mode + 16) % 16;				//0����16���ɗ}����
			uart_printf(" mode:%2d\r", *mode);		//���[�h�ԍ���UART�ő��M
			ms_wait(250);							//�����ҋ@
		}
		//----�f�N�������g----
		else if(DEC){								//�����{�^���������ꂽ�ꍇ
			*mode -= 1;								//���[�h�ԍ����f�N�������g
			*mode = (*mode + 16) % 16;				//0����16���ɗ}����
			uart_printf(" mode:%2d\r", *mode);		//���[�h�ԍ���UART�ő��M
			ms_wait(250);							//�����ҋ@
		}

		//LED�Ō��݂̒l��\��
		pins_write(DISP_LEDS, *mode, 4);			//LED��ActiveLow�̏ꍇ
	}
}

void timer_start(){
	R_PG_Timer_StartCount_MTU_U0_C1();
	R_PG_Timer_StartCount_MTU_U0_C2();
	R_PG_Timer_StartCount_MTU_U0_C3();
	R_PG_Timer_StartCount_MTU_U0_C4();

	R_PG_Timer_StartCount_CMT_U0_C1();

}
	