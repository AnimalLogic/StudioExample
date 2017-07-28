//
// Copyright 2017 Animal Logic
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.//
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "assert.h"

#include "maya/MStatus.h"
#include "maya/MFnMeshData.h"
#include "maya/MFnMesh.h"
#include "maya/MDistance.h"
#include "maya/MFnTypedAttribute.h"
#include "maya/MGlobal.h"
#include "maya/MPointArray.h"
#include "maya/MTime.h"

#include "pxr/usd/usd/attribute.h"
#include "pxr/base/tf/token.h"

#include "UsdDrivenMeshExample.h"
#include "AL/usdmaya/nodes/ProxyShape.h"
#include "Studio/UsdAnimatedPlane.h"
#include "TypeIDs.h"

namespace Studio {
namespace nodes {

AL_MAYA_DEFINE_NODE(UsdDrivenMeshExample, STUDIO_USDDRIVENMESHEXAMPLE, Studio);

MObject UsdDrivenMeshExample::m_scale = MObject::kNullObj;
MObject UsdDrivenMeshExample::m_outMesh = MObject::kNullObj;
MObject UsdDrivenMeshExample::m_time = MObject::kNullObj;

void UsdDrivenMeshExample::init(const SdfPath& path, const AL::usdmaya::nodes::ProxyShape& shape)
{
  MGlobal::displayInfo("UsdDrivenMeshExample::init");
  m_shape = &shape;
  m_pPath = path;
}

UsdPrim UsdDrivenMeshExample::getPrim()
{
  MGlobal::displayInfo("UsdDrivenMeshExample::getPrim");
  if(!m_shape || m_shape->getUsdStage() == nullptr)
  {
    return UsdPrim();
  }

  UsdPrim prim  =  m_shape->getUsdStage()->GetPrimAtPath(m_pPath);
  return prim;
}

MStatus UsdDrivenMeshExample::initialise()
{
  MGlobal::displayInfo("UsdDrivenMeshExample::initialise");
  try
  {
    setNodeType(kTypeName);
    addFrame("USD Layer Info");
    // do not write these nodes to the file. They will be created automagically by the proxy shape
    m_scale = addDistanceAttr("scale", "sc", MDistance(1), kReadable | kWritable | kInternal);
    m_time = addTimeAttr("usdTime", "utm", MTime(0.0), kKeyable | kConnectable | kReadable | kWritable | kStorable | kAffectsAppearance);

    MFnTypedAttribute tAttr;
    m_outMesh = tAttr.create("outMesh", "om", MFnData::kMesh);
    tAttr.setReadable(true);
    tAttr.setWritable(false);
    addAttribute(m_outMesh);

  }
  catch(const MStatus& status)
  {
    return status;
  }

  AL_MAYA_CHECK_ERROR(attributeAffects(m_time, m_scale), "Unable to setup the affected chain");
  AL_MAYA_CHECK_ERROR(attributeAffects(m_time, m_outMesh), "Unable to setup the affected chain");
  AL_MAYA_CHECK_ERROR(attributeAffects(m_scale, m_outMesh), "Unable to setup the affected chain");

  generateAETemplate();
  return MS::kSuccess;
}

MStatus UsdDrivenMeshExample::compute(const MPlug& plug, MDataBlock& dataBlock)
{
  MGlobal::displayInfo("UsdDrivenMeshExample::compute");
  if(plug == m_outMesh)
  {
    MDataHandle scaleHandle = dataBlock.inputValue(m_scale);
    MDistance size = scaleHandle.asDistance();

    MFnMeshData fnData;
    MObject newMesh = fnData.create();

    MFnMesh fnOutputMesh;

    double half_d = size.value()*0.5;

    // generate an array of points
    MPointArray points;
    points.append( half_d, 0 ,-half_d );
    points.append(-half_d, 0 ,-half_d );
    points.append(-half_d, 0 , half_d );
    points.append( half_d, 0 , half_d );

    // the poly connects holds the vertex indicies for each face in one array
    MIntArray polyConnects;
    polyConnects.append(0);
    polyConnects.append(1);
    polyConnects.append(2);
    polyConnects.append(3);

    // for each face, specify how many verts there are
    MIntArray polyCounts;
    polyCounts.append(4);

    // create the mesh. Note: if this is done in a command or importer,
    // the oNewMesh param is not needed (and it will create a node instead)
    fnOutputMesh.create(4,1,points,polyCounts,polyConnects,newMesh);

    // create array of normals
    MVector normal(0.0f,1.0f,0.0f);

    fnOutputMesh.setFaceVertexNormal(normal,0,0);
    fnOutputMesh.setFaceVertexNormal(normal,0,1);
    fnOutputMesh.setFaceVertexNormal(normal,0,2);
    fnOutputMesh.setFaceVertexNormal(normal,0,3);

    MDataHandle outputData = dataBlock.outputValue(m_outMesh);

    outputData.set(newMesh);

    dataBlock.setClean(plug);
    return MS::kSuccess;
  }

  return MS::kUnknownParameter;
}

void UsdDrivenMeshExample::postConstructor()
{
  MGlobal::displayInfo("UsdDrivenMeshExample::postConstructor");
}

bool UsdDrivenMeshExample::getInternalValueInContext(const MPlug& plug, MDataHandle& dataHandle, MDGContext& ctx)
{
  //Put out of USD
  MGlobal::displayInfo("UsdDrivenMeshExample::getInternalValueInContext");
  // something has gone terribly wrong if these are null, yet the handle remains valid.
  assert(m_shape && m_shape->getUsdStage());
  if(!m_shape || !m_shape->getUsdStage() || !getPrim().IsValid())
  {
    return false;
  }

  if(plug == m_scale)
  {
    Studio_UsdAnimatedPlane animatedPlane(getPrim());
    UsdAttribute attr = animatedPlane.GetSizeAttr();

    MPlug timePlug(thisMObject(), m_time);
    MTime tt;
    timePlug.getValue(tt);

    float size;
    attr.Get<float>(&size, (float)tt.value());
    dataHandle.set(MDistance(size));
  }

  return MPxNode::getInternalValueInContext(plug, dataHandle, ctx);
}

bool UsdDrivenMeshExample::setInternalValueInContext(const MPlug& plug, const MDataHandle& dataHandle, MDGContext& ctx)
{
  if(plug == m_scale)
  {
    MPlug timePlug(thisMObject(), m_time);
    MTime tt;
    timePlug.getValue(tt);

    UsdAttribute attr = getPrim().GetAttribute(TfToken("size"));
    attr.Set((float)dataHandle.asDistance().value(), tt.value());
    VtValue value;
    attr.Get(&value);
  }

  return MPxNode::setInternalValueInContext(plug, dataHandle, ctx);
}


//----------------------------------------------------------------------------------------------------------------------
} // nodes
} // Studio
//----------------------------------------------------------------------------------------------------------------------

