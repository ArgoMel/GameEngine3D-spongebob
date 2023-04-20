#include "FBXLoader.h"

CFBXLoader::CFBXLoader()
	: m_Manager(nullptr)
	, m_Scene(nullptr)
	, m_Mixamo(false)
{
}

CFBXLoader::~CFBXLoader()
{
	size_t	size = m_vecMaterial.size();
	for (size_t i = 0; i < size; ++i)
	{
		size_t	size1 = m_vecMaterial[i].size();
		for (size_t j = 0; j < size1; ++j)
		{
			SAFE_DELETE(m_vecMaterial[i][j]);
		}
	}
	size = m_vecMeshContainer.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecMeshContainer[i]);
	}
	size = m_vecBone.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecBone[i]);
	}
	for (int i = 0; i < m_NameArray.Size(); ++i)
	{
		m_NameArray[i]->Clear();
		SAFE_DELETE(m_NameArray[i]);
	}
	size = m_vecClip.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecClip[i]);
	}
	if (m_Scene)
	{
		m_Scene->Clear();
		m_Scene->Destroy();
	}
	if (m_Manager)
	{
		m_Manager->Destroy();
	}
}

bool CFBXLoader::LoadFBX(const char* fullPath, bool isStatic)
{
	m_Manager = FbxManager::Create();
	FbxIOSettings* IOS = FbxIOSettings::Create(m_Manager, IOSROOT);
	m_Manager->SetIOSettings(IOS);
	FbxImporter* importer = FbxImporter::Create(m_Manager, "");
	bool status=importer->Initialize(fullPath, -1, m_Manager->GetIOSettings());
	m_Scene = FbxScene::Create(m_Manager, "");
	importer->Import(m_Scene);
	if (m_Scene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
	{
		m_Scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);
	}
	if (!isStatic)
	{
		m_Scene->FillAnimStackNameArray(m_NameArray);
		if (m_NameArray.GetCount() > 0)
		{
			LoadAnimationClip();
			LoadBone(m_Scene->GetRootNode());			// 본 정보를 읽어온다.
			// 클립이 가지고 있는 프레임을 본 개수만큼 resize 해준다.
			// 원래 Animation Clip에서 하던것이다.
			for (size_t i = 0; i < m_vecClip.size(); ++i)
			{
				m_vecClip[i]->vecBoneKeyFrame.resize(m_vecBone.size());
			}
		}
	}
	Triangulate(m_Scene->GetRootNode());
	LoadMesh(m_Scene->GetRootNode(), isStatic);
	importer->Destroy();
	return true;
}

void CFBXLoader::Triangulate(FbxNode* node)
{
	// FbxNodeAttribute : FbxNode의 속성을 나타낸다.
	FbxNodeAttribute* attr = node->GetNodeAttribute();
	if (attr && (attr->GetAttributeType() == FbxNodeAttribute::eMesh ||
				 attr->GetAttributeType() == FbxNodeAttribute::eNurbs ||
				 attr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter	converter(m_Manager);
		converter.Triangulate(attr, true);
	}
	int	mtrlCount = node->GetMaterialCount();	// 재질 정보를 읽는다.
	if (mtrlCount > 0)
	{
		std::vector<FbxMaterial*>	vecMtrl;
		m_vecMaterial.push_back(vecMtrl);
		for (int i = 0; i < mtrlCount; ++i)
		{
			FbxSurfaceMaterial* mtrl = node->GetMaterial(i);
			if (!mtrl)
			{
				continue;
			}
			LoadMaterial(mtrl);
		}
	}
	int	childCount = node->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		Triangulate(node->GetChild(i));
	}
}

