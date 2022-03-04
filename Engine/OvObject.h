#include "FileReader.h"

// Stripped-down redefinition of OvObject (just for the chunk IDs):
class OvObject
{
public:
    enum class Type : int  ///< Type of entities
    {
        // Foundation types:
        O_OBJECT = 0,
        O_NODE,
        O_OBJECT2D,
        O_OBJECT3D,
        O_LIST,

        // Derived classes:
        O_BUFFER,
        O_SHADER,
        O_TEXTURE,
        O_FILTER,
        O_MATERIAL,
        O_FBO,
        O_QUAD,
        O_BOX,
        O_SKYBOX,
        O_FONT,
        O_CAMERA,
        O_LIGHT,
        O_BONE,
        O_MESH,	   // Keep them...
        O_SKINNED, // ...consecutive        
        O_INSTANCED,
        O_PIPELINE,
        O_EMITTER,

        // Animation type
        O_ANIM,

        // Physics related:
        O_PHYSICS,

        // Terminator:
        O_LAST,
    };
};

