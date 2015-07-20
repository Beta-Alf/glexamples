#include <gloperate/plugin/plugin_api.h>

#include "AnimationExample.h"

#include <glexamples-version.h>

GLOPERATE_PLUGIN_LIBRARY

    GLOPERATE_PAINTER_PLUGIN(AnimationExample
    , "AnimationExample"
    , "An Example demonstrating different ways of animation"
    , GLEXAMPLES_AUTHOR_ORGANIZATION
    , "v1.0.0" )

GLOPERATE_PLUGIN_LIBRARY_END