void CFBXLoader::LoadMaterial(FbxSurfaceMaterial* mtrl)
{
	FbxMaterial* mtrlInfo = new FbxMaterial;	// 재질 정보를 저장할 구조체를 생성한다.
	// 가장 마지막 컨테이너에 재질 정보를 추가한다.
	m_vecMaterial[m_vecMaterial.size() - 1].push_back(mtrlInfo);

	mtrlInfo->Name=mtrl->GetName();

	// Diffuse 정보를 읽어온다.
	mtrlInfo->BaseColor = GetMaterialColor(mtrl, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	mtrlInfo->AmbientColor = GetMaterialColor(mtrl, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	mtrlInfo->SpecularColor = GetMaterialColor(mtrl, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	mtrlInfo->EmissiveColor = GetMaterialColor(mtrl, FbxSurfaceMaterial::sEmissive,	FbxSurfaceMaterial::sEmissiveFactor);
	mtrlInfo->SpecularPower = (float)GetMaterialFactor(mtrl, FbxSurfaceMaterial::sSpecularFactor);
	mtrlInfo->Shininess = (float)GetMaterialFactor(mtrl, FbxSurfaceMaterial::sShininess);
	mtrlInfo->TransparencyFactor = (float)GetMaterialFactor(mtrl, FbxSurfaceMaterial::sTransparencyFactor);
	mtrlInfo->DiffuseTexture = GetMaterialTexture(mtrl, FbxSurfaceMaterial::sDiffuse);
	mtrlInfo->BumpTexture = GetMaterialTexture(mtrl, FbxSurfaceMaterial::sNormalMap);
	if (mtrlInfo->BumpTexture.empty())
	{
		mtrlInfo->BumpTexture = GetMaterialTexture(mtrl, FbxSurfaceMaterial::sBump);
	}
	mtrlInfo->SpecularTexture = GetMaterialTexture(mtrl, FbxSurfaceMaterial::sSpecular);
}

Vector4 CFBXLoader::GetMaterialColor(FbxSurfaceMaterial* mtrl, const char* propertyName, const char* propertyFactorName)
{
	FbxDouble3	result(0, 0, 0);
	double		factor = 0;
	// 인자로 들어온 재질에서 해당 이름을 가진 프로퍼티가 있는지를 판단한다음 얻어온다. 
	//마찬가지로 해당 이름의 프로퍼티 팩터가 있는지를 판단한다음 얻어온다.
	FbxProperty	property = mtrl->FindProperty(propertyName);
	FbxProperty	propertyFactor = mtrl->FindProperty(propertyFactorName);
	// 유효한지 체크한다.
	if (property.IsValid() && propertyFactor.IsValid())
	{
		result = property.Get<FbxDouble3>();
		factor = propertyFactor.Get<FbxDouble>();
		// Factor가 1이 아닐 경우 일종의 비율값으로 사용하여 얻어온 vResult 에 모두 곱해준다.
		if (factor != 1)
		{
			result[0] *= factor;
			result[1] *= factor;
			result[2] *= factor;
		}
	}
	return Vector4((float)result[0], (float)result[1], (float)result[2], (float)factor);
}

double CFBXLoader::GetMaterialFactor(FbxSurfaceMaterial* mtrl, const char* propertyName)
{
	FbxProperty	property = mtrl->FindProperty(propertyName);
	double	factor = 0.0;
	if (property.IsValid())
	{
		factor = property.Get<FbxDouble>();
	}
	return factor;
}

std::string CFBXLoader::GetMaterialTexture(FbxSurfaceMaterial* mtrl, const char* propertyName)
{
	FbxProperty	property = mtrl->FindProperty(propertyName);
	std::string	str = "";
	if (property.IsValid())
	{
		// FbxFileTexture 이 타입이 몇개가 있는지를 얻어오는 함수이다.
		int	texCount = property.GetSrcObjectCount<FbxFileTexture>();
		if (texCount > 0)
		{
			FbxFileTexture* fileTex = property.GetSrcObject<FbxFileTexture>(0);
			if (fileTex)
			{
				str = fileTex->GetFileName();
			}
		}
	}
	return str;
}

void CFBXLoader::LoadMesh(FbxNode* node, bool isStatic)
{
	FbxNodeAttribute* attr = node->GetNodeAttribute();
	if (attr && attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* mesh = node->GetMesh();
		if (mesh)
		{
			LoadMesh(mesh, isStatic);
		}
	}
	int	childCount = node->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		LoadMesh(node->GetChild(i), isStatic);
	}
}

void CFBXLoader::LoadMesh(FbxMesh* mesh, bool isStatic)
{
	//std::vector<FbxLayerElementMaterial*>	 MaterialSet; // 매터리얼
	//std::vector<FbxLayerElementUV*>			 UVSet; // UV
	//std::vector<FbxLayerElementVertexColor*> VertexColorSet;//
	////// 노말맵을 위한 노말, 바이노말, 탄젠트
	//std::vector<FbxLayerElementNormal*>		 NormalSet;
	//std::vector<FbxLayerElementBinormal*>    BinormalSet;
	//std::vector<FbxLayerElementTangent*>     TangentSet;
// 레이어 ( 1번에 랜더링, 여러번에 걸쳐서 랜더링 개념)
	int iLayerCount = mesh->GetLayerCount();
	if (iLayerCount == 0 || mesh->GetLayer(0)->GetNormals() == nullptr)
	{
		mesh->InitNormals();
		mesh->GenerateNormals();
	}
	//노말 탄젠트 바이노말 없을때 생성해준다.
	if (iLayerCount == 0 || mesh->GetLayer(0)->GetTangents() == nullptr ||
		mesh->GetLayer(0)->GetBinormals() == nullptr)
	{
		mesh->InitTangents();
		mesh->InitBinormals();
		mesh->CreateElementBinormal();
		mesh->GenerateTangentsData(0, true, false);
	}
	//for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	//{
	//	FbxLayer* pFbxLayer = mesh->GetLayer(iLayer);// 레이어에 UV 정보가 있음 필수적임
	//	//매터리얼, UV, 버텍스 컬러
	//	if (pFbxLayer->GetMaterials() != nullptr)
	//	{
	//		MaterialSet.push_back(pFbxLayer->GetMaterials());
	//	}
	//	if (pFbxLayer->GetUVs() != nullptr)
	//	{
	//		UVSet.push_back(pFbxLayer->GetUVs());
	//	}
	//	if (pFbxLayer->GetVertexColors() != nullptr)
	//	{
	//		VertexColorSet.push_back(pFbxLayer->GetVertexColors());
	//	}
	//	//노말값
	//	if (pFbxLayer->GetNormals() != nullptr)
	//	{
	//		NormalSet.push_back(pFbxLayer->GetNormals());
	//	}
	//	if (pFbxLayer->GetBinormals() != nullptr)
	//	{
	//		BinormalSet.push_back(pFbxLayer->GetBinormals());
	//	}
	//	if (pFbxLayer->GetTangents() != nullptr)
	//	{
	//		TangentSet.push_back(pFbxLayer->GetTangents());
	//	}
	//}
//

	// MeshContainer를 만들고 컨테이너별 정점과 컨테이너&서브셋 별 인덱스 정보를 얻어와서 메쉬를 구성해야 한다.
	FbxMeshContainer* container = new FbxMeshContainer;
	m_vecMeshContainer.push_back(container);
	// ControlPoint 는 위치정보를 담고 있는 배열이다.
	// 이 배열의 개수는 곧 정점의 개수가 된다.
	int	vtxCount = mesh->GetControlPointsCount();
	// 내부적으로 FbxVector4타입의 배열로 저장하고 있기 때문에 배열의 시작주소를 얻어온다.
	FbxVector4* vtxPos = mesh->GetControlPoints();
	// 컨테이너가 가지고 있는 정점 정보들을 정점수만큼 resize 해준다.
	container->vecPos.resize(vtxCount);
	container->vecNormal.resize(vtxCount);
	container->vecUV.resize(vtxCount);
	container->vecTangent.resize(vtxCount);
	container->vecBinormal.resize(vtxCount);
	//container->vecColor.resize(vtxCount);
	for (int i = 0; i < vtxCount; ++i)
	{
		// y와 z축이 바뀌어 있기 때문에 변경해준다.
		container->vecPos[i].x = (float)vtxPos[i].mData[0];
		container->vecPos[i].y = (float)vtxPos[i].mData[2];
		container->vecPos[i].z = (float)vtxPos[i].mData[1];
	}
	int	polygonCount = mesh->GetPolygonCount();	// 폴리곤 수를 얻어온다.
	UINT	vtxID = 0;
	// 재질 수를 얻어온다.(재질 수 == 서브셋 수)
	int	mtrlCount = mesh->GetNode()->GetMaterialCount();
	mtrlCount = mtrlCount == 0 ? 1 : mtrlCount;
	// 재질 수는 곧 서브셋 수이기 때문에 재질 수만큼 resize 한다.
	container->vecIndex.resize(mtrlCount);
	// 재질 정보를 얻어온다.
	FbxGeometryElementMaterial* material = mesh->GetElementMaterial();
	int count = mesh->GetElementMaterialCount();
	// 삼각형 수만큼 반복한다.
	for (int i = 0; i < polygonCount; ++i)
	{
		// 이 폴리곤을 구성하는 정점의 수를 얻어온다.
		// 삼각형일 경우 3개를 얻어온다.
		int	polygonSize = mesh->GetPolygonSize(i);
		int	idx[3] = {};
		for (int j = 0; j < polygonSize; ++j)
		{
			// 현재 삼각형을 구성하고 있는 버텍스정보 내에서의 인덱스를 구한다.
			int	controlIndex = mesh->GetPolygonVertex(i, j);
			idx[j] = controlIndex;
			LoadNormal(mesh, container, vtxID, controlIndex);
			LoadUV(mesh, container, mesh->GetTextureUVIndex(i, j), controlIndex);
			LoadTangent(mesh, container, vtxID, controlIndex);
			LoadBinormal(mesh, container, vtxID, controlIndex);
			//LoadColor(mesh, container, vtxID, controlIndex);
			++vtxID;
		}
		// 삼각형이 소속된 서브셋 인덱스를 얻어온다.
		int	mtrlID = 0;
		if (material)
		{
			mtrlID = material->GetIndexArray().GetAt(i);
		}
		container->vecIndex[mtrlID].push_back(idx[0]);
		container->vecIndex[mtrlID].push_back(idx[2]);
		container->vecIndex[mtrlID].push_back(idx[1]);
	}
	if (!isStatic)
	{
		LoadAnimation(mesh, container);
	}
}

void CFBXLoader::LoadNormal(FbxMesh* mesh, FbxMeshContainer* container, int vtxID, int controlIndex)
{
	// 메쉬로부터 ElementNormal 정보를 얻어온다.
	FbxGeometryElementNormal* normal = mesh->GetElementNormal();
	int	normalIndex = vtxID;
	// MappingMode와 ReferenceMode에 따라서 인덱스로 사용할 정보가 달라진다.
	if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (normal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			normalIndex = normal->GetIndexArray().GetAt(vtxID);
			break;
		}
	}
	else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (normal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			normalIndex = controlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			normalIndex = normal->GetIndexArray().GetAt(controlIndex);
			break;
		}
	}
	FbxVector4	vNormal = normal->GetDirectArray().GetAt(normalIndex);
	container->vecNormal[controlIndex].x = (float)vNormal.mData[0];
	container->vecNormal[controlIndex].y = (float)vNormal.mData[2];
	container->vecNormal[controlIndex].z = (float)vNormal.mData[1];
}

