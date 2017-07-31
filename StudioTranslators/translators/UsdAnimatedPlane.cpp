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

#include "UsdAnimatedPlane.h"
#include <iostream>

#include "maya/MFnDagNode.h"
#include "maya/MDagModifier.h"
#include "maya/MObject.h"
#include "maya/MDistance.h"


#include "AL/usdmaya/fileio/translators/TranslatorContext.h"
#include "AL/usdmaya/nodes/ProxyShape.h"

#include "Studio/nodes/UsdDrivenMeshExample.h"
#include "Studio/UsdAnimatedPlane.h"

#include "maya/MPlug.h"

namespace Studio
{
// For the second argument pass in the Schema type that this translator will be responsible for
AL_USDMAYA_DEFINE_TRANSLATOR(UsdAnimatedPlane, Studio_UsdAnimatedPlane)

MStatus UsdAnimatedPlane::initialize()
{
  MGlobal::displayInfo("UsdAnimatedPlane::Initialize");
  return MStatus::kSuccess;
}

MStatus UsdAnimatedPlane::import(const UsdPrim& prim, MObject& parent)
{
  MGlobal::displayInfo("UsdAnimatedPlane::import");
  MStatus status;
  MFnDependencyNode fn;
  MObject shape = fn.create(Studio::nodes::UsdDrivenMeshExample::kTypeId, &status);
  Studio::nodes::UsdDrivenMeshExample* drivenMesh = (Studio::nodes::UsdDrivenMeshExample*)fn.userNode();

  const AL::usdmaya::nodes::ProxyShape* proxyShape = context()->getProxyShape();
  if(!proxyShape)
  {
    return MStatus::kFailure;
  }

  drivenMesh->init(prim.GetPrimPath(), *proxyShape);
  MString name = fn.name();

  MDagPath dagPath;
  MFnDagNode mfnDag(parent);
  mfnDag.getPath(dagPath);
  MString fullPath = dagPath.fullPathName();

  MString proxyShapeName = context()->getProxyShape()->name();

  std::stringstream ss;
  // connect our generator node to the mesh
  ss << "createNode \"mesh\" -p " << fullPath;
  MString meshName = MGlobal::executeCommandStringResult(ss.str().c_str());
  ss.str("");

  // connect our generator node to the mesh
  ss << "connectAttr -f (\"" << name << ".outMesh\") (\"" << meshName << ".inMesh\")";
  MGlobal::executeCommand(MString(ss.str().c_str()));
  ss.str("");
  // connect mesh to default shading group to assign a material
  ss << "sets -add \"initialShadingGroup\" " << meshName << std::endl;
  MGlobal::executeCommand(MString(ss.str().c_str()));
  ss.str("");

  MString drivenMeshName = fn.name();
  // connect our generator node to the mesh
  ss << "connectAttr -f (\"" << proxyShape->outTimePlug().name() << "\") (\"" << drivenMesh->timePlug().name() << "\")";
  MGlobal::executeCommand(MString(ss.str().c_str()));
  ss.str("");

  return MStatus::kSuccess;
}

MStatus UsdAnimatedPlane::tearDown(const SdfPath& path)
{
  MGlobal::displayInfo("UsdAnimatedPlane::tearDown");
  context()->removeItems(path);
  return MStatus::kSuccess;
}

MStatus UsdAnimatedPlane::update(const UsdPrim& prim)
{
  MGlobal::displayInfo("UsdAnimatedPlane::update");
  return MStatus::kSuccess;
}

} // namespace Studio
