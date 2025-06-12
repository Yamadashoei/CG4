//#include <3d\Model.h>
#include <3d\Camera.h>
#include <base\DirectXCommon.h>
#include <3d\Material.h>
#include <math\MathUtility.h>
#include <base\StringUtility.h>
#include <base\TextureManager.h>
#include <3d\WorldTransform.h>
#include <algorithm>
#include <cassert>
#include <d3dcompiler.h>
#include <format>
#include <fstream>
#include <numbers>
#include <sstream>
#include "Model2.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace Microsoft::WRL;

namespace KamataEngine {

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const char* Model2::kBaseDirectory = "Resources/";
const char* Model2::kDefaultModelName = "cube";
ModelCommon2* ModelCommon2::sInstance_ = nullptr;

void Model2::StaticInitialize() { ModelCommon2::GetInstance()->Initialize(); }

void Model2::StaticFinalize() { ModelCommon2::GetInstance()->Terminate(); }

Model2* Model2::Create() {
	// メモリ確保
	Model2* instance = new Model2;
	instance->InitializeFromFile(kDefaultModelName, false);

	return instance;
}

Model2* Model2::CreateFromOBJ(const std::string& modelname, bool smoothing) {
	// メモリ確保
	Model2* instance = new Model2;
	instance->InitializeFromFile(modelname, smoothing);

	return instance;
}

Model2* Model2::CreateSphere(uint32_t divisionVertial, uint32_t divisionHorizontal) {
	// メモリ確保
	Model2* instance = new Model2;
	std::vector<Mesh::VertexPosNormalUv> vertices;
	std::vector<uint32_t> indices;

	// 頂点数
	const uint32_t kNumSphereVertices = divisionVertial * divisionHorizontal * 4;
	// インデックス数
	const uint32_t kNumSphereIndices = divisionVertial * divisionHorizontal * 6;

	vertices.resize(kNumSphereVertices);
	indices.resize(kNumSphereIndices);

	float pi = std::numbers::pi_v<float>;

	// 経度分割1つ分の角度
	const float kLonEvery = pi * 2.0f / float(divisionHorizontal);
	// 緯度分割1つ分の角度
	const float kLatEvery = pi / float(divisionVertial);

	// 座標計算
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < divisionVertial; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;
		// 経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < divisionHorizontal; ++lonIndex) {
			uint32_t startIndex = (latIndex * divisionHorizontal + lonIndex) * 4;
			float lon = lonIndex * kLonEvery;
			// 左下
			vertices[startIndex].pos.x = std::cos(lat) * std::cos(lon);
			vertices[startIndex].pos.y = std::sin(lat);
			vertices[startIndex].pos.z = std::cos(lat) * std::sin(lon);
			vertices[startIndex].uv = {float(lonIndex) / float(divisionHorizontal), 1.0f - float(latIndex) / float(divisionVertial)};
			vertices[startIndex].normal = vertices[startIndex].pos;
			vertices[startIndex].normal = MathUtility::Normalize(vertices[startIndex].normal);
			// 左上
			vertices[startIndex + 1].pos.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertices[startIndex + 1].pos.y = std::sin(lat + kLatEvery);
			vertices[startIndex + 1].pos.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertices[startIndex + 1].uv = {float(lonIndex) / float(divisionHorizontal), 1.0f - float(latIndex + 1) / float(divisionVertial)};
			vertices[startIndex + 1].normal = vertices[startIndex + 1].pos;
			vertices[startIndex + 1].normal = MathUtility::Normalize(vertices[startIndex + 1].normal);
			// 右下
			vertices[startIndex + 2].pos.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertices[startIndex + 2].pos.y = std::sin(lat);
			vertices[startIndex + 2].pos.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertices[startIndex + 2].uv = {float(lonIndex + 1) / float(divisionHorizontal), 1.0f - float(latIndex) / float(divisionVertial)};
			vertices[startIndex + 2].normal = vertices[startIndex + 2].pos;
			vertices[startIndex + 2].normal = MathUtility::Normalize(vertices[startIndex + 2].normal);
			// 右上
			vertices[startIndex + 3].pos.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			vertices[startIndex + 3].pos.y = std::sin(lat + kLatEvery);
			vertices[startIndex + 3].pos.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			vertices[startIndex + 3].uv = {float(lonIndex + 1) / float(divisionHorizontal), 1.0f - float(latIndex + 1) / float(divisionVertial)};
			vertices[startIndex + 3].normal = vertices[startIndex + 3].pos;
			vertices[startIndex + 3].normal = MathUtility::Normalize(vertices[startIndex + 3].normal);
		}
	}

	// インデックス計算
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < divisionVertial; ++latIndex) {
		// 経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < divisionHorizontal; ++lonIndex) {
			uint32_t startIndex = (latIndex * divisionHorizontal + lonIndex) * 6;
			uint32_t startIndexVertices = (latIndex * divisionHorizontal + lonIndex) * 4;

			indices[startIndex + 0] = startIndexVertices + 0;
			indices[startIndex + 1] = startIndexVertices + 1;
			indices[startIndex + 2] = startIndexVertices + 2;
			indices[startIndex + 3] = startIndexVertices + 2;
			indices[startIndex + 4] = startIndexVertices + 1;
			indices[startIndex + 5] = startIndexVertices + 3;
		}
	}

	instance->InitializeFromVertices(vertices, indices);

	return instance;
}

