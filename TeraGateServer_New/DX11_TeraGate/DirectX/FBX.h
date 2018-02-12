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
public://인터페이스
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
    void infoToFile(char* fileName);     //인자로 지정된 파일경로, 이름으로 파일출력 ex) ./폴더/파일이름.txt
    void infoNodeToFile(FbxNode* node);
    void render(ID3D11DeviceContext *deviceContext);
    int  getIndexCount();
protected://멤버함수
    void initFBX(char *fbxFileName);
    void       printMatrix(FbxAMatrix mtx);
    FbxAMatrix getNodeMatrix(FbxNode* node, int frameCount=0);
    void       printNodeMatrix(FbxNode* node, int frameCount=0);
    // query함수는 timespan->skeleton->node 이 순서로 할 것.
    // 1. timespan : 애니메이션 시작~끝 프레임 쿼리
    // 2. skeleton : 스켈레톤 하이라키 쿼리
    // 3. node     : 메시(컨트롤 포인트, 폴리곤 컨트롤 포인트), 스킨(본에 속한 컨트롤 포인트별 웨이트), 특정 프레임의 본 transform행렬 쿼리
    void queryTimespan(FbxScene* scene);
    void querySkeletonRootHierarchy(); // 반드시 얘로 스켈레톤 하이라키를 먼저 쿼리한 다음 queryNode할 것!! queryNode쪽에서 deformer이용할 때 하이라키를 참조하기 때문 ㅜㅜㅠ
    void querySkeleton(FbxNode *node, int depth, int index, int parentIndex);
    int  querySkeletonIndexByName(std::string name);
    void queryNode(FbxNode *node);
    void initBuffer(ID3D11Device *device);
    void releaseFBX();
    void releaseBuffer();
    void renderBuffer(ID3D11DeviceContext* deviceContext);
    void tabByFile();
protected://타입
    // FBX파일을 읽어 Vertex타입으로 저장
    // 몇 개의 버텍스가 있을지 모르니 std::vector, push_back을 이용
    // float       _weight;
    // D3DXVECTOR4 _bone;
    // 특정 time의 버텍스 TRS D3DXVECTOR4 = _weight1 * _bone1 + _weight2 * bone2 + ..
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
        FbxString               _name;        // 스켈레톤 이름
        int                     _depth;       // 스켈레톤 뎁스
        int                     _index;       // 스켈레톤 인덱스
        int                     _parentIndex; // 스켈레톤 부모 인덱스
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
//        int         _index;//인덱스
    };
    struct UVNormal
    {
        //int         _controlVertexIndex;
        D3DXVECTOR2 _uv;
        D3DXVECTOR3 _normal;
    };
protected://멤버변수
    //▼FBX변수들
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

    //▼FBX 애니메이션 변수들
    FbxTime       _time;
    FbxAnimStack *_animStack;
    FbxAnimLayer *_animLayer;
    FbxAnimCurve *_animCurve;

    //▼DirectX변수들
    ID3D11Buffer *_vertexBuffer;
    ID3D11Buffer *_indexBuffer;
};
/*
▼클래스 구조
클래스名
멤버타입 _멤버변수;
Scene
FbxNode* _rootNode;
FbxNode
int _attributeCount;
FbxNodeAttribute* _attribute;
FbxNodeAttribute
Etype _eType;

▼애니메이션에 대하여 공식 레퍼런스 의역해본 것
클래스名 
    설명
FbxScene 씬
    하나 이상의 FbxAnimStack을 가질 수 있다.
FbxAnimStack 애니메이션 스택
    애니메이션 값을 저장하는 최정점 컨테이너.
    base layer라 부르는 최소 하나 이상의 FbxAnimLayer를 포함한다.
FbxAnimLayer 애니메이션 레이어
    하나 이상의 FbxAnimCurve노드를 포함한다. 
    애니메이션을 주기 위해서는 최소 하나 이상의 레이어가 필요하다.
FbxAnimCurve 에니메이션 곡선
    FBX object의 FbxProperty가 어떻게 애니메이션되는지, 애니메이션 레이어의 기본값으로부터 어떻게 달라지를 정의한다.
    FbxAnimCurve들은 FbxAnimCurveNode들과 연결되있다.
    같은 FbxAnimCurve는 참조된 FBX Property와 상관없이 여러 FbxAnimCurveNode들로 연결될 수 있다. // 그렇다..?
    그런 연유로, 하나의 FbxAnimCurve는 여러 FBX object들의 여러 FBX property들을 애니메이트할 수 있다.
    FbxAnimCurve의 값들은 Fbx property type에 의존적이며 app에 의해 검사된다. // property참조와 상관없다면서 이건 뭔 소리야..?
    FbxAnimCurve는 딱히 반드시 있어야만 하는 것은 아니며, 메모리와 연산시간을 아끼기 위해 애니메이팅되지 않는 FBX property는 FbxAnimCurve의 사용없이 남을 수 있다(?)
FbxAnimCurveKey 애니메이션 곡선 키
    key&keyframe은 애니메이션 곡선의 시작&종점을 표시한다.
FbxObject 필름박스 오브젝트
    오브젝트는 0 또는 그 이상의 FBX 속성을 가진다.
    FbxNode 클래스는 공간 속 점으로써 오브젝트의 장소를 처리하는 속성을 가진다.
FbxProperty 필름박스 속성
    FbxObject에 속한 견고한 데이터.
    씬을 애니메이트하기 위해 FBX object에 포함된 적당한 FBX property를 준비시킬 수 있다.
    더 많은 정보를 위해서는 FBX Property를 참조.


*/
/*
@ 오토데스크 공홈 레퍼런스 테스트 중
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

    오토데스크 공홈 레퍼런스 테스트 중
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