void CFBXLoader::LoadUV(FbxMesh* mesh, FbxMeshContainer* container, int UVID, int controlIndex)
{
	int count = mesh->GetElementUVCount();	//디버깅할때 중단점 걸면 유용하다
	FbxGeometryElementUV* UV = mesh->GetElementUV(0);
	if (!UV)
	{
		return;
	}
	int	UVIndex = UVID;
	if (UV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (UV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			UVIndex = controlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			UVIndex = UV->GetIndexArray().GetAt(controlIndex);
			break;
		}
	}
	else if (UV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (UV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
			break;
		}
	}
	FbxVector2	vUV = UV->GetDirectArray().GetAt(UVIndex);
	container->vecUV[controlIndex].x = (float)vUV.mData[0] - (int)vUV.mData[0];
	container->vecUV[controlIndex].y = 1.f - (float)(vUV.mData[1] - (int)vUV.mData[1]);
}

void CFBXLoader::LoadTangent(FbxMesh* mesh, FbxMeshContainer* container, int vtxID, int controlIndex)
{
	// 메쉬로부터 ElementTangent 정보를 얻어온다.
	FbxGeometryElementTangent* tangent = mesh->GetElementTangent();
	if (!tangent)
	{
		return;
	}
	container->Bump = true;
	int	tangentIndex = vtxID;
	// MappingMode와 ReferenceMode에 따라서 인덱스로 사용할 정보가 달라진다.
	if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (tangent->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			tangentIndex = tangent->GetIndexArray().GetAt(vtxID);
			break;
		}
	}
	else if (tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (tangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			tangentIndex = controlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			tangentIndex = tangent->GetIndexArray().GetAt(controlIndex);
			break;
		}
	}
	FbxVector4	vTangent = tangent->GetDirectArray().GetAt(tangentIndex);
	container->vecTangent[controlIndex].x = (float)vTangent.mData[0];
	container->vecTangent[controlIndex].y = (float)vTangent.mData[2];
	container->vecTangent[controlIndex].z = (float)vTangent.mData[1];
}