void Model2::PreDraw(ID3D12GraphicsCommandList* commandList) { ModelCommon2::GetInstance()->PreDraw(commandList); }

void Model2::PostDraw() { ModelCommon2::GetInstance()->PostDraw(); }

void Model2::InitializeFromFile(const std::string& modelname, bool smoothing) {
	// モデル読み込み
	LoadModel(modelname, smoothing);

	// メッシュのマテリアルチェック
	for (auto& m : meshes_) {
		// マテリアルの割り当てがない
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial_ == nullptr) {
				// デフォルトマテリアルを生成
				defaultMaterial_ = Material::Create();
				defaultMaterial_->name_ = "no material";
				defaultMaterial_->Update();
			}
			// デフォルトマテリアルをセット
			m->SetMaterial(defaultMaterial_.get());
		}
	}

	// メッシュのバッファ生成
	for (auto& m : meshes_) {
		m->CreateBuffers();
	}

	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials_) {
		m.second->Update();
	}

	// テクスチャの読み込み
	LoadTextures();
}

void Model2::InitializeFromVertices(const std::vector<Mesh::VertexPosNormalUv>& vertices, const std::vector<uint32_t>& indices) {

	// メッシュ生成
	meshes_.emplace_back(std::make_unique<Mesh>());
	Mesh* mesh = meshes_.back().get();

	// メッシュにデータを流し込む
	for (const auto& vertex : vertices) {
		mesh->AddVertex(vertex);
	}
	for (const auto index : indices) {
		mesh->AddIndex(index);
	}

	// デフォルトマテリアルを生成
	defaultMaterial_ = Material::Create();
	defaultMaterial_->name_ = "no material";
	defaultMaterial_->Update();
	// デフォルトマテリアルをセット
	mesh->SetMaterial(defaultMaterial_.get());

	// メッシュのバッファ生成
	for (auto& m : meshes_) {
		m->CreateBuffers();
	}

	// テクスチャの読み込み
	LoadTextures();
}

