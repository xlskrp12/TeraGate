#pragma once
#include"Prev.h"
#include"Texture.h"
#include<fbxsdk.h>
//#include"fbxsdk\scene\animation\fbxanimcurve.h"
#include<vector>
#include<queue>
#include"DisplayAnimation.h"
class FBX
{
public://�������̽�
/*
void outFile(char* fileName);
void outNodeFile();
void render();
int getIndexCount();
*/

    FBX();
    ~FBX();
    void test();
    void init(ID3D11Device *device, char *fbxFileName);
    void release();
    FbxString getAttributeTypeName(FbxNodeAttribute::EType type);
    void infoToFile(char* fileName);     //���ڷ� ������ ���ϰ��, �̸����� ������� ex) ./����/�����̸�.txt
    void infoNodeToFile(FbxNode* node);
    void render(ID3D11DeviceContext *deviceContext);
    int  getIndexCount();
protected://����Լ�
    void initFBX(char *fbxFileName);
    void       printMatrix(FbxAMatrix mtx);
    FbxAMatrix getNodeMatrix(FbxNode* node, int frameCount=0);
    void       printNodeMatrix(FbxNode* node, int frameCount=0);
    // query�Լ��� timespan->skeleton->node �� ������ �� ��.
    // 1. timespan : �ִϸ��̼� ����~�� ������ ����
    // 2. skeleton : ���̷��� ���̶�Ű ����
    // 3. node     : �޽�(��Ʈ�� ����Ʈ, ������ ��Ʈ�� ����Ʈ), ��Ų(���� ���� ��Ʈ�� ����Ʈ�� ����Ʈ), Ư�� �������� �� transform��� ����
    void queryTimespan(FbxScene* scene);
    void querySkeletonRootHierarchy(); // �ݵ�� ��� ���̷��� ���̶�Ű�� ���� ������ ���� queryNode�� ��!! queryNode�ʿ��� deformer�̿��� �� ���̶�Ű�� �����ϱ� ���� �̤̤�
    void querySkeleton(FbxNode *node, int depth, int index, int parentIndex);
    int  querySkeletonIndexByName(std::string name);
    void queryNode(FbxNode *node);
    void initBuffer(ID3D11Device *device);
    void releaseFBX();
    void releaseBuffer();
    void renderBuffer(ID3D11DeviceContext* deviceContext);
    void tabByFile();
protected://Ÿ��
    // FBX������ �о� VertexŸ������ ����
    // �� ���� ���ؽ��� ������ �𸣴� std::vector, push_back�� �̿�
    // float       _weight;
    // D3DXVECTOR4 _bone;
    // Ư�� time�� ���ؽ� TRS D3DXVECTOR4 = _weight1 * _bone1 + _weight2 * bone2 + ..
    struct Weight
    {
        int   _controlPointIndex;
        float _controlPointWeight;
    };
    struct Skin
    {
        std::vector<Weight> _weightData;
        std::string         _name;
    };
    struct Skeleton
    {
        FbxString               _name;        // ���̷��� �̸�
        int                     _depth;       // ���̷��� ����
        int                     _index;       // ���̷��� �ε���
        int                     _parentIndex; // ���̷��� �θ� �ε���
        Skin                    _skin;
        std::vector<FbxAMatrix> _mtxFrame;
        FbxNode          *_linkNode;
    };
    struct ControlPoint
    {
        ControlPoint()  {};
        ~ControlPoint() {};
        D3DXVECTOR3 _pos;
    };
    struct PolygonPoint
    {
        PolygonPoint()  {};
        ~PolygonPoint() {};
        D3DXVECTOR3 _pos;
        D3DXVECTOR2 _uv;
        D3DXVECTOR3 _normal;
//        int         _index;//�ε���
    };
    struct UVNormal
    {
        //int         _controlVertexIndex;
        D3DXVECTOR2 _uv;
        D3DXVECTOR3 _normal;
    };
protected://�������
    //��FBX������
    char  _fbxFileName[200];
    FILE* _file;
    int   _nodeDepth;
    FbxManager              *_manager;
    FbxIOSettings           *_ios;
    FbxImporter             *_importer;
    FbxScene                *_scene;
    FbxNode                 *_rootNode;
    FbxAnimEvaluator        *_evaluator;
    std::vector<ControlPoint>_controlPointData;
    std::vector<PolygonPoint>_polygonPointData;
    std::vector<Skeleton>    _skeletonData;
    FbxTimeSpan              _timespan;

