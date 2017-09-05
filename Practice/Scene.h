//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------

#pragma once

#include "Object.h"
#include "Camera.h"

class CScene
{
public:
    CScene();
    ~CScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseObjects();

	bool ProcessInput();
    void AnimateObjects(float fTimeElapsed);
    void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera=NULL);

	void ReleaseUploadBuffers();

	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }

protected:
	CGameObject					**m_ppObjects = 0;
	int							m_nObjects = 0;

	ID3D12RootSignature			*m_pd3dGraphicsRootSignature = NULL;
};
