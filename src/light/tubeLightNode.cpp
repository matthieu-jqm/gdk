#include "tubeLightNode.h"

using namespace std;

DEFINE_TYPEHANDLE(TubeLightNode)


TubeLightNode::TubeLightNode(const string& name): 
    LightBase(name, new PerspectiveLens())
{
    
}
