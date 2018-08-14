/*
==============================================================
 Name        : interrupt.c
 Copyright   : Copyright (C) ����c��w�}�C�N���}�E�X�N���u
 Description : ���荞�݊֐������ł��D(����)lpc11xx.h�Ńv���g�^�C�v�錾����Ă��܂��D

 �X�V����
 2015/12/4		�R��	�R�����g�ǉ��Atp�̏����𕪂���₷���ύX
 2016/2/24		�R��	2016�N�x�p�Ƀs���ݒ��ύX
 2017/10/14		���� ���������̏������򃍃W�b�N�����C��(�J�E���^���I�[�o�[�t���[����)
==============================================================
 */
#include "global.h"

//+++++++++++++++++++++++++++++++++++++++++++++++
//Mtu3IcCmDIntFunction
//���荞�݊֐��A�����[�^�̊Ǘ����s��
// �����F����
// �߂�l�F����
//+++++++++++++++++++++++++++++++++++++++++++++++
void Mtu3IcCmDIntFunc(void){		//====�����[�^���荞��====

	uint16_t paraL0 = 40000;
	uint16_t paraL1 = 40000;

	pin_write(PA4,0);
	pin_write(PA6,0);

		//====����������====
		//----��������----
		if(MF.FLAG.DECL){															//�����t���O��1�ł���ꍇ
			if(t_cnt_l > minindex) t_cnt_l--; //�Œᑬ�x�ȏ�̏ꍇ�e�[�u���J�E���^�����炷���ƂŌ���
		}
		//----��������----
		else if(MF.FLAG.ACCL){														//�����t���O��1�ł���ꍇ
			if(t_cnt_l < maxindex) t_cnt_l++; //�Œᑬ�x�ȉ��̏ꍇ�e�[�u���J�E���^�𑝂₷���Ƃŉ���
		}

		//----��~���̏���----
		if(MF.FLAG.STOP){															//��~�t���O��1�ł���ꍇ
			t_cnt_l = minindex;														//�e�[�u���J�E���^���N���A
			interval_l = DEFGRC;													//���E���[�^���f�t�H���g
			paraL0 = interval_l;											//�����[�^�̊��荞�݊��ݒ�
			//----�f�t�H���g�̑��x----
		}else if(MF.FLAG.DEF){														//�葬�t���O��1�ł���ꍇ
			paraL0 = DEFGRC - dl;											//�葬�ɑ΂��Đ���������l�ɐݒ�
			//----���̑��̂Ƃ�----
		}else{
			paraL0 = table[t_cnt_l] - dl;									//�����[�^�C���^�[�o���ݒ�
		}

		paraL1 = paraL0 + 4000;									//�o�̓p���X������ݒ�

		R_PG_Timer_SetTGR_C_MTU_U0_C3(paraL0);
		R_PG_Timer_SetTGR_D_MTU_U0_C3(paraL1);

}

void Mtu3IcCmCIntFunc(void){
	pin_write(PA4,1);
	pin_write(PA6,0);
}

//+++++++++++++++++++++++++++++++++++++++++++++++
//Mtu4IcCmDIntFunc
//	16�r�b�g�^�C�}�[1�̊��荞�݊֐��A�E���[�^�̊Ǘ����s��
// �����F����
// �߂�l�F����
//+++++++++++++++++++++++++++++++++++++++++++++++
void Mtu4IcCmDIntFunc(void){			//====�E���[�^���荞��====
	uint16_t paraR0 = 40000;
	uint16_t paraR1 = 40000;

	pin_write(P54,1);
	pin_write(P55,0);

	//====����������====
	//----��������----
	if(MF.FLAG.DECL){															//�����t���O��1�ł���ꍇ
		if(t_cnt_r > minindex) t_cnt_r--; //�Œᑬ�x�ȏ�̏ꍇ�e�[�u���J�E���^�����炷���ƂŌ���
	}
	//----��������----
	else if(MF.FLAG.ACCL){														//�����t���O��1�ł���ꍇ
		if(t_cnt_r < maxindex) t_cnt_r++; //�Œᑬ�x�ȉ��̏ꍇ�e�[�u���J�E���^�𑝂₷���Ƃŉ���
	}
	//----��~���̏���----
	if(MF.FLAG.STOP){															//��~�t���O��1�ł���ꍇ
		t_cnt_r = minindex;														//�e�[�u���J�E���^���N���A
		interval_r = DEFGRC;													//���E���[�^���f�t�H���g
		paraR0 = interval_r;											//�E���[�^�̊��荞�݊��ݒ�
		//----�f�t�H���g�̑��x----
	}else if(MF.FLAG.DEF){														//�葬�t���O��1�ł���ꍇ
		paraR0 = DEFGRC - dr;											//�葬�ɑ΂��Đ���������l�ɐݒ�
		//----���̑��̂Ƃ�----
	}else{
		paraR0 = table[t_cnt_r] - dr;									//�E���[�^�C���^�[�o���ݒ�
	}

	paraR1 = paraR0 + 4000;									//�o�̓p���X������ݒ�

	R_PG_Timer_SetTGR_C_MTU_U0_C4(paraR0);
	R_PG_Timer_SetTGR_D_MTU_U0_C4(paraR1);

}

