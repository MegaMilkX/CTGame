#include "fbx_node.h"

void FbxNode::AddNode(const FbxNode& node){
    children[node.GetName()].emplace_back(node);
}
void FbxNode::AddProp(const FbxProp& prop){
    props.emplace_back(prop);
}
void FbxNode::SetName(const std::string& name){
    this->name = name;
}
const std::string& FbxNode::GetName() const{
    return name;
}
unsigned FbxNode::PropCount() const{
    return props.size();
}
FbxProp& FbxNode::GetProperty(unsigned i){
    return props[i];
}
unsigned FbxNode::ChildCount(const std::string& type) {
    return children[type].size();
}
FbxNode& FbxNode::GetNode(const std::string& type, unsigned i){
    return children[type][i];
}
