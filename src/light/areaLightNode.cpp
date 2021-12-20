#include "areaLightNode.h"

using namespace std;

DEFINE_TYPEHANDLE(AreaLightNode)


AreaLightNode::AreaLightNode(const string& name): 
    LightBase(name, new PerspectiveLens())
{
    
}