    //��FBX �ִϸ��̼� ������
    FbxTime       _time;
    FbxAnimStack *_animStack;
    FbxAnimLayer *_animLayer;
    FbxAnimCurve *_animCurve;

    //��DirectX������
    ID3D11Buffer *_vertexBuffer;
    ID3D11Buffer *_indexBuffer;
};
/*
��Ŭ���� ����
Ŭ����٣
���Ÿ�� _�������;
Scene
FbxNode* _rootNode;
FbxNode
int _attributeCount;
FbxNodeAttribute* _attribute;
FbxNodeAttribute
Etype _eType;

��ִϸ��̼ǿ� ���Ͽ� ���� ���۷��� �ǿ��غ� ��
Ŭ����٣ 
    ����
FbxScene ��
    �ϳ� �̻��� FbxAnimStack�� ���� �� �ִ�.
FbxAnimStack �ִϸ��̼� ����
    �ִϸ��̼� ���� �����ϴ� ������ �����̳�.
    base layer�� �θ��� �ּ� �ϳ� �̻��� FbxAnimLayer�� �����Ѵ�.
FbxAnimLayer �ִϸ��̼� ���̾�
    �ϳ� �̻��� FbxAnimCurve��带 �����Ѵ�. 
    �ִϸ��̼��� �ֱ� ���ؼ��� �ּ� �ϳ� �̻��� ���̾ �ʿ��ϴ�.
FbxAnimCurve ���ϸ��̼� �
    FBX object�� FbxProperty�� ��� �ִϸ��̼ǵǴ���, �ִϸ��̼� ���̾��� �⺻�����κ��� ��� �޶����� �����Ѵ�.
    FbxAnimCurve���� FbxAnimCurveNode��� ������ִ�.
    ���� FbxAnimCurve�� ������ FBX Property�� ������� ���� FbxAnimCurveNode��� ����� �� �ִ�. // �׷���..?
    �׷� ������, �ϳ��� FbxAnimCurve�� ���� FBX object���� ���� FBX property���� �ִϸ���Ʈ�� �� �ִ�.
    FbxAnimCurve�� ������ Fbx property type�� �������̸� app�� ���� �˻�ȴ�. // property������ ������ٸ鼭 �̰� �� �Ҹ���..?
    FbxAnimCurve�� ���� �ݵ�� �־�߸� �ϴ� ���� �ƴϸ�, �޸𸮿� ����ð��� �Ƴ��� ���� �ִϸ����õ��� �ʴ� FBX property�� FbxAnimCurve�� ������ ���� �� �ִ�(?)
FbxAnimCurveKey �ִϸ��̼� � Ű
    key&keyframe�� �ִϸ��̼� ��� ����&������ ǥ���Ѵ�.
FbxObject �ʸ��ڽ� ������Ʈ
    ������Ʈ�� 0 �Ǵ� �� �̻��� FBX �Ӽ��� ������.
    FbxNode Ŭ������ ���� �� �����ν� ������Ʈ�� ��Ҹ� ó���ϴ� �Ӽ��� ������.
FbxProperty �ʸ��ڽ� �Ӽ�
    FbxObject�� ���� �߰��� ������.
    ���� �ִϸ���Ʈ�ϱ� ���� FBX object�� ���Ե� ������ FBX property�� �غ��ų �� �ִ�.
    �� ���� ������ ���ؼ��� FBX Property�� ����.


*/
/*
@ ���䵥��ũ ��Ȩ ���۷��� �׽�Ʈ ��
    class FBXAnim{
    private:

    }

    class FBX{
    ..
    protected:
        FBXAnim _fbxAnim;
    ..
    }

    class Bone{
    ..
    protected:
        FBXAnimCurveNode& myCameraPositionVector;
    }

    ���䵥��ũ ��Ȩ ���۷��� �׽�Ʈ ��
    FbxAnimEvaluator* mySceneEvaluator = MyScene->GetEvaluator();
    FbxTime myTime;
    myTime.SetSecondDouble(0.0);
    FbxNode* myMeshNode = FbxNode::Create (myScene, "");
    FbxMatrix& GlobalMatrix = mySceneEvaluator->GetNodeGlobalTransform(myMeshNode, myTime);
    FbxMatrix& GlobalMatrix = mySceneEvaluator->GetNodeLocalTransform(myMeshNode, myTime);
    FbxCamera* myCamera = FbxCamera::Create (myScene, "");
    FbxAnimCurveNode&  myCameraPositionVector; 
    myCameraPositionVector = mySceneEvaluator->GetPropertyValue (myCamera->Position, myTime);
*/
/*


*/