#include "Quad.h"
#include "Camera.h"

Quad::Quad()
	:pTexture_(nullptr), pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr)
{
}

Quad::~Quad()
{
}

HRESULT Quad::Initialize()
{
	HRESULT hr;
	// 頂点情報
	//XMVECTOR vertices[] =
	//{
		//XMVectorSet(-1.0f,  0.0f, 1.0f, 0.0f),	// 四角形の頂点（左上）
		//XMVectorSet(1.0f,  0.0f, 1.0f, 0.0f),	// 四角形の頂点（右上)
		//XMVectorSet(2.0f,  0.0f, 0.0f, 0.0f),                    
		//XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f),	// 四角形の頂点（右下）
		//XMVectorSet(-1.0f, 0.0f, -1.0f, 0.0f),	// 四角形の頂点（左下）
		//XMVectorSet(-2.0f,  0.0f, 0.0f, 0.0f),
		//XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
	//};

	VERTEX vertices[] =
	{
		{ XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) },   // 四角形の頂点（左上）
		{ XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),	XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) },   // 四角形の頂点（右上）
		{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),	XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) },   // 四角形の頂点（右下）
		{ XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) },   // 四角形の頂点（左下）

		{ XMVectorSet(-1.0f,  1.0f, 1.0f, 0.0f),XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) },   // 四角形の頂点（奥左上）
		{ XMVectorSet(1.0f,  1.0f, 1.0f, 0.0f),	XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) },   // 四角形の頂点（奥右上）
		{ XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f),	XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) },   // 四角形の頂点（奥右下）
		{ XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f),XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) },   // 四角形の頂点（奥左下）
	};

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	//hr = E_FAIL;
	if (FAILED(hr))
	{
		//int MessageBox(
		//	[in, optional] HWND    hWnd,
		//	[in, optional] LPCTSTR lpText,
		//	[in, optional] LPCTSTR lpCaption,
		//	[in]           UINT    uType
		//);
		MessageBox(NULL, L"頂点バッファの作成に失敗", NULL, MB_OK);
		return hr;
	}

	//インデックス情報
	int index[] = { 0,1,2, 0,2,3, 1,5,6, 1,6,2, 5,4,7, 5,7,6, 0,4,7, 0,7,3, 4,5,1, 4,1,0, 7,6,2, 7,2,3 };

	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr)) {
		MessageBox(NULL, L"インデックスバッファの作成に失敗しました", NULL, MB_OK);
		return hr;
	}

	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr)) {
		MessageBox(NULL, L"コンスタントバッファの作成に失敗しました", NULL, MB_OK);
		return hr;
	}

	pTexture_ = new Texture;
	pTexture_->Load("Asset\\dice.png");

	return S_OK;
}

void Quad::Draw(XMMATRIX& worldMatrix)
{
	//コンスタントバッファに渡す情報
	//XMVECTOR position = { 1, 4, -10, 0 };	//カメラの位置
	//XMVECTOR target = { 0, 0, 0, 0 };	//カメラの焦点
	//XMMATRIX view = XMMatrixLookAtLH(position, target, XMVectorSet(0, 1, 0, 0));	//ビュー行列
	//XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);//射影行列
	//                      遠近感　　　　　　　π/４　 幅　　　 高さ

	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(worldMatrix * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matW = XMMatrixTranspose(worldMatrix); //MATRIXの掛け算のやり方がDirectXと違うので転置をとる

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのリソースアクセスを一時止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);

	//ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	//Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
	//ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	//Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	//サンプラーとシェーダーリソースビューをシェーダにセット
	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->DrawIndexed(36, 0, 0);//三角化した時の頂点の数
}

void Quad::Release()
{
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}
