//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Player.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPlayer

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
}

void CPlayer::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, D3D12_CPU_DESCRIPTOR_HANDLE d3dCbvCpuDescriptorHandle)
{
}

void CPlayer::ReleaseShaderVariables()
{
}

void CPlayer::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CPlayer::UpdateShaderVariablesAndPipelineState(ID3D12GraphicsCommandList *pd3dCommandList)
{
	UpdateShaderVariables(pd3dCommandList);

	if (m_pCamera) m_pCamera->UpdateShaderVariablesAndPipelineState(pd3dCommandList);
}
