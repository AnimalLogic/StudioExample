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

#include "maya/MDrawRegistry.h"
#include "maya/MGlobal.h"
#include "maya/MStatus.h"

#include "pxr/pxr.h"

#include "AL/maya/CommandGuiHelper.h"
#include "AL/maya/MenuBuilder.h"

#include "UsdDrivenMeshExample.h"

PXR_NAMESPACE_USING_DIRECTIVE

namespace Studio {
namespace nodes {

MStatus registerPlugin(MFnPlugin& plugin)
{
  MStatus status;

  AL_REGISTER_DEPEND_NODE(plugin, Studio::nodes::UsdDrivenMeshExample);

  CHECK_MSTATUS(AL::maya::MenuBuilder::generatePluginUI(plugin, "Studio_nodes"));
  return status;
}

MStatus unregisterPlugin(MFnPlugin& plugin)
{
  MStatus status;
  AL_UNREGISTER_NODE(plugin, Studio::nodes::UsdDrivenMeshExample);
  return status;
}

//----------------------------------------------------------------------------------------------------------------------
} // nodes
} // Studio
//----------------------------------------------------------------------------------------------------------------------