void CFBXLoader::LoadBinormal(FbxMesh* mesh, FbxMeshContainer* container, int vtxID, int controlIndex)
{
	// 메쉬로부터 ElementBinormal 정보를 얻어온다.
	FbxGeometryElementBinormal* binormal = mesh->GetElementBinormal();
	if (!binormal)
	{
		return;
	}
	container->Bump = true;
	int	binormalIndex = vtxID;
	// MappingMode와 ReferenceMode에 따라서 인덱스로 사용할 정보가 달라진다.
	if (binormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (binormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			binormalIndex =binormal->GetIndexArray().GetAt(vtxID);
			break;
		}
	}
	else if (binormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (binormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			binormalIndex = controlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			binormalIndex = binormal->GetIndexArray().GetAt(controlIndex);
			break;
		}
	}
	FbxVector4	vBinormal = binormal->GetDirectArray().GetAt(binormalIndex);
	container->vecBinormal[controlIndex].x = (float)vBinormal.mData[0];
	container->vecBinormal[controlIndex].y = (float)vBinormal.mData[2];
	container->vecBinormal[controlIndex].z = (float)vBinormal.mData[1];
}

void CFBXLoader::LoadColor(FbxMesh* mesh, FbxMeshContainer* container, int vtxID, int controlIndex)
{
	int count = mesh->GetElementUVCount();	//디버깅할때 중단점 걸면 유용하다
	FbxGeometryElementVertexColor* color = mesh->GetElementVertexColor(0);
	if (!color)
	{
		return;
	}
	int	colorIndex = vtxID;
	if (color->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (color->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			colorIndex = controlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			colorIndex = color->GetIndexArray().GetAt(controlIndex);
			break;
		}
	}
	else if (color->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (color->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
			break;
		}
	}
	FbxColor vColor = color->GetDirectArray().GetAt(colorIndex);
	//container->vecColor[controlIndex].x = (float)vColor.mRed;
	//container->vecColor[controlIndex].y = (float)vColor.mGreen;
	//container->vecColor[controlIndex].z = (float)vColor.mBlue;
	//container->vecColor[controlIndex].w = (float)colorIndex;
}

