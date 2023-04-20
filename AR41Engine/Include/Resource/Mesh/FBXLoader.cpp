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
			LoadBone(m_Scene->GetRootNode());			// �� ������ �о�´�.
			// Ŭ���� ������ �ִ� �������� �� ������ŭ resize ���ش�.
			// ���� Animation Clip���� �ϴ����̴�.
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
	// FbxNodeAttribute : FbxNode�� �Ӽ��� ��Ÿ����.
	FbxNodeAttribute* attr = node->GetNodeAttribute();
	if (attr && (attr->GetAttributeType() == FbxNodeAttribute::eMesh ||
				 attr->GetAttributeType() == FbxNodeAttribute::eNurbs ||
				 attr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter	converter(m_Manager);
		converter.Triangulate(attr, true);
	}
	int	mtrlCount = node->GetMaterialCount();	// ���� ������ �д´�.
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
	FbxMaterial* mtrlInfo = new FbxMaterial;	// ���� ������ ������ ����ü�� �����Ѵ�.
	// ���� ������ �����̳ʿ� ���� ������ �߰��Ѵ�.
	m_vecMaterial[m_vecMaterial.size() - 1].push_back(mtrlInfo);

	mtrlInfo->Name=mtrl->GetName();

	// Diffuse ������ �о�´�.
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
	// ���ڷ� ���� �������� �ش� �̸��� ���� ������Ƽ�� �ִ����� �Ǵ��Ѵ��� ���´�. 
	//���������� �ش� �̸��� ������Ƽ ���Ͱ� �ִ����� �Ǵ��Ѵ��� ���´�.
	FbxProperty	property = mtrl->FindProperty(propertyName);
	FbxProperty	propertyFactor = mtrl->FindProperty(propertyFactorName);
	// ��ȿ���� üũ�Ѵ�.
	if (property.IsValid() && propertyFactor.IsValid())
	{
		result = property.Get<FbxDouble3>();
		factor = propertyFactor.Get<FbxDouble>();
		// Factor�� 1�� �ƴ� ��� ������ ���������� ����Ͽ� ���� vResult �� ��� �����ش�.
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
		// FbxFileTexture �� Ÿ���� ��� �ִ����� ������ �Լ��̴�.
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
	//std::vector<FbxLayerElementMaterial*>	 MaterialSet; // ���͸���
	//std::vector<FbxLayerElementUV*>			 UVSet; // UV
	//std::vector<FbxLayerElementVertexColor*> VertexColorSet;//
	////// �븻���� ���� �븻, ���̳븻, ź��Ʈ
	//std::vector<FbxLayerElementNormal*>		 NormalSet;
	//std::vector<FbxLayerElementBinormal*>    BinormalSet;
	//std::vector<FbxLayerElementTangent*>     TangentSet;
// ���̾� ( 1���� ������, �������� ���ļ� ������ ����)
	int iLayerCount = mesh->GetLayerCount();
	if (iLayerCount == 0 || mesh->GetLayer(0)->GetNormals() == nullptr)
	{
		mesh->InitNormals();
		mesh->GenerateNormals();
	}
	//�븻 ź��Ʈ ���̳븻 ������ �������ش�.
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
	//	FbxLayer* pFbxLayer = mesh->GetLayer(iLayer);// ���̾ UV ������ ���� �ʼ�����
	//	//���͸���, UV, ���ؽ� �÷�
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
	//	//�븻��
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

	// MeshContainer�� ����� �����̳ʺ� ������ �����̳�&����� �� �ε��� ������ ���ͼ� �޽��� �����ؾ� �Ѵ�.
	FbxMeshContainer* container = new FbxMeshContainer;
	m_vecMeshContainer.push_back(container);
	// ControlPoint �� ��ġ������ ��� �ִ� �迭�̴�.
	// �� �迭�� ������ �� ������ ������ �ȴ�.
	int	vtxCount = mesh->GetControlPointsCount();
	// ���������� FbxVector4Ÿ���� �迭�� �����ϰ� �ֱ� ������ �迭�� �����ּҸ� ���´�.
	FbxVector4* vtxPos = mesh->GetControlPoints();
	// �����̳ʰ� ������ �ִ� ���� �������� ��������ŭ resize ���ش�.
	container->vecPos.resize(vtxCount);
	container->vecNormal.resize(vtxCount);
	container->vecUV.resize(vtxCount);
	container->vecTangent.resize(vtxCount);
	container->vecBinormal.resize(vtxCount);
	//container->vecColor.resize(vtxCount);
	for (int i = 0; i < vtxCount; ++i)
	{
		// y�� z���� �ٲ�� �ֱ� ������ �������ش�.
		container->vecPos[i].x = (float)vtxPos[i].mData[0];
		container->vecPos[i].y = (float)vtxPos[i].mData[2];
		container->vecPos[i].z = (float)vtxPos[i].mData[1];
	}
	int	polygonCount = mesh->GetPolygonCount();	// ������ ���� ���´�.
	UINT	vtxID = 0;
	// ���� ���� ���´�.(���� �� == ����� ��)
	int	mtrlCount = mesh->GetNode()->GetMaterialCount();
	mtrlCount = mtrlCount == 0 ? 1 : mtrlCount;
	// ���� ���� �� ����� ���̱� ������ ���� ����ŭ resize �Ѵ�.
	container->vecIndex.resize(mtrlCount);
	// ���� ������ ���´�.
	FbxGeometryElementMaterial* material = mesh->GetElementMaterial();
	int count = mesh->GetElementMaterialCount();
	// �ﰢ�� ����ŭ �ݺ��Ѵ�.
	for (int i = 0; i < polygonCount; ++i)
	{
		// �� �������� �����ϴ� ������ ���� ���´�.
		// �ﰢ���� ��� 3���� ���´�.
		int	polygonSize = mesh->GetPolygonSize(i);
		int	idx[3] = {};
		for (int j = 0; j < polygonSize; ++j)
		{
			// ���� �ﰢ���� �����ϰ� �ִ� ���ؽ����� �������� �ε����� ���Ѵ�.
			int	controlIndex = mesh->GetPolygonVertex(i, j);
			idx[j] = controlIndex;
			LoadNormal(mesh, container, vtxID, controlIndex);
			LoadUV(mesh, container, mesh->GetTextureUVIndex(i, j), controlIndex);
			LoadTangent(mesh, container, vtxID, controlIndex);
			LoadBinormal(mesh, container, vtxID, controlIndex);
			//LoadColor(mesh, container, vtxID, controlIndex);
			++vtxID;
		}
		// �ﰢ���� �Ҽӵ� ����� �ε����� ���´�.
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
	// �޽��κ��� ElementNormal ������ ���´�.
	FbxGeometryElementNormal* normal = mesh->GetElementNormal();
	int	normalIndex = vtxID;
	// MappingMode�� ReferenceMode�� ���� �ε����� ����� ������ �޶�����.
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
	int count = mesh->GetElementUVCount();	//������Ҷ� �ߴ��� �ɸ� �����ϴ�
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
	// �޽��κ��� ElementTangent ������ ���´�.
	FbxGeometryElementTangent* tangent = mesh->GetElementTangent();
	if (!tangent)
	{
		return;
	}
	container->Bump = true;
	int	tangentIndex = vtxID;
	// MappingMode�� ReferenceMode�� ���� �ε����� ����� ������ �޶�����.
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
	// �޽��κ��� ElementBinormal ������ ���´�.
	FbxGeometryElementBinormal* binormal = mesh->GetElementBinormal();
	if (!binormal)
	{
		return;
	}
	container->Bump = true;
	int	binormalIndex = vtxID;
	// MappingMode�� ReferenceMode�� ���� �ε����� ����� ������ �޶�����.
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
	int count = mesh->GetElementUVCount();	//������Ҷ� �ߴ��� �ɸ� �����ϴ�
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
		// m_NameArr�� ����� �̸����� Scene���κ��� FbxAnimStack ��ü�� ���´�.
		FbxAnimStack* animStack = m_Scene->FindMember<FbxAnimStack>(m_NameArray[i]->Buffer());
		if (!animStack)
		{
			continue;
		}
		FbxAnimationClip* clip = new FbxAnimationClip;
		clip->Name = animStack->GetName();
//	mixamo �ִϸ��̼� ���� �޴� ����Ʈ
// mixamo���� �ִϸ��̼��� ����� �� �ִϸ��̼ǰ� ������ �����ϴ� �����ִϸ��̼��� �ջ�ǰ� �̸��� �Ʒ�ó�� ���Ѵ�
		if (clip->Name == "mixamo.com")
		{
			m_Mixamo = true;
		}
		FbxTakeInfo* take = m_Scene->GetTakeInfo(clip->Name.c_str());	//	�ð�����
		clip->StartTime = take->mLocalTimeSpan.GetStart();
		clip->EndTime = take->mLocalTimeSpan.GetStop();
		// GetFrameCount �Լ��� ȣ���ϰ�  time��带 �־��ָ� �ð��� ���������� ��ȯ���ش�. 
		// �������� ¥�� �ִϸ��̼� ������ �����ش�.
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
	int	skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);	// ��Ų �������� �Լ�
	if (skinCount <= 0)
	{
		return;
	}
	int	CPCount = mesh->GetControlPointsCount();	// �޽��� ���� ���� ���´�.
	// ������ ����ġ ������ ���ε��� ������ ���� ����ŭ ��������� �Ѵ�.
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

			int	clusterCount = skin->GetClusterCount();		// Cluster : ������ �ǹ��Ѵ�.
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
		// map�� Ư¡ : Ű�� �̿��� �ε���ó�� ������ ��� �ش� Ű�� ���ٸ� ������ش�.
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
// ��������� ���ο� ���θ� 2�� ������(��ǥ�谡 �޶�)
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
		// ��ü ������ ����ŭ �ݺ��Ѵ�.
		for (FbxLongLong j = start; j <= end; ++j)
		{
			FbxTime	time = {};
			// ���� �����ӿ� �ش��ϴ� FbxTime�� ������.
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
			// ����ġ ���� ���� �������� �����Ѵ�.
			//	����ǥ����
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
