#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "BasicIncludes.h"

namespace NTEngine
{
	class Camera
	{
	public:
		DLLEXPORT Camera();
		DLLEXPORT ~Camera();

		//get camera position
		DLLEXPORT inline XMFLOAT3 GetPosition()const;
		DLLEXPORT inline XMVECTOR GetPositionXM()const;

		//set camera position
		DLLEXPORT inline void SetPosition(float x, float y, float z);
		DLLEXPORT inline void SetPosition(const XMFLOAT3& pos);

		//get basis vectors
		DLLEXPORT inline XMFLOAT3 GetRightVector()const;
		DLLEXPORT inline XMVECTOR GetRightVectorXM()const;

		DLLEXPORT inline XMFLOAT3 GetUpVector()const;
		DLLEXPORT inline XMVECTOR GetUpVectorXM()const;

		DLLEXPORT inline XMFLOAT3 GetForwardVector()const;
		DLLEXPORT inline XMVECTOR GetForwardVectorXM()const;

		//get matrices
		DLLEXPORT inline XMMATRIX GetViewXM()const;
		DLLEXPORT inline XMMATRIX GetProjXM()const;
		DLLEXPORT inline XMMATRIX GetViewProjXM()const;
		


		// Get frustum properties.
		DLLEXPORT inline float GetNearZ()const;
		DLLEXPORT inline float GetFarZ()const;
		DLLEXPORT inline float GetAspect()const;
		DLLEXPORT inline float GetFovY()const;
		DLLEXPORT inline float GetFovX()const;

		// Get near and far plane dimensions in view space coordinates.
		DLLEXPORT inline float GetNearWindowWidth()const;
		DLLEXPORT inline float GetNearWindowHeight()const;

		DLLEXPORT inline float GetFarWindowWidth()const;
		DLLEXPORT inline float GetFarWindowHeight()const;

		//set frustum properties
		//**this needs to be re-called when the screen is resized or resolution is changed**
		DLLEXPORT inline void SetLens(float FovY, float aspectRatio, float zn, float zf);

		//look at function
		//void LookAt(FXMVECTOR Position, FXMVECTOR target, FXMVECTOR WorldUp);
		//void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

		// Strafe/Walk the camera a distance d.
		DLLEXPORT inline void Strafe(float d);
		DLLEXPORT inline void Walk(float d);

		// Rotate the camera.
		DLLEXPORT inline void Pitch(float radians);
		DLLEXPORT inline void RotateY(float radians);
		DLLEXPORT inline void Roll(float radians);

		// After modifying camera position/orientation, call to rebuild the view matrix.
		// no need to call if the camera's position or orientation remains unchanged.
		DLLEXPORT void UpdateViewMatrix();

	private:
		XMFLOAT3 m_Pos;    //position of our camera
		XMFLOAT3 m_Right; //Right vector
		XMFLOAT3 m_Up; //Up vector. Usually it is (0, 1, 0)
		XMFLOAT3 m_forward; //forward or look vector


		//frustum properties

		float m_NearZ; //near plane distance
		float m_FarZ; //far plane distance
		float m_Aspect; //aspect ratio
		float m_FovY; //vertical Fov
		float m_NearWindowHeight;
		float m_FarWindowHeight;


		XMFLOAT4X4 m_View; //view matrix
		XMFLOAT4X4 m_Proj; //projection matrix

	};




}


#endif //_CAMERA_H_