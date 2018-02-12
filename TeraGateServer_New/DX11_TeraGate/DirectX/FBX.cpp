#include"FBX.h"
/*
// 바인드포즈, 레스트포즈의 정의
// https://forums.autodesk.com/t5/fbx-forum/what-s-a-bind-pose/td-p/5960947
  * The FbxPose object can be setup to hold "Bind Pose" data or "Rest Pose" data.
  *
  * The Bind Pose holds the transformation (translation, rotation and scaling)
  * matrix of all the nodes implied in a link deformation. This includes the geometry
  * being deformed, the links deforming the geometry, and recursively all the
  * ancestors nodes of the link. The Bind Pose gives you the transformation of the nodes
  * at the moment of the binding operation when no deformation occurs.
  * 요약 : 바인드포즈로 특정 프레임의 컨트롤 포인트를 얻는건 간접적
  *
  * The Rest Pose is a snapshot of a node transformation. A Rest Pose can be used
  * to store the position of every node of a character at a certain point in
  * time. This pose can then be used as a reference position for animation tasks,
  * like editing walk cycles.
  * 요약 : 레스트포즈로 특정 프레임의 컨트롤 포인트를 얻는건 직접적

// 레스트포즈 예제
// http://help.autodesk.com/view/FBX/2018/ENU/?guid=annotated
for(int poseIndex=0;poseIndex < _scene->GetCharacterPoseCount();poseIndex++)
{
    FbxCharacterPose* pose = _scene->GetCharacterPose(poseIndex);
    FbxCharacter*     character = pose->GetCharacter();
    if(!character) break;
    FbxCharacterLink characterLink;
    FbxCharacter::ENodeid nodeID = FbxCharacter::eHips;
    while(character->GetCharacterLink(nodeID, &characterLink))
    {
        FbxAMatrix& globalPosition = characterLink.mNode->EvaluateGlobalTransform(FBXSDK_TIME_ZERO);
        for(int k=0;k<4;k++)
        {
            //...
        }
    }
}

// 스켈레톤, 링크, 바인드포즈, 레스트포즈, 애니메이션 스택 예제인데 못알아보겠다.
// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html?url=cpp_ref/_export_scene01_2main_8cxx-example.html,topicNumber=cpp_ref__export_scene01_2main_8cxx_example_html8e6c2543-31ff-4af4-a92c-5f167b5d500d


// FBX 노드 변환 계산, 이 사람 본문에서 행렬변환을 할 필요가 없다.
// 특정 시간대의 버텍스들을 얻을 수 있을 것 같다.
// https://stackoverflow.com/questions/35245433/fbx-node-transform-calculation
*/
class Test{
public:
    // 도우미 함수들
    FbxAMatrix getGlobalPosition(FbxNode* node, const FbxTime& time, FbxPose* pose, FbxAMatrix* parentGlobalPosition);
    FbxAMatrix getPoseMatrix(FbxPose* pose, int nodeIndex);
    FbxAMatrix getGeometry(FbxNode* node);
    FbxAMatrix FbxMatrixToGlm(const FbxAMatrix& matrix); // 주어진 행렬을 반전하는 함수인데, glm의 풀네임이 뭐지?
    // 
    void queryMeshRecursive(FbxScene* scene, FbxNode* node, FbxAMatrix& parentGlobalPosition, FbxMesh* mesh/*shared_ptr<Mesh> mesh*/, unsigned& currentNode);
private:
    FbxManager _manager;
};

