#include <jni.h>
#include <string>
#include <sstream>
#include "usbhost.h"
#include <cassert>
#include <unistd.h>
#include <android/log.h>
#include <fcntl.h>
#include <malloc.h>
#include "fds_usb_api.h"


#define LOG(...)  __android_log_print(ANDROID_LOG_INFO, "Usb test", __VA_ARGS__)

std::string toString(char* cstr)
{
    if (cstr == NULL)
        return "null";
    std::string str(cstr);
    free(cstr);
    return str;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_future_1ds_usb_1native_MainActivity_testUsbDeviceNative(JNIEnv *env, jobject thiz, jint fd, jstring jname, jlong pipe_size) {
    std::stringstream res;

    LOG("Create device...");

    usb_host_context *ctx = usb_host_init();
    assert(ctx != NULL);
    usb_device *dev = usb_device_new("fds", fd);
    assert(dev != NULL);

    res  << "\n Product name: " << toString(usb_device_get_product_name(dev))
          << std::endl;

    usb_descriptor_iter it;
    usb_descriptor_iter_init(dev, &it);

	usb_reset(dev);

    int test_len = 1024 ;
    int addr;
    int i=0, j=0;
    int wdata[1024];
    int rdata[1024];
    int err = 0;

    res << "DDR4 Memory Test Read after Write. (1024 burst test) \n";
    res << "  - Address range: 0x00000000 to 0x00020000 \n\n";

    addr = 0x00000000;

    for(i=0;i<32;i++) {
        for (j = 0; j < test_len; j++) {
            wdata[j] = i + j;
        }
        bfmWrite(dev, addr + (4 * i * 1024), wdata, test_len);
        bfmRead(dev, addr + (4 * i * 1024), rdata, test_len);
        for (j = 0; j < test_len; j++) {
            if (wdata[j] != rdata[j]) {
                res << (i) << " -> rdata = : " << (rdata[j]) << " \n\n";
                err++;
            }
        }
    }

    if(err==0) {
        res << "--> DDR4 Memory Test Passed.\n\n";
    } else {
        res << "--> DDR4 Memory Test Failed.(" << (err) << ") \n\n";
    }

    usb_host_cleanup(ctx);

    return env->NewStringUTF(res.str().c_str());
}
