#include "fbx_prop.h"

void FbxProp::SetType(char t) { type = t; }

char FbxProp::GetType() { return type; }

void FbxProp::SetArrayLen(unsigned len){
    arraySize = len;
}
void FbxProp::SetData(const std::vector<char>& data){
    this->data = data;
}
std::string FbxProp::GetString(){
    unsigned strLen = 0;
    for(unsigned i = 0; i < data.size(); ++i)
    {
        if(data[i] == '\0')
            break;
        strLen++;
    }
    return std::string(data.data(), data.data() + strLen);
}
int64_t FbxProp::GetInt64(){
    if(data.size() != sizeof(int64_t))
        return 0;
    return *(int64_t*)(data.data());
}
int32_t FbxProp::GetInt32(){
    if(data.size() != sizeof(int32_t))
        return 0;
    return *(int32_t*)(data.data());
}
float FbxProp::GetFloat(){
    if(data.size() != sizeof(float))
        return 0.0;
    return *(float*)(data.data());
}
double FbxProp::GetDouble(){
    if(data.size() != sizeof(double))
        return 0.0;
    return *(double*)(data.data());
}