void CFBXLoader::LoadAnimationClip()
{
	int	count = m_NameArray.GetCount();
	FbxTime::EMode	timeMode = m_Scene->GetGlobalSettings().GetTimeMode();
	for (int i = 0; i < count; ++i)
	{
		// m_NameArr에 저장된 이름으로 Scene으로부터 FbxAnimStack 객체를 얻어온다.
		FbxAnimStack* animStack = m_Scene->FindMember<FbxAnimStack>(m_NameArray[i]->Buffer());
		if (!animStack)
		{
			continue;
		}
		FbxAnimationClip* clip = new FbxAnimationClip;
		clip->Name = animStack->GetName();
//	mixamo 애니메이션 동작 받는 사이트
// mixamo에서 애니메이션을 만들면 새 애니메이션과 별도로 존재하는 기존애니메이션이 손상되고 이름이 아래처럼 변한다
		if (clip->Name == "mixamo.com")
		{
			m_Mixamo = true;
		}
		FbxTakeInfo* take = m_Scene->GetTakeInfo(clip->Name.c_str());	//	시간정보
		clip->StartTime = take->mLocalTimeSpan.GetStart();
		clip->EndTime = take->mLocalTimeSpan.GetStop();
		// GetFrameCount 함수를 호출하고  time모드를 넣어주면 시간을 프레임으로 변환해준다. 
		// 몇프레임 짜리 애니메이션 인지를 구해준다.
		clip->TimeLength = clip->EndTime.GetFrameCount(timeMode) - clip->StartTime.GetFrameCount(timeMode);
		clip->TimeMode = timeMode;
		m_vecClip.push_back(clip);
	}
}

void CFBXLoader::LoadBone(FbxNode* node)
{
	int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		LoadBoneRecursive(node->GetChild(i), 0, 0, -1);
	}
}

