# Welcome

## Terms
| Term        | Meaning 
|-------------|-----------
| Package     | A bundle of source code which are built into libraries or executables


## Example AL_USDMaya plugin stack
The following demonstrates which packages need to be created to support custom Maya translation logic through AL_USDMaya.

There are up to three code packages which you may need to create to acheve this. You don't have to strictly create all three, but it is the typical case to do so. To determine which of the packages you will need to replicate then you can ask yourself:
* Do I have my own USD schemas I want to translate?
* Do I want to create my own custom Maya nodes during translation?
* Do I want my own translation logic? for example, I want translate my new schema or want to translate an existing schema a bit differently.

If you say yes to any of these questions then you will need to create atleast one of the following packages for AL_USDMaya to achieve this for you.

## Building Studio Example
To build we use [cmake](https://cmake.org) 2.8 or greater. There is an example in the local folder docker/build_studioexample.sh. We use this script in our [Docker](https://www.docker.com) containers to verify if this repository builds outside of the Animal Logic environment and might serve as a tip on how to build the repository, this also applies to our other opensourced projects.

To re-write that example in another way:

```
  cmake -DCMAKE_INSTALL_PREFIX="<PATHS_TO_THE_CMAKE_CONFIG_FILES>" \
        -DCMAKE_MODULE_PATH="<PATHS_TO_INCLUDED_CMAKE_FILES_OR_CONFIG_FILES" \
        -DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo> \
        -DMAYA_LOCATION=<PATH_TO_WHERE_MAYA_IS_INSTALLED> \
        -DUSD_SCHEMA_PATH=<PATH_TO_USD_BASE_SCHEMA_FILE>

    make install

```

## Environment Setup

After you have a successful build, you'll need to make sure your runtime environment is setup with the following

#### PYTHONPATH
Set with all the paths to your installed python pxr plugins. 

#### LD_LIBRARY_PATH
Set with all the paths to your built libraries

#### MAYA_PLUG_IN_PATH
Paths to the libaries with your custom maya nodes.

#### PXR_PLUGINPATH
Paths to where yourt pxr plugins are installed e.g. <SOME_PATH>/share/usd/plugins


## Schemas Package
IS-A schema's are the thing that your USD scene is made up of, they contain accessors to attributes on a prim and also can provide default values to these attributes and are a good way to represent pipeline concepts.
To create your own schemas that will be used in AL_USDMaya, firstly it is a good idea to follow Pixar's tutorials on creating a plugin and about creating schemas. [Schema Creation](https://graphics.pixar.com/usd/docs/api/_usd__page__generating_schemas.html) [Plugin Documentation](https://graphics.pixar.com/usd/docs/api/class_plug_registry.html)

To allow AL_USDMaya to find your schema it will need to inherit from Typed, which can be specified in the schema.usda.in file:

```
class StudioSchema "StudioSchema"(
  inherits = </Typed>
)
{
  float size=1;
}
```
When AL_USDMaya comes across prims with schemas, it will check if the found schema is of a derived type from Typed and if it is, AL_USDMaya will attempt to find the corresponding translator by asking each translator if it supports this derived schema type. If one is found, the search stops and the prim is translated by the found translator.

### Some tips and gotchas:
To test the health of your schemas library I would recommend to test the following things:
1. Test that your schema's library can be imported, before trying to import USD.
```
# start fresh python terminal, directly import your schema
import StudioSchemas
# if this throws an error saying it can't find some type, you may need to update/add your ModuleDeps.cpp file.
```

1. If you get an error mentioning that some base class is missing, most likely the USDTyped base class, you could be missing the ModuleDeps.cpp or need to add some requirements.
1. Test that your default values are working:
```
from pxr import Usd
import StudioSchemas
s = Usd.Stage.CreateInMemory()
r =StudioSchemas.UsdAnimatedPlane.Define(s,"/r")
print r.GetDepthAttr().Get()
```

1. There is a lot of code generation and configuration file generation done during the build of the schema source code. The generated source code currently isn't tracked by this repository, but it is generally recommended to track the source code that is generated(*.cpp *.h files) and not recommended to track the configuration files(plugInfo.json schema.usda) since they are purely derived from their template files(*.cmake.in *.json.in)

1. This needs to be confirmed, but we had issues when the generated Schemas code had the same name for the derived type as it did for the alias
```
TfType::AddAlias<UsdSchemaBase, PolyShape>("PolyShape"); //- Avoid?
TfType::AddAlias<UsdSchemaBase, StudioPolyShape>("PolyShape"); //- Good
```


## Maya Nodes Package
There are no specific requirements that AL_USDMaya requires from the Maya nodes. 

The creation of these Nodes are typically triggered by the translator after AL_USDMaya has encountered a known typed prim.


## Translators Package
The translators contain the logic that create the corresponding Maya representation of the USD hierarchy and are required to be a pixar plugin.

The translators need to be registered with the PlugRegistry before translation. Saying that, All translators must:
1. Inherit from TranslatorBase, which can be found int AL_USDMaya
1. Make sure you call AL_USDMAYA_DECLARE_TRANSLATOR(UsdAnimatedPlane); in the header
1. Make sure you call AL_USDMAYA_DEFINE_TRANSLATOR(UsdAnimatedPlane) in the cpp

## Demo
This demo serves to show 2 things.
1. Show that the stack of packages works
1. Show attributes on Prims can drive values in Maya

Lets start by making sure that all the plugins in Maya and USD are loaded


### Maya Plugins
Load up maya(I have tested this with Maya2017) 

Make sure your Maya plugins are loaded
1. StudioMayaNodesPlugin
1. AL_USDMaya

If one of your Maya plugin isn't showing up
1. make sure your MAYA_PLUGIN_PATH is setup and one of the paths points to the folder where your missing maya plugin is.

### USD Plugins

Make sure your Pixar Plugins are loaded.
1. StudioSchemas
1. StudioTranslators
1. AL.usdmaya

You can also confirm this by running this:
```python
from pxr.Plug import Registry
loadedPlugins = [x.name for x in Registry().GetAllPlugins()]
print "PxrPlugin: StudioSchemas: Loaded" if "StudioSchemas" in loadedPlugins else "PxrPlugin: StudioSchemas: Unavailable"
print "PxrPlugin: StudioTranslators: Loaded" if "StudioTranslators" in loadedPlugins else "PxrPlugin: StudioTranslators: Unavailable"
print "PxrPlugin: AL_USDMaya: Loaded" if "AL.usdmaya" in loadedPlugins else "PxrPlugin: AL_USDMaya: Unavailable"
```

If one of your USD plugins arn't available
1. Check that your PXR_PLUGINPATH is setup and pointing to the folder of your missing plugin which contains the plugInfo.json that contains content along the lines of:
```javascript
{
    "Includes": [ "*/resources/" ]
}
```

1. Check your other pluginInfo.json in your resources folder to make sure it looks similar to this:
```javascript
# Portions of this file auto-generated by usdGenSchema.
# Edits will survive regeneration except for comments and
# changes to types with autoGenerated=true.
{
    "Plugins": [
        {
            "Info": {
                "Types": {
                    "Studio_UsdAnimatedPlane": {
                        "alias": {
                            "UsdSchemaBase": "UsdAnimatedPlane"
                        }, 
                        "autoGenerated": true, 
                        "bases": [
                            "UsdTyped"
                        ]
                    }
                }
            }, 
            "LibraryPath": "../../../../lib/libStudioSchemas.so", 
            "Name": "StudioSchemas", 
            "ResourcePath": "resources", 
            "Root": "..", 
            "Type": "library"
        }
    ]
}
```


### Demo

Congratulations! Getting this far can be quite the journey.

There is a demo scene in StudioTranslators called AnimatedPlaneScene.usda it has a single Prim with type UsdAnimatedPlane which we want to translate into Maya.

To load it into the AL_USDMaya plugin:

```
from maya import cmds
maya.cmds.AL_usdmaya_ProxyShapeImport(f='<STUDIO_EXAMPLE_RESPOSITORY_PATH>/example_data/AnimatedPlaneScene.usda')
```

You should now see a small gray square at the origin along the X-Z plane. 

You can now move the animation slider between frame 1-10, this will change the time in the ProxyShape which will then pass the changed time down to the USDDrivenMeshExample node which will then ask the size attribute for it's value at the current time from USD which will cause itself to recompute the plane size

You can also set the size value at a certain time directly via USD


Lets set the size at frame 5 to size 1.

```
from AL import usdmaya
stageCache = usdmaya.StageCache.Get()
stage = stageCache.GetAllStages()[0]


p1 = stage.GetPrimAtPath("/Root/AnimatedPlane")
a = p1.GetAttribute("size")
a.Set(1,5)
```

Now scrub the animation slider to frame 5 and notice it shrinks down.


### Plugin creation advice
There are certain steps which need to be met before Pixar's PluginRegistry is able to find and then consume your plugin.
From our experience, the easiest way to start to create a Pixar plugin is by looking through the the cmake modules Pixar provide, particularly pxr_plugin this method will:
1. Compile all the passed in source files
1. Creates and installs a DSO pixar plugin from the passed in source files and a DSO from the boost:python cpp files.
1. Installs the Resources needed by the Pixar plugin(e,g, pluginInfo.json, there are 2 pluginInfos!)


Using Pixar's plugin macro in AL_USDMaya:

```
pxr_plugin(${PXR_PACKAGE}
    LIBRARIES
        ar
        arch
        usdImaging
        usdImagingGL
        AL_USDMaya
        ${MAYA_LIBRARIES}
    INCLUDE_DIRS
        ${MAYA_INCLUDE_DIRS}
        "../../lib/AL_USDMaya"
    CPPFILES
        plugin.cpp
)
```

You might find that we don't often use this cmake module often. The reason for this is that we wanted to have more control of the build and installation process.

### Further work
1. We are looking to merging all our CMake configuration into a single repository to avoid copying it around to all the different packages

### FAQ
Nothing yet, Please email us! usdmaya@al.com.au 

