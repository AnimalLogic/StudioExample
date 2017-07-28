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
