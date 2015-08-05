#include "Camera.h"
#include "Math.h"

using namespace DirectX;


namespace NTEngine
{
	Camera::Camera()
		:
		m_Pos(0.0f, 0.0f, 0.0f),
		m_Right(1.0f, 0.0f, 0.0f),
		m_Up(0.0f, 1.0f, 0.0f),
		m_forward(0.0f, 0.0f, 1.0f) //look at vector
	{
		SetLens(0.25f * Math::PI /*45 degree*/, 1.0f, 2.0f, 500.0f);
	}


	Camera::~Camera()
	{

	}

	void Camera::SetPosition(float x, float y, float z)
	{
		m_Pos.x = x;
		m_Pos.y = y;
		m_Pos.z = z;
	}

	void Camera::SetPosition(const XMFLOAT3& p)
	{
		m_Pos = p;
	}

	XMFLOAT3 Camera::GetPosition()const
	{
		return m_Pos;
	}

	XMVECTOR Camera::GetPositionXM()const
	{
		return XMLoadFloat3(&m_Pos);
	}

	XMFLOAT3 Camera::GetRightVector()const
	{
		return m_Right;
	}

	XMVECTOR Camera::GetRightVectorXM()const
	{
		return XMLoadFloat3(&m_Right);
	}

	XMFLOAT3 Camera::GetUpVector()const
	{
		return m_Up;
	}

	XMVECTOR Camera::GetUpVectorXM()const
	{
		return XMLoadFloat3(&m_Up);
	}

	XMFLOAT3 Camera::GetForwardVector()const
	{
		return m_forward;
	}

	XMVECTOR Camera::GetForwardVectorXM()const
	{
		return XMLoadFloat3(&m_forward);
	}

	float Camera::GetNearZ()const
	{
		return m_NearZ;
	}

	float Camera::GetFarZ()const
	{
		return m_FarZ;
	}

	float Camera::GetAspect()const
	{
		return m_Aspect;
	}

	float Camera::GetFovY()const
	{
		return m_FovY;
	}

	float Camera::GetFovX()const
	{
		float halfWidth = 0.5f*GetNearWindowWidth();
		return 2.0f*atan(halfWidth / m_NearZ);
	}

	float Camera::GetNearWindowWidth()const
	{
		return m_Aspect * m_NearWindowHeight;
	}

	float Camera::GetNearWindowHeight()const
	{
		return m_NearWindowHeight;
	}

	float Camera::GetFarWindowHeight()const
	{
		return m_FarWindowHeight;
	}

	float Camera::GetFarWindowWidth()const
	{
		return m_Aspect * m_FarWindowHeight;
	}

	XMMATRIX Camera::GetViewXM()const
	{
		return XMLoadFloat4x4(&m_View);
	}

	XMMATRIX Camera::GetProjXM()const
	{
		return XMLoadFloat4x4(&m_Proj);
	}

	XMMATRIX Camera::GetViewProjXM()const
	{
		return GetViewXM() * GetProjXM();
	}

	void Camera::Strafe(float d)
	{
		// Position += d * Right vector

		XMVECTOR r = XMLoadFloat3(&m_Right); //right vector
		XMVECTOR p = XMLoadFloat3(&m_Pos); //position vector

		p += d * r;

		XMStoreFloat3(&m_Pos, p);
	}

	void Camera::Walk(float d)
	{
		//Position += d * forward vector

		XMVECTOR f = XMLoadFloat3(&m_forward); //forward vector 
		XMVECTOR p = XMLoadFloat3(&m_Pos); //position vector

		p += d * f;

		XMStoreFloat3(&m_Pos, p);
	}

	void Camera::Pitch(float radians)
	{
		// Rotate up and forward vector about the right vector.
		//here, the right vector remains unchanged

		XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_Right), radians);

		XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
		XMStoreFloat3(&m_forward, XMVector3TransformNormal(XMLoadFloat3(&m_forward), R));
	}

	void Camera::Roll(float radians)
	{
		//rotate up and right vector around the forward vector
		//here, the forward vector remains unchanged

		XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_forward), radians);

		XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
		XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));
	}

	void Camera::RotateY(float radians)
	{
		//rotate the basis vectors about y axis
		//here, the y axis of the camera or the up basis vector remains unchanged

		XMMATRIX R = XMMatrixRotationY(radians);

		XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));

		//we can ignore this
		//XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));

		XMStoreFloat3(&m_forward, XMVector3TransformNormal(XMLoadFloat3(&m_forward), R));
	}

	void Camera::SetLens(float FovY, float AspectRatio, float zn, float zf)
	{
		//store a copy of these parameters because we'll need them later
		m_FovY = FovY;
		m_Aspect = AspectRatio;
		m_NearZ = zn;
		m_FarZ = zf;

		m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
		m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);

		XMMATRIX P = XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ);
		XMStoreFloat4x4(&m_Proj, P);
	}

	void Camera::UpdateViewMatrix()
	{
		//our camera's basis vectors
		XMVECTOR R = XMLoadFloat3(&m_Right); //right vector
		XMVECTOR U = XMLoadFloat3(&m_Up); //up vector
		XMVECTOR F = XMLoadFloat3(&m_forward); //forward vector

		//position vector of our camera
		XMVECTOR P = XMLoadFloat3(&m_Pos);

		//normalize the forward vector
		F = XMVector3Normalize(F);

		//derive the up vector (we need to do that in order to get "corrected" up vector) and normalize it
		U = XMVector3Cross(F, R); //forward vector (cross) right vector = up vector
		U = XMVector3Normalize(U); //normalize the vector because it may not be of unit length

		//derive the right vector (we need to do that in order to get "corrected" right vector)
		//no need to normalize because U & F are already normalized
		R = XMVector3Cross(U, F);

		//we are directly computing the view matrix
		//i.e the matrix we get is transformation from view space to world space but we want inverse
		//so, we're directly computing inverse matrix of that matrix which is view matrix

		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, F));

		XMStoreFloat3(&m_Right, R);
		XMStoreFloat3(&m_Up, U);
		XMStoreFloat3(&m_forward, F);

		m_View(0, 0) = m_Right.x;
		m_View(1, 0) = m_Right.y;
		m_View(2, 0) = m_Right.z;
		m_View(3, 0) = x;

		m_View(0, 1) = m_Up.x;
		m_View(1, 1) = m_Up.y;
		m_View(2, 1) = m_Up.z;
		m_View(3, 1) = y;

		m_View(0, 2) = m_forward.x;
		m_View(1, 2) = m_forward.y;
		m_View(2, 2) = m_forward.z;
		m_View(3, 2) = z;

		m_View(0, 3) = 0.0f;
		m_View(1, 3) = 0.0f;
		m_View(2, 3) = 0.0f;
		m_View(3, 3) = 1.0f;

	}


}