#include "EditorGizmo.h"
#include "../Scene/SceneManager.h"
#include "../Component/CameraComponent.h"

CEditorGizmo::CEditorGizmo()
    : m_UseWindow(true)
    , m_GizmoCount(1)
    , m_LastUsing(0)
    , m_CamDistance(8.f)
{
}

CEditorGizmo::~CEditorGizmo()
{
}

bool CEditorGizmo::Init()
{
    return true;
}

void CEditorGizmo::Render()
{
	CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
	float cameraView[16];
	float cameraProjection[16];
	Matrix view = camera->GetViewMatrix();
	Matrix proj = camera->GetProjMatrix();
	for (int i = 0; i < 16; ++i)
	{
		if (i % 4 == 0)
		{
			cameraView[i] = view.v[i / 4].x;
			cameraProjection[i] = proj.v[i / 4].x;
		}
		else if (i % 4 == 1)
		{
			cameraView[i] = view.v[i / 4].y;
			cameraProjection[i] = proj.v[i / 4].y;
		}
		else if (i % 4 == 2)
		{
			cameraView[i] = view.v[i / 4].z;
			cameraProjection[i] = proj.v[i / 4].z;
		}
		else if (i % 4 == 3)
		{
			cameraView[i] = view.v[i / 4].w;
			cameraProjection[i] = proj.v[i / 4].w;
		}
	}
    for (int matId = 0; matId < m_GizmoCount; matId++)
    {
        ImGuizmo::SetID(matId);
        EditTransform(cameraView, cameraProjection, objectMatrix[matId], m_LastUsing == matId);
        if (ImGuizmo::IsUsing())
        {
			m_LastUsing = matId;
        }
    }
}

void CEditorGizmo::EditTransform(float* cameraView, float* cameraProjection, float* matrix,	bool editTransformDecomposition)
{
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;
	if (editTransformDecomposition)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_T))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_E))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
			mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation);
		ImGui::InputFloat3("Rt", matrixRotation);
		ImGui::InputFloat3("Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_S))
			useSnap = !useSnap;
		ImGui::Checkbox("##UseSnap", &useSnap);
		ImGui::SameLine();

		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}
		ImGui::Checkbox("Bound Sizing", &boundSizing);
		if (boundSizing)
		{
			ImGui::PushID(3);
			ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
			ImGui::SameLine();
			ImGui::InputFloat3("Snap", boundsSnap);
			ImGui::PopID();
		}
	}

	ImGuiIO& io = ImGui::GetIO();
	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;
	static ImGuiWindowFlags gizmoWindowFlags = 0;
	if (m_UseWindow)
	{
		ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
		ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
		viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
		viewManipulateTop = ImGui::GetWindowPos().y;
		gizmoWindowFlags=ImGui::SetCurrentWindow();
	}
	else
	{
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	}

	ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
	ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], m_GizmoCount);
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap
		? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

	ImGuizmo::ViewManipulate(cameraView, m_CamDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop),
		ImVec2(128, 128), 0x10101010);

	if (m_UseWindow)
	{
		ImGui::End();
		ImGui::PopStyleColor(1);
	}
}