void CFBXLoader::LoadBoneRecursive(FbxNode* node, int depth, int index, int parent)
{
	FbxNodeAttribute* attr = node->GetNodeAttribute();
	if (attr && attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		FbxBone* bone = new FbxBone;
		bone->Name = node->GetName();
		if (m_Mixamo)
		{
			bone->Name.erase(0, 10);
		}
		bone->Depth = depth;
		bone->ParentIndex = parent;
		m_vecBone.push_back(bone);
	}
	int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		LoadBoneRecursive(node->GetChild(i), depth + 1, (int)m_vecBone.size(), index);
	}
}

void CFBXLoader::LoadAnimation(FbxMesh* mesh, FbxMeshContainer* container)
{
	int	skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);	// 스킨 꺼내오는 함수
	if (skinCount <= 0)
	{
		return;
	}
	int	CPCount = mesh->GetControlPointsCount();	// 메쉬의 정점 수를 얻어온다.
	// 정점의 가중치 정보와 본인덱스 정보는 정점 수만큼 만들어져야 한다.
	container->vecBlendWeight.resize(CPCount);
	container->vecBlendIndex.resize(CPCount);
	container->Animation = true;
	FbxAMatrix	matTransform = GetTransform(mesh->GetNode());
	for (int i = 0; i < skinCount; ++i)
	{
		FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(i, FbxDeformer::eSkin);
		if (!skin)
		{
			continue;
		}
		FbxSkin::EType	skinningType = skin->GetSkinningType();
		if (skinningType == FbxSkin::eRigid ||
			skinningType == FbxSkin::eLinear ||
			skinningType == FbxSkin::eBlend)
		{

			int	clusterCount = skin->GetClusterCount();		// Cluster : 관절을 의미한다.
			for (int j = 0; j < clusterCount; ++j)
			{
				FbxCluster* cluster = skin->GetCluster(j);
				if (!cluster->GetLink())
				{
					continue;
				}
				std::string	boneName = cluster->GetLink()->GetName();
				if (m_Mixamo)
				{
					boneName.erase(0, 10);
				}
				int	boneIndex = FindBoneFromName(boneName);
				LoadWeightAndIndex(cluster, boneIndex, container);
				LoadOffsetMatrix(cluster, matTransform, boneIndex, container);
				m_vecBone[boneIndex]->matBone = matTransform;
				LoadTimeTransform(mesh->GetNode(), cluster, matTransform, boneIndex);
			}
		}
	}
	ChangeWeightAndIndices(container);
}

FbxAMatrix CFBXLoader::GetTransform(FbxNode* node)
{
	const FbxVector4	vT = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4	vR = node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4	vS = node->GetGeometricScaling(FbxNode::eSourcePivot);
	return FbxAMatrix(vT, vR, vS);
}

int CFBXLoader::FindBoneFromName(const std::string& name)
{
	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		if (m_vecBone[i]->Name == name)
		{
			return (int)i;
		}
	}
	return -1;
}

void CFBXLoader::LoadWeightAndIndex(FbxCluster* cluster, int boneIndex, FbxMeshContainer* container)
{
	int	controlIndicesCount = cluster->GetControlPointIndicesCount();
	for (int i = 0; i < controlIndicesCount; ++i)
	{
		FbxWeight	weight = {};
		weight.Index = boneIndex;
		weight.Weight = cluster->GetControlPointWeights()[i];
		int	clusterIndex = cluster->GetControlPointIndices()[i];
		// map의 특징 : 키를 이용해 인덱스처럼 접근할 경우 해당 키가 없다면 만들어준다.
		container->mapWeights[clusterIndex].push_back(weight);
	}
}

void CFBXLoader::LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matTransform, int boneIndex, 
	FbxMeshContainer* container)
{
	FbxAMatrix	matCluster;
	FbxAMatrix	matClusterLink;
	cluster->GetTransformMatrix(matCluster);
	cluster->GetTransformLinkMatrix(matClusterLink);

	FbxVector4	v1 = { 1.0, 0.0, 0.0, 0.0 };
	FbxVector4	v2 = { 0.0, 0.0, 1.0, 0.0 };
	FbxVector4	v3 = { 0.0, 1.0, 0.0, 0.0 };
	FbxVector4	v4 = { 0.0, 0.0, 0.0, 1.0 };

	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;
// 기존행렬의 가로와 세로를 2번 뒤집음(좌표계가 달라서)
	/*
	1 0 0 0   1 2 3 4   1 0 0 0
	0 0 1 0   5 6 7 8   0 0 1 0
	0 1 0 0 * 9 0 1 2 * 0 1 0 0
	0 0 0 1   3 4 5 6   0 0 0 1

	1 2 3 4   1 0 0 0
	9 0 1 2   0 0 1 0
	5 6 7 8 * 0 1 0 0
	3 4 5 6   0 0 0 1

	1 3 2 4
	9 1 0 2
	5 7 6 8
	3 5 4 6
	*/

	FbxAMatrix	matOffset;
	matOffset = matClusterLink.Inverse() * matCluster * matTransform;
	matOffset = matReflect * matOffset * matReflect;
	m_vecBone[boneIndex]->matOffset = matOffset;
}

