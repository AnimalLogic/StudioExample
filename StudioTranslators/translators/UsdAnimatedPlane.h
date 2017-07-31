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
#include "AL/usdmaya/fileio/translators/TranslatorBase.h"

IGNORE_USD_WARNINGS_PUSH
#include "pxr/usd/usd/stage.h"
IGNORE_USD_WARNINGS_POP

using namespace AL::usdmaya::fileio::translators;

namespace Studio
{

class UsdAnimatedPlane : public TranslatorBase
{
public:
  AL_USDMAYA_DECLARE_TRANSLATOR(UsdAnimatedPlane);

  MStatus initialize() override;
  MStatus import(const UsdPrim& prim, MObject& parent) override;
  MStatus tearDown(const SdfPath& path) override;
  MStatus update(const UsdPrim& path) override;
  bool supportsUpdate() const override 
    { return true; }

};

} // namespace Studio
