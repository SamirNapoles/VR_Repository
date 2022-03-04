#pragma once


// Stripped-down redefinition of OvMesh (just for the subtypes):
class OvMesh
{
public:
    enum class Subtype : int ///< Kind of mesh
    {
        // Foundation types:
        DEFAULT = 0,
        NORMALMAPPED,
        TESSELLATED,

        // Terminator:
        LAST,
    };
};