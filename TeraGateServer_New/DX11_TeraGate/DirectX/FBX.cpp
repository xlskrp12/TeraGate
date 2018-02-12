#include"FBX.h"
/*
// ���ε�����, ����Ʈ������ ����
// https://forums.autodesk.com/t5/fbx-forum/what-s-a-bind-pose/td-p/5960947
  * The FbxPose object can be setup to hold "Bind Pose" data or "Rest Pose" data.
  *
  * The Bind Pose holds the transformation (translation, rotation and scaling)
  * matrix of all the nodes implied in a link deformation. This includes the geometry
  * being deformed, the links deforming the geometry, and recursively all the
  * ancestors nodes of the link. The Bind Pose gives you the transformation of the nodes
  * at the moment of the binding operation when no deformation occurs.
  * ��� : ���ε������ Ư�� �������� ��Ʈ�� ����Ʈ�� ��°� ������
  *
  * The Rest Pose is a snapshot of a node transformation. A Rest Pose can be used
  * to store the position of every node of a character at a certain point in
  * time. This pose can then be used as a reference position for animation tasks,
  * like editing walk cycles.
  * ��� : ����Ʈ����� Ư�� �������� ��Ʈ�� ����Ʈ�� ��°� ������

// ����Ʈ���� ����
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

// ���̷���, ��ũ, ���ε�����, ����Ʈ����, �ִϸ��̼� ���� �����ε� ���˾ƺ��ڴ�.
// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html?url=cpp_ref/_export_scene01_2main_8cxx-example.html,topicNumber=cpp_ref__export_scene01_2main_8cxx_example_html8e6c2543-31ff-4af4-a92c-5f167b5d500d


// FBX ��� ��ȯ ���, �� ��� �������� ��ĺ�ȯ�� �� �ʿ䰡 ����.
// Ư�� �ð����� ���ؽ����� ���� �� ���� �� ����.
// https://stackoverflow.com/questions/35245433/fbx-node-transform-calculation
*/
class Test{
public:
    // ����� �Լ���
    FbxAMatrix getGlobalPosition(FbxNode* node, const FbxTime& time, FbxPose* pose, FbxAMatrix* parentGlobalPosition);
    FbxAMatrix getPoseMatrix(FbxPose* pose, int nodeIndex);
    FbxAMatrix getGeometry(FbxNode* node);
    FbxAMatrix FbxMatrixToGlm(const FbxAMatrix& matrix); // �־��� ����� �����ϴ� �Լ��ε�, glm�� Ǯ������ ����?
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
                        parentGlobalPositionMatrix = getGlobalPosition(node->GetParent(), time, pose, parentGlobalPosition/*������ �����Ȱǰ�?*/);
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
// �־��� ����� �����ϴ� �Լ��ε�, glm�� Ǯ������ ����?
// ��ư ���� �̰� ȣ���� �ʿ� ����.
FbxAMatrix Test::FbxMatrixToGlm(const FbxAMatrix& matrix)
{
    FbxAMatrix mtx;
    return mtx;
}
void Test::queryMeshRecursive(FbxScene* scene, FbxNode* node, FbxAMatrix& parentGlobalPosition, FbxMesh* mesh/*shared_ptr<Mesh> mesh*/, unsigned& currentNode)
{
    FbxNodeAttribute* attribute     = node->GetNodeAttribute();
    // 15�����Ӳ� 
    FbxAMatrix globalPosition       = getGlobalPosition(node, 15, scene->GetPose(-1), &parentGlobalPosition);
    FbxAMatrix geometryOffset       = getGeometry(node);
    FbxAMatrix globalOffsetPosition = globalPosition * geometryOffset;
    if(attribute)
    {
        if(attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh* mesh = node->GetMesh();
            //triangleȭ ��Ű�°� ����. �Լ� ȣ���� �� �ȵȴ�.
            const unsigned int vertexCount = mesh->GetControlPointsCount();
            const unsigned int polygonCount = mesh->GetPolygonCount();
            if(vertexCount == 0) return;
            // ���ؽ� ����, ����Ʈ������
        }//if(attribute->...)
    }// if(attribute)
}//������ �ȵǸ� �̰ɷ� �غ���
//http://help.autodesk.com/view/FBX/2018/ENU/?guid=FBX_Developer_Help_cpp_ref_export_scene03_2main_8cxx_example_html
//----------------------------------------------------------------------------------------
void FBX::test()
{
    for(int skeletonIndex=0;skeletonIndex<_skeletonData.size();skeletonIndex++)
    {
        printf("���̷��� �̸� : %s\n", _skeletonData[skeletonIndex]._name);
        for(int frameIndex=0;frameIndex<_skeletonData[skeletonIndex]._mtxFrame.size();frameIndex++)
        {
            if(frameIndex==0 || frameIndex==18)
            {
                printf("������ %d\n", frameIndex);
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
    printf("succeed��Fbx::init()\n");
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
    fprintf(_file, "��Fbx::infoByFile(\"%s\");\n", fileName);
    fprintf(_file, " _meshData.size() : %d // ���ؽ� ��\n", _polygonPointData.size());
    //�ִԽ��� ����
    FbxAnimStack   *stack    = _scene->GetCurrentAnimationStack();
    FbxTimeSpan     timespan = stack->GetLocalTimeSpan();
    fprintf(_file, " FbxAnimStack\n");
    fprintf(_file, " ��stack name      : %s\n", stack->GetName());
    fprintf(_file, " ��start frame     : %d\n", timespan.GetStart().GetFrameCount()); //�ִϸ��̼� ���� ���� ������
    fprintf(_file, " ��stop  frame     : %d\n", timespan.GetStop().GetFrameCount());  //�ִϸ��̼� ���� ��   ������
    fprintf(_file, " ��total anim-time : %.2f sec\n", timespan.GetStop().GetSecondDouble());//�ִϸ��̼��� ����Ǵµ� �ɸ��� �� �ð�

    infoNodeToFile(_rootNode);

    for(int i=0;i<_skeletonData.size();i++)
    {
        for(int j=0;j<_skeletonData[i]._depth;j++)
            fprintf(_file, "\t");
        fprintf(_file, "skeleton �̸� : %s, ���� : %d, �ε��� : %d, �θ��ε��� : %d\n", _skeletonData[i]._name, _skeletonData[i]._depth, _skeletonData[i]._index, _skeletonData[i]._parentIndex);
    }
    fclose(_file);
}
void FBX::infoNodeToFile(FbxNode* node)
{
    // Ư�� �ð����� ��� transform����� ��� ��带 transform���·� �����.
    FbxTime time;
    time.SetFrame(18, FbxTime::eFrames30);
    FbxAnimEvaluator*eval = node->GetAnimationEvaluator();
    FbxAMatrix mtx = eval->GetNodeLocalTransform(node, time);
    node->LclTranslation.Set(FbxDouble3(mtx.GetT().mData[0], mtx.GetT().mData[1], mtx.GetT().mData[2]));
    node->LclRotation.Set(FbxDouble3(mtx.GetR().mData[0], mtx.GetR().mData[1], mtx.GetR().mData[2]));
    node->LclScaling.Set(FbxDouble3(mtx.GetS().mData[0], mtx.GetS().mData[1], mtx.GetS().mData[2]));

    //The Animation stack is a collection of animation layers.          ����     { ���̾�, ���̾� ..     }
    //The animation layer is a collection of animation curve nodes.     ���̾�   { Ŀ����, Ŀ���� .. }
    //Animation curve node is a collection of animation curves.         Ŀ���� { Ŀ��, Ŀ�� ..         }
    //And then! Animation curves are what we can get the keyframe from. Ŀ��     { Ű������, Ű������ .. }
    //The animation curve are corresponding with node's property.       Ŀ��� ���Ӽ��� �ش�ȴ�

    // �̰ɷ� �� ����� trs�� ���� �� ����.
    // node->GetAnimationEvaluator()->GetNodeLocalTransform(node, time)���� Ư�� �������� �������� ���� trs���� ��Ȯ�� ���� �� �ִ�.
    FbxDouble3 t = node->LclTranslation.Get();
    FbxDouble3 r = node->LclRotation.Get();
    FbxDouble3 s = node->LclScaling.Get();
    tabByFile(); fprintf(_file, "��node depth %d\n", _nodeDepth);
    tabByFile(); fprintf(_file, " nodeName       : %s\n", node->GetName());
    tabByFile(); fprintf(_file, " transform      : t(%.2f, %.2f, %.2f)\tr(%.2f, %.2f, %.2f)\ts(%.2f, %.2f, %.2f)\n", t[0], t[1], t[2], r[0], r[1], r[2], s[0], s[1], s[2]);
    tabByFile(); fprintf(_file, " childNodeCount : %d\n", node->GetChildCount());
    tabByFile(); fprintf(_file, " attributeCount : %d\n", node->GetNodeAttributeCount());
    // ������ ����� ��Ʈ����Ʈ �ݺ�
    for (int attributeIndex = 0; attributeIndex < node->GetNodeAttributeCount(); attributeIndex++)
    {
        // ��尡 ���� ��Ʈ����Ʈ�� �ε����� get
        FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(attributeIndex);
        tabByFile(); fprintf(_file, " attributeName  : %s\n", getAttributeTypeName(attribute->GetAttributeType()));
        // ��Ʈ����Ʈ�� �޽���
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
            tabByFile(); fprintf(_file, "\tpolygon point count : %d // uv, normal������ control point�� �ߺ�������\n", mesh->GetPolygonVertexCount());
            ////�ߺ����� ���ؽ� �̾Ƴ���
            ////_msize(������)/sizeof(*������)�� �����Ҵ�� ������ �迭 ����� ���� ��Ʈ�� �ε����ν� ���
            //for(int controlIndex = 0; controlIndex < _msize(controler)/sizeof(*controler); controlIndex++)
            //{
            //        vertex._pos = D3DXVECTOR3(
            //            (float)controler[controlIndex].mData[0],
            //            (float)controler[controlIndex].mData[1],
            //            (float)controler[controlIndex].mData[2]);
            //}
            //�̻����� ��Ȳ
            //for(������ ����)
            //{
            //    for(������ ���ؽ� ����)
            //    {
            //        int ��Ʈ�� �ε��� = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
            //        vertex._index    = ..; // �ű�
            //        vertex._pos      = ..; // ����
            //        vertex._uv       = ..;
            //        vertex._normal   = ..;
            //    }
            //}
            // ��Ʈ�� ����Ʈ ���� ����
            tabByFile();fprintf(_file, "\tmesh-controlPoint   :\n"); 
            for(int i=0;i<mesh->GetControlPointsCount();i++)
            {
                FbxVector4 vertex = mesh->GetControlPointAt(i);
                tabByFile();fprintf(_file, "\t\tcontrolPoint[%d] (%+.2f, %+.2f, %+.2f, %+.2f)\n", i, vertex.mData[0], vertex.mData[1], vertex.mData[2], vertex.mData[3]); 
            }
            // ������ ����Ʈ ���� ����(��Ʈ�� ����Ʈ�� �ߺ��Ǿ� �ִ�.)
            tabByFile();fprintf(_file, "\tmesh-polygonPoint   :\n"); 
            for (int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); polygonIndex++)
            {
                for (int vertexIndex = 0; vertexIndex < mesh->GetPolygonSize(polygonIndex); vertexIndex++)
                {
                    // �ߺ����� ��Ʈ�� �ε��� ��
                    controlIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
                    // ��ġ����
                    vertex._pos = D3DXVECTOR3(
                        (float)controlPoint[controlIndex].mData[0],
                        (float)controlPoint[controlIndex].mData[1],
                        (float)controlPoint[controlIndex].mData[2]);
                    // UV
                    // UV�ؽ��� ���� ����Ʈ �ۼ�.
                    // �Ϲ������� fbx���� �ϳ��� �ؽ��� �ϳ��� ����ϹǷ� ����Ʈ�� 0�� �ε����� ����ϸ� �� ������ ������..
                    // ���� �ؽ��ĸ� ����ϰ� �Ǵ� ��� ����ؾ����� �𸣰ڴ�. ���߿� �����غ���.
                    FbxStringList uvNameList;
                    mesh->GetUVSetNames(uvNameList);
                    //printf("uv �̸� ���� : %d\n", uvNameList.GetCount());
                    //printf("uv �̸� : %s\n", uvNameList[0]);
                    mesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvNameList[0], uvVector, isUV);                    // ���� �� [in]������ �ε���, [in]���ؽ� �ε���, [in]uv�̸�, [out]uv����, [?] �÷���?
                    vertex._uv = D3DXVECTOR2(
                        (float)uvVector[0],
                        (float)1-uvVector[1]);// uv�� v�� �Ʒ��� ������ 1�� ������.

                    // ��ֺ���
                    mesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, normalVector);
                    vertex._normal = D3DXVECTOR3(
                        (float)normalVector.mData[0],
                        (float)normalVector.mData[1],
                        (float)normalVector.mData[2]);
                    tabByFile(); fprintf(_file, "\t\tcontrolPoint[%d] polygon[%d] vertex[%d] : pos(%+.2f, %+.2f, %+.2f)\t  normal(%+.2f, %+.2f, %+.2f)\t  uv(%+.2f, %+.2f)\n", controlIndex,  polygonIndex, vertexIndex, vertex._pos.x, vertex._pos.y, vertex._pos.z, vertex._normal.x, vertex._normal.y, vertex._normal.z, vertex._uv.x, vertex._uv.y);
                }//vertexIndex
            }//polygonIndex
            /*
                ���� : FbxSkin Class ��ũ���� 
                       http://help.autodesk.com/view/FBX/2018/ENU/?guid=__cpp_ref_class_fbx_skin_html
                       A skin deformer contains clusters (FbxCluster).
                       Each cluster acts on a subset of the geometry's control points, with different weights.
                       For example, a mesh of humanoid shape can have a skin attached, that describes the way the humanoid mesh is deformed by bones.
                       When the bones are animated, the clusters act on the geometry to animate it too.
                �ǿ� : skin deformer�� cluster�� �����Ѵ�.
                       �� cluster�� geometry�� control points�� �ٸ� weights�� subset ����ȴ�.                                // subset�� �� �� ��������..?
                       ������ �ΰ��� mesh��, mesh�� bone������ �󸶳� ����(deformed)�Ǵ����� ����� skin attached�� ���� �� �ִ�. //
                       bone�� �ִϸ���Ʈ�� ��, geometry(mesh�� ���ϴ� ��)�� �ִϸ���Ʈ��Ű�� ���� cluster�� ����ȴ�.
            */
            // mesh, deformer�δ� Ư�� �ð����� transform��Ⱑ �Ұ����Ѱ� ����.
            // ���⼭�� �׳� ��Ų��(���� ���� ��Ʈ������Ʈ&����Ʈ ����)�� ��� �����ؾ߰ڴ�.
            tabByFile(); fprintf(_file, "// mesh, deformer(skin)�δ� Ư�� �ð����� transform��Ⱑ �Ұ����ϴ�. //\n");
            for(int deformerIndex=0;deformerIndex<mesh->GetDeformerCount();deformerIndex++)
            {                
                //��Ų
                FbxSkin    *fbxSkin   = (FbxSkin*)mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin);
                if(NULL == fbxSkin)
                    continue;
                tabByFile(); fprintf(_file, "skinDeformerCount : %d//mesh->GetDeformerCount()\n", mesh->GetDeformerCount());
                tabByFile(); fprintf(_file, "skinClusterCount  : %d//skin->GetClusterCount()\n", fbxSkin->GetClusterCount());
                tabByFile(); fprintf(_file, "skinData          :\n");
                for(int boneIndex = 0;boneIndex<fbxSkin->GetClusterCount();boneIndex++)
                {
                    FbxCluster *cluster  = fbxSkin->GetCluster(boneIndex);
                    FbxNode    *linkNode = cluster->GetLink();// �� ��忡 ���� ���۷���
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
            // ���Ұ�
            // ���������ν��� ���ؽ��� �ߺ� �����ϴ� �� ����
            // �޽ø� �̷�� ���ؽ��� ���� ����!
                    //�̰� ���� ���ؽ��̴� �ٸ� �Լ� �־��µ� ��� ���� �̤�
                    //controlIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
                    //vertex._pos = D3DXVECTOR3(
                    //    (float)controler[controlIndex].mData[0],
                    //    (float)controler[controlIndex].mData[1],
                    //    (float)controler[controlIndex].mData[2]);
        }//if(attributeŸ���� eMesh��)
    }//for(attributeIndex;;)
    // ���� ���鿡 ���� ���
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
    // _magnager, _ios, _scene, _rootNode, _importer������ �ʱ�ȭ
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
        printf("%s ����\n", s);
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

    // query�Լ��� timespan->skeleton->node �� ������ �� ��.
    // 1. timespan : �ִϸ��̼� ����~�� ������ ����
    // 2. skeleton : ���̷��� ���̶�Ű ����
    // 3. node     : �޽�(��Ʈ�� ����Ʈ, ������ ��Ʈ�� ����Ʈ), ��Ų(���� ���� ��Ʈ�� ����Ʈ�� ����Ʈ), Ư�� �������� �� transform��� ����
    queryTimespan(_scene);
    querySkeletonRootHierarchy();
    queryNode(_rootNode);
}
void FBX::queryTimespan(FbxScene* scene)
{
    FbxAnimStack* stack;
    stack     = scene->GetCurrentAnimationStack();    
    _timespan = stack->GetLocalTimeSpan();
    // �ִϸ��̼� Ű�� ���� �������� ���� ��� (����, ��ž) �������� ���� (0, 100)�� �ȴ�.
    // ���������� // _timespan.GetStart().GetFrameCount();
    // ��ž������ // _timespan.GetStop().GetFrameCount();
}
void       FBX::printMatrix(FbxAMatrix mtx)
{
    printf("��mtx\n");
    printf("T\t%+.2f\t%+.2f\t%+.2f\t%+.2f\t\n", mtx.GetT().mData[0], mtx.GetT().mData[1], mtx.GetT().mData[2], mtx.GetT().mData[3]);
    printf("R\t%+.2f\t%+.2f\t%+.2f\t%+.2f\t\n", mtx.GetR().mData[0], mtx.GetR().mData[1], mtx.GetR().mData[2], mtx.GetR().mData[3]);
    printf("S\t%+.2f\t%+.2f\t%+.2f\t%+.2f\t\n", mtx.GetS().mData[0], mtx.GetS().mData[1], mtx.GetS().mData[2], mtx.GetS().mData[3]);
}
FbxAMatrix FBX::getNodeMatrix(FbxNode* node, int frameCount)
{
    // �μ��� ������ frameCount�� �ƽ������� ����Ű �������̽� ���� �ִ� ������ 1, 2, 3, 4.. �̷��� ���Ѵ�.
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

    printf("��nodeName : %s\t\tframeCount : %d\n", node->GetName(), frameCount);
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
        // ���̷���(�ִϸ��̼� ���� ��)
        if(FbxNodeAttribute::eSkeleton == node->GetNodeAttributeByIndex(attrIndex)->GetAttributeType())
        {
            Skeleton skeleton;
            skeleton._name        = node->GetName();
            skeleton._depth       = depth;
            skeleton._index       = index;
            skeleton._parentIndex = parentIndex;
            _skeletonData.push_back(skeleton);
            // printf("�̸�%s ����%d �ε���%d �θ��ε���%d\n", node->GetName(), depth, index, parentIndex);
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
    //�忩�� trs�� ��Ʈ�� ����Ʈ�� �����غ���
    // Ư�� �ð����� ��� transform����� ��� ��带 transform���·� �����.
    FbxTime time;
    time.SetFrame(18, FbxTime::eFrames30);
    FbxAnimEvaluator*eval = node->GetAnimationEvaluator();
    FbxAMatrix mtx = eval->GetNodeLocalTransform(node, time);
    node->LclTranslation.Set(FbxDouble3(mtx.GetT().mData[0], mtx.GetT().mData[1], mtx.GetT().mData[2]));
    node->LclRotation.Set(FbxDouble3(mtx.GetR().mData[0], mtx.GetR().mData[1], mtx.GetR().mData[2]));
    node->LclScaling.Set(FbxDouble3(mtx.GetS().mData[0], mtx.GetS().mData[1], mtx.GetS().mData[2]));

    //The Animation stack is a collection of animation layers.          ����     { ���̾�, ���̾� ..     }
    //The animation layer is a collection of animation curve nodes.     ���̾�   { Ŀ����, Ŀ���� .. }
    //Animation curve node is a collection of animation curves.         Ŀ���� { Ŀ��, Ŀ�� ..         }
    //And then! Animation curves are what we can get the keyframe from. Ŀ��     { Ű������, Ű������ .. }
    //The animation curve are corresponding with node's property.       Ŀ��� ���Ӽ��� �ش�ȴ�

    // �̰ɷ� �� ����� trs�� ���� �� ����.
    // node->GetAnimationEvaluator()->GetNodeLocalTransform(node, time)���� Ư�� �������� �������� ���� trs���� ��Ȯ�� ���� �� �ִ�.
    FbxDouble3 t = node->LclTranslation.Get();
    FbxDouble3 r = node->LclRotation.Get();
    FbxDouble3 s = node->LclScaling.Get();
    //�㿩�� trs�� ��Ʈ�� ����Ʈ�� �����غ���


    // ������ ����� ��Ʈ����Ʈ �ݺ�
    for (int i = 0; i < node->GetNodeAttributeCount(); i++)
    {
        // ��尡 ���� ��Ʈ����Ʈ�� �ε����� get
        FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);
        // ���̷���(�ִϸ��̼� ���� ��)
        if(attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
        {
            FbxSkeleton *skeleton = (FbxSkeleton*)attribute;
        }
        // ������ ��Ʈ����Ʈ�� �޽���
        if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh    *mesh         = (FbxMesh*)attribute;
            FbxVector4 *controlPoint = mesh->GetControlPoints();
            PolygonPoint      vertex;
            int         controlIndex;
            FbxVector4  normalVector;
            FbxVector2  uvVector;
            bool        isUV;
            // ��Ʈ�� ����Ʈ ������ ����
            for(int controlIndex = 0; controlIndex < mesh->GetControlPointsCount(); controlIndex++)
            {
                ControlPoint point;
                point._pos = D3DXVECTOR3(
                    (float)controlPoint[controlIndex].mData[0],
                    (float)controlPoint[controlIndex].mData[1],
                    (float)controlPoint[controlIndex].mData[2]);
                _controlPointData.push_back(point);
            }
            // �޽��κ��� ���ؽ� ������ ����
            for (int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); polygonIndex++)
            {
                for (int vertexIndex = 0; vertexIndex < mesh->GetPolygonSize(polygonIndex); vertexIndex++)
                {
                    // �ߺ����� ��Ʈ�� ���ؽ� ��
                    controlIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
                    // ��ġ����
                    vertex._pos = D3DXVECTOR3(
                        (float)controlPoint[controlIndex].mData[0], //�ƽ�mData[0]++ == �ٷ�������++
                        (float)controlPoint[controlIndex].mData[1], //�ƽ�mData[1]-- == �ٷ�������++
                        (float)controlPoint[controlIndex].mData[2]);//�ƽ�mData[2]++ == �ٷ����̰�++
                    
                    // UV
                    // UV�ؽ��� ���� ����Ʈ �ۼ�.
                    // �Ϲ������� fbx���� �ϳ��� �ؽ��� �ϳ��� ����ϹǷ� ����Ʈ�� 0�� �ε����� ����ϸ� �� ������ ������..
                    // ���� �ؽ��ĸ� ����ϰ� �Ǵ� ��� ����ؾ����� �𸣰ڴ�. ���߿� �����غ���.
                    FbxStringList uvNameList;
                    mesh->GetUVSetNames(uvNameList);
                    //printf("uv �̸� ���� : %d\n", uvNameList.GetCount());
                    //printf("uv �̸� : %s\n", uvNameList[0]);
                    mesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvNameList[0], uvVector, isUV);                    // ���� �� [in]������ �ε���, [in]���ؽ� �ε���, [in]uv�̸�, [out]uv����, [?] �÷���?
                    vertex._uv = D3DXVECTOR2(
                        (float)uvVector[0],
                        (float)1-uvVector[1]);// uv�� v�� �Ʒ��� ������ 1�� ������.

                    // ��ֺ���
                    mesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, normalVector);
                    vertex._normal = D3DXVECTOR3(
                        (float)normalVector.mData[0],
                        (float)normalVector.mData[1],
                        (float)normalVector.mData[2]);
                    _polygonPointData.push_back(vertex);
                }
            }
            // mesh, deformer������ Ư�� �ð����� transform�� ���� �� �ִ�!!
            // _time, time�� ȥ���ؼ� ���ٺ��� ���� �Ǽ��� ���� ��̴��Ŵ�!!!!!!
            for(int deformerIndex=0;deformerIndex<mesh->GetDeformerCount();deformerIndex++)
            {                
                //infoToNode(node, time)���ڸ� �ٲ㼭
                //time�� ���� ��� ���¸� ��°ɷ� �ϸ� ���?
                //FbxAMatrix& mtxGlobal = _evaluator->GetNodeGlobalTransform(node, _time);
                //printf("queryBone(node\"%s\", frame\"%d\");\n", node->GetName(), time.GetFrameCount());
                //printf("%+.2f\t%+.2f\t%+.2f\t%+.2f\n",   mtxGlobal.GetT().mData[0], mtxGlobal.GetT().mData[1], mtxGlobal.GetT().mData[2], mtxGlobal.GetT().mData[3]);
                //printf("%+.2f\t%+.2f\t%+.2f\t%+.2f\n",   mtxGlobal.GetR().mData[0], mtxGlobal.GetR().mData[1], mtxGlobal.GetR().mData[2], mtxGlobal.GetR().mData[3]);
                //printf("%+.2f\t%+.2f\t%+.2f\t%+.2f\n\n", mtxGlobal.GetS().mData[0], mtxGlobal.GetS().mData[1], mtxGlobal.GetS().mData[2], mtxGlobal.GetS().mData[3]);

                //��skin, deformer�ܾ �򰥷��� �ּ�
                //mesh�� Deformer(skin)�� ������ �ִ�.
                //Deformer(skin)�� ��Ų�� ���� �Ʒ��ִ� ��Ų-��(cluster)�� ������ �ִ�.
                //��Ų-��(cluster)�� ��Ʈ�� ����Ʈ �ε���, �ε��� �� ����Ʈ ���� ������ �ִ�.
                //��Ų-��(cluster)�� ��ũ��带 ������ �ִ�.
                //��Ų-�� ��ũ���� ��ȯ����� ������.
                FbxSkin    *fbxSkin      = (FbxSkin*)mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin);
                int         clusterCount = fbxSkin->GetClusterCount();//Ŭ������ : ��Ų�� ������ �ִ� ���� ����(Ŭ������) ����
                if(NULL == fbxSkin)
                    continue;

