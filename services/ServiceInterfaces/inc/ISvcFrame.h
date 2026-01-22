#ifndef ISVC_FRAME_H
#define ISVC_FRAME_H

#include <string>

class ISvcFrame {
public:
    virtual ~ISvcFrame() = default;

    virtual std::string ReadCurDirPath() = 0;
    virtual bool loadConfig(const std::string &curPath) = 0;
    virtual void saveConfig() = 0;
};

#endif // ISVC_FRAME_H
