#pragma once
typedef class PhysicalLaws
{
public:
    PhysicalLaws();
    ~PhysicalLaws();
    void init();
    void release();
    void update();
    void debug();
protected:
    // 인덱스 멤버
    const static int XYZ = 3;
    const static int X   = 0;
    const static int Y   = 1;
    const static int Z   = 2;
    // 중력가속도
    const static double GRAVITY_ACCEL;

    // 질량, 위치, 속도, 가속도, 힘
    double _mass       = 0;
    double _pos[XYZ]   = { 0, 0, 0 };
    double _speed[XYZ] = { 0, 0, 0 };
    double _accel[XYZ] = { 0, 0, 0 };
    double _force[XYZ] = { 0, 0, 0 };
}PhysicalLaws;
const double PhysicalLaws::GRAVITY_ACCEL = (-9.8f);