void Model2::LoadModel(const std::string& modelname, bool smoothing) {
	const string modelFileName = modelname + ".obj";
	const string directoryPath = kBaseDirectory + modelname + "/";

	// ファイルストリーム
	std::ifstream file;
	// .objファイルを開く
	file.open(directoryPath + modelFileName);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		auto message = std::format(
		    L"モデルデータファイル「{0}」"
		    "の読み込みに失敗しました。\n指定したパスが正しいか、必須リソースのコピー"
		    "を忘れていないか確認してください。",
		    ConvertStringMultiByteToWide(modelname));
		MessageBoxW(nullptr, message.c_str(), L"Not found .obj", 0);
		assert(false);
		exit(1);
	}

	name_ = modelname;

	// メッシュ生成
	meshes_.emplace_back(std::make_unique<Mesh>());
	Mesh* mesh = meshes_.back().get();
	uint16_t indexCountTex = 0;

	vector<Vector3> positions; // 頂点座標
	vector<Vector3> normals;   // 法線ベクトル
	vector<Vector2> texcoords; // テクスチャUV
	// 1行ずつ読み込む
	string line;
	while (getline(file, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		// マテリアル
		if (key == "mtllib") {
			// マテリアルのファイル名読み込み
			string materialFileName;
			line_stream >> materialFileName;
			// マテリアル読み込み
			LoadMaterial(directoryPath, materialFileName);
		}
		// 先頭文字列がgならグループの開始
		if (key == "g") {

			// カレントメッシュの情報が揃っているなら
			if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0) {
				// 頂点法線の平均によるエッジの平滑化
				if (smoothing) {
					mesh->CalculateSmoothedVertexNormals();
				}
				// 次のメッシュ生成
				meshes_.emplace_back(std::make_unique<Mesh>());
				mesh = meshes_.back().get();
				indexCountTex = 0;
			}

			// グループ名読み込み
			string groupName;
			line_stream >> groupName;

			// メッシュに名前をセット
			mesh->SetName(groupName);
		}
		// 先頭文字列がvなら頂点座標
		if (key == "v") {
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// X反転により、右手系のモデルデータを左手系に変換
			position.x = -position.x;
			positions.emplace_back(position);
		}
		// 先頭文字列がvtならテクスチャ
		if (key == "vt") {
			// U,V成分読み込み
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V方向反転
			texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		// 先頭文字列がvnなら法線ベクトル
		if (key == "vn") {
			// X,Y,Z成分読み込み
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}
		// 先頭文字列がusemtlならマテリアルを割り当てる
		if (key == "usemtl") {
			if (mesh->GetMaterial() == nullptr) {
				// マテリアルの名読み込み
				string materialName;
				line_stream >> materialName;

				// マテリアル名で検索し、マテリアルを割り当てる
				auto itr = materials_.find(materialName);
				if (itr != materials_.end()) {
					mesh->SetMaterial(itr->second.get());
				}
			}
		}
		// 先頭文字列がfならポリゴン（三角形）
		if (key == "f") {
			int faceIndexCount = 0;
			// 半角スペース区切りで行の続きを読み込む
			string index_string;
			std::array<uint16_t, 4> tempIndices;
			while (getline(line_stream, index_string, ' ')) {
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				uint32_t indexPosition, indexNormal, indexTexcoord;
				// 頂点番号
				index_stream >> indexPosition;

				Material* material = mesh->GetMaterial();
				index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
				// マテリアル、テクスチャがある場合
				if (material && material->textureFilename_.size() > 0) {
					index_stream >> indexTexcoord;
					index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
					index_stream >> indexNormal;
					// 頂点データの追加
					Mesh::VertexPosNormalUv vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					mesh->AddVertex(vertex);
					// エッジ平滑化用のデータを追加
					if (smoothing) {
						mesh->AddSmoothData(indexPosition, (uint32_t)mesh->GetVertexCount() - 1);
					}
				} else {
					char c;
					index_stream >> c;
					// スラッシュ2連続の場合、頂点番号のみ
					if (c == '/') {
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = {0, 0, 1};
						vertex.uv = {0, 0};
						mesh->AddVertex(vertex);
					} else {
						index_stream.seekg(-1, ios_base::cur); // 1文字戻る
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
						index_stream >> indexNormal;
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = {0, 0};
						mesh->AddVertex(vertex);
						// エッジ平滑化用のデータを追加
						if (smoothing) {
							mesh->AddSmoothData(indexPosition, (uint32_t)mesh->GetVertexCount() - 1);
						}
					}
				}

				assert(faceIndexCount < 4 && "5角形ポリゴン以上は非対応です");

				// インデックスデータの追加
				tempIndices[faceIndexCount] = indexCountTex;

				indexCountTex++;
				faceIndexCount++;
			}

			// インデックスデータの順序変更で時計回り→反時計回り変換
			mesh->AddIndex(tempIndices[0]);
			mesh->AddIndex(tempIndices[2]);
			mesh->AddIndex(tempIndices[1]);
			// 四角形なら三角形を追加
			if (faceIndexCount == 4) {
				mesh->AddIndex(tempIndices[0]);
				mesh->AddIndex(tempIndices[3]);
				mesh->AddIndex(tempIndices[2]);
			}
		}
	}
	file.close();

	// 頂点法線の平均によるエッジの平滑化
	if (smoothing) {
		mesh->CalculateSmoothedVertexNormals();
	}
}

