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

#pragma once

#include "AL/usdmaya/Common.h"
#include "AL/maya/NodeHelper.h"

#include "pxr/pxr.h"
#include "pxr/usd/usd/stage.h"

#include "maya/MPxTransform.h"

PXR_NAMESPACE_USING_DIRECTIVE

namespace Studio {
namespace nodes {
class UsdDrivenMeshExample :
                          public MPxNode,
                          public AL::maya::NodeHelper
{
public:
  inline UsdDrivenMeshExample() : MPxNode(), NodeHelper(), m_shape(nullptr) {}

  AL_DECL_ATTRIBUTE(comment);
  AL_DECL_ATTRIBUTE(scale);
  AL_DECL_ATTRIBUTE(time);
  AL_DECL_ATTRIBUTE(outMesh);

  void init(const SdfPath& path, const AL::usdmaya::nodes::ProxyShape& shape);
  UsdPrim getPrim();

  //--------------------------------------------------------------------------------------------------------------------
  /// \name   Virtual overrides
  //--------------------------------------------------------------------------------------------------------------------
  MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override;

  //--------------------------------------------------------------------------------------------------------------------
  /// MPxNode overrides
  //--------------------------------------------------------------------------------------------------------------------
  void postConstructor() override;
  bool getInternalValueInContext(const MPlug& plug, MDataHandle& dataHandle, MDGContext& ctx) override;
  bool setInternalValueInContext(const MPlug& plug, const MDataHandle& dataHandle, MDGContext& ctx) override;

  //--------------------------------------------------------------------------------------------------------------------
  /// Type Info & Registration
  //--------------------------------------------------------------------------------------------------------------------
  AL_MAYA_DECLARE_NODE();

private:
  const AL::usdmaya::nodes::ProxyShape* m_shape; ///< reference to the proxy shape
  SdfPath m_pPath; ///< Path to the translated prim
};

//----------------------------------------------------------------------------------------------------------------------
} // nodes
} // Studio
//----------------------------------------------------------------------------------------------------------------------