void Mtu4IcCmCIntFunc(void){
	pin_write(P54,1);
	pin_write(P55,0);
}
/*------------------------------------------------------------
		32bit�^�C�}���荞��
------------------------------------------------------------*/
//+++++++++++++++++++++++++++++++++++++++++++++++
//Cmt0IntFunc
//32�r�b�g�^�C�}�[0�̊��荞�݊֐��A�Z���T�l�̎擾�Ɛ�����s��
// �����F����
// �߂�l�F����
//+++++++++++++++++++++++++++++++++++++++++++++++
void Cmt1IntFunc(void){
	pulse_l++;
	 
	switch(tp){
	//----�Z���T����----
	case 0:
		//���Z���T�l�̎擾
		pin_write(PE1,1);								//������LED��ON
		
		S12AD.ADANS0.WORD = 0x02;
		R_PG_ADC_12_StartConversionSW_S12AD0();					//�������܂�܂ŏ����ҋ@
		R_PG_ADC_12_GetResult_S12AD0(ad_res);
		ad_l = ad_res[1];										//������̃A�i���O�l���L�^
		
		break;
	case 1:
		pin_write(PE1,0);
		
		//���ʃZ���T�l�̎擾
		pin_write(PE0,1);								//������LED��ON
		pin_write(PE2,1);								//������LED��ON
		pin_write(PE4,1);
		

		S12AD.ADANS0.WORD = 0x01;
		R_PG_ADC_12_StartConversionSW_S12AD0();
		R_PG_ADC_12_GetResult_S12AD0(ad_res);
		ad_fr = ad_res[0];
		
		S12AD.ADANS0.WORD = 0x04;
		R_PG_ADC_12_StartConversionSW_S12AD0();
		R_PG_ADC_12_GetResult_S12AD0(ad_res);
		ad_ff = ad_res[2];
		
		S12AD.ADANS0.WORD = 0x0f;
		R_PG_ADC_12_StartConversionSW_S12AD0();
		R_PG_ADC_12_GetResult_S12AD0(ad_res);
		ad_fl = ad_res[4];
		

		break;
	case 2:
		pin_write(PE0,0);								//������LED��ON
		pin_write(PE2,0);								//������LED��ON
		pin_write(PE4,0);

	
		//�E�Z���T�l�̎擾
		pin_write(PE3,1);								//������LED��ON
		//stay(1000);
		
		//S12AD.ADCSR.BIT.ADST = 0;

		S12AD.ADANS0.WORD = 0x08;
		R_PG_ADC_12_StartConversionSW_S12AD0();
		//S12AD.ADCSR.BIT.ADST = 1;
		R_PG_ADC_12_GetResult_S12AD0(ad_res);

		ad_r = ad_res[3];								//������̃A�i���O�l���L�^
//		UART_printf("[%4d]\n", ad_l);
															//������LED��OFF
		break;

		//----���䏈��----
	case 3:
		pin_write(PE3,0);
		
		//����t���O������ΐ���
		if(MF.FLAG.CTRL){
			//��l����̍�������
			dif_l = (int16_t)(ad_l - base_l);
			dif_r = (int16_t)(ad_r - base_r);

			//����̔��f
			if((SREF_MIN_L < dif_l) && (dif_l < SREF_MAX_L))
				dl = CONT * dif_l;	//��ᐧ��l������
			else
				dl = 0;	//�����O��Ă���ΐ���͂����Ȃ�
			if((SREF_MIN_R < dif_r) && (dif_r < SREF_MAX_R))
				dr = CONT * dif_r;
			else
				dr = 0;

		}else{
			//����t���O���Ȃ���ΐ���l0
			dl = dr = 0;
		}
		break;
	}

	//====�^�X�N�|�C���^��i�߂�====
	tp = (tp+1) % 4;

}

/*void S12ad0IntFunc(void){
	R_PG_ADC_12_GetResult_S12AD0(ad_res);
}*/