void Model2::LoadMaterial(const std::string& directoryPath, const std::string& filename) {
	// ファイルストリーム
	std::ifstream file;
	// マテリアルファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		auto message = std::format(
		    L"マテリアルデータファイル「{0}」"
		    "の読み込みに失敗しました。\n指定したパスが正しいか、必須リソースのコピー"
		    "を忘れていないか確認してください。",
		    ConvertStringMultiByteToWide(filename));
		MessageBoxW(nullptr, message.c_str(), L"Not found .mtl", 0);
		assert(false);
		exit(1);
	}

	std::unique_ptr<Material> material;

	// 1行ずつ読み込む
	string line;
	while (getline(file, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		// 先頭のタブ文字は無視する
		if (key[0] == '\t') {
			key.erase(key.begin()); // 先頭の文字を削除
		}

		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl") {

			// 既にマテリアルがあれば
			if (material) {
				// マテリアルをコンテナに登録
				AddMaterial(material);
			}

			// 新しいマテリアルを生成
			material = Material::Create();
			// マテリアル名読み込み
			line_stream >> material->name_;
		}
		// 先頭文字列がKaならアンビエント色
		if (key == "Ka") {
			line_stream >> material->ambient_.x;
			line_stream >> material->ambient_.y;
			line_stream >> material->ambient_.z;
		}
		// 先頭文字列がKdならディフューズ色
		if (key == "Kd") {
			line_stream >> material->diffuse_.x;
			line_stream >> material->diffuse_.y;
			line_stream >> material->diffuse_.z;
		}
		// 先頭文字列がKsならスペキュラー色
		if (key == "Ks") {
			line_stream >> material->specular_.x;
			line_stream >> material->specular_.y;
			line_stream >> material->specular_.z;
		}
		// 先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd") {

			Vector3 uvw = {1, 1, 1};
			Vector3 offset = {0, 0, 0};

			std::string input;
			do {
				line_stream >> input;

				// スケール
				if (input == "-s") {
					line_stream >> material->uvScale_.x;
					line_stream >> material->uvScale_.y;
					line_stream >> material->uvScale_.z;
				}
				// 原点オフセット
				else if (input == "-o") {
					line_stream >> material->uvOffset_.x;
					line_stream >> material->uvOffset_.y;
					line_stream >> material->uvOffset_.z;
				} else {
					// テクスチャのファイル名読み込み
					material->textureFilename_ = input;
					break;
				}
			} while (true);

			// フルパスからファイル名を取り出す
			size_t pos1;
			pos1 = material->textureFilename_.rfind('\\');
			if (pos1 != string::npos) {
				material->textureFilename_ = material->textureFilename_.substr(pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}

			pos1 = material->textureFilename_.rfind('/');
			if (pos1 != string::npos) {
				material->textureFilename_ = material->textureFilename_.substr(pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}
		}
	}
	// ファイルを閉じる
	file.close();

	if (material) {
		// マテリアルを登録
		AddMaterial(material);
	}
}

void Model2::AddMaterial(std::unique_ptr<Material>& material) {
	// コンテナに登録
	materials_.emplace(material->name_, std::move(material));
}

void Model2::LoadTextures() {
	int textureIndex = 0;
	string directoryPath = name_ + "/";

	for (auto& m : materials_) {
		std::unique_ptr<Material>& material = m.second;

		// テクスチャあり
		if (material->textureFilename_.size() > 0) {
			// マテリアルにテクスチャ読み込み
			material->LoadTexture(directoryPath);
			textureIndex++;
		}
		// テクスチャなし
		else {
			// マテリアルにテクスチャ読み込み
			material->LoadTexture("");
			textureIndex++;
		}
	}
}

void Model2::Draw(const WorldTransform& worldTransform, const Camera& camera, const ObjectColor* objectColor) {

	ModelCommon2* common = ModelCommon2::GetInstance();

	// ライトコマンドを積む
	common->LightCommand(lightGroup_);

	// トランスフォームコマンドを積む
	common->TransformCommand(worldTransform, camera);

	// オブジェクトアルファのコマンドを積む
	const ObjectColor* useObjectColor = common->GetObjectColor();
	if (objectColor) {
		useObjectColor = objectColor;
	}
	useObjectColor->SetGraphicsCommand(common->GetCommandList(), (UINT)RoomParameter::kObjectColor);

	// 全メッシュを描画
	for (auto& mesh : meshes_) {
		mesh->Draw(common->GetCommandList(), (UINT)RoomParameter::kMaterial, (UINT)RoomParameter::kTexture);
	}
}

