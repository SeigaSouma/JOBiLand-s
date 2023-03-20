//<====================================================
//
//�t�H�O[��]����(fog.cpp)
//Author kazuki watanabe
//
//<====================================================
#include "fog.h"
#include "Input.h"

//�}�N����`
#define MAX_START (550.0f)//���`���̊J�n�ʒu�̍ő�l
#define MAX_END (2500.0f)//���`���̏I���ʒu�̍ő�l

//�v���g�^�C�v�錾
void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density);
void SetupPixelFog(DWORD Color, DWORD Mode, BOOL UseRange);
void ChangeFogCol(void);

Fog g_Fog;//�����

//<===========================
//���̏���������
//<===========================
void InitFog(void)
{
	g_Fog.FogCol = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);//���̐F
	g_Fog.FogType = D3DFOG_LINEAR;//���̎��
}

//<===========================
//���̏I������
//<===========================
void UninitFog(void)
{

}

//<===========================
//���̍X�V����
//<===========================
void UpdateFog(void)
{
	SetupPixelFog(g_Fog.FogCol, g_Fog.FogType, 1);//�s�N�Z���t�H�O�̐ݒ�
#ifdef _DEBUG
	ChangeFogCol();//���̐F�ύX
#endif
}

//<===========================================
//���_�t�H�O�̐ݒ�
//<===========================================
void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	float Start = MAX_START;//���`���̊J�n�ʒu
	float End = MAX_END;//���`���̏I���ʒu

	//���̃u�����h��L���ɂ���
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//�F��ݒ�
	pDevice->SetRenderState(D3DRS_FOGCOLOR, Color);

	//���̃p�����[�^�[��ݒ�
	//�������`�����[�h��������
	if (Mode == D3DFOG_LINEAR)
	{
		pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
		pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));
	}
	//���̂ق�
	else
	{
		pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));

		//�����x��0.0f����1.0f�͈̔͂Őݒ肷��
	}
	//���_�t�H�O�����ŃT�|�[�g����Ă���ꍇ�͂��͈̔͂̃t�H�O��L���ɂ���
	//����UseRange��0�ȊO�̏ꍇ�A
	if (UseRange != 0)
	{
		pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
	}

}

//<===========================================
//�s�N�Z���t�H�O�̐ݒ�
//<===========================================
void SetupPixelFog(DWORD Color, DWORD Mode, BOOL UseRange)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//<========================================
	//���`���[�h�p�̕ϐ�
	float Start = MAX_START;//���`���̊J�n�ʒu
	float End = MAX_END;//���`���̏I���ʒu
	//
	//<========================================

	float Density = 0.0005f;//�����x(0.0f����1.0f�͈̔�)

#ifdef _DEBUG
	//�f�o�b�O�p�̖����x�ύX
	if (GetKeyboardPress(DIK_F4) == true)
	{
		Density = 0.0015f;
	}
#endif
	//���̃u�����h��L���ɂ���
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//�F��ݒ�
	pDevice->SetRenderState(D3DRS_FOGCOLOR, Color);

	//�������`�����[�h��������
	if (Mode == D3DFOG_LINEAR)
	{//���`���̃p�����[�^��ݒ肷��

		pDevice->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
		pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
		pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));
	}
	//���̂ق�
	else
	{
		pDevice->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
		pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));
	}

	//�t�H�O�����ŃT�|�[�g����Ă���ꍇ�͂��͈̔͂̃t�H�O��L���ɂ���
	//����UseRange��0�ȊO�̏ꍇ�A
	if (UseRange != 0)
	{
		pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
	}
}

//<===========================================
//�t�H�O�̐F�ύX����
//<===========================================
void ChangeFogCol(void)
{
	//�ԐF
	if (GetKeyboardPress(DIK_R) == true)
	{
		//�ԐF�̒l�����Z���Ă���
		g_Fog.FogCol.r += 0.01f;
	}
	//�ΐF
	if (GetKeyboardPress(DIK_G) == true)
	{
		//�ΐF�̒l�����Z���Ă���
		g_Fog.FogCol.g += 0.01f;
	}
	//�F
	if (GetKeyboardPress(DIK_B) == true)
	{
		//�F�̒l�����Z���Ă���
		g_Fog.FogCol.b += 0.01f;
	}

	//<=========================================
	//�␳����
	//<=========================================
	//�ԐF��1.01�𒴂������ɂȂ�����
	if (g_Fog.FogCol.r > 1.01f)
	{
		//0�ɂ���
		g_Fog.FogCol.r = 0.0f;
	}
	//�ΐF��1.01�𒴂������ɂȂ�����
	if (g_Fog.FogCol.g > 1.01f)
	{
		//0�ɂ���
		g_Fog.FogCol.g = 0.0f;
	}
	//�F��1.01�𒴂������ɂȂ�����
	if (g_Fog.FogCol.b > 1.01f)
	{
		//0�ɂ���
		g_Fog.FogCol.b = 0.0f;
	}
	//
	//<=========================================
}