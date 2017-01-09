#include"Camera.h"
// 카메라의 위치&회전 상태를 원점으로 설정
Camera::Camera()
{
    _positionX = 0.0f;
    _positionY = 0.0f;
    _positionZ = 0.0f;

    _rotationX = 0.0f;
    _rotationY = 0.0f;
    _rotationZ = 0.0f;
}
Camera::Camera(const Camera& other)
{

}
Camera::~Camera()
{

}
void Camera::setPosition(float x, float y, float z)
{
    _positionX = x;
    _positionY = y;
    _positionZ = z;
    return;
}
void Camera::setRotation(float x, float y, float z)
{
    _rotationX = x;
    _rotationY = y;
    _rotationZ = z;
    return;
}
D3DXVECTOR3 Camera::getPosition()
{
    return D3DXVECTOR3(_positionX, _positionY, _positionZ);
}
D3DXVECTOR3 Camera::getRotation()
{
    return D3DXVECTOR3(_rotationX, _rotationY, _rotationZ);
}
/*****************************************************************************************************
카메라의 위치&회전 정보를 이용해 뷰 행렬을 갱신한다.
우선 up벡터와 위치, 회전 등의 변수를 초기화한다.
화면의 원점에서 우선 카메라의 xyz축 회전 정도로 카메라의 회전을 수행한다.
3D세계의 올바른 위치로 카메라를 옮긴다.
position, lookAt, up이 잘 설정되있다면 D3DXMatrixLookAtLH함수를 사용해
현재 카메라의 위치와 회전 정도를 표현하는 뷰 행렬을 만들어낸다.
*****************************************************************************************************/
void Camera::render()
{
    D3DXVECTOR3 up, position, lookAt;
    float yaw, pitch, roll;
    D3DXMATRIX rotationMatrix;

    // Setup the vector that points upwards.
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    // Setup the position of the camera in the world.
    position.x = _positionX;
    position.y = _positionY;
    position.z = _positionZ;

    // Setup where the camera is looking by default.
    lookAt.x = 0.0f;
    lookAt.y = 0.0f;
    lookAt.z = 1.0f;

    // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
    pitch = _rotationX * 0.0174532925f;
    yaw   = _rotationY * 0.0174532925f;
    roll  = _rotationZ * 0.0174532925f;

    // Create the rotation matrix from the yaw, pitch, and roll values.
    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
    D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
    D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

    // Translate the rotated camera position to the location of the viewer.
    lookAt = position + lookAt;

    // Finally create the view matrix from the three updated vectors.
    D3DXMatrixLookAtLH(&_viewMatrix, &position, &lookAt, &up);

    return;
}

/*****************************************************************************************************
뷰 행렬을 생성하는 render함수를 수행한 이후 getViewMatrix함수를 사용해 뷰 행렬을 얻어올 수 있게된다.
뷰 행렬은 HLSL정점 셰이더에서 사용하는 중요한 세 행렬 중 하나다.
*****************************************************************************************************/
void Camera::getViewMatrix(D3DXMATRIX& viewMatrix)
{
    viewMatrix = _viewMatrix;
    return;
}