void Model2::Draw(const WorldTransform& worldTransform, const Camera& camera, uint32_t textureHadle, const ObjectColor* objectColor) {

	ModelCommon2* common = ModelCommon2::GetInstance();

	// ライトコマンドを積む
	common->LightCommand(lightGroup_);

	// トランスフォームコマンドを積む
	common->TransformCommand(worldTransform, camera);

	// オブジェクトアルファのコマンドを積む
	const ObjectColor* useObjectColor = common->GetObjectColor();
	if (objectColor) {
		useObjectColor = objectColor;
	}
	useObjectColor->SetGraphicsCommand(common->GetCommandList(), (UINT)RoomParameter::kObjectColor);

	// 全メッシュを描画
	for (auto& mesh : meshes_) {
		mesh->Draw(common->GetCommandList(), (UINT)RoomParameter::kMaterial, (UINT)RoomParameter::kTexture, textureHadle);
	}
}

void Model2::SetAlpha(float alpha) {

	for (auto& pair : materials_) {
		std::unique_ptr<Material>& material = pair.second;
		material->alpha_ = alpha;
		material->Update();
	}

	if (defaultMaterial_) {
		defaultMaterial_->alpha_ = alpha;
		defaultMaterial_->Update();
	}
}

ModelCommon2* ModelCommon2::GetInstance() {
	if (!ModelCommon2::sInstance_) {
		ModelCommon2::sInstance_ = new ModelCommon2();
	}
	return ModelCommon2::sInstance_;
}

void ModelCommon2::Terminate() {
	delete sInstance_;
	sInstance_ = nullptr;
}

void ModelCommon2::Initialize() {
	// パイプライン初期化
	InitializeGraphicsPipeline();

	// ライト生成
	defaultLightGroup_.reset(LightGroup::Create());

	// オブジェクトアルファ生成
	defaultObjectColor_ = std::make_unique<ObjectColor>();
	defaultObjectColor_->Initialize();
}

void ModelCommon2::LightCommand(const LightGroup* lightGroup) {
	// ライトコマンドを積む
	if (lightGroup) {
		lightGroup->Draw(commandList_, static_cast<UINT>(Model2::RoomParameter::kLight));
	} else {
		defaultLightGroup_->Draw(commandList_, static_cast<UINT>(Model2::RoomParameter::kLight));
	}
}

void ModelCommon2::TransformCommand(const WorldTransform& worldTransform, const Camera& camera) {
	// CBVをセット（ワールド行列）
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Model2::RoomParameter::kWorldTransform), worldTransform.GetConstBuffer()->GetGPUVirtualAddress());

	// CBVをセット（カメラ）
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Model2::RoomParameter::kCamera), camera.GetConstBuffer()->GetGPUVirtualAddress());
}

void ModelCommon2::PreDraw(ID3D12GraphicsCommandList* commandList) {
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(commandList_ == nullptr);

	// コマンドリストをセット
	commandList_ = commandList;

	// パイプラインステートの設定
	commandList->SetPipelineState(pipelineState_.Get());
	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelCommon2::PostDraw() {
	// コマンドリストを解除
	commandList_ = nullptr;
}

void ModelCommon2::InitializeGraphicsPipeline() {

	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;    // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
	    L"Resources/shaders/ObjVS.hlsl", // シェーダファイル名
	    nullptr,
	    D3D_COMPILE_STANDARD_FILE_INCLUDE,               // インクルード可能にする
	    "main", "vs_5_0",                                // エントリーポイント名、シェーダーモデル指定
	    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
	    0, &vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
	    L"Resources/shaders/ObjPS.hlsl", // シェーダファイル名
	    nullptr,
	    D3D_COMPILE_STANDARD_FILE_INCLUDE,               // インクルード可能にする
	    "main", "ps_5_0",                                // エントリーポイント名、シェーダーモデル指定
	    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
	    0, &psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	    {// xy座標(1行で書いたほうが見やすい)
	     "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	    {// 法線ベクトル(1行で書いたほうが見やすい)
	     "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	    {// uv座標(1行で書いたほうが見やすい)
	     "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//  デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;

	// 通常
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	// 加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	//減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//blenddesc.DestBlend = D3D12_BLEND_ONE

	// 共通設定
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;




	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;                            // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1;                            // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[6];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootSignature_.Get();

	// グラフィックスパイプラインの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));
}

} // namespace KamataEngine
