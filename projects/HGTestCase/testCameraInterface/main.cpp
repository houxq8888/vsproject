#include "CameraRecognizeInterface.h"

using namespace HGMACHINE;
int main(){
    std::vector<std::string> list = getCameraList("USB");
    for (auto &i:list){
        printf("%s\n",i.c_str());
    }
    openCamera("USB","/dev/video0");
    HGImg2D img=getImgOneShot("USB","/dev/video0");
    return 0;
}