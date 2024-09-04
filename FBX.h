#pragma once
#include "Direct3D.h"
#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>
#include "Texture.h"
#include "Transform.h"
#include <filesystem>

//#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

class FBX
{
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP; //ワールドビュープロジェクション
		XMMATRIX    matNormal; //法線ベクトルの変換用
		XMFLOAT4	diffuseColor;
		int		isTextured;
	};
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal; //ノーマル追加(法線ベクトル)
	};

	ID3D11Buffer* pVertexBuffer_; //頂点バッファ用のメモリ
	ID3D11Buffer** pIndexBuffer_; //インデックスバッファ用のメモリ
	ID3D11Buffer* pConstantBuffer_; //コンスタントバッファ用のメモリ
	std::vector<MATERIAL> pMaterialList_; //マテリアルの配列
	std::vector<int> indexCount_;

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;
	//int indexCount_;

	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);
public:
	FBX();
	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform);
	void    Release();
};

