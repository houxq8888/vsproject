#ifndef HXQDETECTDIMENTIONVUXINTERFACE_H
#define HXQDETECTDIMENTIONVUXINTERFACE_H




namespace HGMACHINE {

#ifdef __cplusplus
extern "C" {
#endif


    void detDimentionVux(const HGImg2D &img, const HGRect2D &roi);

    float getDimentionValue();

    HGImg2D getDst();

}

#ifdef __cplusplus
}
#endif
#endif // HXQDETECTDIMENTIONVUXINTERFACE_H