void CFBXLoader::LoadTimeTransform(FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matTransform, int boneIndex)
{
	FbxVector4	v1 = { 1.0, 0.0, 0.0, 0.0 };
	FbxVector4	v2 = { 0.0, 0.0, 1.0, 0.0 };
	FbxVector4	v3 = { 0.0, 1.0, 0.0, 0.0 };
	FbxVector4	v4 = { 0.0, 0.0, 0.0, 1.0 };

	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	if (m_Mixamo)
	{
		std::vector<FbxAnimationClip*>::iterator	iter;
		std::vector<FbxAnimationClip*>::iterator	iterEnd = m_vecClip.end();
		for (iter = m_vecClip.begin(); iter != iterEnd;)
		{
			if ((*iter)->Name != "mixamo.com")
			{
				SAFE_DELETE((*iter));
				iter = m_vecClip.erase(iter);
				iterEnd = m_vecClip.end();
			}
			else
			{
				++iter;
			}
		}
	}
	for (size_t i = 0; i < m_vecClip.size(); ++i)
	{
		FbxLongLong	start = m_vecClip[i]->StartTime.GetFrameCount(m_vecClip[i]->TimeMode);
		FbxLongLong	end = m_vecClip[i]->EndTime.GetFrameCount(m_vecClip[i]->TimeMode);
		m_vecClip[i]->vecBoneKeyFrame[boneIndex].BoneIndex = boneIndex;
		// 전체 프레임 수만큼 반복한다.
		for (FbxLongLong j = start; j <= end; ++j)
		{
			FbxTime	time = {};
			// 현재 프레임에 해당하는 FbxTime을 만들어낸다.
			time.SetFrame(j, m_vecClip[i]->TimeMode);
			// EvaluateGlobalTransform
			FbxAMatrix	matOffset = node->EvaluateGlobalTransform(time) * matTransform;
			FbxAMatrix	matCur = matOffset.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(time);
			matCur = matReflect * matCur * matReflect;
			FbxKeyFrame	keyFrame = {};
			keyFrame.Time = time.GetSecondDouble();
			keyFrame.matTransform = matCur;
			m_vecClip[i]->vecBoneKeyFrame[boneIndex].vecKeyFrame.push_back(keyFrame);
		}
	}
}

void CFBXLoader::ChangeWeightAndIndices(FbxMeshContainer* container)
{
	std::unordered_map<int, std::vector<FbxWeight>>::iterator	iter;
	std::unordered_map<int, std::vector<FbxWeight>>::iterator	iterEnd = container->mapWeights.end();
	for (iter = container->mapWeights.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second.size() > 4)
		{
			// 가중치 값에 따라 내림차순 정렬한다.
			//	람다표현식
			sort(iter->second.begin(), iter->second.end(), [](const FbxWeight& lhs, const FbxWeight& rhs)
				{
					return lhs.Weight > rhs.Weight;
				});

			double	sum = 0.0;
			for (int i = 0; i < 4; ++i)
			{
				sum += iter->second[i].Weight;
			}
			double	interpolate = 1.f - sum;
			std::vector<FbxWeight>::iterator	iterErase = iter->second.begin() + 4;
			iter->second.erase(iterErase, iter->second.end());
			iter->second[0].Weight += interpolate;
		}
		Vector4	vWeight;
		Vector4	vIndex;
		for (int i = 0; i < iter->second.size(); ++i)
		{
			vWeight[i] = (float)iter->second[i].Weight;
			vIndex[i] = (float)iter->second[i].Index;
		}
		container->vecBlendWeight[iter->first] = vWeight;
		container->vecBlendIndex[iter->first] = vIndex;
	}
}