FbxAMatrix Test::getGlobalPosition(FbxNode* node, const FbxTime& time, FbxPose* pose, FbxAMatrix* parentGlobalPosition)
{
    FbxAMatrix globalPositionMatrix;
    bool       positionFound = false;
    if(pose)
    {
        int nodeIndex = pose->Find(node);
        if(-1 < nodeIndex)
        {
            if(pose->IsBindPose() || !pose->IsLocalMatrix(nodeIndex))
            {
                globalPositionMatrix = getPoseMatrix(pose, nodeIndex);
            }
            else
            {
                FbxAMatrix parentGlobalPositionMatrix;
                if(parentGlobalPosition)
                {
                    parentGlobalPositionMatrix = *parentGlobalPosition;
                }
                else
                {
                    if(node->GetParent())
                        parentGlobalPositionMatrix = getGlobalPosition(node->GetParent(), time, pose, parentGlobalPosition/*본문에 누락된건가?*/);
                }
                FbxAMatrix localPositionMatrix;
                localPositionMatrix  = getPoseMatrix(pose, nodeIndex);
                globalPositionMatrix = parentGlobalPositionMatrix * localPositionMatrix;
            }//if(pose->IsBindPose() || !pose->IsLocalMatrix(nodeIndex))
            positionFound = true;
        }//if(-1<nodeIndex)
    }//if(pose)
    if(!positionFound)
    {
        globalPositionMatrix = node->EvaluateGlobalTransform(time);
    }
    return globalPositionMatrix;
}
FbxAMatrix Test::getPoseMatrix(FbxPose* pose, int nodeIndex)
{
    FbxAMatrix poseMatrix;
    FbxMatrix  matrix;
    matrix = pose->GetMatrix(nodeIndex);
    memcpy((double*)poseMatrix, (double*)matrix, sizeof(matrix.mData));
    return poseMatrix;
}
FbxAMatrix Test::getGeometry(FbxNode* node)
{
    const FbxVector4 t = node->GetGeometricTranslation(FbxNode::eSourcePivot);
    const FbxVector4 r = node->GetGeometricRotation(FbxNode::eSourcePivot);
    const FbxVector4 s = node->GetGeometricScaling(FbxNode::eSourcePivot);
    return FbxAMatrix(t, r, s);
}
// 주어진 행렬을 반전하는 함수인데, glm의 풀네임이 뭐지?
// 여튼 현재 이건 호출할 필요 없다.
FbxAMatrix Test::FbxMatrixToGlm(const FbxAMatrix& matrix)
{
    FbxAMatrix mtx;
    return mtx;
}
void Test::queryMeshRecursive(FbxScene* scene, FbxNode* node, FbxAMatrix& parentGlobalPosition, FbxMesh* mesh/*shared_ptr<Mesh> mesh*/, unsigned& currentNode)
{
    FbxNodeAttribute* attribute     = node->GetNodeAttribute();
    // 15프레임꺼 
    FbxAMatrix globalPosition       = getGlobalPosition(node, 15, scene->GetPose(-1), &parentGlobalPosition);
    FbxAMatrix geometryOffset       = getGeometry(node);
    FbxAMatrix globalOffsetPosition = globalPosition * geometryOffset;
    if(attribute)
    {
        if(attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh* mesh = node->GetMesh();
            //triangle화 시키는거 생략. 함수 호출이 잘 안된다.
            const unsigned int vertexCount = mesh->GetControlPointsCount();
            const unsigned int polygonCount = mesh->GetPolygonCount();
            if(vertexCount == 0) return;
            // 버텍스 추출, 로컬트랜스폼
        }//if(attribute->...)
    }// if(attribute)
}//위에꺼 안되면 이걸로 해보자
//http://help.autodesk.com/view/FBX/2018/ENU/?guid=FBX_Developer_Help_cpp_ref_export_scene03_2main_8cxx_example_html
//----------------------------------------------------------------------------------------
void FBX::test()
{
    for(int skeletonIndex=0;skeletonIndex<_skeletonData.size();skeletonIndex++)
    {
        printf("스켈레톤 이름 : %s\n", _skeletonData[skeletonIndex]._name);
        for(int frameIndex=0;frameIndex<_skeletonData[skeletonIndex]._mtxFrame.size();frameIndex++)
        {
            if(frameIndex==0 || frameIndex==18)
            {
                printf("프레임 %d\n", frameIndex);
                printMatrix(_skeletonData[skeletonIndex]._mtxFrame[frameIndex]);
            }
        }
    }

}
FBX::FBX()
{
    _manager    = NULL;
    _ios        = NULL;
    _importer   = NULL;
    _scene      = NULL;
    _rootNode   = NULL;
    _nodeDepth = 0;
}
FBX::~FBX()
{
    release();
}
void FBX::init(ID3D11Device* device, char* fbxFileName)
{
    strcpy_s(_fbxFileName, sizeof(_fbxFileName), fbxFileName);
    initFBX(fbxFileName);
    initBuffer(device);
#ifdef _DEBUG
    printf("succeed▶Fbx::init()\n");
#endif
}
void FBX::release()
{
    releaseFBX();
    releaseBuffer();
}
void FBX::tabByFile()
{
    for(int i=0;i<_nodeDepth;i++)
        fprintf(_file, "\t");
}
FbxString FBX::getAttributeTypeName(FbxNodeAttribute::EType type)
{
    switch (type) {
    case FbxNodeAttribute::eUnknown:            return "unidentified";
    case FbxNodeAttribute::eNull:               return "null";
    case FbxNodeAttribute::eMarker:             return "marker";
    case FbxNodeAttribute::eSkeleton:           return "skeleton";
    case FbxNodeAttribute::eMesh:               return "mesh";
    case FbxNodeAttribute::eNurbs:              return "nurbs";
    case FbxNodeAttribute::ePatch:              return "patch";
    case FbxNodeAttribute::eCamera:             return "camera";
    case FbxNodeAttribute::eCameraStereo:       return "stereo";
    case FbxNodeAttribute::eCameraSwitcher:     return "camera switcher";
    case FbxNodeAttribute::eLight:              return "light";
    case FbxNodeAttribute::eOpticalReference:   return "optical reference";
    case FbxNodeAttribute::eOpticalMarker:      return "marker";
    case FbxNodeAttribute::eNurbsCurve:         return "nurbs curve";
    case FbxNodeAttribute::eTrimNurbsSurface:   return "trim nurbs surface";
    case FbxNodeAttribute::eBoundary:           return "boundary";
    case FbxNodeAttribute::eNurbsSurface:       return "nurbs surface";
    case FbxNodeAttribute::eShape:              return "shape";
    case FbxNodeAttribute::eLODGroup:           return "lodgroup";
    case FbxNodeAttribute::eSubDiv:             return "subdiv";
    default:                                    return "unknown";
    }
}
void FBX::infoToFile(char* fileName)
{
    fopen_s(&_file, fileName, "w+");
    fprintf(_file, "▼Fbx::infoByFile(\"%s\");\n", fileName);
    fprintf(_file, " _meshData.size() : %d // 버텍스 수\n", _polygonPointData.size());
    //애님스택 정보
    FbxAnimStack   *stack    = _scene->GetCurrentAnimationStack();
    FbxTimeSpan     timespan = stack->GetLocalTimeSpan();
    fprintf(_file, " FbxAnimStack\n");
    fprintf(_file, " └stack name      : %s\n", stack->GetName());
    fprintf(_file, " └start frame     : %d\n", timespan.GetStart().GetFrameCount()); //애니메이션 스택 시작 프레임
    fprintf(_file, " └stop  frame     : %d\n", timespan.GetStop().GetFrameCount());  //애니메이션 스택 끝   프레임
    fprintf(_file, " └total anim-time : %.2f sec\n", timespan.GetStop().GetSecondDouble());//애니메이션이 재생되는데 걸리는 총 시간

    infoNodeToFile(_rootNode);

    for(int i=0;i<_skeletonData.size();i++)
    {
        for(int j=0;j<_skeletonData[i]._depth;j++)
            fprintf(_file, "\t");
        fprintf(_file, "skeleton 이름 : %s, 깊이 : %d, 인덱스 : %d, 부모인덱스 : %d\n", _skeletonData[i]._name, _skeletonData[i]._depth, _skeletonData[i]._index, _skeletonData[i]._parentIndex);
    }
    fclose(_file);
}
void FBX::infoNodeToFile(FbxNode* node)
{
    // 특정 시간대의 노드 transform행렬을 얻어 노드를 transform상태로 만든다.
    FbxTime time;
    time.SetFrame(18, FbxTime::eFrames30);
    FbxAnimEvaluator*eval = node->GetAnimationEvaluator();
    FbxAMatrix mtx = eval->GetNodeLocalTransform(node, time);
    node->LclTranslation.Set(FbxDouble3(mtx.GetT().mData[0], mtx.GetT().mData[1], mtx.GetT().mData[2]));
    node->LclRotation.Set(FbxDouble3(mtx.GetR().mData[0], mtx.GetR().mData[1], mtx.GetR().mData[2]));
    node->LclScaling.Set(FbxDouble3(mtx.GetS().mData[0], mtx.GetS().mData[1], mtx.GetS().mData[2]));

    //The Animation stack is a collection of animation layers.          스택     { 레이어, 레이어 ..     }
    //The animation layer is a collection of animation curve nodes.     레이어   { 커브노드, 커브노드 .. }
    //Animation curve node is a collection of animation curves.         커브노드 { 커브, 커브 ..         }
    //And then! Animation curves are what we can get the keyframe from. 커브     { 키프레임, 키프레임 .. }
    //The animation curve are corresponding with node's property.       커브는 노드속성애 해당된다

    // 이걸로 본 노드의 trs를 얻을 땐 정상.
    // node->GetAnimationEvaluator()->GetNodeLocalTransform(node, time)으로 특정 프레임을 지정했을 때의 trs역시 정확히 얻어올 수 있다.
    FbxDouble3 t = node->LclTranslation.Get();
    FbxDouble3 r = node->LclRotation.Get();
    FbxDouble3 s = node->LclScaling.Get();
    tabByFile(); fprintf(_file, "▼node depth %d\n", _nodeDepth);
    tabByFile(); fprintf(_file, " nodeName       : %s\n", node->GetName());
    tabByFile(); fprintf(_file, " transform      : t(%.2f, %.2f, %.2f)\tr(%.2f, %.2f, %.2f)\ts(%.2f, %.2f, %.2f)\n", t[0], t[1], t[2], r[0], r[1], r[2], s[0], s[1], s[2]);
    tabByFile(); fprintf(_file, " childNodeCount : %d\n", node->GetChildCount());
    tabByFile(); fprintf(_file, " attributeCount : %d\n", node->GetNodeAttributeCount());
    // 지정된 노드의 어트리뷰트 반복
    for (int attributeIndex = 0; attributeIndex < node->GetNodeAttributeCount(); attributeIndex++)
    {
        // 노드가 가진 어트리뷰트를 인덱스로 get
        FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(attributeIndex);
        tabByFile(); fprintf(_file, " attributeName  : %s\n", getAttributeTypeName(attribute->GetAttributeType()));
        // 어트리뷰트가 메쉬면
        if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh    *mesh = (FbxMesh*)attribute;
            FbxVector4 *controlPoint = mesh->GetControlPoints();
            PolygonPoint      vertex;
            int         controlIndex;
            FbxVector4  normalVector;
            FbxVector2  uvVector;
            bool        isUV;
            tabByFile(); fprintf(_file, " meshData       :\n");
            tabByFile(); fprintf(_file, "\tcontrol point count : %d\n", _msize(controlPoint)/sizeof(*controlPoint));
            tabByFile(); fprintf(_file, "\tpolygon       count : %d\n", mesh->GetPolygonCount());
            tabByFile(); fprintf(_file, "\tpolygon point count : %d // uv, normal때문에 control point가 중복되있음\n", mesh->GetPolygonVertexCount());
            ////중복없이 버텍스 뽑아내기
            ////_msize(포인터)/sizeof(*포인터)로 동적할당된 포인터 배열 사이즈를 구해 컨트롤 인덱스로써 사용
            //for(int controlIndex = 0; controlIndex < _msize(controler)/sizeof(*controler); controlIndex++)
            //{
            //        vertex._pos = D3DXVECTOR3(
            //            (float)controler[controlIndex].mData[0],
            //            (float)controler[controlIndex].mData[1],
            //            (float)controler[controlIndex].mData[2]);
            //}
            //이상적인 상황
            //for(폴리곤 개수)
            //{
            //    for(폴리곤 버텍스 개수)
            //    {
            //        int 컨트롤 인덱스 = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
            //        vertex._index    = ..; // 신규
            //        vertex._pos      = ..; // 삭제
            //        vertex._uv       = ..;
            //        vertex._normal   = ..;
            //    }
            //}
            // 컨트롤 포인트 정보 추출
            tabByFile();fprintf(_file, "\tmesh-controlPoint   :\n"); 
            for(int i=0;i<mesh->GetControlPointsCount();i++)
            {
                FbxVector4 vertex = mesh->GetControlPointAt(i);
                tabByFile();fprintf(_file, "\t\tcontrolPoint[%d] (%+.2f, %+.2f, %+.2f, %+.2f)\n", i, vertex.mData[0], vertex.mData[1], vertex.mData[2], vertex.mData[3]); 
            }
            // 폴리곤 포인트 정보 추출(컨트롤 포인트가 중복되어 있다.)
            tabByFile();fprintf(_file, "\tmesh-polygonPoint   :\n"); 
            for (int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); polygonIndex++)
            {
                for (int vertexIndex = 0; vertexIndex < mesh->GetPolygonSize(polygonIndex); vertexIndex++)
                {
                    // 중복없는 컨트롤 인덱스 값
                    controlIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
                    // 위치벡터
                    vertex._pos = D3DXVECTOR3(
                        (float)controlPoint[controlIndex].mData[0],
                        (float)controlPoint[controlIndex].mData[1],
                        (float)controlPoint[controlIndex].mData[2]);
                    // UV
                    // UV텍스쳐 네임 리스트 작성.
                    // 일반적으로 fbx파일 하나당 텍스쳐 하나를 사용하므로 리스트의 0번 인덱스를 사용하면 별 문제가 없지만..
                    // 여러 텍스쳐를 사용하게 되는 경우 어떻게해야할지 모르겠다. 나중에 생각해보자.
                    FbxStringList uvNameList;
                    mesh->GetUVSetNames(uvNameList);
                    //printf("uv 이름 개수 : %d\n", uvNameList.GetCount());
                    //printf("uv 이름 : %s\n", uvNameList[0]);
                    mesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvNameList[0], uvVector, isUV);                    // 인자 ▶ [in]폴리곤 인덱스, [in]버텍스 인덱스, [in]uv이름, [out]uv벡터, [?] 플래그?
                    vertex._uv = D3DXVECTOR2(
                        (float)uvVector[0],
                        (float)1-uvVector[1]);// uv의 v는 아래로 갈수록 1에 가깝다.

                    // 노멀벡터
                    mesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, normalVector);
                    vertex._normal = D3DXVECTOR3(
                        (float)normalVector.mData[0],
                        (float)normalVector.mData[1],
                        (float)normalVector.mData[2]);
                    tabByFile(); fprintf(_file, "\t\tcontrolPoint[%d] polygon[%d] vertex[%d] : pos(%+.2f, %+.2f, %+.2f)\t  normal(%+.2f, %+.2f, %+.2f)\t  uv(%+.2f, %+.2f)\n", controlIndex,  polygonIndex, vertexIndex, vertex._pos.x, vertex._pos.y, vertex._pos.z, vertex._normal.x, vertex._normal.y, vertex._normal.z, vertex._uv.x, vertex._uv.y);
                }//vertexIndex
            }//polygonIndex
            /*
                원문 : FbxSkin Class 디스크립션 
                       http://help.autodesk.com/view/FBX/2018/ENU/?guid=__cpp_ref_class_fbx_skin_html
                       A skin deformer contains clusters (FbxCluster).
                       Each cluster acts on a subset of the geometry's control points, with different weights.
                       For example, a mesh of humanoid shape can have a skin attached, that describes the way the humanoid mesh is deformed by bones.
                       When the bones are animated, the clusters act on the geometry to animate it too.
                의역 : skin deformer는 cluster를 포함한다.
                       각 cluster는 geometry의 control points나 다른 weights의 subset 적용된다.                                // subset은 또 뭔 개념이지..?
                       예컨데 인간형 mesh는, mesh가 bone에의해 얼마나 변형(deformed)되는지를 기술한 skin attached를 가질 수 있다. //
                       bone이 애니메이트될 때, geometry(mesh를 말하는 듯)도 애니메이트시키기 위해 cluster가 적용된다.
            */
            // mesh, deformer로는 특정 시간대의 transform얻기가 불가능한것 같다.
            // 여기서는 그냥 스킨값(본이 가진 컨트롤포인트&웨이트 집합)만 얻고 만족해야겠다.
            tabByFile(); fprintf(_file, "// mesh, deformer(skin)로는 특정 시간대의 transform얻기가 불가능하다. //\n");
            for(int deformerIndex=0;deformerIndex<mesh->GetDeformerCount();deformerIndex++)
            {                
                //스킨
                FbxSkin    *fbxSkin   = (FbxSkin*)mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin);
                if(NULL == fbxSkin)
                    continue;
                tabByFile(); fprintf(_file, "skinDeformerCount : %d//mesh->GetDeformerCount()\n", mesh->GetDeformerCount());
                tabByFile(); fprintf(_file, "skinClusterCount  : %d//skin->GetClusterCount()\n", fbxSkin->GetClusterCount());
                tabByFile(); fprintf(_file, "skinData          :\n");
                for(int boneIndex = 0;boneIndex<fbxSkin->GetClusterCount();boneIndex++)
                {
                    FbxCluster *cluster  = fbxSkin->GetCluster(boneIndex);
                    FbxNode    *linkNode = cluster->GetLink();// 본 노드에 대한 레퍼런스
                    FbxAMatrix  mtxBindPose, mtxTransform;
                    cluster->GetTransformLinkMatrix(mtxBindPose);
                    cluster->GetTransformMatrix(mtxTransform);

                    linkNode->EvaluateGlobalTransform(time);

                    int    *clusterControlPoint       = cluster->GetControlPointIndices();
                    double *clusterControlPointWeight = cluster->GetControlPointWeights();
                    int     clusterControlPointCount  = cluster->GetControlPointIndicesCount();
                    tabByFile(); fprintf(_file, "\tlinkNode[%d] name                : %s\n", boneIndex, linkNode->GetName());
                    tabByFile(); fprintf(_file, "\tlinkNode[%d] bine pose           : t(%.2f, %.2f, %.2f)\tr(%.2f, %.2f, %.2f)\ts(%.2f, %.2f, %.2f)\n", boneIndex, mtxBindPose.GetT().mData[0], mtxBindPose.GetT().mData[1], mtxBindPose.GetT().mData[2], mtxBindPose.GetR().mData[0], mtxBindPose.GetR().mData[1], mtxBindPose.GetR().mData[2], mtxBindPose.GetS().mData[0], mtxBindPose.GetS().mData[1], mtxBindPose.GetS().mData[2]);
                    tabByFile(); fprintf(_file, "\tlinkNode[%d] transform           : t(%.2f, %.2f, %.2f)\tr(%.2f, %.2f, %.2f)\ts(%.2f, %.2f, %.2f)\n", boneIndex, mtxTransform.GetT().mData[0], mtxTransform.GetT().mData[1], mtxTransform.GetT().mData[2], mtxTransform.GetR().mData[0], mtxTransform.GetR().mData[1], mtxTransform.GetR().mData[2], mtxTransform.GetS().mData[0], mtxTransform.GetS().mData[1], mtxTransform.GetS().mData[2]);
                    tabByFile(); fprintf(_file, "\tlinkNode[%d] control point count : %d\n", boneIndex, clusterControlPointCount);
                    tabByFile(); fprintf(_file, "\tlinkNode[%d] data                :\n", boneIndex);
                    for(int clusterControlPointIndex=0;clusterControlPointIndex<clusterControlPointCount;clusterControlPointIndex++)
                    {
                        tabByFile(); fprintf(_file, "\t\t\controlPoint[%d] pos(%+.2f, %+.2f, %+.2f) Weight(%+.2f)\n", clusterControlPoint[clusterControlPointIndex], controlPoint[clusterControlPoint[clusterControlPointIndex]].mData[0], controlPoint[clusterControlPoint[clusterControlPointIndex]].mData[1], controlPoint[clusterControlPoint[clusterControlPointIndex]].mData[2], (float)clusterControlPointWeight[clusterControlPointIndex]);
                    }
                }//for(boneIndex;;)
            }//for(deformerCount;;)
            // ▼할거
            // 폴리곤으로써의 버텍스를 중복 추출하는 것 말고
            // 메시를 이루는 버텍스만 따로 추출!
                    //이거 말고 버텍스뽑는 다른 함수 있었는데 어디 있지 ㅜㅜ
                    //controlIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
                    //vertex._pos = D3DXVECTOR3(
                    //    (float)controler[controlIndex].mData[0],
                    //    (float)controler[controlIndex].mData[1],
                    //    (float)controler[controlIndex].mData[2]);
        }//if(attribute타입이 eMesh면)
    }//for(attributeIndex;;)
    // 하위 노드들에 대해 재귀
    _nodeDepth++;
    for (int i = 0; i < node->GetChildCount(); i++)
    {
        infoNodeToFile(node->GetChild(i));
    }
    _nodeDepth--;
}
void FBX::render(ID3D11DeviceContext* deviceContext)
{
    renderBuffer(deviceContext);
}
int  FBX::getIndexCount()
{
    return _polygonPointData.size();
}
void FBX::initFBX(char* fbxFileName)
{
    bool result;
    // _magnager, _ios, _scene, _rootNode, _importer순으로 초기화
    _manager   = FbxManager::Create(); 
    _ios       = FbxIOSettings::Create(_manager, IOSROOT);
    _manager->SetIOSettings(_ios);
    _scene     = FbxScene::Create(_manager, "");
    _rootNode  = _scene->GetRootNode();
    _importer  = FbxImporter::Create(_manager, "");
    result     = _importer->Initialize(fbxFileName, -1, _manager->GetIOSettings());
    if (false == result)
    {
        CString cs;
        char s[255];
        sprintf_s(s, sizeof(s), "_importer->Initialize() // filaed fbx file load \"%s\"", fbxFileName);
        printf("%s 에러\n", s);
        cs = s;
        MessageBox(NULL, cs, L"Error", NULL);
    }
#ifdef _DEBUG
    if (false == result)
    {
        printf("fail to import %s\n", fbxFileName);
        getchar();
    }
    else
        printf("succeed to import %s\n", fbxFileName);
#endif
    _importer->Import(_scene);
    _importer->Destroy();
    _polygonPointData.clear();

    // query함수는 timespan->skeleton->node 이 순서로 할 것.
    // 1. timespan : 애니메이션 시작~끝 프레임 쿼리
    // 2. skeleton : 스켈레톤 하이라키 쿼리
    // 3. node     : 메시(컨트롤 포인트, 폴리곤 컨트롤 포인트), 스킨(본에 속한 컨트롤 포인트별 웨이트), 특정 프레임의 본 transform행렬 쿼리
    queryTimespan(_scene);
    querySkeletonRootHierarchy();
    queryNode(_rootNode);
}
void FBX::queryTimespan(FbxScene* scene)
{
    FbxAnimStack* stack;
    stack     = scene->GetCurrentAnimationStack();    
    _timespan = stack->GetLocalTimeSpan();
    // 애니메이션 키가 따로 정해지지 않은 경우 (시작, 스탑) 프레임은 각각 (0, 100)이 된다.
    // 시작프레임 // _timespan.GetStart().GetFrameCount();
    // 스탑프레임 // _timespan.GetStop().GetFrameCount();
}
void       FBX::printMatrix(FbxAMatrix mtx)
{
    printf("▼mtx\n");
    printf("T\t%+.2f\t%+.2f\t%+.2f\t%+.2f\t\n", mtx.GetT().mData[0], mtx.GetT().mData[1], mtx.GetT().mData[2], mtx.GetT().mData[3]);
    printf("R\t%+.2f\t%+.2f\t%+.2f\t%+.2f\t\n", mtx.GetR().mData[0], mtx.GetR().mData[1], mtx.GetR().mData[2], mtx.GetR().mData[3]);
    printf("S\t%+.2f\t%+.2f\t%+.2f\t%+.2f\t\n", mtx.GetS().mData[0], mtx.GetS().mData[1], mtx.GetS().mData[2], mtx.GetS().mData[3]);
}
FbxAMatrix FBX::getNodeMatrix(FbxNode* node, int frameCount)
{
    // 인수로 정해진 frameCount는 맥스에서의 오토키 인터페이스 보면 있는 프레임 1, 2, 3, 4.. 이런걸 뜻한다.
    FbxAnimEvaluator* evaluator;
    FbxTime time;
    FbxAMatrix mtx;

    evaluator = _scene->GetAnimationEvaluator();
    time.SetFrame(frameCount);
    mtx = evaluator->GetNodeGlobalTransform(node, _time);

    return mtx;
}
void FBX::printNodeMatrix(FbxNode* node, int frameCount)
{
    FbxAnimEvaluator* evaluator;
    FbxTime           time;     
    FbxAMatrix        mtx;

    evaluator = _scene->GetAnimationEvaluator();
    time.SetFrame(frameCount, FbxTime::eFrames30);
    mtx = evaluator->GetNodeGlobalTransform(node, time);

    printf("▼nodeName : %s\t\tframeCount : %d\n", node->GetName(), frameCount);
    printf("T\t%+.2f\t%+.2f\t%+.2f\t%+.2f\t\n", mtx.GetT().mData[0], mtx.GetT().mData[1], mtx.GetT().mData[2], mtx.GetT().mData[3]);
    printf("R\t%+.2f\t%+.2f\t%+.2f\t%+.2f\t\n", mtx.GetR().mData[0], mtx.GetR().mData[1], mtx.GetR().mData[2], mtx.GetR().mData[3]);
    printf("S\t%+.2f\t%+.2f\t%+.2f\t%+.2f\t\n", mtx.GetS().mData[0], mtx.GetS().mData[1], mtx.GetS().mData[2], mtx.GetS().mData[3]);
}
void FBX::querySkeletonRootHierarchy()
{
    for(int childIndex=0;childIndex<_rootNode->GetChildCount();childIndex++)
    {
        querySkeleton(_rootNode->GetChild(childIndex), 0, 0, -1);
    }
}
void FBX::querySkeleton(FbxNode* node, int depth, int index, int parentIndex)
{
    for (int attrIndex = 0; attrIndex < node->GetNodeAttributeCount(); attrIndex++)
    {
        // 스켈레톤(애니메이션 시험 중)
        if(FbxNodeAttribute::eSkeleton == node->GetNodeAttributeByIndex(attrIndex)->GetAttributeType())
        {
            Skeleton skeleton;
            skeleton._name        = node->GetName();
            skeleton._depth       = depth;
            skeleton._index       = index;
            skeleton._parentIndex = parentIndex;
            _skeletonData.push_back(skeleton);
            // printf("이름%s 깊이%d 인덱스%d 부모인덱스%d\n", node->GetName(), depth, index, parentIndex);
        }
    }
    for(int childIndex=0;childIndex<node->GetChildCount();childIndex++)
    {
        querySkeleton(node->GetChild(childIndex), depth+1, _skeletonData.size(), index);
    }
}
int  FBX::querySkeletonIndexByName(std::string skeletonName)
{
    for(int i=0;i<_skeletonData.size();i++)
    {
        std::string name = _skeletonData[i]._name;
        if(name == skeletonName)
            return i;
    }
    return -1;
}
void FBX::queryNode(FbxNode* node)
{
    //▼여기 trs를 컨트롤 포인트에 적용해보자
    // 특정 시간대의 노드 transform행렬을 얻어 노드를 transform상태로 만든다.
    FbxTime time;
    time.SetFrame(18, FbxTime::eFrames30);
    FbxAnimEvaluator*eval = node->GetAnimationEvaluator();
    FbxAMatrix mtx = eval->GetNodeLocalTransform(node, time);
    node->LclTranslation.Set(FbxDouble3(mtx.GetT().mData[0], mtx.GetT().mData[1], mtx.GetT().mData[2]));
    node->LclRotation.Set(FbxDouble3(mtx.GetR().mData[0], mtx.GetR().mData[1], mtx.GetR().mData[2]));
    node->LclScaling.Set(FbxDouble3(mtx.GetS().mData[0], mtx.GetS().mData[1], mtx.GetS().mData[2]));

    //The Animation stack is a collection of animation layers.          스택     { 레이어, 레이어 ..     }
    //The animation layer is a collection of animation curve nodes.     레이어   { 커브노드, 커브노드 .. }
    //Animation curve node is a collection of animation curves.         커브노드 { 커브, 커브 ..         }
    //And then! Animation curves are what we can get the keyframe from. 커브     { 키프레임, 키프레임 .. }
    //The animation curve are corresponding with node's property.       커브는 노드속성애 해당된다

    // 이걸로 본 노드의 trs를 얻을 땐 정상.
    // node->GetAnimationEvaluator()->GetNodeLocalTransform(node, time)으로 특정 프레임을 지정했을 때의 trs역시 정확히 얻어올 수 있다.
    FbxDouble3 t = node->LclTranslation.Get();
    FbxDouble3 r = node->LclRotation.Get();
    FbxDouble3 s = node->LclScaling.Get();
    //▲여기 trs를 컨트롤 포인트에 적용해보자


    // 지정된 노드의 어트리뷰트 반복
    for (int i = 0; i < node->GetNodeAttributeCount(); i++)
    {
        // 노드가 가진 어트리뷰트를 인덱스로 get
        FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);
        // 스켈레톤(애니메이션 시험 중)
        if(attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
        {
            FbxSkeleton *skeleton = (FbxSkeleton*)attribute;
        }
        // 지정된 어트리뷰트가 메쉬면
        if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh    *mesh         = (FbxMesh*)attribute;
            FbxVector4 *controlPoint = mesh->GetControlPoints();
            PolygonPoint      vertex;
            int         controlIndex;
            FbxVector4  normalVector;
            FbxVector2  uvVector;
            bool        isUV;
            // 컨트롤 포인트 정보를 추출
            for(int controlIndex = 0; controlIndex < mesh->GetControlPointsCount(); controlIndex++)
            {
                ControlPoint point;
                point._pos = D3DXVECTOR3(
                    (float)controlPoint[controlIndex].mData[0],
                    (float)controlPoint[controlIndex].mData[1],
                    (float)controlPoint[controlIndex].mData[2]);
                _controlPointData.push_back(point);
            }
            // 메쉬로부터 버텍스 정보를 추출
            for (int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); polygonIndex++)
            {
                for (int vertexIndex = 0; vertexIndex < mesh->GetPolygonSize(polygonIndex); vertexIndex++)
                {
                    // 중복없는 컨트롤 버텍스 값
                    controlIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
                    // 위치벡터
                    vertex._pos = D3DXVECTOR3(
                        (float)controlPoint[controlIndex].mData[0], //맥스mData[0]++ == 다렉오른쪽++
                        (float)controlPoint[controlIndex].mData[1], //맥스mData[1]-- == 다렉앞으로++
                        (float)controlPoint[controlIndex].mData[2]);//맥스mData[2]++ == 다렉높이값++
                    
                    // UV
                    // UV텍스쳐 네임 리스트 작성.
                    // 일반적으로 fbx파일 하나당 텍스쳐 하나를 사용하므로 리스트의 0번 인덱스를 사용하면 별 문제가 없지만..
                    // 여러 텍스쳐를 사용하게 되는 경우 어떻게해야할지 모르겠다. 나중에 생각해보자.
                    FbxStringList uvNameList;
                    mesh->GetUVSetNames(uvNameList);
                    //printf("uv 이름 개수 : %d\n", uvNameList.GetCount());
                    //printf("uv 이름 : %s\n", uvNameList[0]);
                    mesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvNameList[0], uvVector, isUV);                    // 인자 ▶ [in]폴리곤 인덱스, [in]버텍스 인덱스, [in]uv이름, [out]uv벡터, [?] 플래그?
                    vertex._uv = D3DXVECTOR2(
                        (float)uvVector[0],
                        (float)1-uvVector[1]);// uv의 v는 아래로 갈수록 1에 가깝다.

                    // 노멀벡터
                    mesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, normalVector);
                    vertex._normal = D3DXVECTOR3(
                        (float)normalVector.mData[0],
                        (float)normalVector.mData[1],
                        (float)normalVector.mData[2]);
                    _polygonPointData.push_back(vertex);
                }
            }
            // mesh, deformer에서도 특정 시간대의 transform을 얻을 수 있다!!
            // _time, time을 혼용해서 쓰다보니 생긴 실수로 여태 헤맸던거다!!!!!!
            for(int deformerIndex=0;deformerIndex<mesh->GetDeformerCount();deformerIndex++)
            {                
                //infoToNode(node, time)인자를 바꿔서
                //time일 때의 노드 상태를 얻는걸로 하면 어떨가?
                //FbxAMatrix& mtxGlobal = _evaluator->GetNodeGlobalTransform(node, _time);
                //printf("queryBone(node\"%s\", frame\"%d\");\n", node->GetName(), time.GetFrameCount());
                //printf("%+.2f\t%+.2f\t%+.2f\t%+.2f\n",   mtxGlobal.GetT().mData[0], mtxGlobal.GetT().mData[1], mtxGlobal.GetT().mData[2], mtxGlobal.GetT().mData[3]);
                //printf("%+.2f\t%+.2f\t%+.2f\t%+.2f\n",   mtxGlobal.GetR().mData[0], mtxGlobal.GetR().mData[1], mtxGlobal.GetR().mData[2], mtxGlobal.GetR().mData[3]);
                //printf("%+.2f\t%+.2f\t%+.2f\t%+.2f\n\n", mtxGlobal.GetS().mData[0], mtxGlobal.GetS().mData[1], mtxGlobal.GetS().mData[2], mtxGlobal.GetS().mData[3]);

                //▼skin, deformer단어가 헷갈려서 주석
                //mesh는 Deformer(skin)을 가지고 있다.
                //Deformer(skin)은 스킨의 영향 아래있는 스킨-본(cluster)를 가지고 있다.
                //스킨-본(cluster)는 컨트롤 포인트 인덱스, 인덱스 별 웨이트 값을 가지고 있다.
                //스킨-본(cluster)는 링크노드를 가지고 있다.
                //스킨-본 링크노드는 변환행렬을 가진다.
                FbxSkin    *fbxSkin      = (FbxSkin*)mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin);
                int         clusterCount = fbxSkin->GetClusterCount();//클러스터 : 스킨이 가지고 있는 본의 집합(클러스터) 개수
                if(NULL == fbxSkin)
                    continue;

                for(int clusterIndex = 0;clusterIndex<clusterCount;clusterIndex++)
                {
                    FbxCluster *cluster  = fbxSkin->GetCluster(clusterIndex);
                    FbxNode    *linkNode = cluster->GetLink();// 노드에 대한 레퍼런스
                    //FbxAMatrix  mtxGeometryTransform, mtxTransform, mtxTransformLink, mtxGlobalBindposeInverse;
                    //mtxGeometryTransform = FbxAMatrix(node->GetGeometricTranslation(FbxNode::eSourcePivot), node->GetGeometricRotation(FbxNode::eSourcePivot), node->GetGeometricScaling(FbxNode::eSourcePivot));
                    //cluster->GetTransformMatrix(mtxTransform);        // 바인딩 타임 때 메시의 트랜스폼 행렬
                    //cluster->GetTransformLinkMatrix(mtxTransformLink);// 조인트 공간->월드 공간으로 묶는(?) 바인딩 타임 때 클러스터(조인트)의 트랜스폼 행렬
                    //mtxGlobalBindposeInverse = mtxTransformLink.Inverse() * mtxTransform * mtxGeometryTransform;

                    //특정 프레임에 본 변형행렬
                    //_time.SetFrame(15);
                    //printMatrix(_evaluator->GetNodeGlobalTransform(linkNode, _time));
                    int    *clusterControlPoint        = cluster->GetControlPointIndices();
                    double *clusterControlPointWeight  = cluster->GetControlPointWeights();
                    int     clusterControlPointCount   = cluster->GetControlPointIndicesCount();

                    for(int skelIndex = 0; skelIndex<_skeletonData.size();skelIndex++)
                    {
                        if(_skeletonData[skelIndex]._name == linkNode->GetName())
                        {
                            _skeletonData[skelIndex]._linkNode = linkNode;
                            // 스켈레톤에 스킨값 작성
                            for(int clusterControlPointIndex=0;clusterControlPointIndex<clusterControlPointCount;clusterControlPointIndex++)
                            {
                                Weight      weight;
                                weight._controlPointIndex  = clusterControlPoint[clusterControlPointIndex];
                                weight._controlPointWeight = (float)clusterControlPointWeight[clusterControlPointIndex];
                                _skeletonData[skelIndex]._skin._weightData.push_back(weight);
                            }
                            // 스켈레톤에 프레임별 변형행렬 작성
                            for(int frame = 0;frame<_timespan.GetStop().GetFrameCount();frame++)
                            {
                                FbxTime    time;
                                time.SetFrame(frame, FbxTime::eFrames30);
                                FbxAMatrix mtxTransform = linkNode->GetAnimationEvaluator()->GetNodeGlobalTransform(linkNode, time);
                                _skeletonData[skelIndex]._mtxFrame.push_back(mtxTransform);
                            }
                        }
                    }
                    ////스킨에 컨트롤포인트 인덱스, 웨이트가 잘 들어갔는지 확인하는 코드. 확인결과 잘 들어가져있음.
                    //for(int i=0;i<_skinData.size();i++)
                    //{
                    //    if(i==0)
                    //        printf("bonedata.size() : %d\n", _skinData.size());
                    //    for(int j=0;j<(int)_skinData[i]._weightData.size();j++)
                    //    {
                    //        if(j==0)
                    //            printf("skinData[%d]._weightdata.size() : %d\n", i, _skinData[i]._weightData.size());
                    //        printf("skinData[%d] : index[%d] weight[%.2f]\n", i, _skinData[i]._weightData[j]._controlPointIndex, _skinData[i]._weightData[j]._controlPointWeight);
                    //    }
                    //}
                }//for(boneIndex;;)
            }//for(deformerCount;;)
        }
    }
    // 하위 노드들에 대해 재귀
    for (int i = 0; i < node->GetChildCount(); i++)
    {
        queryNode(node->GetChild(i));
    }
}
void FBX::initBuffer(ID3D11Device* device)
{
    PolygonPoint          *vertex;
    unsigned long         *index;
    D3D11_BUFFER_DESC      vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexBufferData, indexBufferData;
    HRESULT                result;

    //▼정점배열, 인덱스배열 생성
    vertex = new PolygonPoint[_polygonPointData.size()];
    index  = new unsigned long[_polygonPointData.size()];
    //FBX파일로부터 읽은 _meshData값들을 정점배열로 복사
    std::vector<PolygonPoint>::iterator it = _polygonPointData.begin();
    for (int i = 0; i < _polygonPointData.size(); i++, it++)
    {
        //정점배열 초기화
        vertex[i]._pos    = it->_pos;
        vertex[i]._uv     = it->_uv;
        vertex[i]._normal = it->_normal;
        //인덱스배열 초기화
        index[i] = i;
    }
    //정점버퍼 디스크립션
    vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth           = sizeof(PolygonPoint) * _polygonPointData.size();
    vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags      = 0;
    vertexBufferDesc.MiscFlags           = 0;
    vertexBufferDesc.StructureByteStride = 0;
    //정점버퍼 데이터
    vertexBufferData.pSysMem             = vertex;
    vertexBufferData.SysMemPitch         = 0;
    vertexBufferData.SysMemSlicePitch    = 0;
    //정점버퍼 생성
    result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &_vertexBuffer);
    if (FAILED(result))MessageBox(NULL, L"ID3D11Device::CreateBuffer() 정점버퍼", L"Error", NULL);
    //인덱스버퍼 디스크립션
    indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth           = sizeof(unsigned long) * _polygonPointData.size();
    indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags      = 0;
    indexBufferDesc.MiscFlags           = 0;
    indexBufferDesc.StructureByteStride = 0;
    //인덱스버퍼 데이터
    indexBufferData.pSysMem          = index;
    indexBufferData.SysMemPitch      = 0;
    indexBufferData.SysMemSlicePitch = 0;
    //인덱스버퍼 생성
    result = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &_indexBuffer);

    if (FAILED(result))MessageBox(NULL, L"ID3D11Devie::CreateBuffer() 인덱스 버퍼", L"Error", NULL);
    //정점, 인덱스버퍼로 이미 값들을 복사해 넘겼기 때문에 더 이상 필요없는 자원해제
    delete[]vertex; vertex = NULL;
    delete[]index;  index  = NULL;
}
void FBX::releaseBuffer()
{
    if (_indexBuffer)
    {
        _indexBuffer->Release();
        _indexBuffer = NULL;
    }
    if (_vertexBuffer)
    {
        _vertexBuffer->Release();
        _vertexBuffer = NULL;
    }
}
void FBX::releaseFBX()
{
    _polygonPointData.clear();
    _manager->Destroy();
}
void FBX::renderBuffer(ID3D11DeviceContext *deviceContext)
{
    static unsigned int stride = sizeof(PolygonPoint);
    static unsigned int offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);//입력조립기(IA, InputAssembler) 정점버퍼   활성화
    deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);   //입력조립기(IA, InputAssembler) 인덱스버퍼 활성화
    //정점버퍼 렌더시 기본형설정 
    //이 친구 없어도 렌더되긴 함
    // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : 삼각형
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}