                for(int clusterIndex = 0;clusterIndex<clusterCount;clusterIndex++)
                {
                    FbxCluster *cluster  = fbxSkin->GetCluster(clusterIndex);
                    FbxNode    *linkNode = cluster->GetLink();// ��忡 ���� ���۷���
                    //FbxAMatrix  mtxGeometryTransform, mtxTransform, mtxTransformLink, mtxGlobalBindposeInverse;
                    //mtxGeometryTransform = FbxAMatrix(node->GetGeometricTranslation(FbxNode::eSourcePivot), node->GetGeometricRotation(FbxNode::eSourcePivot), node->GetGeometricScaling(FbxNode::eSourcePivot));
                    //cluster->GetTransformMatrix(mtxTransform);        // ���ε� Ÿ�� �� �޽��� Ʈ������ ���
                    //cluster->GetTransformLinkMatrix(mtxTransformLink);// ����Ʈ ����->���� �������� ����(?) ���ε� Ÿ�� �� Ŭ������(����Ʈ)�� Ʈ������ ���
                    //mtxGlobalBindposeInverse = mtxTransformLink.Inverse() * mtxTransform * mtxGeometryTransform;

                    //Ư�� �����ӿ� �� �������
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
                            // ���̷��濡 ��Ų�� �ۼ�
                            for(int clusterControlPointIndex=0;clusterControlPointIndex<clusterControlPointCount;clusterControlPointIndex++)
                            {
                                Weight      weight;
                                weight._controlPointIndex  = clusterControlPoint[clusterControlPointIndex];
                                weight._controlPointWeight = (float)clusterControlPointWeight[clusterControlPointIndex];
                                _skeletonData[skelIndex]._skin._weightData.push_back(weight);
                            }
                            // ���̷��濡 �����Ӻ� ������� �ۼ�
                            for(int frame = 0;frame<_timespan.GetStop().GetFrameCount();frame++)
                            {
                                FbxTime    time;
                                time.SetFrame(frame, FbxTime::eFrames30);
                                FbxAMatrix mtxTransform = linkNode->GetAnimationEvaluator()->GetNodeGlobalTransform(linkNode, time);
                                _skeletonData[skelIndex]._mtxFrame.push_back(mtxTransform);
                            }
                        }
                    }
                    ////��Ų�� ��Ʈ������Ʈ �ε���, ����Ʈ�� �� ������ Ȯ���ϴ� �ڵ�. Ȯ�ΰ�� �� ��������.
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
    // ���� ���鿡 ���� ���
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

    //�������迭, �ε����迭 ����
    vertex = new PolygonPoint[_polygonPointData.size()];
    index  = new unsigned long[_polygonPointData.size()];
    //FBX���Ϸκ��� ���� _meshData������ �����迭�� ����
    std::vector<PolygonPoint>::iterator it = _polygonPointData.begin();
    for (int i = 0; i < _polygonPointData.size(); i++, it++)
    {
        //�����迭 �ʱ�ȭ
        vertex[i]._pos    = it->_pos;
        vertex[i]._uv     = it->_uv;
        vertex[i]._normal = it->_normal;
        //�ε����迭 �ʱ�ȭ
        index[i] = i;
    }
    //�������� ��ũ����
    vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth           = sizeof(PolygonPoint) * _polygonPointData.size();
    vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags      = 0;
    vertexBufferDesc.MiscFlags           = 0;
    vertexBufferDesc.StructureByteStride = 0;
    //�������� ������
    vertexBufferData.pSysMem             = vertex;
    vertexBufferData.SysMemPitch         = 0;
    vertexBufferData.SysMemSlicePitch    = 0;
    //�������� ����
    result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &_vertexBuffer);
    if (FAILED(result))MessageBox(NULL, L"ID3D11Device::CreateBuffer() ��������", L"Error", NULL);
    //�ε������� ��ũ����
    indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth           = sizeof(unsigned long) * _polygonPointData.size();
    indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags      = 0;
    indexBufferDesc.MiscFlags           = 0;
    indexBufferDesc.StructureByteStride = 0;
    //�ε������� ������
    indexBufferData.pSysMem          = index;
    indexBufferData.SysMemPitch      = 0;
    indexBufferData.SysMemSlicePitch = 0;
    //�ε������� ����
    result = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &_indexBuffer);

    if (FAILED(result))MessageBox(NULL, L"ID3D11Devie::CreateBuffer() �ε��� ����", L"Error", NULL);
    //����, �ε������۷� �̹� ������ ������ �Ѱ�� ������ �� �̻� �ʿ���� �ڿ�����
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
    deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);//�Է�������(IA, InputAssembler) ��������   Ȱ��ȭ
    deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);   //�Է�������(IA, InputAssembler) �ε������� Ȱ��ȭ
    //�������� ������ �⺻������ 
    //�� ģ�� ��� �����Ǳ� ��
    // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : �ﰢ��
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}