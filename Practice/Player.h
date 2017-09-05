#pragma once

#define DIR_FORWARD				0x01
#define DIR_BACKWARD			0x02
#define DIR_LEFT				0x04
#define DIR_RIGHT				0x08
#define DIR_UP					0x10
#define DIR_DOWN				0x20

#include "Object.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
protected:
	CCamera						*m_pCamera = NULL;

public:
	CPlayer();
	virtual ~CPlayer();

	CCamera *GetCamera() { return(m_pCamera); }
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }

	static void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, D3D12_CPU_DESCRIPTOR_HANDLE d3dCbvCpuDescriptorHandle);
	static void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void UpdateShaderVariablesAndPipelineState(ID3D12GraphicsCommandList *pd3dCommandList);
};


